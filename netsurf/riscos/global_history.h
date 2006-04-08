/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2005 Richard Wilson <info@tinct.net>
 */

/** \file
 * Global history (interface).
 */

#ifndef _NETSURF_RISCOS_GLOBALHISTORY_H_
#define _NETSURF_RISCOS_GLOBALHISTORY_H_

#include "netsurf/content/url_store.h"

#define GLOBAL_HISTORY_RECENT_URLS 16

void ro_gui_global_history_initialise(void);
int ro_gui_global_history_help(int x, int y);
void ro_gui_global_history_save(void);


#endif
