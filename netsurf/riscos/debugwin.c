/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *		  http://www.opensource.org/licenses/gpl-license
 * Copyright 2004 James Bursa <bursa@users.sourceforge.net>
 */

/** \file
 * Debug display window (implementation).
 */

#include <stdio.h>
#include <stdlib.h>
#include "oslib/wimp.h"
#include "netsurf/riscos/dialog.h"
#include "netsurf/riscos/wimp_event.h"
#include "netsurf/utils/log.h"
#include "netsurf/utils/utils.h"

/** Update interval / cs. */
#define DEBUGWIN_UPDATE 500

static void ro_gui_debugwin_resize(void);
static void ro_gui_debugwin_update(void *p);
static void ro_gui_debugwin_close(wimp_w w);
static void ro_gui_debugwin_redraw_plot(wimp_draw *redraw);
static void ro_gui_debugwin_redraw(wimp_draw *redraw);

void ro_gui_debugwin_open(void)
{
	ro_gui_wimp_event_register_close_window(dialog_debug,
			ro_gui_debugwin_close);
	ro_gui_wimp_event_register_redraw_window(dialog_debug,
			ro_gui_debugwin_redraw);
	ro_gui_debugwin_resize();
	ro_gui_dialog_open(dialog_debug);
	schedule_remove(ro_gui_debugwin_update, 0);
	schedule(DEBUGWIN_UPDATE, ro_gui_debugwin_update, 0);
}


void ro_gui_debugwin_resize(void)
{
	unsigned int count = 2;
	struct content *content;
	os_box box;
	os_error *error;

	for (content = content_list; content; content = content->next)
		count++;

	box.x0 = 0;
	box.y0 = -count * 28;
	box.x1 = 1200;
	box.y1 = 0;
	error = xwimp_set_extent(dialog_debug, &box);
	if (error) {
		LOG(("xwimp_set_extent: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
	}
}


void ro_gui_debugwin_update(void *p)
{
	os_error *error;
	ro_gui_debugwin_resize();
	error = xwimp_force_redraw(dialog_debug, 0, -10000, 10000, 0);
	if (error) {
		LOG(("xwimp_force_redraw: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
	}
	schedule(DEBUGWIN_UPDATE, ro_gui_debugwin_update, 0);
}


void ro_gui_debugwin_close(wimp_w w)
{
	os_error *error;
	error = xwimp_close_window(dialog_debug);
	if (error) {
		LOG(("xwimp_close_window: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
	}
	schedule_remove(ro_gui_debugwin_update, 0);
	ro_gui_wimp_event_finalise(dialog_debug);
}


void ro_gui_debugwin_redraw(wimp_draw *redraw)
{
	osbool more;
	os_error *error;

	error = xwimp_redraw_window(redraw, &more);
	if (error) {
		LOG(("xwimp_redraw_window: 0x%x: %s",
				error->errnum, error->errmess));
		warn_user("WimpError", error->errmess);
		return;
	}
	while (more) {
		ro_gui_debugwin_redraw_plot(redraw);
		error = xwimp_get_rectangle(redraw, &more);
		if (error) {
			LOG(("xwimp_get_rectangle: 0x%x: %s",
					error->errnum, error->errmess));
			warn_user("WimpError", error->errmess);
			return;
		}
	}
}


void ro_gui_debugwin_redraw_plot(wimp_draw *redraw)
{
	char s[20];
	int x0 = redraw->box.x0 - redraw->xscroll;
	int y0 = redraw->box.y1 - redraw->yscroll;
	int i = 1;
	int y;
	unsigned int users;
	unsigned int size = 0;
	struct content *content;
	struct content_user *user;

	xwimp_set_font_colours(wimp_COLOUR_BLACK, wimp_COLOUR_LIGHT_GREY);
	xwimptextop_paint(0, "url", x0 + 4, y0 - 20);
	xwimptextop_paint(0, "type", x0 + 600, y0 - 20);
	xwimptextop_paint(0, "fresh", x0 + 680, y0 - 20);
	xwimptextop_paint(0, "mime_type", x0 + 760, y0 - 20);
	xwimptextop_paint(0, "users", x0 + 910, y0 - 20);
	xwimptextop_paint(0, "status", x0 + 990, y0 - 20);
	xwimptextop_paint(0, "size", x0 + 1100, y0 - 20);

	xwimp_set_font_colours(wimp_COLOUR_BLACK, wimp_COLOUR_WHITE);
	for (content = content_list; content; content = content->next, i++) {
		y = y0 - i * 28 - 20;
		xwimptextop_paint(wimptextop_RJUSTIFY, content->url,
				x0 + 580, y);
		xwimptextop_paint(0, content_type_name[content->type],
				x0 + 600, y);
		xwimptextop_paint(0, content->fresh ? "�" : "�",
				x0 + 710, y);
		if (content->mime_type)
			xwimptextop_paint(0, content->mime_type,
					x0 + 760, y);
		users = 0;
		for (user = content->user_list->next; user; user = user->next)
			users++;
		snprintf(s, sizeof s, "%u", users);
		xwimptextop_paint(wimptextop_RJUSTIFY, s, x0 + 960, y);
		xwimptextop_paint(0, content_status_name[content->status],
				x0 + 990, y);
		snprintf(s, sizeof s, "%u", content->size);
		xwimptextop_paint(wimptextop_RJUSTIFY, s, x0 + 1190, y);
		size += content->size;
	}
	snprintf(s, sizeof s, "%u", size);
	xwimptextop_paint(wimptextop_RJUSTIFY, s, x0 + 1190, y0 - i * 28 - 20);
}
