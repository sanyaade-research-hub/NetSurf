/*
 * Copyright 2004 James Bursa <james@netsurf-browser.org>
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
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

#include <assert.h>

#include "css/utils.h"

#include "desktop/options.h"
#include "utils/log.h"

/** Screen DPI in fixed point units: defaults to 90, which RISC OS uses */
css_fixed nscss_screen_dpi = INTTOFIX(90);

/**
 * Convert an absolute CSS length to points.
 *
 * \param  length  Length to convert
 * \param  unit    Corresponding unit
 * \return	   length in points
 */
css_fixed nscss_len2pt(css_fixed length, css_unit unit)
{
	/* Length must not be relative */
	assert(unit != CSS_UNIT_EM && unit != CSS_UNIT_EX);

	switch (unit) {
	/* We assume the screen and any other output has the same dpi */
	/* 1in = DPIpx => 1px = (72/DPI)pt */
	case CSS_UNIT_PX: return FDIV(FMULI(length, 72), nscss_screen_dpi);
	/* 1in = 72pt */
	case CSS_UNIT_IN: return FMULI(length, 72);
	/* 1in = 2.54cm => 1cm = (72/2.54)pt */
	case CSS_UNIT_CM: return FMUL(length, 
				FDIV(INTTOFIX(72), FLTTOFIX(2.54)));
	/* 1in = 25.4mm => 1mm = (72/25.4)pt */
	case CSS_UNIT_MM: return FMUL(length, 
				FDIV(INTTOFIX(72), FLTTOFIX(25.4)));
	case CSS_UNIT_PT: return length;
	/* 1pc = 12pt */
	case CSS_UNIT_PC: return FMULI(length, 12);
	default: break;
	}

	return 0;
}


/**
 * Convert a CSS length to pixels.
 *
 * \param  length  Length to convert
 * \param  unit    Corresponding unit
 * \param  style   Computed style applying to length. May be NULL if unit is 
 *                 neither em nor ex
 * \return	   length in pixels
 */
css_fixed nscss_len2px(css_fixed length, css_unit unit, 
		const css_computed_style *style)
{
	/* We assume the screen and any other output has the same dpi */
	const css_fixed lendpi = FMUL(length, nscss_screen_dpi);

	assert(style != NULL || (unit != CSS_UNIT_EM && unit != CSS_UNIT_EX));

	switch (unit) {
	case CSS_UNIT_EM:
	case CSS_UNIT_EX:
	{
		css_fixed font_size = 0;
		css_unit font_unit = CSS_UNIT_PT;

		css_computed_font_size(style, &font_size, &font_unit);

		/* Convert to points */
		font_size = nscss_len2pt(font_size, font_unit);

		/* Clamp to configured minimum */
		if (font_size < FDIVI(INTTOFIX(option_font_min_size), 10)) {
			font_size = FDIVI(INTTOFIX(option_font_min_size), 10);
		}

		/* Expand relative length */
		length = FMUL(length, 
				nscss_len2px(font_size, CSS_UNIT_PT, style));

		/* Scale ex units: we use a fixed ratio of 1ex = 0.6em */
		if (unit == CSS_UNIT_EX)
			length = FMUL(length, FLTTOFIX(0.6));
			
		return length;
	}
	case CSS_UNIT_PX: return length;
	/* 1in = DPIpx */
	case CSS_UNIT_IN: return lendpi;
	/* 1in = 2.54cm => 1cm = (DPI/2.54)px */
	case CSS_UNIT_CM: return FDIV(lendpi, FLTTOFIX(2.54));
	/* 1in = 25.4mm => 1mm = (DPI/25.4)px */
	case CSS_UNIT_MM: return FDIV(lendpi, FLTTOFIX(25.4));
	/* 1in = 72pt => 1pt = (DPI/72)px */
	case CSS_UNIT_PT: return FDIV(lendpi, INTTOFIX(72));
	/* 1pc = 12pt => 1in = 6pc => 1pc = (DPI/6)px */
	case CSS_UNIT_PC: return FDIV(lendpi, INTTOFIX(6));
	default: break;
	}

	return 0;
}

