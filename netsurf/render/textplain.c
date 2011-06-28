/*
 * Copyright 2006 James Bursa <bursa@users.sourceforge.net>
 * Copyright 2006 Adrian Lees <adrianl@users.sourceforge.net>
 *
 * This file is part of NetSurf, http://www.netsurf-browser.org/
 *
 * NetSurf is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * NetSurf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/** \file
 * Content for text/plain (implementation).
 */

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <strings.h>
#include <math.h>

#include <parserutils/input/inputstream.h>

#include "content/content_protected.h"
#include "content/hlcache.h"
#include "css/css.h"
#include "css/utils.h"
#include "desktop/browser.h"
#include "desktop/gui.h"
#include "desktop/options.h"
#include "desktop/plotters.h"
#include "desktop/search.h"
#include "desktop/selection.h"
#include "render/box.h"
#include "render/font.h"
#include "render/textplain.h"
#include "utils/http.h"
#include "utils/log.h"
#include "utils/messages.h"
#include "utils/talloc.h"
#include "utils/utils.h"
#include "utils/utf8.h"

struct textplain_line {
	size_t	start;
	size_t	length;
};

typedef struct textplain_content {
	struct content base;

	char *encoding;
	void *inputstream;
	char *utf8_data;
	size_t utf8_data_size;
	size_t utf8_data_allocated;
	unsigned long physical_line_count;
	struct textplain_line *physical_line;
	int formatted_width;
} textplain_content;


#define CHUNK 32768 /* Must be a power of 2 */
#define MARGIN 4


#define TAB_WIDTH 8  /* must be power of 2 currently */
#define TEXT_SIZE 10 * FONT_SIZE_SCALE  /* Unscaled text size in pt */

static plot_font_style_t textplain_style = {
	.family = PLOT_FONT_FAMILY_MONOSPACE,
	.size = TEXT_SIZE,
	.weight = 400,
	.flags = FONTF_NONE,
	.background = 0xffffff,
	.foreground = 0x000000,
};

static int textplain_tab_width = 256;  /* try for a sensible default */

static nserror textplain_create(const content_handler *handler,
		lwc_string *imime_type, const http_parameter *params,
		llcache_handle *llcache, const char *fallback_charset,
		bool quirks, struct content **c);
static nserror textplain_create_internal(textplain_content *c, 
		const char *charset);
static bool textplain_process_data(struct content *c, 
		const char *data, unsigned int size);
static bool textplain_convert(struct content *c);
static void textplain_mouse_track(struct content *c, struct browser_window *bw,
			browser_mouse_state mouse, int x, int y);
static void textplain_mouse_action(struct content *c, struct browser_window *bw,
			browser_mouse_state mouse, int x, int y);
static void textplain_reformat(struct content *c, int width, int height);
static void textplain_destroy(struct content *c);
static bool textplain_redraw(struct content *c, struct content_redraw_data *data,
		const struct rect *clip);
static nserror textplain_clone(const struct content *old, 
		struct content **newc);
static content_type textplain_content_type(lwc_string *mime_type);

static parserutils_error textplain_charset_hack(const uint8_t *data, size_t len,
		uint16_t *mibenum, uint32_t *source);
static bool textplain_drain_input(textplain_content *c, 
		parserutils_inputstream *stream, parserutils_error terminator);
static bool textplain_copy_utf8_data(textplain_content *c,
		const uint8_t *buf, size_t len);
static int textplain_coord_from_offset(const char *text, size_t offset,
	size_t length);
static float textplain_line_height(void);

static const content_handler textplain_content_handler = {
	.create = textplain_create,
	.process_data = textplain_process_data,
	.data_complete = textplain_convert,
	.reformat = textplain_reformat,
	.destroy = textplain_destroy,
	.mouse_track = textplain_mouse_track,
	.mouse_action = textplain_mouse_action,
	.redraw = textplain_redraw,
	.clone = textplain_clone,
	.type = textplain_content_type,
	.no_share = true,
};

static lwc_string *textplain_mime_type;

/**
 * Initialise the text content handler
 */
nserror textplain_init(void)
{
	lwc_error lerror;
	nserror error;

	lerror = lwc_intern_string("text/plain", SLEN("text/plain"), 
			&textplain_mime_type);
	if (lerror != lwc_error_ok)
		return NSERROR_NOMEM;

	error = content_factory_register_handler(textplain_mime_type, 
			&textplain_content_handler);
	if (error != NSERROR_OK)
		lwc_string_unref(textplain_mime_type);

	return error;
}

/**
 * Clean up after the text content handler
 */
void textplain_fini(void)
{
	lwc_string_unref(textplain_mime_type);
}

/**
 * Create a CONTENT_TEXTPLAIN.
 */

nserror textplain_create(const content_handler *handler,
		lwc_string *imime_type, const http_parameter *params,
		llcache_handle *llcache, const char *fallback_charset,
		bool quirks, struct content **c)
{
	textplain_content *text;
	nserror error;
	const char *encoding;

	text = talloc_zero(0, textplain_content);
	if (text == NULL)
		return NSERROR_NOMEM;

	error = content__init(&text->base, handler, imime_type, params,
			llcache, fallback_charset, quirks);
	if (error != NSERROR_OK) {
		talloc_free(text);
		return error;
	}

	error = http_parameter_list_find_item(params, "charset", &encoding);
	if (error != NSERROR_OK) {
		encoding = "Windows-1252";
	}

	error = textplain_create_internal(text, encoding);
	if (error != NSERROR_OK) {
		talloc_free(text);
		return error;
	}

	*c = (struct content *) text;

	return NSERROR_OK;
}

/*
 * Hack around bug in libparserutils: if the client provides an
 * encoding up front, but does not provide a charset detection
 * callback, then libparserutils will replace the provided encoding
 * with UTF-8. This breaks our input handling.
 *
 * We avoid this by providing a callback that does precisely nothing,
 * thus preserving whatever charset information we decided on in
 * textplain_create.
 */
parserutils_error textplain_charset_hack(const uint8_t *data, size_t len,
		uint16_t *mibenum, uint32_t *source)
{
	return PARSERUTILS_OK;
} 

nserror textplain_create_internal(textplain_content *c, const char *encoding)
{
	char *utf8_data;
	parserutils_inputstream *stream;
	parserutils_error error;
	union content_msg_data msg_data;

	textplain_style.size = (option_font_size * FONT_SIZE_SCALE) / 10;

	utf8_data = talloc_array(c, char, CHUNK);
	if (utf8_data == NULL)
		goto no_memory;

	error = parserutils_inputstream_create(encoding, 0, 
			textplain_charset_hack, ns_realloc, NULL, &stream);
	if (error == PARSERUTILS_BADENCODING) {
		/* Fall back to Windows-1252 */
		error = parserutils_inputstream_create("Windows-1252", 0,
				textplain_charset_hack, ns_realloc, NULL,
				&stream);
	}
	if (error != PARSERUTILS_OK) {
		talloc_free(utf8_data);
		goto no_memory;
	}
	
	c->encoding = strdup(encoding);
	if (c->encoding == NULL) {
		talloc_free(utf8_data);
		parserutils_inputstream_destroy(stream);
		goto no_memory;
	}
		
	c->inputstream = stream;
	c->utf8_data = utf8_data;
	c->utf8_data_size = 0;
	c->utf8_data_allocated = CHUNK;
	c->physical_line = 0;
	c->physical_line_count = 0;
	c->formatted_width = 0;

	return NSERROR_OK;

no_memory:
	msg_data.error = messages_get("NoMemory");
	content_broadcast(&c->base, CONTENT_MSG_ERROR, msg_data);
	return NSERROR_NOMEM;
}

bool textplain_drain_input(textplain_content *c, 
		parserutils_inputstream *stream,
		parserutils_error terminator)
{
	static const uint8_t *u_fffd = (const uint8_t *) "\xef\xbf\xfd";
	const uint8_t *ch;
	size_t chlen, offset = 0;

	while (parserutils_inputstream_peek(stream, offset, &ch, &chlen) != 
			terminator) {
		/* Replace all instances of NUL with U+FFFD */
		if (chlen == 1 && *ch == 0) {
			if (offset > 0) {
				/* Obtain pointer to start of input data */
				parserutils_inputstream_peek(stream, 0, 
						&ch, &chlen);
				/* Copy from it up to the start of the NUL */
				if (textplain_copy_utf8_data(c, ch, 
						offset) == false)
					return false;
			}

			/* Emit U+FFFD */
			if (textplain_copy_utf8_data(c, u_fffd, 3) == false)
				return false;

			/* Advance inputstream past the NUL we just read */
			parserutils_inputstream_advance(stream, offset + 1);
			/* Reset the read offset */
			offset = 0;
		} else {
			/* Accumulate input */
			offset += chlen;

			if (offset > CHUNK) {
				/* Obtain pointer to start of input data */
				parserutils_inputstream_peek(stream, 0, 
						&ch, &chlen);

				/* Emit the data we've read */
				if (textplain_copy_utf8_data(c, ch, 
						offset) == false)
					return false;

				/* Advance the inputstream */
				parserutils_inputstream_advance(stream, offset);
				/* Reset the read offset */
				offset = 0;
			}
		}
	}

	if (offset > 0) {
		/* Obtain pointer to start of input data */
		parserutils_inputstream_peek(stream, 0, &ch, &chlen);	
		/* Emit any data remaining */
		if (textplain_copy_utf8_data(c, ch, offset) == false)
			return false;

		/* Advance the inputstream past the data we've read */
		parserutils_inputstream_advance(stream, offset);
	}

	return true;
}

bool textplain_copy_utf8_data(textplain_content *c, 
		const uint8_t *buf, size_t len)
{
	if (c->utf8_data_size + len >= c->utf8_data_allocated) {
		/* Compute next multiple of chunk above the required space */
		size_t allocated = (c->utf8_data_size + len + 
				CHUNK - 1) & ~(CHUNK - 1);
		char *utf8_data = talloc_realloc(c,
				c->utf8_data,
				char, allocated);
		if (utf8_data == NULL)
			return false;

		c->utf8_data = utf8_data;
		c->utf8_data_allocated = allocated;
	}

	memcpy(c->utf8_data + c->utf8_data_size, buf, len);
	c->utf8_data_size += len;

	return true;
}


/**
 * Process data for CONTENT_TEXTPLAIN.
 */

bool textplain_process_data(struct content *c, 
		const char *data, unsigned int size)
{
	textplain_content *text = (textplain_content *) c;
	parserutils_inputstream *stream = text->inputstream;
	union content_msg_data msg_data;
	parserutils_error error;

	error = parserutils_inputstream_append(stream, 
			(const uint8_t *) data, size);
	if (error != PARSERUTILS_OK) {
		goto no_memory;
	}

	if (textplain_drain_input(text, stream, PARSERUTILS_NEEDDATA) == false)
		goto no_memory;

	return true;

no_memory:
	msg_data.error = messages_get("NoMemory");
	content_broadcast(c, CONTENT_MSG_ERROR, msg_data);
	return false;
}


/**
 * Convert a CONTENT_TEXTPLAIN for display.
 */

bool textplain_convert(struct content *c)
{
	textplain_content *text = (textplain_content *) c;
	parserutils_inputstream *stream = text->inputstream;
	parserutils_error error;

	error = parserutils_inputstream_append(stream, NULL, 0);
	if (error != PARSERUTILS_OK) {
		return false;
	}

	if (textplain_drain_input(text, stream, PARSERUTILS_EOF) == false)
		return false;

	parserutils_inputstream_destroy(stream);
	text->inputstream = NULL;

	content_set_ready(c);
	content_set_done(c);
	content_set_status(c, messages_get("Done"));

	return true;
}


/**
 * Reformat a CONTENT_TEXTPLAIN to a new width.
 */

void textplain_reformat(struct content *c, int width, int height)
{
	textplain_content *text = (textplain_content *) c;
	char *utf8_data = text->utf8_data;
	size_t utf8_data_size = text->utf8_data_size;
	unsigned long line_count = 0;
	struct textplain_line *line = text->physical_line;
	struct textplain_line *line1;
	size_t i, space, col;
	size_t columns = 80;
	int character_width;
	size_t line_start;

	/* compute available columns (assuming monospaced font) - use 8
	 * characters for better accuracy */
	if (!nsfont.font_width(&textplain_style, "ABCDEFGH", 8, &character_width))
		return;
	columns = (width - MARGIN - MARGIN) * 8 / character_width;
	textplain_tab_width = (TAB_WIDTH * character_width) / 8;

	text->formatted_width = width;

	text->physical_line_count = 0;

	if (!line) {
		text->physical_line = line =
				talloc_array(c, struct textplain_line, 1024 + 3);
		if (!line)
			goto no_memory;
	}

	line[line_count++].start = line_start = 0;
	space = 0;
	for (i = 0, col = 0; i != utf8_data_size; i++) {
		bool term = (utf8_data[i] == '\n' || utf8_data[i] == '\r');
		size_t next_col = col + 1;

		if (utf8_data[i] == '\t')
			next_col = (next_col + TAB_WIDTH - 1) & ~(TAB_WIDTH - 1);

		if (term || next_col >= columns) {
			if (line_count % 1024 == 0) {
				line1 = talloc_realloc(c, line,
						struct textplain_line, line_count + 1024 + 3);
				if (!line1)
					goto no_memory;
				text->physical_line = line = line1;
			}
			if (term) {
				line[line_count-1].length = i - line_start;

				/* skip second char of CR/LF or LF/CR pair */
				if (i + 1 < utf8_data_size &&
					utf8_data[i+1] != utf8_data[i] &&
					(utf8_data[i+1] == '\n' || utf8_data[i+1] == '\r'))
					i++;
			}
			else {
				if (space) {
					/* break at last space in line */
					i = space;
					line[line_count-1].length = (i + 1) - line_start;
				}
				else
					line[line_count-1].length = i - line_start;
			}
			line[line_count++].start = line_start = i + 1;
			col = 0;
			space = 0;
		} else {
			col++;
			if (utf8_data[i] == ' ')
				space = i;
		}
	}
	line[line_count-1].length = i - line[line_count-1].start;
	line[line_count].start = utf8_data_size;

	text->physical_line_count = line_count;
	c->width = width;
	c->height = line_count * textplain_line_height() + MARGIN + MARGIN;

	return;

no_memory:
	LOG(("out of memory (line_count %lu)", line_count));
	return;
}


/**
 * Destroy a CONTENT_TEXTPLAIN and free all resources it owns.
 */

void textplain_destroy(struct content *c)
{
	textplain_content *text = (textplain_content *) c;

	if (text->encoding != NULL)
		free(text->encoding);

	if (text->inputstream != NULL)
		parserutils_inputstream_destroy(text->inputstream);
}


nserror textplain_clone(const struct content *old, struct content **newc)
{
	const textplain_content *old_text = (textplain_content *) old;
	textplain_content *text;
	nserror error;
	const char *data;
	unsigned long size;

	text = talloc_zero(0, textplain_content);
	if (text == NULL)
		return NSERROR_NOMEM;

	error = content__clone(old, &text->base);
	if (error != NSERROR_OK) {
		content_destroy(&text->base);
		return error;
	}

	/* Simply replay create/process/convert */
	error = textplain_create_internal(text, old_text->encoding);
	if (error != NSERROR_OK) {
		content_destroy(&text->base);
		return error;
	}

	data = content__get_source_data(&text->base, &size);
	if (size > 0) {
		if (textplain_process_data(&text->base, data, size) == false) {
			content_destroy(&text->base);
			return NSERROR_NOMEM;
		}
	}

	if (old->status == CONTENT_STATUS_READY ||
			old->status == CONTENT_STATUS_DONE) {
		if (textplain_convert(&text->base) == false) {
			content_destroy(&text->base);
			return NSERROR_CLONE_FAILED;
		}
	}

	return NSERROR_OK;
}

content_type textplain_content_type(lwc_string *mime_type)
{
	return CONTENT_TEXTPLAIN;
}

/**
 * Handle mouse tracking (including drags) in a TEXTPLAIN content window.
 *
 * \param  c	  content of type textplain
 * \param  bw	  browser window
 * \param  mouse  state of mouse buttons and modifier keys
 * \param  x	  coordinate of mouse
 * \param  y	  coordinate of mouse
 */

void textplain_mouse_track(struct content *c, struct browser_window *bw,
		browser_mouse_state mouse, int x, int y)
{
	switch (bw->drag_type) {

		case DRAGGING_SELECTION: {
			hlcache_handle *h = bw->current_content;
			int dir = -1;
			size_t idx;

			if (selection_dragging_start(bw->sel)) dir = 1;

			idx = textplain_offset_from_coords(h, x, y, dir);
			selection_track(bw->sel, mouse, idx);
		}
		break;

		default:
			textplain_mouse_action(c, bw, mouse, x, y);
			break;
	}
}


/**
 * Handle mouse clicks and movements in a TEXTPLAIN content window.
 *
 * \param  c	  content of type textplain
 * \param  bw	  browser window
 * \param  click  type of mouse click
 * \param  x	  coordinate of mouse
 * \param  y	  coordinate of mouse
 */

void textplain_mouse_action(struct content *c, struct browser_window *bw,
		browser_mouse_state mouse, int x, int y)
{
	hlcache_handle *h = bw->current_content;
	gui_pointer_shape pointer = GUI_POINTER_DEFAULT;
	const char *status = 0;
	size_t idx;
	int dir = 0;

	bw->drag_type = DRAGGING_NONE;

	if (!bw->sel) return;

	idx = textplain_offset_from_coords(h, x, y, dir);
	if (selection_click(bw->sel, mouse, idx)) {

		if (selection_dragging(bw->sel)) {
			bw->drag_type = DRAGGING_SELECTION;
			status = messages_get("Selecting");
		}
		else
			status = content_get_status_message(h);
	}
	else {
		if (bw->loading_content)
			status = content_get_status_message(
					bw->loading_content);
		else
			status = content_get_status_message(h);

		if (mouse & (BROWSER_MOUSE_DRAG_1 | BROWSER_MOUSE_DRAG_2)) {
			browser_window_page_drag_start(bw, x, y);
			pointer = GUI_POINTER_MOVE;
		}
	}

	if (status != NULL)
		browser_window_set_status(bw, status);

	browser_window_set_pointer(bw, pointer);
}


/**
 * Draw a CONTENT_TEXTPLAIN using the current set of plotters (plot).
 *
 * \param  c	 content of type CONTENT_TEXTPLAIN
 * \param  data	 redraw data for this content redraw
 * \param  clip	 current clip region
 * \return true if successful, false otherwise
 *
 * x, y, clip_[xy][01] are in target coordinates.
 */

bool textplain_redraw(struct content *c, struct content_redraw_data *data,
		const struct rect *clip)
{
	textplain_content *text = (textplain_content *) c;
	struct browser_window *bw = current_redraw_browser;
	char *utf8_data = text->utf8_data;
	long lineno;
	int x = data->x;
	int y = data->y;
	unsigned long line_count = text->physical_line_count;
	float line_height = textplain_line_height();
	float scaled_line_height = line_height * data->scale;
	long line0 = (clip->y0 - y * data->scale) / scaled_line_height - 1;
	long line1 = (clip->y1 - y * data->scale) / scaled_line_height + 1;
	struct textplain_line *line = text->physical_line;
	size_t length;
	plot_style_t *plot_style_highlight;

	if (line0 < 0)
		line0 = 0;
	if (line1 < 0)
		line1 = 0;
	if (line_count < (unsigned long) line0)
		line0 = line_count;
	if (line_count < (unsigned long) line1)
		line1 = line_count;
	if (line1 < line0)
		line1 = line0;

	if (!plot.rectangle(clip->x0, clip->y0, clip->x1, clip->y1,
			plot_style_fill_white))
		return false;

	if (!line)
		return true;

	/* choose a suitable background colour for any highlighted text */
	if ((data->background_colour & 0x808080) == 0x808080)
		plot_style_highlight = plot_style_fill_black;
	else
		plot_style_highlight = plot_style_fill_white;

	/* Set up font plot style */
	textplain_style.background = data->background_colour;

	x = (x + MARGIN) * data->scale;
	y = (y + MARGIN) * data->scale;
	for (lineno = line0; lineno != line1; lineno++) {
		const char *text = utf8_data + line[lineno].start;
		int tab_width = textplain_tab_width * data->scale;
		size_t offset = 0;
		int tx = x;

		if (!tab_width) tab_width = 1;

		length = line[lineno].length;
		if (!length)
			continue;

		while (offset < length) {
			size_t next_offset = offset;
			int width;
			int ntx;

			while (next_offset < length && text[next_offset] != '\t')
				next_offset = utf8_next(text, length, next_offset);

			if (!text_redraw(text + offset, next_offset - offset,
					line[lineno].start + offset, 0,
					&textplain_style,
					tx, y + (lineno * scaled_line_height),
					clip, line_height, data->scale, false))
				return false;

			if (next_offset >= length)
				break;

			/* locate end of string and align to next tab position */
			if (nsfont.font_width(&textplain_style, &text[offset],
					next_offset - offset, &width))
				tx += (int)(width * data->scale);

			ntx = x + ((1 + (tx - x) / tab_width) * tab_width);

			/* if the tab character lies within the selection, if any,
			   then we must draw it as a filled rectangle so that it's
			   consistent with background of the selected text */

			if (bw) {
				unsigned tab_ofst = line[lineno].start + next_offset;
				struct selection *sel = bw->sel;
				bool highlighted = false;

				if (selection_defined(sel)) {
					unsigned start_idx, end_idx;
					if (selection_highlighted(sel,
						tab_ofst, tab_ofst + 1,
						&start_idx, &end_idx))
						highlighted = true;
				}

				if (!highlighted && (bw->search_context 
						!= NULL)) {
					unsigned start_idx, end_idx;
					if (search_term_highlighted(bw,
							tab_ofst, tab_ofst + 1,
							&start_idx, &end_idx,
							bw->search_context))
						highlighted = true;
				}

				if (highlighted) {
					int sy = y + (lineno * scaled_line_height);
					if (!plot.rectangle(tx, sy, 
							    ntx, sy + scaled_line_height,
							    plot_style_highlight))
						return false;
				}
			}

			offset = next_offset + 1;
			tx = ntx;
		}
	}

	return true;
}

/**
 * Retrieve number of lines in content
 *
 * \param h  Content to retrieve line count from
 * \return Number of lines
 */
unsigned long textplain_line_count(hlcache_handle *h)
{
	textplain_content *c = 
			(textplain_content *) hlcache_handle_get_content(h);

	assert(c != NULL);

	return c->physical_line_count;
}

/**
 * Retrieve the size (in bytes) of text data
 *
 * \param h  Content to retrieve size of
 * \return Size, in bytes, of data
 */
size_t textplain_size(hlcache_handle *h)
{
	textplain_content *c = 
			(textplain_content *) hlcache_handle_get_content(h);

	assert(c != NULL);

	return c->utf8_data_size;
}

/**
 * Return byte offset within UTF8 textplain content, given the co-ordinates
 * of a point within a textplain content. 'dir' specifies the direction in
 * which to search (-1 = above-left, +1 = below-right) if the co-ordinates are not
 * contained within a line.
 *
 * \param  h     content of type CONTENT_TEXTPLAIN
 * \param  x     x ordinate of point
 * \param  y     y ordinate of point
 * \param  dir   direction of search if not within line
 * \return byte offset of character containing (or nearest to) point
 */

size_t textplain_offset_from_coords(hlcache_handle *h, int x, int y, int dir)
{
	textplain_content *c = 
			(textplain_content *) hlcache_handle_get_content(h);
	float line_height = textplain_line_height();
	struct textplain_line *line;
	const char *text;
	unsigned nlines;
	size_t length;
	int idx;

	assert(c != NULL);

	y = (int)((float)(y - MARGIN) / line_height);
	x -= MARGIN;

	nlines = c->physical_line_count;
	if (!nlines)
		return 0;

	if (y <= 0) y = 0;
	else if ((unsigned)y >= nlines)
		y = nlines - 1;

	line = &c->physical_line[y];
	text = c->utf8_data + line->start;
	length = line->length;
	idx = 0;

	while (x > 0) {
		size_t next_offset = 0;
		int width = INT_MAX;

		while (next_offset < length && text[next_offset] != '\t')
			next_offset = utf8_next(text, length, next_offset);

		if (next_offset < length)
			nsfont.font_width(&textplain_style, text, next_offset, &width);

		if (x <= width) {
			int pixel_offset;
			size_t char_offset;

			nsfont.font_position_in_string(&textplain_style,
				text, next_offset, x,
				&char_offset, &pixel_offset);

			idx += char_offset;
			break;
		}

		x -= width;
		length -= next_offset;
		text += next_offset;
		idx += next_offset;

		/* check if it's within the tab */
		width = textplain_tab_width - (width % textplain_tab_width);
		if (x <= width) break;

		x -= width;
		length--;
		text++;
		idx++;
	}

	return line->start + idx;
}


/**
 * Given a byte offset within the text, return the line number
 * of the line containing that offset (or -1 if offset invalid)
 *
 * \param  h       content of type CONTENT_TEXTPLAIN
 * \param  offset  byte offset within textual representation
 * \return line number, or -1 if offset invalid (larger than size)
 */

int textplain_find_line(hlcache_handle *h, unsigned offset)
{
	textplain_content *c = 
			(textplain_content *) hlcache_handle_get_content(h);
	struct textplain_line *line;
	int nlines;
	int lineno = 0;

	assert(c != NULL);

	line = c->physical_line;
	nlines = c->physical_line_count;

	if (offset > c->utf8_data_size)
		return -1;

/* \todo - implement binary search here */
	while (lineno < nlines && line[lineno].start < offset)
		lineno++;
	if (line[lineno].start > offset)
		lineno--;

	return lineno;
}


/**
 * Convert a character offset within a line of text into the
 * horizontal co-ordinate, taking into account the font being
 * used and any tabs in the text
 *
 * \param  text    line of text
 * \param  offset  char offset within text
 * \param  length  line length
 * \return x ordinate
 */

int textplain_coord_from_offset(const char *text, size_t offset, size_t length)
{
	int x = 0;

	while (offset > 0) {
		size_t next_offset = 0;
		int tx;

		while (next_offset < offset && text[next_offset] != '\t')
			next_offset = utf8_next(text, length, next_offset);

		nsfont.font_width(&textplain_style, text, next_offset, &tx);
		x += tx;

		if (next_offset >= offset)
			break;

		/* align to next tab boundary */
		next_offset++;
		x = (1 + (x / textplain_tab_width)) * textplain_tab_width;
		offset -= next_offset;
		text += next_offset;
		length -= next_offset;
	}

	return x;
}


/**
 * Given a range of byte offsets within a UTF8 textplain content,
 * return a box that fully encloses the text
 *
 * \param  h      content of type CONTENT_TEXTPLAIN
 * \param  start  byte offset of start of text range
 * \param  end    byte offset of end
 * \param  r      rectangle to be completed
 */

void textplain_coords_from_range(hlcache_handle *h, unsigned start, 
		unsigned end, struct rect *r)
{
	textplain_content *c = 
			(textplain_content *) hlcache_handle_get_content(h);
	float line_height = textplain_line_height();
	char *utf8_data;
	struct textplain_line *line;
	unsigned lineno = 0;
	unsigned nlines;

	assert(c != NULL);
	assert(start <= end);
	assert(end <= c->utf8_data_size);

	utf8_data = c->utf8_data;
	nlines = c->physical_line_count;
	line = c->physical_line;

	/* find start */
	lineno = textplain_find_line(h, start);

	r->y0 = (int)(MARGIN + lineno * line_height);

	if (lineno + 1 <= nlines || line[lineno + 1].start >= end) {
		/* \todo - it may actually be more efficient just to run
			forwards most of the time */

		/* find end */
		lineno = textplain_find_line(h, end);

		r->x0 = 0;
		r->x1 = c->formatted_width;
	}
	else {
		/* single line */
		const char *text = utf8_data + line[lineno].start;

		r->x0 = textplain_coord_from_offset(text, start - line[lineno].start,
				line[lineno].length);

		r->x1 = textplain_coord_from_offset(text, end - line[lineno].start,
				line[lineno].length);
	}

	r->y1 = (int)(MARGIN + (lineno + 1) * line_height);
}


/**
 * Return a pointer to the requested line of text.
 *
 * \param  h		    content of type CONTENT_TEXTPLAIN
 * \param  lineno	    line number
 * \param  poffset	    receives byte offset of line start within text
 * \param  plen		    receives length of returned line
 * \return pointer to text, or NULL if invalid line number
 */

char *textplain_get_line(hlcache_handle *h, unsigned lineno,
		size_t *poffset, size_t *plen)
{
	textplain_content *c = 
			(textplain_content *) hlcache_handle_get_content(h);
	struct textplain_line *line;

	assert(c != NULL);

	if (lineno >= c->physical_line_count)
		return NULL;
	line = &c->physical_line[lineno];

	*poffset = line->start;
	*plen = line->length;
	return c->utf8_data + line->start;
}


/**
 * Return a pointer to the raw UTF-8 data, as opposed to the reformatted
 * text to fit the window width. Thus only hard newlines are preserved
 * in the saved/copied text of a selection.
 *
 * \param  h		    content of type CONTENT_TEXTPLAIN
 * \param  start	    starting byte offset within UTF-8 text
 * \param  end		    ending byte offset
 * \param  plen		    receives validated length
 * \return pointer to text, or NULL if no text
 */

char *textplain_get_raw_data(hlcache_handle *h, unsigned start, unsigned end,
		size_t *plen)
{
	textplain_content *c = 
			(textplain_content *) hlcache_handle_get_content(h);
	size_t utf8_size;

	assert(c != NULL);

	utf8_size = c->utf8_data_size;

	/* any text at all? */
	if (!utf8_size) return NULL;

	/* clamp to valid offset range */
	if (start >= utf8_size) start = utf8_size;
	if (end >= utf8_size) end = utf8_size;

	*plen = end - start;

	return c->utf8_data + start;
}

/**
 * Calculate the line height, in pixels
 * 
 * \return Line height, in pixels
 */
float textplain_line_height(void)
{
	/* Size is in points, so convert to pixels. 
	 * Then use a constant line height of 1.2 x font size.
	 */
	return FIXTOFLT(FDIV((FMUL(FLTTOFIX(1.2), FMUL(nscss_screen_dpi, 
		INTTOFIX((textplain_style.size / FONT_SIZE_SCALE))))), F_72));
}

