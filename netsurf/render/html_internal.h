/*
 * Copyright 2004 James Bursa <bursa@users.sourceforge.net>
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
 * Content for text/html (private data).
 */

#ifndef NETSURF_RENDER_HTML_INTERNAL_H_
#define NETSURF_RENDER_HTML_INTERNAL_H_

#include "content/content_protected.h"
#include "render/html.h"

/** Data specific to CONTENT_HTML. */
typedef struct html_content {
	struct content base;

	/** Parser object handle */
	void *parser_binding;
	/** Document tree */
	xmlDoc *document;
	/** Quirkyness of document */
	binding_quirks_mode quirks;

	/** Encoding of source, NULL if unknown. */
	char *encoding;
	/** Source of encoding information. */
	binding_encoding_source encoding_source;

	/** Base URL (may be a copy of content->url). */
	char *base_url;
	/** Base target */
	char *base_target;

	/** Box tree, or NULL. */
	struct box *layout;
	/** Document background colour. */
	colour background_colour;
	/** Font callback table */
	const struct font_functions *font_func;

	/** Number of entries in stylesheet_content. */
	unsigned int stylesheet_count;
	/** Stylesheets. Each may be NULL. */
	struct html_stylesheet *stylesheets;
	/**< Style selection context */
	css_select_ctx *select_ctx;

	/** Number of entries in object_list. */
	unsigned int num_objects;
	/** List of objects. */
	struct content_html_object *object_list;
	/** Forms, in reverse order to document. */
	struct form *forms;
	/** Hash table of imagemaps. */
	struct imagemap **imagemaps;

	/** Browser window containing this document, or NULL if not open. */
	struct browser_window *bw;

	/** Frameset information */
	struct content_html_frames *frameset;

	/** Inline frame information */
	struct content_html_iframe *iframe;

	/** Content of type CONTENT_HTML containing this, or NULL if not an 
	 * object within a page. */
	struct html_content *page;
	/** Box containing this, or NULL if not an object. */
	struct box *box;
} html_content;


bool html_fetch_object(html_content *c, const char *url, struct box *box,
		content_type permitted_types,
		int available_width, int available_height,
		bool background);

void html_set_status(html_content *c, const char *extra);

/* in render/html_redraw.c */
bool html_redraw(struct content *c, int x, int y,
		int width, int height, const struct rect *clip,
		float scale, colour background_colour);

/* in render/html_interaction.c */
void html_mouse_track(struct content *c, struct browser_window *bw,
			browser_mouse_state mouse, int x, int y);
void html_mouse_action(struct content *c, struct browser_window *bw,
			browser_mouse_state mouse, int x, int y);
void html_overflow_scroll_callback(void *client_data,
		struct scroll_msg_data *scroll_data);

#endif

