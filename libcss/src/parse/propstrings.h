/*
 * This file is part of LibCSS.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2008 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef css_parse_propstrings_h_
#define css_parse_propstrings_h_

#include "utils/utils.h"

enum {
	/* Universal selector */
	UNIVERSAL,

	/* At-rules */
	CHARSET, IMPORT, MEDIA, PAGE,

	/* Media types */
	AURAL, BRAILLE, EMBOSSED, HANDHELD, PRINT, PROJECTION, 
	SCREEN, SPEECH, TTY, TV, ALL,

	/* Pseudo classes */
	FIRST_CHILD, LINK, VISITED, HOVER, ACTIVE, FOCUS, LANG, 
	/* LEFT, RIGHT, -- already in properties */ FIRST,

	/* Pseudo elements */
	FIRST_LINE, FIRST_LETTER, BEFORE, AFTER,

	/* Properties */
	FIRST_PROP,

	AZIMUTH = FIRST_PROP, BACKGROUND_ATTACHMENT, BACKGROUND_COLOR, 
	BACKGROUND_IMAGE, BACKGROUND_POSITION, BACKGROUND_REPEAT, 
	BORDER_BOTTOM_COLOR, BORDER_BOTTOM_STYLE, BORDER_BOTTOM_WIDTH, 
	BORDER_COLLAPSE, BORDER_LEFT_COLOR, BORDER_LEFT_STYLE, 
	BORDER_LEFT_WIDTH, BORDER_RIGHT_COLOR, BORDER_RIGHT_STYLE, 
	BORDER_RIGHT_WIDTH, BORDER_SPACING, BORDER_TOP_COLOR, BORDER_TOP_STYLE, 
	BORDER_TOP_WIDTH, BOTTOM, CAPTION_SIDE, CLEAR, CLIP, COLOR, CONTENT, 
	COUNTER_INCREMENT, COUNTER_RESET, CUE_AFTER, CUE_BEFORE, CURSOR, 
	DIRECTION, DISPLAY, ELEVATION, EMPTY_CELLS, FLOAT, FONT_FAMILY, 
	FONT_SIZE, FONT_STYLE, FONT_VARIANT, FONT_WEIGHT, HEIGHT, LEFT,
	LETTER_SPACING, LINE_HEIGHT, LIST_STYLE_IMAGE, LIST_STYLE_POSITION, 
	LIST_STYLE_TYPE, MARGIN_BOTTOM, MARGIN_LEFT, MARGIN_RIGHT, MARGIN_TOP, 
	MAX_HEIGHT, MAX_WIDTH, MIN_HEIGHT, MIN_WIDTH, ORPHANS, OUTLINE_COLOR, 
	OUTLINE_STYLE, OUTLINE_WIDTH, OVERFLOW, PADDING_BOTTOM, PADDING_LEFT, 
	PADDING_RIGHT, PADDING_TOP, PAGE_BREAK_AFTER, PAGE_BREAK_BEFORE, 
	PAGE_BREAK_INSIDE, PAUSE_AFTER, PAUSE_BEFORE, PITCH_RANGE, PITCH, 
	PLAY_DURING, POSITION, QUOTES, RICHNESS, RIGHT, SPEAK_HEADER, 
	SPEAK_NUMERAL, SPEAK_PUNCTUATION, SPEAK, SPEECH_RATE, STRESS, 
	TABLE_LAYOUT, TEXT_ALIGN, TEXT_DECORATION, TEXT_INDENT, TEXT_TRANSFORM, 
	TOP, UNICODE_BIDI, VERTICAL_ALIGN, VISIBILITY, VOICE_FAMILY, VOLUME, 
	WHITE_SPACE, WIDOWS, WIDTH, WORD_SPACING, Z_INDEX,

	LAST_PROP = Z_INDEX,

	/* Other keywords */
	INHERIT, IMPORTANT, NONE, BOTH, FIXED, SCROLL, TRANSPARENT,
	NO_REPEAT, REPEAT_X, REPEAT_Y, REPEAT, HIDDEN, DOTTED, DASHED,
	SOLID, DOUBLE, GROOVE, RIDGE, INSET, OUTSET, THIN, MEDIUM, THICK,
	COLLAPSE, SEPARATE, AUTO, LTR, RTL, INLINE, BLOCK, LIST_ITEM, RUN_IN,
	INLINE_BLOCK, TABLE, INLINE_TABLE, TABLE_ROW_GROUP, TABLE_HEADER_GROUP,
	TABLE_FOOTER_GROUP, TABLE_ROW, TABLE_COLUMN_GROUP, TABLE_COLUMN,
	TABLE_CELL, TABLE_CAPTION, BELOW, LEVEL, ABOVE, HIGHER, LOWER,
	SHOW, HIDE, XX_SMALL, X_SMALL, SMALL, LARGE, X_LARGE, XX_LARGE,
	LARGER, SMALLER, NORMAL, ITALIC, OBLIQUE, SMALL_CAPS, BOLD, BOLDER,
	LIGHTER, INSIDE, OUTSIDE, DISC, CIRCLE, SQUARE, DECIMAL, 
	DECIMAL_LEADING_ZERO, LOWER_ROMAN, UPPER_ROMAN, LOWER_GREEK,
	LOWER_LATIN, UPPER_LATIN, ARMENIAN, GEORGIAN, LOWER_ALPHA, UPPER_ALPHA,
	INVERT, VISIBLE, ALWAYS, AVOID, X_LOW, LOW, HIGH, X_HIGH, STATIC,
	RELATIVE, ABSOLUTE, ONCE, DIGITS, CONTINUOUS, CODE, SPELL_OUT, X_SLOW,
	SLOW, FAST, X_FAST, FASTER, SLOWER, CENTER, JUSTIFY, CAPITALIZE,
	UPPERCASE, LOWERCASE, EMBED, BIDI_OVERRIDE, BASELINE, SUB, SUPER,
	TEXT_TOP, MIDDLE, TEXT_BOTTOM, SILENT, X_SOFT, SOFT, LOUD, X_LOUD,
	PRE, NOWRAP, PRE_WRAP, PRE_LINE, LEFTWARDS, RIGHTWARDS, LEFT_SIDE,
	FAR_LEFT, CENTER_LEFT, CENTER_RIGHT, FAR_RIGHT, RIGHT_SIDE, BEHIND, 
	RECT, OPEN_QUOTE, CLOSE_QUOTE, NO_OPEN_QUOTE, NO_CLOSE_QUOTE, ATTR, 
	COUNTER, COUNTERS, CROSSHAIR, DEFAULT, POINTER, MOVE, E_RESIZE, 
	NE_RESIZE, NW_RESIZE, N_RESIZE, SE_RESIZE, SW_RESIZE, S_RESIZE, 
	W_RESIZE, TEXT, WAIT, HELP, PROGRESS, SERIF, SANS_SERIF, CURSIVE, 
	FANTASY, MONOSPACE, MALE, FEMALE, CHILD, MIX, UNDERLINE, OVERLINE, 
	LINE_THROUGH, BLINK, RGB,

	/* Named colours */
	FIRST_COLOUR,

	ALICEBLUE = FIRST_COLOUR, ANTIQUEWHITE, AQUA, AQUAMARINE, AZURE,
	BEIGE, BISQUE, BLACK, BLANCHEDALMOND, BLUE, BLUEVIOLET, BROWN,
	BURLYWOOD, CADETBLUE, CHARTREUSE, CHOCOLATE, CORAL, CORNFLOWERBLUE,
	CORNSILK, CRIMSON, CYAN, DARKBLUE, DARKCYAN, DARKGOLDENROD, DARKGRAY,
	DARKGREEN, DARKGREY, DARKKHAKI, DARKMAGENTA, DARKOLIVEGREEN, DARKORANGE,
	DARKORCHID, DARKRED, DARKSALMON, DARKSEAGREEN, DARKSLATEBLUE,
	DARKSLATEGRAY, DARKSLATEGREY, DARKTURQUOISE, DARKVIOLET, DEEPPINK,
	DEEPSKYBLUE, DIMGRAY, DIMGREY, DODGERBLUE, FELDSPAR, FIREBRICK,
	FLORALWHITE, FORESTGREEN, FUCHSIA, GAINSBORO, GHOSTWHITE, GOLD, 
	GOLDENROD, GRAY, GREEN, GREENYELLOW, GREY, HONEYDEW, HOTPINK,
	INDIANRED, INDIGO, IVORY, KHAKI, LAVENDER, LAVENDERBLUSH, LAWNGREEN,
	LEMONCHIFFON, LIGHTBLUE, LIGHTCORAL, LIGHTCYAN, LIGHTGOLDENRODYELLOW,
	LIGHTGRAY, LIGHTGREEN, LIGHTGREY, LIGHTPINK, LIGHTSALMON, LIGHTSEAGREEN,
	LIGHTSKYBLUE, LIGHTSLATEBLUE, LIGHTSLATEGRAY, LIGHTSLATEGREY, 
	LIGHTSTEELBLUE, LIGHTYELLOW, LIME, LIMEGREEN, LINEN, MAGENTA, MAROON,
	MEDIUMAQUAMARINE, MEDIUMBLUE, MEDIUMORCHID, MEDIUMPURPLE, 
	MEDIUMSEAGREEN, MEDIUMSLATEBLUE, MEDIUMSPRINGGREEN, MEDIUMTURQUOISE,
	MEDIUMVIOLETRED, MIDNIGHTBLUE, MINTCREAM, MISTYROSE, MOCCASIN,
	NAVAJOWHITE, NAVY, OLDLACE, OLIVE, OLIVEDRAB, ORANGE, ORANGERED,
	ORCHID, PALEGOLDENROD, PALEGREEN, PALETURQUOISE, PALEVIOLETRED,
	PAPAYAWHIP, PEACHPUFF, PERU, PINK, PLUM, POWDERBLUE, PURPLE, RED,
	ROSYBROWN, ROYALBLUE, SADDLEBROWN, SALMON, SANDYBROWN, SEAGREEN,
	SEASHELL, SIENNA, SILVER, SKYBLUE, SLATEBLUE, SLATEGRAY, SLATEGREY,
	SNOW, SPRINGGREEN, STEELBLUE, TAN, TEAL, THISTLE, TOMATO, TURQUOISE, 
	VIOLET, VIOLETRED, WHEAT, WHITE, WHITESMOKE, YELLOW, YELLOWGREEN,

	LAST_COLOUR = YELLOWGREEN,

	LAST_KNOWN
};

/* Must be synchronised with above enum */
static struct {
	const char *data;
	size_t len;
} stringmap[LAST_KNOWN] = {
	{ "*", SLEN("*") },

	{ "charset", SLEN("charset") },
	{ "import", SLEN("import") },
	{ "media", SLEN("media") },
	{ "page", SLEN("page") },

	{ "aural", SLEN("aural") },
	{ "braille", SLEN("braille") },
	{ "embossed", SLEN("embossed") },
	{ "handheld", SLEN("handheld") },
	{ "print", SLEN("print") },
	{ "projection", SLEN("projection") },
	{ "screen", SLEN("screen") },
	{ "speech", SLEN("speech") },
	{ "tty", SLEN("tty") },
	{ "tv", SLEN("tv") },
	{ "all", SLEN("all") },

	{ "first-child", SLEN("first-child") },
	{ "link", SLEN("link") },
	{ "visited", SLEN("visited") },
	{ "hover", SLEN("hover") },
	{ "active", SLEN("active") },
	{ "focus", SLEN("focus") },
	{ "lang", SLEN("lang") },
	{ "first", SLEN("first") },

	{ "first-line", SLEN("first-line") },
	{ "first-letter", SLEN("first-letter") },
	{ "before", SLEN("before") },
	{ "after", SLEN("after") },

	{ "azimuth", SLEN("azimuth") },
	{ "background-attachment", SLEN("background-attachment") },
	{ "background-color", SLEN("background-color") },
	{ "background-image", SLEN("background-image") },
	{ "background-position", SLEN("background-position") },
	{ "background-repeat", SLEN("background-repeat") },
	{ "border-bottom-color", SLEN("border-bottom-color") },
	{ "border-bottom-style", SLEN("border-bottom-style") },
	{ "border-bottom-width", SLEN("border-bottom-width") },
	{ "border-collapse", SLEN("border-collapse") },
	{ "border-left-color", SLEN("border-left-color") },
	{ "border-left-style", SLEN("border-left-style") },
	{ "border-left-width", SLEN("border-left-width") },
	{ "border-right-color", SLEN("border-right-color") },
	{ "border-right-style", SLEN("border-right-style") },
	{ "border-right-width", SLEN("border-right-width") },
	{ "border-spacing", SLEN("border-spacing") },
	{ "border-top-color", SLEN("border-top-color") },
	{ "border-top-style", SLEN("border-top-style") },
	{ "border-top-width", SLEN("border-top-width") },
	{ "bottom", SLEN("bottom") },
	{ "caption-side", SLEN("caption-side") },
	{ "clear", SLEN("clear") },
	{ "clip", SLEN("clip") },
	{ "color", SLEN("color") },
	{ "content", SLEN("content") },
	{ "counter-increment", SLEN("counter-increment") },
	{ "counter-reset", SLEN("counter-reset") },
	{ "cue-after", SLEN("cue-after") },
	{ "cue-before", SLEN("cue-before") },
	{ "cursor", SLEN("cursor") },
	{ "direction", SLEN("direction") },
	{ "display", SLEN("display") },
	{ "elevation", SLEN("elevation") },
	{ "empty-cells", SLEN("empty-cells") },
	{ "float", SLEN("float") },
	{ "font-family", SLEN("font-family") },
	{ "font-size", SLEN("font-size") },
	{ "font-style", SLEN("font-style") },
	{ "font-variant", SLEN("font-variant") },
	{ "font-weight", SLEN("font-weight") },
	{ "height", SLEN("height") },
	{ "left", SLEN("left") },
	{ "letter-spacing", SLEN("letter-spacing") },
	{ "line-height", SLEN("line-height") },
	{ "list-style-image", SLEN("list-style-image") },
	{ "list-style-position", SLEN("list-style-position") },
	{ "list-style-type", SLEN("list-style-type") },
	{ "margin-bottom", SLEN("margin-bottom") },
	{ "margin-left", SLEN("margin-left") },
	{ "margin-right", SLEN("margin-right") },
	{ "margin-top", SLEN("margin-top") },
	{ "max-height", SLEN("max-height") },
	{ "max-width", SLEN("max-width") },
	{ "min-height", SLEN("min-height") },
	{ "min-width", SLEN("min-width") },
	{ "orphans", SLEN("orphans") },
	{ "outline-color", SLEN("outline-color") },
	{ "outline-style", SLEN("outline-style") },
	{ "outline-width", SLEN("outline-width") },
	{ "overflow", SLEN("overflow") },
	{ "padding-bottom", SLEN("padding-bottom") },
	{ "padding-left", SLEN("padding-left") },
	{ "padding-right", SLEN("padding-right") },
	{ "padding-top", SLEN("padding-top") },
	{ "page-break-after", SLEN("page-break-after") },
	{ "page-break-before", SLEN("page-break-before") },
	{ "page-break-inside", SLEN("page-break-inside") },
	{ "pause-after", SLEN("pause-after") },
	{ "pause-before", SLEN("pause-before") },
	{ "pitch-range", SLEN("pitch-range") },
	{ "pitch", SLEN("pitch") },
	{ "play-during", SLEN("play-during") },
	{ "position", SLEN("position") },
	{ "quotes", SLEN("quotes") },
	{ "richness", SLEN("richness") },
	{ "right", SLEN("right") },
	{ "speak-header", SLEN("speak-header") },
	{ "speak-numeral", SLEN("speak-numeral") },
	{ "speak-punctuation", SLEN("speak-punctuation") },
	{ "speak", SLEN("speak") },
	{ "speech-rate", SLEN("speech-rate") },
	{ "stress", SLEN("stress") },
	{ "table-layout", SLEN("table-layout") },
	{ "text-align", SLEN("text-align") },
	{ "text-decoration", SLEN("text-decoration") },
	{ "text-indent", SLEN("text-indent") },
	{ "text-transform", SLEN("text-transform") },
	{ "top", SLEN("top") },
	{ "unicode-bidi", SLEN("unicode-bidi") },
	{ "vertical-align", SLEN("vertical-align") },
	{ "visibility", SLEN("visibility") },
	{ "voice-family", SLEN("voice-family") },
	{ "volume", SLEN("volume") },
	{ "white-space", SLEN("white-space") },
	{ "widows", SLEN("widows") },
	{ "width", SLEN("width") },
	{ "word-spacing", SLEN("word-spacing") },
	{ "z-index", SLEN("z-index") },

	{ "inherit", SLEN("inherit") },
	{ "important", SLEN("important") },
	{ "none", SLEN("none") },
	{ "both", SLEN("both") },
	{ "fixed", SLEN("fixed") },
	{ "scroll", SLEN("scroll") },
	{ "transparent", SLEN("transparent") },
	{ "no-repeat", SLEN("no-repeat") },
	{ "repeat-x", SLEN("repeat-x") },
	{ "repeat-y", SLEN("repeat-y") },
	{ "repeat", SLEN("repeat") },
	{ "hidden", SLEN("hidden") },
	{ "dotted", SLEN("dotted") },
	{ "dashed", SLEN("dashed") },
	{ "solid", SLEN("solid") },
	{ "double", SLEN("double") },
	{ "groove", SLEN("groove") },
	{ "ridge", SLEN("ridge") },
	{ "inset", SLEN("inset") },
	{ "outset", SLEN("outset") },
	{ "thin", SLEN("thin") },
	{ "medium", SLEN("medium") },
	{ "thick", SLEN("thick") },
	{ "collapse", SLEN("collapse") },
	{ "separate", SLEN("separate") },
	{ "auto", SLEN("auto") },
	{ "ltr", SLEN("ltr") },
	{ "rtl", SLEN("rtl") },
	{ "inline", SLEN("inline") },
	{ "block", SLEN("block") },
	{ "list-item", SLEN("list-item") },
	{ "run-in", SLEN("run-in") },
	{ "inline-block", SLEN("inline-block") },
	{ "table", SLEN("table") },
	{ "inline-table", SLEN("inline-table") },
	{ "table-row-group", SLEN("table-row-group") },
	{ "table-header-group", SLEN("table-header-group") },
	{ "table-footer-group", SLEN("table-footer-group") },
	{ "table-row", SLEN("table-row") },
	{ "table-column-group", SLEN("table-column-group") },
	{ "table-column", SLEN("table-column") },
	{ "table-cell", SLEN("table-cell") },
	{ "table-caption", SLEN("table-caption") },
	{ "below", SLEN("below") },
	{ "level", SLEN("level") },
	{ "above", SLEN("above") },
	{ "higher", SLEN("higher") },
	{ "lower", SLEN("lower") },
	{ "show", SLEN("show") },
	{ "hide", SLEN("hide") },
	{ "xx-small", SLEN("xx-small") },
	{ "x-small", SLEN("x-small") },
	{ "small", SLEN("small") },
	{ "large", SLEN("large") },
	{ "x-large", SLEN("x-large") },
	{ "xx-large", SLEN("xx-large") },
	{ "larger", SLEN("larger") },
	{ "smaller", SLEN("smaller") },
	{ "normal", SLEN("normal") },
	{ "italic", SLEN("italic") },
	{ "oblique", SLEN("oblique") },
	{ "small-caps", SLEN("small-caps") },
	{ "bold", SLEN("bold") },
	{ "bolder", SLEN("bolder") },
	{ "lighter", SLEN("lighter") },
	{ "inside", SLEN("inside") },
	{ "outside", SLEN("outside") },
	{ "disc", SLEN("disc") },
	{ "circle", SLEN("circle") },
	{ "square", SLEN("square") },
	{ "decimal", SLEN("decimal") },
	{ "decimal-leading-zero", SLEN("decimal-leading-zero") },
	{ "lower-roman", SLEN("lower-roman") },
	{ "upper-roman", SLEN("upper-roman") },
	{ "lower-greek", SLEN("lower-greek") },
	{ "lower-latin", SLEN("lower-latin") },
	{ "upper-latin", SLEN("upper-latin") },
	{ "armenian", SLEN("armenian") },
	{ "georgian", SLEN("georgian") },
	{ "lower-alpha", SLEN("lower-alpha") },
	{ "upper-alpha", SLEN("upper-alpha") },
	{ "invert", SLEN("invert") },
	{ "visible", SLEN("visible") },
	{ "always", SLEN("always") },
	{ "avoid", SLEN("avoid") },
	{ "x-low", SLEN("x-low") },
	{ "low", SLEN("low") },
	{ "high", SLEN("high") },
	{ "x-high", SLEN("x-high") },
	{ "static", SLEN("static") },
	{ "relative", SLEN("relative") },
	{ "absolute", SLEN("absolute") },
	{ "once", SLEN("once") },
	{ "digits", SLEN("digits") },
	{ "continuous", SLEN("continuous") },
	{ "code", SLEN("code") },
	{ "spell-out", SLEN("spell-out") },
	{ "x-slow", SLEN("x-slow") },
	{ "slow", SLEN("slow") },
	{ "fast", SLEN("fast") },
	{ "x-fast", SLEN("x-fast") },
	{ "faster", SLEN("faster") },
	{ "slower", SLEN("slower") },
	{ "center", SLEN("center") },
	{ "justify", SLEN("justify") },
	{ "capitalize", SLEN("capitalize") },
	{ "uppercase", SLEN("uppercase") },
	{ "lowercase", SLEN("lowercase") },
	{ "embed", SLEN("embed") },
	{ "bidi-override", SLEN("bidi-override") },
	{ "baseline", SLEN("baseline") },
	{ "sub", SLEN("sub") },
	{ "super", SLEN("super") },
	{ "text-top", SLEN("text-top") },
	{ "middle", SLEN("middle") },
	{ "text-bottom", SLEN("text-bottom") },
	{ "silent", SLEN("silent") },
	{ "x-soft", SLEN("x-soft") },
	{ "soft", SLEN("soft") },
	{ "loud", SLEN("loud") },
	{ "x-loud", SLEN("x-loud") },
	{ "pre", SLEN("pre") },
	{ "nowrap", SLEN("nowrap") },
	{ "pre-wrap", SLEN("pre-wrap") },
	{ "pre-line", SLEN("pre-line") },
	{ "leftwards", SLEN("leftwards") },
	{ "rightwards", SLEN("rightwards") },
	{ "left-side", SLEN("left-side") },
	{ "far-left", SLEN("far-left") },
	{ "center-left", SLEN("center-left") },
	{ "center-right", SLEN("center-right") },
	{ "far-right", SLEN("far-right") },
	{ "right-side", SLEN("right-side") },
	{ "behind", SLEN("behind") },
	{ "rect", SLEN("rect") },
	{ "open-quote", SLEN("open-quote") },
	{ "close-quote", SLEN("close-quote") },
	{ "no-open-quote", SLEN("no-open-quote") },
	{ "no-close-quote", SLEN("no-close-quote") },
	{ "attr", SLEN("attr") },
	{ "counter", SLEN("counter") },
	{ "counters", SLEN("counters") },
	{ "crosshair", SLEN("crosshair") },
	{ "default", SLEN("default") },
	{ "pointer", SLEN("pointer") },
	{ "move", SLEN("move") },
	{ "e-resize", SLEN("e-resize") },
	{ "ne-resize", SLEN("ne-resize") },
	{ "nw-resize", SLEN("nw-resize") },
	{ "n-resize", SLEN("n-resize") },
	{ "se-resize", SLEN("se-resize") },
	{ "sw-resize", SLEN("sw-resize") },
	{ "s-resize", SLEN("s-resize") },
	{ "w-resize", SLEN("w-resize") },
	{ "text", SLEN("text") },
	{ "wait", SLEN("wait") },
	{ "help", SLEN("help") },
	{ "progress", SLEN("progress") },
	{ "serif", SLEN("serif") },
	{ "sans-serif", SLEN("sans-serif") },
	{ "cursive", SLEN("cursive") },
	{ "fantasy", SLEN("fantasy") },
	{ "monospace", SLEN("monospace") },
	{ "male", SLEN("male") },
	{ "female", SLEN("female") },
	{ "child", SLEN("child") },
	{ "mix", SLEN("mix") },
	{ "underline", SLEN("underline") },
	{ "overline", SLEN("overline") },
	{ "line-through", SLEN("line-through") },
	{ "blink", SLEN("blink") },
	{ "rgb", SLEN("rgb") },

	{ "aliceblue", SLEN("aliceblue") },
	{ "antiquewhite", SLEN("antiquewhite") },
	{ "aqua", SLEN("aqua") },
	{ "aquamarine", SLEN("aquamarine") },
	{ "azure", SLEN("azure") },
	{ "beige", SLEN("beige") },
	{ "bisque", SLEN("bisque") },
	{ "black", SLEN("black") },
	{ "blanchedalmond", SLEN("blanchedalmond") },
	{ "blue", SLEN("blue") },
	{ "blueviolet", SLEN("blueviolet") },
	{ "brown", SLEN("brown") },
	{ "burlywood", SLEN("burlywood") },
	{ "cadetblue", SLEN("cadetblue") },
	{ "chartreuse", SLEN("chartreuse") },
	{ "chocolate", SLEN("chocolate") },
	{ "coral", SLEN("coral") },
	{ "cornflowerblue", SLEN("cornflowerblue") },
	{ "cornsilk", SLEN("cornsilk") },
	{ "crimson", SLEN("crimson") },
	{ "cyan", SLEN("cyan") },
	{ "darkblue", SLEN("darkblue") },
	{ "darkcyan", SLEN("darkcyan") },
	{ "darkgoldenrod", SLEN("darkgoldenrod") },
	{ "darkgray", SLEN("darkgray") },
	{ "darkgreen", SLEN("darkgreen") },
	{ "darkgrey", SLEN("darkgrey") },
	{ "darkkhaki", SLEN("darkkhaki") },
	{ "darkmagenta", SLEN("darkmagenta") },
	{ "darkolivegreen", SLEN("darkolivegreen") },
	{ "darkorange", SLEN("darkorange") },
	{ "darkorchid", SLEN("darkorchid") },
	{ "darkred", SLEN("darkred") },
	{ "darksalmon", SLEN("darksalmon") },
	{ "darkseagreen", SLEN("darkseagreen") },
	{ "darkslateblue", SLEN("darkslateblue") },
	{ "darkslategray", SLEN("darkslategray") },
	{ "darkslategrey", SLEN("darkslategrey") },
	{ "darkturquoise", SLEN("darkturquoise") },
	{ "darkviolet", SLEN("darkviolet") },
	{ "deeppink", SLEN("deeppink") },
	{ "deepskyblue", SLEN("deepskyblue") },
	{ "dimgray", SLEN("dimgray") },
	{ "dimgrey", SLEN("dimgrey") },
	{ "dodgerblue", SLEN("dodgerblue") },
	{ "feldspar", SLEN("feldspar") },
	{ "firebrick", SLEN("firebrick") },
	{ "floralwhite", SLEN("floralwhite") },
	{ "forestgreen", SLEN("forestgreen") },
	{ "fuchsia", SLEN("fuchsia") },
	{ "gainsboro", SLEN("gainsboro") },
	{ "ghostwhite", SLEN("ghostwhite") },
	{ "gold", SLEN("gold") },
	{ "goldenrod", SLEN("goldenrod") },
	{ "gray", SLEN("gray") },
	{ "green", SLEN("green") },
	{ "greenyellow", SLEN("greenyellow") },
	{ "grey", SLEN("grey") },
	{ "honeydew", SLEN("honeydew") },
	{ "hotpink", SLEN("hotpink") },
	{ "indianred", SLEN("indianred") },
	{ "indigo", SLEN("indigo") },
	{ "ivory", SLEN("ivory") },
	{ "khaki", SLEN("khaki") },
	{ "lavender", SLEN("lavender") },
	{ "lavenderblush", SLEN("lavenderblush") },
	{ "lawngreen", SLEN("lawngreen") },
	{ "lemonchiffon", SLEN("lemonchiffon") },
	{ "lightblue", SLEN("lightblue") },
	{ "lightcoral", SLEN("lightcoral") },
	{ "lightcyan", SLEN("lightcyan") },
	{ "lightgoldenrodyellow", SLEN("lightgoldenrodyellow") },
	{ "lightgray", SLEN("lightgray") },
	{ "lightgreen", SLEN("lightgreen") },
	{ "lightgrey", SLEN("lightgrey") },
	{ "lightpink", SLEN("lightpink") },
	{ "lightsalmon", SLEN("lightsalmon") },
	{ "lightseagreen", SLEN("lightseagreen") },
	{ "lightskyblue", SLEN("lightskyblue") },
	{ "lightslateblue", SLEN("lightslateblue") },
	{ "lightslategray", SLEN("lightslategray") },
	{ "lightslategrey", SLEN("lightslategrey") },
	{ "lightsteelblue", SLEN("lightsteelblue") },
	{ "lightyellow", SLEN("lightyellow") },
	{ "lime", SLEN("lime") },
	{ "limegreen", SLEN("limegreen") },
	{ "linen", SLEN("linen") },
	{ "magenta", SLEN("magenta") },
	{ "maroon", SLEN("maroon") },
	{ "mediumaquamarine", SLEN("mediumaquamarine") },
	{ "mediumblue", SLEN("mediumblue") },
	{ "mediumorchid", SLEN("mediumorchid") },
	{ "mediumpurple", SLEN("mediumpurple") },
	{ "mediumseagreen", SLEN("mediumseagreen") },
	{ "mediumslateblue", SLEN("mediumslateblue") },
	{ "mediumspringgreen", SLEN("mediumspringgreen") },
	{ "mediumturquoise", SLEN("mediumturquoise") },
	{ "mediumvioletred", SLEN("mediumvioletred") },
	{ "midnightblue", SLEN("midnightblue") },
	{ "mintcream", SLEN("mintcream") },
	{ "mistyrose", SLEN("mistyrose") },
	{ "moccasin", SLEN("moccasin") },
	{ "navajowhite", SLEN("navajowhite") },
	{ "navy", SLEN("navy") },
	{ "oldlace", SLEN("oldlace") },
	{ "olive", SLEN("olive") },
	{ "olivedrab", SLEN("olivedrab") },
	{ "orange", SLEN("orange") },
	{ "orangered", SLEN("orangered") },
	{ "orchid", SLEN("orchid") },
	{ "palegoldenrod", SLEN("palegoldenrod") },
	{ "palegreen", SLEN("palegreen") },
	{ "paleturquoise", SLEN("paleturquoise") },
	{ "palevioletred", SLEN("palevioletred") },
	{ "papayawhip", SLEN("papayawhip") },
	{ "peachpuff", SLEN("peachpuff") },
	{ "peru", SLEN("peru") },
	{ "pink", SLEN("pink") },
	{ "plum", SLEN("plum") },
	{ "powderblue", SLEN("powderblue") },
	{ "purple", SLEN("purple") },
	{ "red", SLEN("red") },
	{ "rosybrown", SLEN("rosybrown") },
	{ "royalblue", SLEN("royalblue") },
	{ "saddlebrown", SLEN("saddlebrown") },
	{ "salmon", SLEN("salmon") },
	{ "sandybrown", SLEN("sandybrown") },
	{ "seagreen", SLEN("seagreen") },
	{ "seashell", SLEN("seashell") },
	{ "sienna", SLEN("sienna") },
	{ "silver", SLEN("silver") },
	{ "skyblue", SLEN("skyblue") },
	{ "slateblue", SLEN("slateblue") },
	{ "slategray", SLEN("slategray") },
	{ "slategrey", SLEN("slategrey") },
	{ "snow", SLEN("snow") },
	{ "springgreen", SLEN("springgreen") },
	{ "steelblue", SLEN("steelblue") },
	{ "tan", SLEN("tan") },
	{ "teal", SLEN("teal") },
	{ "thistle", SLEN("thistle") },
	{ "tomato", SLEN("tomato") },
	{ "turquoise", SLEN("turquoise") },
	{ "violet", SLEN("violet") },
	{ "violetred", SLEN("violetred") },
	{ "wheat", SLEN("wheat") },
	{ "white", SLEN("white") },
	{ "whitesmoke", SLEN("whitesmoke") },
	{ "yellow", SLEN("yellow") },
	{ "yellowgreen", SLEN("yellowgreen") }
};

#endif

