/*
 * This file is part of libdom.
 * Licensed under the MIT License,
 *                http://www.opensource.org/licenses/mit-license.php
 * Copyright 2009 Bo Yang <struggleyb.nku.com>
 */

#include <assert.h>

#include "html/html_element.h"

#include "core/node.h"
#include "core/attr.h"
#include "core/document.h"
#include "utils/utils.h"

dom_exception _dom_html_element_initialise(struct dom_document *doc,
		struct dom_html_element *el, struct lwc_string_s *name, 
		struct lwc_string_s *namespace, struct lwc_string_s *prefix)
{
	dom_exception err;

	err = _dom_element_initialise(doc, &el->base, name, namespace, prefix);
	if (err != DOM_NO_ERR)
		return err;
	
	el->form = NULL;
	return err;
}

void _dom_html_element_finalise(struct dom_document *doc,
		struct dom_html_element *ele)
{
	dom_node_unref(ele->form);
	ele->form = NULL;

	_dom_element_finalise(doc, &ele->base);
}

/*------------------------------------------------------------------------*/
/* The protected virtual functions */

/* The virtual destroy function, see src/core/node.c for detail */
void _dom_virtual_html_element_destroy(dom_node_internal *node)
{
	UNUSED(node);
	assert("Should never be here" == NULL);
}

/* The virtual allocation function, see src/core/node.c for detail */
dom_exception _dom_html_element_alloc(struct dom_document *doc,
		struct dom_node_internal *n, struct dom_node_internal **ret)
{
	UNUSED(doc);
	UNUSED(n);
	UNUSED(ret);

	assert("Should never be here" == NULL);

	return DOM_NO_MEM_ERR;
}

/* The virtual copy function, see src/core/node.c for detail */
dom_exception _dom_html_element_copy(struct dom_node_internal *new,
		struct dom_node_internal *old)
{
	return _dom_element_copy(new, old);
}

/*-----------------------------------------------------------------------*/
/* API functions */

dom_exception _dom_html_element_get_id(dom_html_element *element,
                                       struct dom_string **id)
{
        dom_exception ret;
        dom_document *doc;
        dom_string *idstr;
        
        ret = dom_node_get_owner_document(element, &doc);
        if (ret != DOM_NO_ERR)
                return ret;
        
        ret = _dom_document_create_string(doc, (const uint8_t *) "id", 
			SLEN("id"), &idstr);
        if (ret != DOM_NO_ERR)
                return ret;
        
        ret = dom_element_get_attribute(element, idstr, id);
        
        dom_string_unref(idstr);
        
        return ret;
}

dom_exception _dom_html_element_set_id(dom_html_element *element,
                                       struct dom_string *id)
{
        dom_exception ret;
        dom_document *doc;
        dom_string *idstr;
        
        ret = dom_node_get_owner_document(element, &doc);
        if (ret != DOM_NO_ERR)
                return ret;
        
        ret = _dom_document_create_string(doc, (const uint8_t *) "id",
			SLEN("id"), &idstr);
        if (ret != DOM_NO_ERR)
                return ret;
        
        ret = dom_element_set_attribute(element, idstr, id);
        
        dom_string_unref(idstr);
        
        return ret;
}

/*-----------------------------------------------------------------------*/
/* Common functions */

/**
 * Get the a bool property
 *
 * \param ele   The dom_html_element object
 * \param name  The name of the attribute
 * \param len   The length of ::name
 * \param has   The returned status
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception dom_html_element_get_bool_property(dom_html_element *ele,
		const char *name, unsigned long len, bool *has)
{
	dom_document *doc = dom_node_get_owner(ele);
	dom_string *str = NULL;
	dom_attr *a = NULL;
	dom_exception err;

	err = _dom_document_create_string(doc, (const uint8_t *) name,
			len, &str);
	if (err != DOM_NO_ERR)
		goto fail;

	err = dom_element_get_attribute_node(ele, str, &a);
	if (err != DOM_NO_ERR)
		goto cleanup1;

	if (a != NULL) {
		*has = true;
	} else {
		*has = false;
	}

	dom_node_unref(a);

cleanup1:
	dom_string_unref(str);

fail:
	return err;
}

/**
 * Set a bool property
 *
 * \param ele   The dom_html_element object
 * \param name  The name of the attribute
 * \param len   The length of ::name
 * \param has   The status
 * \return DOM_NO_ERR on success, appropriate dom_exception on failure.
 */
dom_exception dom_html_element_set_bool_property(dom_html_element *ele,
		const char *name, unsigned long len, bool has)
{
	dom_document *doc = dom_node_get_owner(ele);
	dom_string *str = NULL;
	dom_attr *a = NULL;
	dom_exception err;

	err = _dom_document_create_string(doc, (const uint8_t *) name,
			len, &str);
	if (err != DOM_NO_ERR)
		goto fail;

	err = dom_element_get_attribute_node(ele, str, &a);
	if (err != DOM_NO_ERR)
		goto cleanup1;
	
	if (a != NULL && has == false) {
		dom_attr *res = NULL;

		err = dom_element_remove_attribute_node(ele, a, &res);
		if (err != DOM_NO_ERR)
			goto cleanup2;

		dom_node_unref(res);
	} else if (a == NULL && has == true) {
		dom_attr *res = NULL;
		lwc_string *lstr = NULL;

		err = _dom_string_intern(str, &lstr);
		if (err != DOM_NO_ERR)
			goto cleanup1;

		err = _dom_attr_create(doc, lstr, NULL, NULL, true, &a);
		if (err != DOM_NO_ERR) {
			lwc_string_unref(lstr);
			goto cleanup1;
		}
		lwc_string_unref(lstr);

		err = dom_element_set_attribute_node(ele, a, &res);
		if (err != DOM_NO_ERR)
			goto cleanup2;

		dom_node_unref(res);
	}

cleanup2:
	dom_node_unref(a);

cleanup1:
	dom_string_unref(str);

fail:
	return err;
}

/**
 * Get the form element if this element is a form control
 *
 * \param ele   The element object
 * \param form  The form object
 */
void _dom_html_element_get_form(dom_html_element *ele,
		struct dom_html_form_element **form)
{
	*form = ele->form;
	dom_node_ref(*form);
}

/**
 * Set the form element if this element is a form control
 *
 * \param ele   The element object
 * \param form  The form object
 */
void _dom_html_element_associate_form(dom_html_element *ele,
		struct dom_html_form_element *form)
{
	/* Unref the form if there is any */
	dom_node_unref(form);
	ele->form = form;
	dom_node_ref(form);
}

