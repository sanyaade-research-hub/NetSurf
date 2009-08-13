/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 Bo Yang <struggleyb.nku@gmail.com>
 */

#ifndef dom_events_custom_event_h_
#define dom_events_custom_event_h_

#include <stdbool.h>
#include <dom/core/exceptions.h>

struct dom_string;

typedef struct dom_custom_event dom_custom_event;

dom_exception _dom_custom_event_get_detail(dom_custom_event *evt, 
		void **detail);
#define dom_custom_event_get_detail(e, d) \
		_dom_custom_event_get_detail((dom_custom_event *) (e),\
		(void **) (d))

dom_exception _dom_custom_event_init_ns(dom_custom_event *evt, 
		struct dom_string *namespace, struct dom_string *type,
		bool bubble, bool cancelable, void *detail);
#define dom_custom_event_init_ns(e, n, t, b, c, d) \
		_dom_custom_event_init_ns((dom_custom_event *) (e), \
		(struct dom_string *) (n), (struct dom_string *) (t), \
		(bool) (b), (bool) (c), (void *) (d))

#endif
