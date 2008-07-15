/*
 * Tree construction tester.
 */

#define _GNU_SOURCE

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <hubbub/hubbub.h>
#include <hubbub/parser.h>
#include <hubbub/tree.h>

#include "utils/utils.h"

#include "testutils.h"

typedef struct attr_t attr_t;
typedef struct node_t node_t;
typedef struct buf_t buf_t;

struct attr_t {
	hubbub_ns ns;
	char *name;
	char *value;
};

struct node_t {
	enum { DOCTYPE, COMMENT, ELEMENT, CHARACTER } type;

	union {
		struct {
			char *name;
			char *public_id;
			char *system_id;
		} doctype;

		struct {
			hubbub_ns ns;
			char *name;
			attr_t *attrs;
			size_t n_attrs;
		} element;

		char *content;		/**< For comments, characters **/
	} data;

	node_t *next;
	node_t *prev;

	node_t *child;
	node_t *parent;

	uint32_t refcnt;
};

struct buf_t {
	char *buf;
	size_t len;
	size_t pos;
};


#define NUM_NAMESPACES 7
const char const *ns_names[NUM_NAMESPACES] =
		{ NULL, NULL /*html*/, "math", "svg", "xlink", "xml", "xmlns" };


node_t *Document;



static void node_print(buf_t *buf, node_t *node, unsigned depth);


static const uint8_t *pbuffer;

static void buffer_handler(const uint8_t *buffer, size_t len, void *pw);
static int create_comment(void *ctx, const hubbub_string *data, void **result);
static int create_doctype(void *ctx, const hubbub_doctype *doctype,
		void **result);
static int create_element(void *ctx, const hubbub_tag *tag, void **result);
static int create_text(void *ctx, const hubbub_string *data, void **result);
static int ref_node(void *ctx, void *node);
static int unref_node(void *ctx, void *node);
static int append_child(void *ctx, void *parent, void *child, void **result);
static int insert_before(void *ctx, void *parent, void *child, void *ref_child,
		void **result);
static int remove_child(void *ctx, void *parent, void *child, void **result);
static int clone_node(void *ctx, void *node, bool deep, void **result);
static int reparent_children(void *ctx, void *node, void *new_parent);
static int get_parent(void *ctx, void *node, bool element_only, void **result);
static int has_children(void *ctx, void *node, bool *result);
static int form_associate(void *ctx, void *form, void *node);
static int add_attributes(void *ctx, void *node,
		const hubbub_attribute *attributes, uint32_t n_attributes);
static int set_quirks_mode(void *ctx, hubbub_quirks_mode mode);

static void delete_node(node_t *node);
static void delete_attr(attr_t *attr);

static hubbub_tree_handler tree_handler = {
	create_comment,
	create_doctype,
	create_element,
	create_text,
	ref_node,
	unref_node,
	append_child,
	insert_before,
	remove_child,
	clone_node,
	reparent_children,
	get_parent,
	has_children,
	form_associate,
	add_attributes,
	set_quirks_mode,
	NULL
};

static void *myrealloc(void *ptr, size_t len, void *pw)
{
	UNUSED(pw);

	return realloc(ptr, len);
}

static const uint8_t *ptr_from_hubbub_string(const hubbub_string *string)
{
	const uint8_t *data;

	switch (string->type) {
	case HUBBUB_STRING_OFF:
		data = pbuffer + string->data.off;
		break;
	case HUBBUB_STRING_PTR:
		data = string->data.ptr;
		break;
	}

	return data;
}



/*
 * Create, initialise, and return, a parser instance.
 */
static hubbub_parser *setup_parser(void)
{
	hubbub_parser *parser;
	hubbub_parser_optparams params;

	parser = hubbub_parser_create("UTF-8", "UTF-8", myrealloc, NULL);
	assert(parser != NULL);

	params.buffer_handler.handler = buffer_handler;
	params.buffer_handler.pw = NULL;
	assert(hubbub_parser_setopt(parser, HUBBUB_PARSER_BUFFER_HANDLER,
			&params) == HUBBUB_OK);

	params.tree_handler = &tree_handler;
	assert(hubbub_parser_setopt(parser, HUBBUB_PARSER_TREE_HANDLER,
			&params) == HUBBUB_OK);

	params.document_node = (void *)1;
	assert(hubbub_parser_setopt(parser, HUBBUB_PARSER_DOCUMENT_NODE,
			&params) == HUBBUB_OK);

	return parser;
}


void buffer_handler(const uint8_t *buffer, size_t len, void *pw)
{
	UNUSED(len);
	UNUSED(pw);

	pbuffer = buffer;
}


/*** Buffer handling bits ***/
static void buf_clear(buf_t *buf)
{
	if (!buf || !buf->buf) return;

	buf->buf[0] = '\0';
	buf->pos = 0;
}

static void buf_add(buf_t *buf, const char *str)
{
	size_t len = strlen(str) + 1;

	if (!buf) {
		printf("%s", str);
		return;
	}

	if (buf->buf == NULL) {
		buf->len = ((len + 1024) / 1024) * 1024;
		buf->buf = calloc(1, buf->len);
	}

	while (buf->pos + len > buf->len) {
		buf->len *= 2;
		buf->buf = realloc(buf->buf, buf->len);
	}

	strcat(buf->buf, str);
	buf->pos += len;
}



/* States for reading in data from the tree construction file */
enum reading_state {
	ERASE_DATA,
	EXPECT_DATA,
	READING_DATA,
	READING_DATA_AFTER_FIRST,
	READING_ERRORS,
	READING_TREE,
};

int main(int argc, char **argv)
{
	FILE *fp;
	char line[2048];

	bool reprocess = false;
	bool passed = true;

	hubbub_parser *parser;
	enum reading_state state = EXPECT_DATA;

	buf_t expected = { NULL, 0, 0 };
	buf_t got = { NULL, 0, 0 };


	if (argc != 3) {
		printf("Usage: %s <aliases_file> <filename>\n", argv[0]);
		return 1;
	}

	/* Initialise library */
	assert(hubbub_initialise(argv[1], myrealloc, NULL) == HUBBUB_OK);

	fp = fopen(argv[2], "rb");
	if (fp == NULL) {
		printf("Failed opening %s\n", argv[2]);
		return 1;
	}

	/* We rely on lines not being anywhere near 2048 characters... */
	while (reprocess || (passed && fgets(line, sizeof line, fp) == line)) {
		reprocess = false;

		switch (state)
		{
		case ERASE_DATA:
			buf_clear(&got);
			buf_clear(&expected);

			hubbub_parser_destroy(parser);
			while (Document) {
				node_t *victim = Document;
				Document = victim->next;
				delete_node(victim);
			}
			Document = NULL;

			state = EXPECT_DATA;

 		case EXPECT_DATA:
			if (strcmp(line, "#data\n") == 0) {
				parser = setup_parser();
				state = READING_DATA;
			}
			break;

		case READING_DATA:
		case READING_DATA_AFTER_FIRST:
			if (strcmp(line, "#errors\n") == 0) {
				assert(hubbub_parser_completed(parser) == HUBBUB_OK);
				state = READING_ERRORS;
			} else {
				size_t len = strlen(line);

				if (state == READING_DATA_AFTER_FIRST) {
					assert(hubbub_parser_parse_chunk(parser,
						(uint8_t *)"\n",
						1) == HUBBUB_OK);
				} else {
					state = READING_DATA_AFTER_FIRST;
				}

				printf(": %s", line);
				assert(hubbub_parser_parse_chunk(parser, (uint8_t *)line,
						len - 1) == HUBBUB_OK);
			}
			break;


		case READING_ERRORS:
			if (strcmp(line, "#document-fragment\n") == 0) {
				state = ERASE_DATA;
				reprocess = true;
			}

			if (strcmp(line, "#document\n") == 0)
				state = READING_TREE;
			else {
			}
			break;

		case READING_TREE:
			if (strcmp(line, "#data\n") == 0) {
				node_print(&got, Document, 0);

				/* Trim off the last newline */
				expected.buf[strlen(expected.buf) - 1] = '\0';

				passed = !strcmp(got.buf, expected.buf);
				if (!passed) {
					printf("expected:\n");
					printf("%s", expected.buf);
					printf("got:\n");
					printf("%s", got.buf);
				}

				state = ERASE_DATA;
				reprocess = true;
			} else {
				buf_add(&expected, line);
			}
			break;
		}
	}

	if (Document != NULL) {
		hubbub_parser_destroy(parser);
		while (Document) {
			node_t *victim = Document;
			Document = victim->next;
			delete_node(victim);
		}
	}

	printf("%s\n", passed ? "PASS" : "FAIL");

	fclose(fp);

	free(got.buf);
	free(expected.buf);

	assert(hubbub_finalise(myrealloc, NULL) == HUBBUB_OK);

	return 0;
}


/*** Tree construction functions ***/

int create_comment(void *ctx, const hubbub_string *data, void **result)
{
	node_t *node = calloc(1, sizeof *node);

	node->type = COMMENT;
	node->data.content = strndup((char *)ptr_from_hubbub_string(data),
			data->len);
	node->refcnt = 1;

	*result = node;

	return 0;
}

int create_doctype(void *ctx, const hubbub_doctype *doctype, void **result)
{
	node_t *node = calloc(1, sizeof *node);

	node->type = DOCTYPE;
	node->data.doctype.name = strndup(
			(char *)ptr_from_hubbub_string(&doctype->name),
			doctype->name.len);

	if (!doctype->public_missing) {
		node->data.doctype.public_id = strndup(
				(char *)ptr_from_hubbub_string(
					&doctype->public_id),
				doctype->public_id.len);
	}

	if (!doctype->system_missing) {
		node->data.doctype.system_id = strndup(
				(char *)ptr_from_hubbub_string(
					&doctype->system_id),
				doctype->system_id.len);
	}
	node->refcnt = 1;

	*result = node;

	return 0;
}

int create_element(void *ctx, const hubbub_tag *tag, void **result)
{
	node_t *node = calloc(1, sizeof *node);

	assert(tag->ns < NUM_NAMESPACES);

	node->type = ELEMENT;
	node->data.element.ns = tag->ns;
	node->data.element.name = strndup(
			(char *)ptr_from_hubbub_string(&tag->name),
			tag->name.len);
	node->data.element.n_attrs = tag->n_attributes;

	node->data.element.attrs = calloc(node->data.element.n_attrs,
			sizeof *node->data.element.attrs);

	for (size_t i = 0; i < tag->n_attributes; i++) {
		attr_t *attr = &node->data.element.attrs[i];

		assert(tag->attributes[i].ns < NUM_NAMESPACES);

		attr->ns = tag->attributes[i].ns;

		attr->name = strndup((char *)ptr_from_hubbub_string(
						&tag->attributes[i].name),
				tag->attributes[i].name.len);

		attr->value = strndup((char *)ptr_from_hubbub_string(
						&tag->attributes[i].value),
				tag->attributes[i].value.len);
	}
	node->refcnt = 1;

	*result = node;

	return 0;
}

int create_text(void *ctx, const hubbub_string *data, void **result)
{
	node_t *node = calloc(1, sizeof *node);

	node->type = CHARACTER;
	node->data.content = strndup((char *)ptr_from_hubbub_string(data),
			data->len);
	node->refcnt = 1;

	*result = node;

	return 0;
}

int ref_node(void *ctx, void *node)
{
	node_t *n = node;

	if (node != (void *) 1)
		n->refcnt++;

	return 0;
}

int unref_node(void *ctx, void *node)
{
	node_t *n = node;

	if (n != (void *) 1) {
		assert(n->refcnt > 0);

		n->refcnt--;

		printf("Unreferencing node %p (%d)\n", node, n->refcnt);

		if (n->refcnt == 0 && n->parent == NULL) {
			delete_node(n);
		}
	}

	return 0;
}

int append_child(void *ctx, void *parent, void *child, void **result)
{
	node_t *tparent = parent;
	node_t *tchild = child;

	node_t *insert = NULL;

	tchild->next = tchild->prev = NULL;

#ifndef NDEBUG
	printf("appending (%x):\n", (unsigned)tchild);
	node_print(NULL, tchild, 0);
	printf("to:\n");
	if (parent != (void *)1)
		node_print(NULL, tparent, 0);
#endif

	*result = child;

	if (parent == (void *)1) {
		if (Document) {
			insert = Document;
		} else {
			Document = tchild;
		}
	} else {
		if (tparent->child == NULL) {
			tparent->child = tchild;
		} else {
			insert = tparent->child;
		}
	}

	if (insert) {
		while (insert->next != NULL) {
			insert = insert->next;
		}

		if (tchild->type == CHARACTER && insert->type == CHARACTER) {
			insert->data.content = realloc(insert->data.content,
					strlen(insert->data.content) +
					strlen(tchild->data.content) + 1);
			strcat(insert->data.content, tchild->data.content);
			*result = insert;
		} else {
			insert->next = tchild;
			tchild->prev = insert;
		}
	}

	if (*result == child)
		tchild->parent = tparent;

	ref_node(ctx, *result);

	return 0;
}

/* insert 'child' before 'ref_child', under 'parent' */
int insert_before(void *ctx, void *parent, void *child, void *ref_child,
		void **result)
{
	node_t *tparent = parent;
	node_t *tchild = child;
	node_t *tref = ref_child;

#ifndef NDEBUG
	printf("inserting (%x):\n", (unsigned)tchild);
	node_print(NULL, tchild, 0);
	printf("before:\n");
	node_print(NULL, tref, 0);
	printf("under:\n");
	if (parent != (void *)1)
		node_print(NULL, tparent, 0);
#endif

	if (tchild->type == CHARACTER && tref->prev &&
			tref->prev->type == CHARACTER) {
		node_t *insert = tref->prev;

		insert->data.content = realloc(insert->data.content,
				strlen(insert->data.content) +
				strlen(tchild->data.content) + 1);
		strcat(insert->data.content, tchild->data.content);

		*result = insert;
	} else {
		tchild->parent = parent;

		tchild->prev = tref->prev;
		tchild->next = tref;
		tref->prev = tchild;

		if (tchild->prev)
			tchild->prev->next = tchild;
		else
			tparent->child = tchild;

		*result = child;
	}

	ref_node(ctx, *result);

	return 0;
}

int remove_child(void *ctx, void *parent, void *child, void **result)
{
	node_t *tparent = parent;
	node_t *tchild = child;

	assert(tparent->child);
	assert(tchild->parent == tparent);

	printf("Removing child %p\n", child);

	if (tchild->parent->child == tchild) {
		tchild->parent->child = tchild->next;
	}

	if (tchild->prev)
		tchild->prev->next = tchild->next;

	if (tchild->next)
		tchild->next->prev = tchild->prev;

	/* now reset all the child's pointers */
	tchild->next = tchild->prev = tchild->parent = NULL;

	*result = child;

	ref_node(ctx, *result);

	return 0;
}

int clone_node(void *ctx, void *node, bool deep, void **result)
{
	node_t *old_node = node;
	node_t *new_node = calloc(1, sizeof *new_node);

	new_node->type = old_node->type;

	switch (old_node->type) {
	case DOCTYPE:
		new_node->data.doctype.name = 
				strdup(old_node->data.doctype.name);
		if (old_node->data.doctype.public_id)
			new_node->data.doctype.public_id = 
				strdup(old_node->data.doctype.public_id);
		if (old_node->data.doctype.system_id)
			new_node->data.doctype.system_id =
				strdup(old_node->data.doctype.system_id);
		break;
	case COMMENT:
	case CHARACTER:
		new_node->data.content = strdup(old_node->data.content);
		break;
	case ELEMENT:
		new_node->data.element.ns = old_node->data.element.ns;
		new_node->data.element.name = 
				strdup(old_node->data.element.name);
		new_node->data.element.attrs = 
				calloc(old_node->data.element.n_attrs, 
					sizeof *new_node->data.element.attrs);
		for (size_t i = 0; i < old_node->data.element.n_attrs; i++) {
			attr_t *attr = &new_node->data.element.attrs[i];

			attr->ns = old_node->data.element.attrs[i].ns;
			attr->name = 
				strdup(old_node->data.element.attrs[i].name);
			attr->value =
				strdup(old_node->data.element.attrs[i].value);
		}
		new_node->data.element.n_attrs = old_node->data.element.n_attrs;
		break;
	}

	*result = new_node;

	new_node->child = new_node->parent =
			new_node->next = new_node->prev =
			NULL;

	new_node->refcnt = 1;

	if (deep == false)
		return 0;

	node_t *last = NULL;

	for (node_t *child = old_node->child; child != NULL; 
			child = child->next) {
		node_t *n;

		clone_node(ctx, child, true, (void **) &n);

		n->refcnt = 0;

		if (last == NULL) {
			new_node->child = n;
		} else {
			last->next = n;
			n->prev = last;
		}

		n->parent = new_node;
		last = n;
	}

	return 0;
}

/* Take all of the child nodes of "node" and append them to "new_parent" */
int reparent_children(void *ctx, void *node, void *new_parent)
{
	node_t *parent = new_parent;
	node_t *old_parent = node;

	node_t *insert;
	node_t *kids;

	kids = old_parent->child;
	if (!kids) return 0;

	old_parent->child = NULL;

	insert = parent->child;
	if (!insert) {
		parent->child = kids;
	} else {
		while (insert->next != NULL) {
			insert = insert->next;
		}

		insert->next = kids;
		kids->prev = insert;
	}

	while (kids) {
		kids->parent = parent;
		kids = kids->next;
	}

	return 0;
}

int get_parent(void *ctx, void *node, bool element_only, void **result)
{
	*result = ((node_t *)node)->parent;

	if (*result != NULL)
		ref_node(ctx, *result);

	return 0;
}

int has_children(void *ctx, void *node, bool *result)
{
	*result = ((node_t *)node)->child ? true : false;

	return 0;
}

int form_associate(void *ctx, void *form, void *node)
{
	return 0;
}

int add_attributes(void *ctx, void *vnode,
		const hubbub_attribute *attributes, uint32_t n_attributes)
{
	node_t *node = vnode;
	size_t old_elems = node->data.element.n_attrs;

	node->data.element.n_attrs += n_attributes;

	node->data.element.attrs = realloc(node->data.element.attrs,
			node->data.element.n_attrs *
				sizeof *node->data.element.attrs);

	for (size_t i = 0; i < n_attributes; i++) {
		attr_t *attr = &node->data.element.attrs[old_elems + i];

		assert(attributes[i].ns < NUM_NAMESPACES);

		attr->ns = attributes[i].ns;

		attr->name = strndup((char *)ptr_from_hubbub_string(
						&attributes[i].name),
				attributes[i].name.len);

		attr->value = strndup((char *)ptr_from_hubbub_string(
						&attributes[i].value),
				attributes[i].value.len);
	}


	return 0;
}

int set_quirks_mode(void *ctx, hubbub_quirks_mode mode)
{
	return 0;
}



/*** Serialising bits ***/

static int compare_attrs(const void *a, const void *b) {
	const attr_t *first = a;
	const attr_t *second = b;

	return strcmp(first->name, second->name);
}




static void indent(buf_t *buf, unsigned depth)
{
	buf_add(buf, "| ");
	for (unsigned i = 0; i < depth; i++) {
		buf_add(buf, "  ");
	}
}

static void print_ns(buf_t *buf, hubbub_ns ns)
{
	if (ns_names[ns] != NULL) {
		buf_add(buf, ns_names[ns]);
		buf_add(buf, " ");
	}
}

static void node_print(buf_t *buf, node_t *node, unsigned depth)
{
	if (!node) return;

	indent(buf, depth);

	switch (node->type)
	{
	case DOCTYPE:
		buf_add(buf, "<!DOCTYPE ");
		buf_add(buf, node->data.doctype.name);

		if (node->data.doctype.public_id) {
			buf_add(buf, " ");
			buf_add(buf, node->data.doctype.public_id);
		}
		if (node->data.doctype.system_id) {
			buf_add(buf, " ");
			buf_add(buf, node->data.doctype.system_id);
		}

		buf_add(buf, ">\n");
		break;
	case ELEMENT:
		buf_add(buf, "<");
		print_ns(buf, node->data.element.ns);
		buf_add(buf, node->data.element.name);
		buf_add(buf, ">\n");

		qsort(node->data.element.attrs, node->data.element.n_attrs,
				sizeof *node->data.element.attrs,
				compare_attrs);

		for (size_t i = 0; i < node->data.element.n_attrs; i++) {
			indent(buf, depth + 1);
			print_ns(buf, node->data.element.attrs[i].ns);
			buf_add(buf, node->data.element.attrs[i].name);
			buf_add(buf, "=");
			buf_add(buf, "\"");
			buf_add(buf, node->data.element.attrs[i].value);
			buf_add(buf, "\"\n");
		}

		break;
	case CHARACTER:
		buf_add(buf, "\"");
		buf_add(buf, node->data.content);
		buf_add(buf, "\"\n");
		break;
	case COMMENT:
		buf_add(buf, "<!-- ");
		buf_add(buf, node->data.content);
		buf_add(buf, " -->\n");
		break;
	default:
		printf("Unexpected node type %d\n", node->type);
		assert(0);
	}

	if (node->child) {
		node_print(buf, node->child, depth + 1);
	}

	if (node->next) {
		node_print(buf, node->next, depth);
	}
}

static void delete_node(node_t *node)
{
	if (node == NULL)
		return;

	if (node->refcnt != 0) {
		printf("Node %p has non-zero refcount %d\n", 
				(void *) node, node->refcnt);
		assert(0);
	}

	switch (node->type) {
	case DOCTYPE:
		free(node->data.doctype.name);
		free(node->data.doctype.public_id);
		free(node->data.doctype.system_id);
		break;
	case COMMENT:
	case CHARACTER:
		free(node->data.content);
		break;
	case ELEMENT:
		free(node->data.element.name);
		for (size_t i = 0; i < node->data.element.n_attrs; i++)
			delete_attr(&node->data.element.attrs[i]);
		free(node->data.element.attrs);
		break;
	}

	node_t *c, *d;

	for (c = node->child; c != NULL; c = d) {
		d = c->next;

		delete_node(c);
	}

	memset(node, 0xdf, sizeof(node_t));

	free(node);
}

static void delete_attr(attr_t *attr)
{
	if (attr == NULL)
		return;

	free(attr->name);
	free(attr->value);

	memset(attr, 0xdf, sizeof(attr_t));
}

