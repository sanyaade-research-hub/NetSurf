/*
 * Copyright 2009 Chris Young <chris@unsatisfactorysoftware.co.uk>
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

#ifndef AMIGA_GUI_OPTIONS_H
#define AMIGA_GUI_OPTIONS_H

#include <exec/types.h>
#include <intuition/classusr.h>
#include "amiga/gui.h"

enum
{
	GID_OPTS_MAIN=0,
	GID_OPTS_CANCEL,
	GID_OPTS_LAST
};

struct ami_gui_opts_window {
	struct Window *win;
	Object *objects[OID_LAST];
	struct Gadget *gadgets[GID_OPTS_LAST];
	struct nsObject *node;
	ULONG pad[6];
};

void ami_opts_open(void);
BOOL ami_opts_event(void);
#endif
