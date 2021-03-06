/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 Bo Yang <struggleyb.nku.com>
 */

#include <assert.h>
#include <stdlib.h>

#include "html/html_form_element.h"

#include "html/html_collection.h"
#include "html/html_document.h"

#include "core/node.h"
#include "utils/utils.h"

static struct dom_element_protected_vtable _protect_vtable = {
	{
		DOM_NODE_PROTECT_VTABLE_HTML_FORM_ELEMENT
	},
	DOM_HTML_FORM_ELEMENT_PROTECT_VTABLE
};

static bool _dom_is_form_control(struct dom_node_internal *node);

/**
 * Create a dom_html_form_element object
 *
 * \param doc  The document object
 * \param ele  The returned element object
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception _dom_html_form_element_create(struct dom_html_document *doc,
		struct dom_html_form_element **ele)
{
	struct dom_node_internal *node;

	*ele = malloc(sizeof(dom_html_form_element));
	if (*ele == NULL)
		return DOM_NO_MEM_ERR;
	
	/* Set up vtables */
	node = (struct dom_node_internal *) *ele;
	node->base.vtable = &_dom_element_vtable;
	node->vtable = &_protect_vtable;

	return _dom_html_form_element_initialise(doc, *ele);
}

/**
 * Initialise a dom_html_form_element object
 *
 * \param doc  The document object
 * \param ele  The dom_html_form_element object
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception _dom_html_form_element_initialise(struct dom_html_document *doc,
		struct dom_html_form_element *ele)
{
	dom_string *name = NULL;
	dom_exception err;

	err = dom_string_create((const uint8_t *) "FORM", SLEN("FORM"), &name);
	if (err != DOM_NO_ERR)
		return err;
	
	err = _dom_html_element_initialise(doc, &ele->base, name, NULL, NULL);
	dom_string_unref(name);

	ele->col = NULL;

	return err;
}

/**
 * Finalise a dom_html_form_element object
 *
 * \param ele  The dom_html_form_element object
 */
void _dom_html_form_element_finalise(struct dom_html_form_element *ele)
{
	_dom_html_element_finalise(&ele->base);
}

/**
 * Destroy a dom_html_form_element object
 *
 * \param ele  The dom_html_form_element object
 */
void _dom_html_form_element_destroy(struct dom_html_form_element *ele)
{
	_dom_html_form_element_finalise(ele);
	free(ele);
}

/*------------------------------------------------------------------------*/
/* The protected virtual functions */

/* The virtual function used to parse attribute value, see src/core/element.c
 * for detail */
dom_exception _dom_html_form_element_parse_attribute(dom_element *ele,
		dom_string *name, dom_string *value,
		dom_string **parsed)
{
	UNUSED(ele);
	UNUSED(name);

	dom_string_ref(value);
	*parsed = value;

	return DOM_NO_ERR;
}

/* The virtual destroy function, see src/core/node.c for detail */
void _dom_virtual_html_form_element_destroy(dom_node_internal *node)
{
	_dom_html_form_element_destroy((struct dom_html_form_element *) node);
}

/* The virtual copy function, see src/core/node.c for detail */
dom_exception _dom_html_form_element_copy(dom_node_internal *old,
		dom_node_internal **copy)
{
	return _dom_html_element_copy(old, copy);
}

/*-----------------------------------------------------------------------*/
/* Public APIs */

/**
 * Get the form controls under this form element
 *
 * \param ele  The form object
 * \param col  The collection of form controls
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception dom_html_form_element_get_elements(dom_html_form_element *ele,
		struct dom_html_collection **col)
{
	dom_exception err;

	if (ele->col == NULL) {
		dom_html_document *doc = (dom_html_document *) dom_node_get_owner(ele);
		assert(doc != NULL);
		err = _dom_html_collection_create(doc,
				(dom_node_internal *) ele,
				_dom_is_form_control, col);
		if (err != DOM_NO_ERR)
			return err;

		ele->col = *col;
	}

	*col = ele->col;
	dom_html_collection_ref(*col);

	return DOM_NO_ERR;
}

/**
 * Get the number of form controls under this form element
 *
 * \param ele  The form object
 * \param len  The number of controls
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception dom_html_form_element_get_length(dom_html_form_element *ele,
		unsigned long *len)
{
	dom_exception err;

	if (ele->col == NULL) {
		dom_html_document *doc = (dom_html_document *) dom_node_get_owner(ele);
		assert(doc != NULL);
		err = _dom_html_collection_create(doc,
				(dom_node_internal *) ele,
				_dom_is_form_control, &ele->col);
		if (err != DOM_NO_ERR)
			return err;
	}

	return dom_html_collection_get_length(ele->col, len);
}

/**
 * Submit this form
 *
 * \param ele  The form object
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception dom_html_form_element_submit(dom_html_form_element *ele)
{
	struct dom_document *doc = dom_node_get_owner(ele);
	bool success = false;
	assert(doc != NULL);

	/* Dispatch an event and let the default action handler to deal with
	 * the submit action, and a 'submit' event is bubbling and cancelable
	 */
	return _dom_dispatch_generic_event(doc, (dom_event_target *) ele,
			(const uint8_t *) "submit", SLEN("submit"), true,
			true, &success);
}

/**
 * Reset this form
 *
 * \param ele  The form object
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception dom_html_form_element_reset(dom_html_form_element *ele)
{
	struct dom_document *doc = dom_node_get_owner(ele);
	bool success = false;
	assert(doc != NULL);

	/* Dispatch an event and let the default action handler to deal with
	 * the reset action, and a 'reset' event is bubbling and cancelable
	 */
	return _dom_dispatch_generic_event(doc, (dom_event_target *) ele,
			(const uint8_t *) "reset", SLEN("reset"), true,
			true, &success);
}

/*-----------------------------------------------------------------------*/
/* Internal functions */

/* Callback function to test whether certain node is a form control, see 
 * src/html/html_collection.h for detail. */
static bool _dom_is_form_control(struct dom_node_internal *node)
{
	UNUSED(node);

	assert(node->type == DOM_ELEMENT_NODE);

	/** \todo: implement */
	return false;
}

