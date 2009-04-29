/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2007 John-Mark Bell <jmb@netsurf-browser.org>
 */

#ifndef dom_core_namednodemap_h_
#define dom_core_namednodemap_h_

#include <dom/core/exceptions.h>

struct dom_node;
struct dom_string;

typedef struct dom_namednodemap dom_namednodemap;

void dom_namednodemap_ref(struct dom_namednodemap *map);
void dom_namednodemap_unref(struct dom_namednodemap *map);

dom_exception dom_namednodemap_get_length(struct dom_namednodemap *map,
		unsigned long *length);

dom_exception _dom_namednodemap_get_named_item(struct dom_namednodemap *map,
		struct dom_string *name, struct dom_node **node);

#define dom_namednodemap_get_named_item(m, n, r)  \
		_dom_namednodemap_get_named_item((dom_namednodemap *) (m), \
		(dom_string *) (n), (dom_node **) (r))
		

dom_exception _dom_namednodemap_set_named_item(struct dom_namednodemap *map,
		struct dom_node *arg, struct dom_node **node);

#define dom_namednodemap_set_named_item(m, a, n) \
		_dom_namednodemap_set_named_item((dom_namednodemap *) (m), \
		(dom_node *) (a), (dom_node **) (n))


dom_exception _dom_namednodemap_remove_named_item(
		struct dom_namednodemap *map, struct dom_string *name,
		struct dom_node **node);

#define dom_namednodemap_remove_named_item(m, n, r) \
		_dom_namednodemap_remove_named_item((dom_namednodemap *) (m), \
		(dom_string *) (n), (dom_node **) (n))


dom_exception _dom_namednodemap_item(struct dom_namednodemap *map,
		unsigned long index, struct dom_node **node);

#define dom_namednodemap_item(m, i, n) _dom_namednodemap_item( \
		(dom_namednodemap *) (m), (unsigned long) (i), \
		(dom_node **) (n))


dom_exception _dom_namednodemap_get_named_item_ns(
		struct dom_namednodemap *map, struct dom_string *namespace,
		struct dom_string *localname, struct dom_node **node);

#define dom_namednodemap_get_named_item_ns(m, n, l, r) \
		_dom_namednodemap_get_named_item_ns((dom_namednodemap *) (m), \
		(dom_string *) (n), (dom_string *) (l), (dom_node **) (n))


dom_exception _dom_namednodemap_set_named_item_ns(
		struct dom_namednodemap *map, struct dom_node *arg,
		struct dom_node **node);

#define dom_namednodemap_set_named_item_ns(m, a, n) \
		_dom_namednodemap_set_named_item_ns((dom_namednodemap *) (m), \
		(dom_node *) (a), (dom_node **) (n))


dom_exception _dom_namednodemap_remove_named_item_ns(
		struct dom_namednodemap *map, struct dom_string *namespace,
		struct dom_string *localname, struct dom_node **node);

#define dom_namednodemap_remove_named_item_ns(m, n, l, r) \
		_dom_namednodemap_remove_named_item_ns((dom_namednodemap *) (m), \
		(dom_string *) (n), (dom_string *) (l), (dom_node **) (r))

#endif
