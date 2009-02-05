/*
 * This file is part of LibCSS
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef libcss_properties_h_
#define libcss_properties_h_

enum css_background_attachment {
	CSS_BACKGROUND_ATTACHMENT_FIXED		= 0x0,
	CSS_BACKGROUND_ATTACHMENT_SCROLL	= 0x1,
	CSS_BACKGROUND_ATTACHMENT_INHERIT	= 0x2
};

enum css_background_color {
	CSS_BACKGROUND_COLOR_TRANSPARENT	= 0x0,
	CSS_BACKGROUND_COLOR_COLOR		= 0x1,
	CSS_BACKGROUND_COLOR_INHERIT		= 0x2
};

enum css_background_image {
	/* Consult pointer in struct to determine which */
	CSS_BACKGROUND_IMAGE_NONE		= 0x0,
	CSS_BACKGROUND_IMAGE_IMAGE		= 0x0,
	CSS_BACKGROUND_IMAGE_INHERIT		= 0x1
};

enum css_background_position {
	CSS_BACKGROUND_POSITION_SET		= 0x0,
	CSS_BACKGROUND_POSITION_INHERIT		= 0x1
};

enum css_background_repeat {
	CSS_BACKGROUND_REPEAT_NO_REPEAT		= 0x0,
	CSS_BACKGROUND_REPEAT_REPEAT_X		= 0x1,
	CSS_BACKGROUND_REPEAT_REPEAT_Y		= 0x2,
	CSS_BACKGROUND_REPEAT_REPEAT		= 0x3,
	CSS_BACKGROUND_REPEAT_INHERIT		= 0x4
};

enum css_border_collapse {
	CSS_BORDER_COLLAPSE_SEPARATE		= 0x0,
	CSS_BORDER_COLLAPSE_COLLAPSE		= 0x1,
	CSS_BORDER_COLLAPSE_INHERIT		= 0x2
};

enum css_border_spacing {
	CSS_BORDER_SPACING_SET			= 0x0,
	CSS_BORDER_SPACING_INHERIT		= 0x1
};

enum css_border_color {
	CSS_BORDER_COLOR_TRANSPARENT		= 0x0,
	CSS_BORDER_COLOR_COLOR			= 0x1,
	CSS_BORDER_COLOR_INHERIT		= 0x2
};

enum css_border_style {
	CSS_BORDER_STYLE_NONE			= 0x0,
	CSS_BORDER_STYLE_HIDDEN			= 0x1,
	CSS_BORDER_STYLE_DOTTED			= 0x2,
	CSS_BORDER_STYLE_DASHED			= 0x3,
	CSS_BORDER_STYLE_SOLID			= 0x4,
	CSS_BORDER_STYLE_DOUBLE			= 0x5,
	CSS_BORDER_STYLE_GROOVE			= 0x6,
	CSS_BORDER_STYLE_RIDGE			= 0x7,
	CSS_BORDER_STYLE_INSET			= 0x8,
	CSS_BORDER_STYLE_OUTSET			= 0x9,
	CSS_BORDER_STYLE_INHERIT		= 0xa
};

enum css_border_width {
	CSS_BORDER_WIDTH_THIN			= 0x0,
	CSS_BORDER_WIDTH_MEDIUM			= 0x1,
	CSS_BORDER_WIDTH_THICK			= 0x2,
	CSS_BORDER_WIDTH_WIDTH			= 0x3,
	CSS_BORDER_WIDTH_INHERIT		= 0x4
};

enum css_bottom {
	CSS_BOTTOM_SET				= 0x0,
	CSS_BOTTOM_AUTO				= 0x1,
	CSS_BOTTOM_INHERIT			= 0x2
};

enum css_caption_side {
	CSS_CAPTION_SIDE_TOP			= 0x0,
	CSS_CAPTION_SIDE_BOTTOM			= 0x1,
	CSS_CAPTION_SIDE_INHERIT		= 0x2
};

enum css_clear {
	CSS_CLEAR_NONE				= 0x0,
	CSS_CLEAR_LEFT				= 0x1,
	CSS_CLEAR_RIGHT				= 0x2,
	CSS_CLEAR_BOTH				= 0x3,
	CSS_CLEAR_INHERIT			= 0x4
};

enum css_clip {
	CSS_CLIP_AUTO				= 0x0,
	CSS_CLIP_RECT				= 0x1,
	CSS_CLIP_INHERIT			= 0x2
};

enum css_color {
	CSS_COLOR_COLOR				= 0x0,
	CSS_COLOR_INHERIT			= 0x1
};

/** \todo content
enum css_content {
};
*/

enum css_counter_increment {
	/* Consult pointer in struct to determine which */
	CSS_COUNTER_INCREMENT_NAMED		= 0x0,
	CSS_COUNTER_INCREMENT_NONE		= 0x0,
	CSS_COUNTER_INCREMENT_INHERIT		= 0x1
};

enum css_counter_reset {
	/* Consult pointer in struct to determine which */
	CSS_COUNTER_RESET_NAMED			= 0x0,
	CSS_COUNTER_RESET_NONE			= 0x0,
	CSS_COUNTER_RESET_INHERIT		= 0x1
};

enum css_cursor {
	/* URLs exist if pointer is non-NULL */
	CSS_CURSOR_AUTO				= 0x00,
	CSS_CURSOR_CROSSHAIR			= 0x01,
	CSS_CURSOR_DEFAULT			= 0x02,
	CSS_CURSOR_POINTER			= 0x03,
	CSS_CURSOR_MOVE				= 0x04,
	CSS_CURSOR_E_RESIZE			= 0x05,
	CSS_CURSOR_NE_RESIZE			= 0x06,
	CSS_CURSOR_NW_RESIZE			= 0x07,
	CSS_CURSOR_N_RESIZE			= 0x08,
	CSS_CURSOR_SE_RESIZE			= 0x09,
	CSS_CURSOR_SW_RESIZE			= 0x0a,
	CSS_CURSOR_S_RESIZE			= 0x0b,
	CSS_CURSOR_W_RESIZE			= 0x0c,
	CSS_CURSOR_TEXT				= 0x0d,
	CSS_CURSOR_WAIT				= 0x0e,
	CSS_CURSOR_HELP				= 0x0f,
	CSS_CURSOR_PROGRESS			= 0x10,
	CSS_CURSOR_INHERIT			= 0x11
};

enum css_direction {
	CSS_DIRECTION_LTR			= 0x0,
	CSS_DIRECTION_RTL			= 0x1,
	CSS_DIRECTION_INHERIT			= 0x2
};

enum css_display {
	CSS_DISPLAY_INLINE			= 0x00,
	CSS_DISPLAY_BLOCK			= 0x01,
	CSS_DISPLAY_LIST_ITEM			= 0x02,
	CSS_DISPLAY_RUN_IN			= 0x03,
	CSS_DISPLAY_INLINE_BLOCK		= 0x04,
	CSS_DISPLAY_TABLE			= 0x05,
	CSS_DISPLAY_INLINE_TABLE		= 0x06,
	CSS_DISPLAY_TABLE_ROW_GROUP		= 0x07,
	CSS_DISPLAY_TABLE_HEADER_GROUP		= 0x08,
	CSS_DISPLAY_TABLE_FOOTER_GROUP		= 0x09,
	CSS_DISPLAY_TABLE_ROW			= 0x0a,
	CSS_DISPLAY_TABLE_COLUMN_GROUP		= 0x0b,
	CSS_DISPLAY_TABLE_COLUMN		= 0x0c,
	CSS_DISPLAY_TABLE_CELL			= 0x0d,
	CSS_DISPLAY_TABLE_CAPTION		= 0x0e,
	CSS_DISPLAY_NONE			= 0x0f,
	CSS_DISPLAY_INHERIT			= 0x10
};

enum css_empty_cells {
	CSS_EMPTY_CELLS_SHOW			= 0x0,
	CSS_EMPTY_CELLS_HIDE			= 0x1,
	CSS_EMPTY_CELLS_INHERIT			= 0x2
};

enum css_float {
	CSS_FLOAT_LEFT				= 0x0,
	CSS_FLOAT_RIGHT				= 0x1,
	CSS_FLOAT_NONE				= 0x2,
	CSS_FLOAT_INHERIT			= 0x3
};

enum css_font_family {
	CSS_FONT_FAMILY_SET			= 0x0,
	CSS_FONT_FAMILY_INHERIT			= 0x1
};

enum css_font_size {
	CSS_FONT_SIZE_XX_SMALL			= 0x0,
	CSS_FONT_SIZE_X_SMALL			= 0x1,
	CSS_FONT_SIZE_SMALL			= 0x2,
	CSS_FONT_SIZE_MEDIUM			= 0x3,
	CSS_FONT_SIZE_LARGE			= 0x4,
	CSS_FONT_SIZE_X_LARGE			= 0x5,
	CSS_FONT_SIZE_XX_LARGE			= 0x6,
	CSS_FONT_SIZE_LARGER			= 0x7,
	CSS_FONT_SIZE_SMALLER			= 0x8,
	CSS_FONT_SIZE_DIMENSION			= 0x9,
	CSS_FONT_SIZE_INHERIT			= 0xa
};

enum css_font_style {
	CSS_FONT_STYLE_NORMAL			= 0x0,
	CSS_FONT_STYLE_ITALIC			= 0x1,
	CSS_FONT_STYLE_OBLIQUE			= 0x2,
	CSS_FONT_STYLE_INHERIT			= 0x3
};

enum css_font_variant {
	CSS_FONT_VARIANT_NORMAL			= 0x0,
	CSS_FONT_VARIANT_SMALL_CAPS		= 0x1,
	CSS_FONT_VARIANT_INHERIT		= 0x2
};

enum css_font_weight {
	CSS_FONT_WEIGHT_NORMAL			= 0x0,
	CSS_FONT_WEIGHT_BOLD			= 0x1,
	CSS_FONT_WEIGHT_BOLDER			= 0x2,
	CSS_FONT_WEIGHT_LIGHTER			= 0x3,
	CSS_FONT_WEIGHT_100			= 0x4,
	CSS_FONT_WEIGHT_200			= 0x5,
	CSS_FONT_WEIGHT_300			= 0x6,
	CSS_FONT_WEIGHT_400			= 0x7,
	CSS_FONT_WEIGHT_500			= 0x8,
	CSS_FONT_WEIGHT_600			= 0x9,
	CSS_FONT_WEIGHT_700			= 0xa,
	CSS_FONT_WEIGHT_800			= 0xb,
	CSS_FONT_WEIGHT_900			= 0xc,
	CSS_FONT_WEIGHT_INHERIT			= 0xd
};

enum css_height {
	CSS_HEIGHT_SET				= 0x0,
	CSS_HEIGHT_AUTO				= 0x1,
	CSS_HEIGHT_INHERIT			= 0x2
};

enum css_left {
	CSS_LEFT_SET				= 0x0,
	CSS_LEFT_AUTO				= 0x1,
	CSS_LEFT_INHERIT			= 0x2
};

enum css_letter_spacing {
	CSS_LETTER_SPACING_SET			= 0x0,
	CSS_LETTER_SPACING_NORMAL		= 0x1,
	CSS_LETTER_SPACING_INHERIT		= 0x2
};

enum css_line_height {
	CSS_LINE_HEIGHT_NUMBER			= 0x0,
	CSS_LINE_HEIGHT_DIMENSION		= 0x1,
	CSS_LINE_HEIGHT_NORMAL			= 0x2,
	CSS_LINE_HEIGHT_INHERIT			= 0x3
};

enum css_list_style_image {
	/* Consult pointer in struct to determine which */
	CSS_LIST_STYLE_IMAGE_URI		= 0x0,
	CSS_LIST_STYLE_IMAGE_NONE		= 0x0,
	CSS_LIST_STYLE_IMAGE_INHERIT		= 0x1
};

enum css_list_style_position {
	CSS_LIST_STYLE_POSITION_INSIDE		= 0x0,
	CSS_LIST_STYLE_POSITION_OUTSIDE		= 0x1,
	CSS_LIST_STYLE_POSITION_INHERIT		= 0x2
};

enum css_list_style_type {
	CSS_LIST_STYLE_TYPE_DISC		= 0x0,
	CSS_LIST_STYLE_TYPE_CIRCLE		= 0x1,
	CSS_LIST_STYLE_TYPE_SQUARE		= 0x2,
	CSS_LIST_STYLE_TYPE_DECIMAL		= 0x3,
	CSS_LIST_STYLE_TYPE_DECIMAL_LEADING_ZERO= 0x4,
	CSS_LIST_STYLE_TYPE_LOWER_ROMAN		= 0x5,
	CSS_LIST_STYLE_TYPE_UPPER_ROMAN		= 0x6,
	CSS_LIST_STYLE_TYPE_LOWER_GREEK		= 0x7,
	CSS_LIST_STYLE_TYPE_LOWER_LATIN		= 0x8,
	CSS_LIST_STYLE_TYPE_UPPER_LATIN		= 0x9,
	CSS_LIST_STYLE_TYPE_ARMENIAN		= 0xa,
	CSS_LIST_STYLE_TYPE_GEORGIAN		= 0xb,
	CSS_LIST_STYLE_TYPE_LOWER_ALPHA		= 0xc,
	CSS_LIST_STYLE_TYPE_UPPER_ALPHA		= 0xd,
	CSS_LIST_STYLE_TYPE_NONE		= 0xe,
	CSS_LIST_STYLE_TYPE_INHERIT		= 0xf
};

enum css_margin {
	CSS_MARGIN_SET				= 0x0,
	CSS_MARGIN_AUTO				= 0x1,
	CSS_MARGIN_INHERIT			= 0x2
};

enum css_max_height {
	CSS_MAX_HEIGHT_SET			= 0x0,
	CSS_MAX_HEIGHT_NONE			= 0x1,
	CSS_MAX_HEIGHT_INHERIT			= 0x2
};

enum css_max_width {
	CSS_MAX_WIDTH_SET			= 0x0,
	CSS_MAX_WIDTH_NONE			= 0x1,
	CSS_MAX_WIDTH_INHERIT			= 0x2
};

enum css_min_height {
	CSS_MIN_HEIGHT_SET			= 0x0,
	CSS_MIN_HEIGHT_INHERIT			= 0x1
};

enum css_min_width {
	CSS_MIN_WIDTH_SET			= 0x0,
	CSS_MIN_WIDTH_INHERIT			= 0x1
};

enum css_outline_color {
	CSS_OUTLINE_COLOR_COLOR			= 0x0,
	CSS_OUTLINE_COLOR_INVERT		= 0x1,
	CSS_OUTLINE_COLOR_INHERIT		= 0x2
};

enum css_outline_style {
	CSS_OUTLINE_STYLE_NONE			= CSS_BORDER_STYLE_NONE,
	CSS_OUTLINE_STYLE_DOTTED		= CSS_BORDER_STYLE_DOTTED,
	CSS_OUTLINE_STYLE_DASHED		= CSS_BORDER_STYLE_DASHED,
	CSS_OUTLINE_STYLE_SOLID			= CSS_BORDER_STYLE_SOLID,
	CSS_OUTLINE_STYLE_DOUBLE		= CSS_BORDER_STYLE_DOUBLE,
	CSS_OUTLINE_STYLE_GROOVE		= CSS_BORDER_STYLE_GROOVE,
	CSS_OUTLINE_STYLE_RIDGE			= CSS_BORDER_STYLE_RIDGE,
	CSS_OUTLINE_STYLE_INSET			= CSS_BORDER_STYLE_INSET,
	CSS_OUTLINE_STYLE_OUTSET		= CSS_BORDER_STYLE_OUTSET,
	CSS_OUTLINE_STYLE_INHERIT		= CSS_BORDER_STYLE_INHERIT
};

enum css_outline_width {
	CSS_OUTLINE_WIDTH_THIN			= CSS_BORDER_WIDTH_THIN,
	CSS_OUTLINE_WIDTH_MEDIUM		= CSS_BORDER_WIDTH_MEDIUM,
	CSS_OUTLINE_WIDTH_THICK			= CSS_BORDER_WIDTH_THICK,
	CSS_OUTLINE_WIDTH_WIDTH			= CSS_BORDER_WIDTH_WIDTH,
	CSS_OUTLINE_WIDTH_INHERIT		= CSS_BORDER_WIDTH_INHERIT
};

enum css_overflow {
	CSS_OVERFLOW_VISIBLE			= 0x0,
	CSS_OVERFLOW_HIDDEN			= 0x1,
	CSS_OVERFLOW_SCROLL			= 0x2,
	CSS_OVERFLOW_AUTO			= 0x3,
	CSS_OVERFLOW_INHERIT			= 0x4
};

enum css_padding {
	CSS_PADDING_SET				= 0x0,
	CSS_PADDING_INHERIT			= 0x1
};

enum css_position {
	CSS_POSITION_STATIC			= 0x0,
	CSS_POSITION_RELATIVE			= 0x1,
	CSS_POSITION_ABSOLUTE			= 0x2,
	CSS_POSITION_FIXED			= 0x3,
	CSS_POSITION_INHERIT			= 0x4
};

enum css_quotes {
	/* Consult pointer in struct to determine which */
	CSS_QUOTES_STRING			= 0x0,
	CSS_QUOTES_NONE				= 0x0,
	CSS_QUOTES_INHERIT			= 0x1
};

enum css_right {
	CSS_RIGHT_SET				= 0x0,
	CSS_RIGHT_AUTO				= 0x1,
	CSS_RIGHT_INHERIT			= 0x2
};

enum css_table_layout {
	CSS_TABLE_LAYOUT_AUTO			= 0x0,
	CSS_TABLE_LAYOUT_FIXED			= 0x1,
	CSS_TABLE_LAYOUT_INHERIT		= 0x2
};

enum css_text_align {
	CSS_TEXT_ALIGN_LEFT			= 0x0,
	CSS_TEXT_ALIGN_RIGHT			= 0x1,
	CSS_TEXT_ALIGN_CENTER			= 0x2,
	CSS_TEXT_ALIGN_JUSTIFY			= 0x3,
	CSS_TEXT_ALIGN_INHERIT			= 0x4
};

enum css_text_decoration {
	CSS_TEXT_DECORATION_NONE		= 0x00,
	CSS_TEXT_DECORATION_BLINK		= (1<<3),
	CSS_TEXT_DECORATION_LINE_THROUGH	= (1<<2),
	CSS_TEXT_DECORATION_OVERLINE		= (1<<1),
	CSS_TEXT_DECORATION_UNDERLINE		= (1<<0),
	CSS_TEXT_DECORATION_INHERIT		= 0x10
};

enum css_text_indent {
	CSS_TEXT_INDENT_SET			= 0x0,
	CSS_TEXT_INDENT_INHERIT			= 0x1
};

enum css_text_transform {
	CSS_TEXT_TRANSFORM_CAPITALIZE		= 0x0,
	CSS_TEXT_TRANSFORM_UPPERCASE		= 0x1,
	CSS_TEXT_TRANSFORM_LOWERCASE		= 0x2,
	CSS_TEXT_TRANSFORM_NONE			= 0x3,
	CSS_TEXT_TRANSFORM_INHERIT		= 0x4
};

enum css_top {
	CSS_TOP_SET				= 0x0,
	CSS_TOP_AUTO				= 0x1,
	CSS_TOP_INHERIT				= 0x2
};

enum css_unicode_bidi {
	CSS_UNICODE_BIDI_NORMAL			= 0x0,
	CSS_UNICODE_BIDI_EMBED			= 0x1,
	CSS_UNICODE_BIDI_BIDI_OVERRIDE		= 0x2,
	CSS_UNICODE_BIDI_INHERIT		= 0x3
};

enum css_vertical_align {
	CSS_VERTICAL_ALIGN_BASELINE		= 0x0,
	CSS_VERTICAL_ALIGN_SUB			= 0x1,
	CSS_VERTICAL_ALIGN_SUPER		= 0x2,
	CSS_VERTICAL_ALIGN_TOP			= 0x3,
	CSS_VERTICAL_ALIGN_TEXT_TOP		= 0x4,
	CSS_VERTICAL_ALIGN_MIDDLE		= 0x5,
	CSS_VERTICAL_ALIGN_BOTTOM		= 0x6,
	CSS_VERTICAL_ALIGN_TEXT_BOTTOM		= 0x7,
	CSS_VERTICAL_ALIGN_SET			= 0x8,
	CSS_VERTICAL_ALIGN_INHERIT		= 0x9
};

enum css_visibility {
	CSS_VISIBILITY_VISIBLE			= 0x0,
	CSS_VISIBILITY_HIDDEN			= 0x1,
	CSS_VISIBILITY_COLLAPSE			= 0x2,
	CSS_VISIBILITY_INHERIT			= 0x3
};

enum css_white_space {
	CSS_WHITE_SPACE_NORMAL			= 0x0,
	CSS_WHITE_SPACE_PRE			= 0x1,
	CSS_WHITE_SPACE_NOWRAP			= 0x2,
	CSS_WHITE_SPACE_PRE_WRAP		= 0x3,
	CSS_WHITE_SPACE_PRE_LINE		= 0x4,
	CSS_WHITE_SPACE_INHERIT			= 0x5
};

enum css_width {
	CSS_WIDTH_SET				= 0x0,
	CSS_WIDTH_AUTO				= 0x1,
	CSS_WIDTH_INHERIT			= 0x2
};

enum css_word_spacing {
	CSS_WORD_SPACING_SET			= 0x0,
	CSS_WORD_SPACING_NORMAL			= 0x1,
	CSS_WORD_SPACING_INHERIT		= 0x2
};

enum css_z_index {
	CSS_Z_INDEX_SET				= 0x0,
	CSS_Z_INDEX_AUTO			= 0x1,
	CSS_Z_INDEX_INHERIT			= 0x2
};

#endif
