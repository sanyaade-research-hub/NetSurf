/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *		  http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#include "bytecode/bytecode.h"
#include "bytecode/opcodes.h"
#include "select/propset.h"
#include "select/propget.h"
#include "utils/utils.h"

#include "select/properties/properties.h"
#include "select/properties/helpers.h"

css_error cascade_padding_top(uint32_t opv, css_style *style, 
		css_select_state *state)
{
	return cascade_length(opv, style, state, set_padding_top);
}

css_error set_padding_top_from_hint(const css_hint *hint,
		css_computed_style *style)
{
	return set_padding_top(style, hint->status,
			hint->data.length.value, hint->data.length.unit);
}

css_error initial_padding_top(css_select_state *state)
{
	return set_padding_top(state->result, CSS_PADDING_SET, 0, CSS_UNIT_PX);
}

css_error compose_padding_top(const css_computed_style *parent,
		const css_computed_style *child,
		css_computed_style *result)
{
	css_fixed length = 0;
	css_unit unit = CSS_UNIT_PX;
	uint8_t type = get_padding_top(child, &length, &unit);

	if (type == CSS_PADDING_INHERIT) {
		type = get_padding_top(parent, &length, &unit);
	}

	return set_padding_top(result, type, length, unit);
}

uint32_t destroy_padding_top(void *bytecode)
{
	return generic_destroy_length(bytecode);
}
