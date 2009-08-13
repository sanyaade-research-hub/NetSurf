/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 Bo Yang <struggleyb.nku@gmail.com>
 */

#ifndef dom_events_text_event_h_
#define dom_events_text_event_h_

#include <stdbool.h>
#include <dom/core/exceptions.h>

struct dom_abstract_view;
struct dom_string;

typedef struct dom_text_event dom_text_event;

dom_exception _dom_text_event_get_data(dom_text_event *evt, 
		struct dom_string **data);
#define dom_text_event_get_data(e, d) _dom_text_event_get_data(\
		(dom_text_event *) (e), (struct dom_string **) (d))

dom_exception _dom_text_event_init(dom_text_event *evt, 
		struct dom_string *type, bool bubble, bool cancelable,
		struct dom_abstract_view *view, struct dom_string *data);
#define dom_text_event_init(e, t, b, c, v, d) _dom_text_event_init( \
		(dom_text_event *) (e), (struct dom_string *) (t), (bool) (b), \
		(bool) (c), (struct dom_abstract_view *) (v),\
		(struct dom_string *) (d))

dom_exception _dom_text_event_init_ns(dom_text_event *evt, 
		struct dom_string *namespace, struct dom_string *type,
		bool bubble, bool cancelable, struct dom_abstract_view *view,
		struct dom_string *data);
#define dom_text_event_init_ns(e, n, t, b, c, v, d) _dom_text_event_init_ns( \
		(dom_text_event *) (e), (struct dom_string *) (n), \
		(struct dom_string *) (t), (bool) (b), (bool) (c), \
		(struct dom_abstract_view *) (v), (struct dom_string *) (d))

#endif
