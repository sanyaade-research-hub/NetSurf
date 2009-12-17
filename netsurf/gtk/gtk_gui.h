/*
 * Copyright 2005 James Bursa <bursa@users.sourceforge.net>
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

#ifndef GTK_GUI_H
#define GTK_GUI_H

#include <inttypes.h>
#include <stdbool.h>
#include <gtk/gtk.h>
#include <glade/glade.h>

extern bool gui_in_multitask;
extern GladeXML *gladeNetsurf;
extern GladeXML *gladePassword;
extern GladeXML *gladeWarning;
extern GladeXML *gladeLogin;
extern GladeXML *gladeSsl;
extern char *glade_netsurf_file_location;
extern char *glade_password_file_location;
extern char *glade_warning_file_location;
extern char *glade_login_file_location;
extern char *glade_ssl_file_location;
extern char *glade_toolbar_file_location;
extern char *toolbar_indices_file_location;
extern char *options_file_location;
extern char *res_dir_location;
extern char *print_options_file_location;

extern GtkWindow *wndAbout;

extern GtkWindow *wndTooltip;
extern GtkLabel *labelTooltip;

extern GtkDialog *wndOpenFile;

uint32_t gtk_gui_gdkkey_to_nskey(GdkEventKey *);

#endif /* GTK_GUI_H */

