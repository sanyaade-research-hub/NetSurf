/*
 * This file is part of NetSurf, http://netsurf.sourceforge.net/
 * Licensed under the GNU General Public License,
 *                http://www.opensource.org/licenses/gpl-license
 * Copyright 2004 James Bursa <bursa@users.sourceforge.net>
 */

/** \file
 * Content for image/jpeg (interface).
 */

#ifndef _NETSURF_IMAGE_JPEG_H_
#define _NETSURF_IMAGE_JPEG_H_

#include <stdbool.h>

struct bitmap;
struct content;

struct content_jpeg_data {
	int dummy; /* NOT USED but to satisfy Norcroft */
};

bool nsjpeg_convert(struct content *c, int width, int height);
void nsjpeg_destroy(struct content *c);
bool nsjpeg_redraw(struct content *c, int x, int y,
		int width, int height,
		int clip_x0, int clip_y0, int clip_x1, int clip_y1,
		float scale, unsigned long background_colour);
bool nsjpeg_redraw_tiled(struct content *c, int x, int y,
		int width, int height,
		int clip_x0, int clip_y0, int clip_x1, int clip_y1,
		float scale, unsigned long background_colour,
		bool repeat_x, bool repeat_y);

#endif
