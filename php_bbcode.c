/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2008 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Xavier De Cock <void@php.net>                                |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_bbcode.h"
#include "lib/bbcode2.h"
#include "lib/bstrlib.h"

static int le_bbcode;

#define PHP_BBCODE_CONTENT_CB 1
#define PHP_BBCODE_PARAM_CB   2

#if (PHP_MAJOR_VERSION == 5 && PHP_MINOR_VERSION > 2) || PHP_MAJOR_VERSION > 5
# define PHP_BBCODE_CALLABLE TSRMLS_CC
#else
# define PHP_BBCODE_CALLABLE
#endif

/* {{{ _php_bbcode_callback_handler
   Common code for content and parameter handlers */
static int _php_bbcode_callback_handler(int cb_type, bstring content, bstring param, zval *func_name)
{
	zval *retval = NULL;
	zval ***zargs = NULL;
	char *callable = NULL;
	int i, res;
	bstring target;
	char *cb_name = "";
	TSRMLS_FETCH();

	switch (cb_type) {
		case PHP_BBCODE_CONTENT_CB:
			target = content;
			break;

		case PHP_BBCODE_PARAM_CB:
			target = param;
			break;
	}

	zargs = (zval ***) emalloc(sizeof(zval **) * 2);
	zargs[0] = emalloc(sizeof(zval *));
	MAKE_STD_ZVAL(*zargs[0]);
	ZVAL_STRINGL(*zargs[0], bdata(content), blength(content), 1);
	zargs[1] = emalloc(sizeof(zval *));
	MAKE_STD_ZVAL(*zargs[1]);
	ZVAL_STRINGL(*zargs[1], bdata(param), blength(param), 1);

	res = call_user_function_ex(EG(function_table), NULL, func_name, &retval, 2, zargs, 1, NULL TSRMLS_CC);
	
	if (res != SUCCESS) {
		if (!zend_is_callable(func_name, 0, &callable PHP_BBCODE_CALLABLE)) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "function `%s' is not callable", callable);
		} else {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "callback function %s() failed", callable);
		}
		efree(callable);
	} else if (&retval != NULL) {
		convert_to_string_ex(&retval);
		if (Z_STRLEN_P(retval)) {
			bassignblk(target, Z_STRVAL_P(retval), Z_STRLEN_P(retval));
		} else {
			bdelete(target, 0, blength(target));
		}
		zval_ptr_dtor(&retval);
	}

	/* Free zargs */
	for (i = 0; i < 2; i++) {
		zval_ptr_dtor(zargs[i]);
		efree(zargs[i]);
	}
	efree(zargs);

	return 0;
}
/* }}} */

/* {{{ _php_bbcode_content_handler */
static int _php_bbcode_content_handler(bstring content, bstring param, void *func_data)
{
	return _php_bbcode_callback_handler(PHP_BBCODE_CONTENT_CB, content, param, func_data);
}
/* }}} */

/* {{{ _php_bbcode_param_handler */
static int _php_bbcode_param_handler(bstring content, bstring param, void *func_data)
{
	return _php_bbcode_callback_handler(PHP_BBCODE_PARAM_CB, content, param, func_data);
}
/* }}} */

/* {{{ _php_bbcode_add_element
   Fills a bbcode_container */
static void _php_bbcode_add_element(bbcode_parser_p parser, char *tag_name, int tag_name_len, zval *content TSRMLS_DC)
{
	zval **e;
	long type;
	char *callback_name;
	char empty[] = "";
	char all[] = "all";
	int (*content_handling_func)(bstring content, bstring param, void *func_data) = NULL;
	int (*param_handling_func)(bstring content, bstring param, void *func_data) = NULL;
	HashTable *ht = NULL;
	long flags = 0;
	char *childs = all;
	int childs_len = 3;
	char *parents = all;
	int parents_len = 3;
	char *open_tag, *close_tag, *default_arg;
	int open_tag_len = 0, close_tag_len = 0, default_arg_len = 0;
	zval *content_handling = NULL;
	zval *param_handling = NULL;

	if (Z_TYPE_P(content) == IS_ARRAY) {
		ht = HASH_OF(content);
	}

	/* flags */
	if ((SUCCESS == zend_hash_find(ht, "flags", sizeof("flags"), (void *) &e)) &&
		(Z_TYPE_PP(e) == IS_LONG)
	) {
		flags = Z_LVAL_PP(e);
	}

	/* type */
	if ((SUCCESS == zend_hash_find(ht, "type", sizeof("type"), (void *) &e)) &&
		(Z_TYPE_PP(e) == IS_LONG)
	) {
		type = Z_LVAL_PP(e);
	} else {
		php_error_docref(NULL TSRMLS_CC, E_WARNING, "No type specified for tag [%s]", tag_name);
		return;
	}

	/* open_tag */
	if ((SUCCESS == zend_hash_find(ht, "open_tag", sizeof("open_tag"), (void *) &e)) &&
		(Z_TYPE_PP(e) == IS_STRING) && Z_STRLEN_PP(e)
	) {
		open_tag = Z_STRVAL_PP(e);
		open_tag_len = Z_STRLEN_PP(e);
	} else {
		open_tag = empty;
	}

	/* close_tag */
	if ((SUCCESS == zend_hash_find(ht, "close_tag", sizeof("close_tag"), (void *) &e)) &&
		(Z_TYPE_PP(e) == IS_STRING) && Z_STRLEN_PP(e)
	) {
		close_tag = Z_STRVAL_PP(e);
		close_tag_len = Z_STRLEN_PP(e);
	} else {
		close_tag = empty;
	}

	/* default_arg */
	if ((SUCCESS == zend_hash_find(ht, "default_arg", sizeof("default_arg"), (void *) &e)) &&
		(Z_TYPE_PP(e) == IS_STRING) && Z_STRLEN_PP(e)
	) {
		default_arg = Z_STRVAL_PP(e);
		default_arg_len = Z_STRLEN_PP(e);
	} else {
		default_arg = empty;
	}

	/* content_handling */
	if ((SUCCESS == zend_hash_find(ht, "content_handling", sizeof("content_handling"), (void *) &e)) && 
		((Z_TYPE_PP(e) == IS_STRING && Z_STRLEN_PP(e)) || (Z_TYPE_PP(e) == IS_ARRAY))
	) {
		SEPARATE_ZVAL(e);
		if (Z_TYPE_PP(e) != IS_STRING && Z_TYPE_PP(e) != IS_ARRAY){
			convert_to_string_ex(e);
		}
		if (!zend_is_callable(*e, 0, &callback_name PHP_BBCODE_CALLABLE)) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "First argument is expected to be a valid callback, '%s' was given", callback_name);
			efree(callback_name);
			return;
		}
		efree(callback_name);
		content_handling = *e;
		content_handling_func = _php_bbcode_content_handler;
	} else {
		content_handling_func = NULL;
		content_handling = NULL;
	}

	/* param_handling */
	if ((SUCCESS == zend_hash_find(ht, "param_handling", sizeof("param_handling"), (void *) &e)) &&
		((Z_TYPE_PP(e) == IS_STRING && Z_STRLEN_PP(e)) || (Z_TYPE_PP(e) == IS_ARRAY))
	) {
		SEPARATE_ZVAL(e);
		if (Z_TYPE_PP(e) != IS_STRING && Z_TYPE_PP(e) != IS_ARRAY){
			convert_to_string_ex(e);
		}
		if (!zend_is_callable(*e, 0, &callback_name PHP_BBCODE_CALLABLE)) {
			php_error_docref(NULL TSRMLS_CC, E_WARNING, "First argument is expected to be a valid callback, '%s' was given", callback_name);
			efree(callback_name);
			return;
		}
		efree(callback_name);
		param_handling = *e;
		param_handling_func = _php_bbcode_param_handler;
	} else {
		param_handling_func = NULL;
		param_handling = NULL;
	}

	/* childs */
	if ((SUCCESS == zend_hash_find(ht, "childs", sizeof("childs"), (void *) &e)) &&
		(Z_TYPE_PP(e) == IS_STRING)
	) {
		childs = Z_STRVAL_PP(e);
		childs_len = Z_STRLEN_PP(e);
	}

	/* parents */
	if ((SUCCESS == zend_hash_find(ht, "parents", sizeof("parents"), (void *) &e)) &&
		(Z_TYPE_PP(e) == IS_STRING) && Z_STRLEN_PP(e)
	) {
		parents = Z_STRVAL_PP(e);
		parents_len = Z_STRLEN_PP(e);
	}

	bbcode_parser_add_ruleset(parser, type, flags,
		tag_name, tag_name_len,
		open_tag, open_tag_len,
		close_tag, close_tag_len,
		default_arg, default_arg_len,
		parents, parents_len,
		childs, childs_len, 
		param_handling_func,
		content_handling_func,
		(void *)param_handling,
		(void *)content_handling
	);
}
/* }}} */

/* {{{ Resource destructor */
static ZEND_RSRC_DTOR_FUNC(php_bbcode_dtor)
{
	bbcode_parser_p parser = (bbcode_parser_p)rsrc->ptr;
	if (parser) {
		bbcode_parser_free(parser);
	}
}
/* }}} */

/* {{{ Module INIT / SHUTDOWN */
static PHP_MINIT_FUNCTION(bbcode)
{
	le_bbcode = zend_register_list_destructors_ex(php_bbcode_dtor, NULL, PHP_BBCODE_RES_NAME, module_number);

	/* BBCODE Types */
	REGISTER_LONG_CONSTANT("BBCODE_TYPE_NOARG",					BBCODE_TYPE_NOARG, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_TYPE_SINGLE",				BBCODE_TYPE_SINGLE, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_TYPE_ARG",					BBCODE_TYPE_ARG, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_TYPE_OPTARG",				BBCODE_TYPE_OPTARG, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_TYPE_ROOT",					BBCODE_TYPE_ROOT, CONST_CS|CONST_PERSISTENT);

	/* BBCODE Flags */
	REGISTER_LONG_CONSTANT("BBCODE_FLAGS_ARG_PARSING",			BBCODE_FLAGS_ARG_PARSING, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_FLAGS_CDATA_NOT_ALLOWED",	BBCODE_FLAGS_CDATA_NOT_ALLOWED, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_FLAGS_SMILEYS_ON",			BBCODE_FLAGS_SMILEYS_ON, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_FLAGS_SMILEYS_OFF",			BBCODE_FLAGS_SMILEYS_OFF, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_FLAGS_ONE_OPEN_PER_LEVEL",	BBCODE_FLAGS_ONE_OPEN_PER_LEVEL, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_FLAGS_REMOVE_IF_EMPTY",		BBCODE_FLAGS_REMOVE_IF_EMPTY, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_FLAGS_DENY_REOPEN_CHILD",	BBCODE_FLAGS_DENY_REOPEN_CHILD, CONST_CS|CONST_PERSISTENT);

	/* Parser Flags */

	/* Quotes styles */
	REGISTER_LONG_CONSTANT("BBCODE_ARG_DOUBLE_QUOTE",			BBCODE_ARG_DOUBLE_QUOTE, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_ARG_SINGLE_QUOTE",			BBCODE_ARG_SINGLE_QUOTE, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_ARG_HTML_QUOTE",				BBCODE_ARG_HTML_QUOTE, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_ARG_QUOTE_ESCAPING",			BBCODE_ARG_QUOTE_ESCAPING, CONST_CS|CONST_PERSISTENT);

	/* Parsing Options */
	REGISTER_LONG_CONSTANT("BBCODE_AUTO_CORRECT",				BBCODE_AUTO_CORRECT, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_CORRECT_REOPEN_TAGS",		BBCODE_CORRECT_REOPEN_TAGS, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_DISABLE_TREE_BUILD",			BBCODE_DISABLE_TREE_BUILD, CONST_CS|CONST_PERSISTENT);

	/* Smiley Options */
	REGISTER_LONG_CONSTANT("BBCODE_DEFAULT_SMILEYS_ON",			BBCODE_DEFAULT_SMILEYS_ON, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_DEFAULT_SMILEYS_OFF",		BBCODE_DEFAULT_SMILEYS_OFF, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_FORCE_SMILEYS_OFF",			BBCODE_FORCE_SMILEYS_OFF, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_SMILEYS_CASE_INSENSITIVE",	BBCODE_SMILEYS_CASE_INSENSITIVE, CONST_CS|CONST_PERSISTENT);

	/* FLAG SET / ADD / REMOVE */
	REGISTER_LONG_CONSTANT("BBCODE_SET_FLAGS_SET",				BBCODE_SET_FLAGS_SET, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_SET_FLAGS_ADD",				BBCODE_SET_FLAGS_ADD, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_SET_FLAGS_REMOVE",			BBCODE_SET_FLAGS_REMOVE, CONST_CS|CONST_PERSISTENT);

	return SUCCESS;
}
/* }}} */

#if Void_0
typedef struct _bbcode_object {
	zend_object	std;
	int			rsrc_id;
} bbcode_object;

/* {{{ TODO: Not finished yet! */
static void _php_bbcode_object_free_storage(void *object TSRMLS_DC)
{
	bbcode_object *intern = (bbcode_object *)object;
	zval *pointer;
	zval *return_value;

	zend_object_std_dtor(&intern->std TSRMLS_CC);

	bbcode_container *container;

	if (intern->rsrc_id) {
			ALLOC_INIT_ZVAL(pointer);
			ALLOC_INIT_ZVAL(return_value);
			Z_TYPE_P(return_value) = IS_LONG;
			Z_TYPE_P(pointer) = IS_LONG;
			Z_LVAL_P(pointer) = intern->rsrc_id;
			ZEND_FETCH_RESOURCE(container, bbcode_container*, &pointer, -1, PHP_BBCODE_RES_NAME, le_bbcode);
			bbcode_destroy_tag_stack(container);
			efree(return_value);
			zval_ptr_dtor(&pointer);
			zend_list_delete(intern->rsrc_id);
	}
	efree(object);
}
static void _php_bbcode_object_new(zend_class_entry *class_type, zend_object_handlers *handlers, zend_object_value *retval TSRMLS_DC)
{
	bbcode_object *intern;
	zval *tmp;

	intern = emalloc(sizeof(bbcode_object));
	memset(intern, 0, sizeof(bbcode_object));

	zend_object_std_init(&intern->std, class_type TSRMLS_CC);
	zend_hash_copy(intern->std.properties, &class_type->default_properties, (copy_ctor_func_t) zval_add_ref, (void *) &tmp, sizeof(zval *));

	retval->handle = zend_objects_store_put(intern,
									(zend_objects_store_dtor_t)zend_objects_destroy_object,
									(zend_objects_free_object_storage_t) _php_bbcode_object_free_storage,
									NULL TSRMLS_CC);
	retval->handlers = handlers;
}
/* }}} */
#endif

/* {{{ proto resource bbcode_container bbcode_create([array initial_tags])
   Creates a new bbcode_container and adds elements given in the array to the resource. */
static PHP_FUNCTION(bbcode_create)
{
	zval *bbcode_entry = NULL;
	bbcode_parser_p parser = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|a", &bbcode_entry) == FAILURE) {
		RETURN_NULL();
	}

	/* Container init */
	parser = bbcode_parser_create();
	if (parser == NULL){
		php_error_docref(NULL TSRMLS_CC, E_ERROR, "Unable to allocate memory for tag_stack");
	}
	bbcode_parser_set_flags(parser, BBCODE_AUTO_CORRECT|BBCODE_ARG_DOUBLE_QUOTE|BBCODE_ARG_SINGLE_QUOTE|BBCODE_ARG_HTML_QUOTE|BBCODE_DEFAULT_SMILEYS_ON);

	/* If array given initialisation */
	if (bbcode_entry != NULL) {
		int i;
		HashTable *myht = NULL;

		if (Z_TYPE_P(bbcode_entry) == IS_ARRAY) {
			myht = HASH_OF(bbcode_entry);
		}

		i = myht ? zend_hash_num_elements(myht) : 0;
		if (i > 0) {
			char *key;
			zval **data;
			ulong index;
			uint key_len;
			HashPosition pos;
			HashTable *tmp_ht;

			/* Array walking */
			zend_hash_internal_pointer_reset_ex(myht, &pos);
			for (;; zend_hash_move_forward_ex(myht, &pos)) {
				i = zend_hash_get_current_key_ex(myht, &key, &key_len, &index, 0, &pos);
				if (i == HASH_KEY_NON_EXISTANT) {
					break;
				}

				if (zend_hash_get_current_data_ex(myht, (void **) &data, &pos) == SUCCESS) {
					tmp_ht = HASH_OF(*data);
					if (tmp_ht) {
						tmp_ht->nApplyCount++;
					}
					if (i == HASH_KEY_IS_STRING) {
						/* Add Element to container */
						_php_bbcode_add_element(parser, key, key_len - 1, *data TSRMLS_CC);
					}
					if (tmp_ht) {
							tmp_ht->nApplyCount--;
					}
				}
			}
		}
	}

	/* Register resource and return it */
	ZEND_REGISTER_RESOURCE(return_value, parser, le_bbcode);
}
/* }}} */

/* {{{ proto boolean bbcode_add_element(resource bbcode_container, string tag_name, array tag_definition)
   Adds a new tag to the given bbcode_container. */
static PHP_FUNCTION(bbcode_add_element)
{
	zval *z_bbcode_parser;
	zval *bbcode_entry;
	bbcode_parser_p parser = NULL;
	char *tag_name;
	int tag_name_len;

	/* tag_definition is an array with these elements:

	- required:
		o 'type' => Type of the tag (view BBCODE_TYPE_* to view available tags)

	- optional:
		o 'flags' =0 => features of the tag (view BBCODE_FLAGS_* to view capacities)
		o 'open_tag' = '' => the replacement open_tag (1)
		o 'close_tag' = '' => the replacement close_tag (1)
		o 'default_argument' = '' => For Optionnal argument tags argument taken if not given [1]
		o 'content_handling' = '' => Function name called as callback for content ( string new_content content_function ( content, param ); )
		o 'param_handling' = '' => Function name called as callback for content ( string new_content content_function ( content, param ); )
		o 'childs' = 'all' => Comma separated list of accepted childs (use !list to make a list of non accepted childs, no space between tag_names)
		o 'parents' = 'all' => Comma separated list of tags of which this tag could be a child

		[1] note that the {CONTENT} string is automatically replaced by the content of the tag and {PARAM} by the parameter
	*/

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rsa", &z_bbcode_parser, &tag_name, &tag_name_len, &bbcode_entry) == FAILURE) {
		RETURN_NULL();
	}

	ZEND_FETCH_RESOURCE(parser, bbcode_parser_p, &z_bbcode_parser, -1, PHP_BBCODE_RES_NAME, le_bbcode);
	_php_bbcode_add_element(parser, tag_name, tag_name_len, bbcode_entry TSRMLS_CC);
	RETURN_TRUE;
}
/* }}} */

/* {{{ proto boolean bbcode_destroy(resource bbcode_container)
   Frees memory resources of the given bbcode_container. */
static PHP_FUNCTION(bbcode_destroy)
{
	zval *z_bbcode_parser;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_bbcode_parser) == FAILURE) {
		RETURN_NULL();
	}

	RETURN_BOOL(zend_list_delete(Z_LVAL_P(z_bbcode_parser)) == SUCCESS);
}
/* }}} */

/* {{{ proto string bbcode_parse(resource bbcode_container, string to_parse)
   Returns the bbcode parsed value of the to_parse string with definitions given by bbcode_container. */
static PHP_FUNCTION(bbcode_parse)
{
	zval *z_bbcode_parser;
	bbcode_parser_p parser;
	char *string;
	int str_len;
	char *ret_string;
	int ret_size;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &z_bbcode_parser, &string, &str_len) == FAILURE) {
		RETURN_NULL();
	}

	ZEND_FETCH_RESOURCE(parser, bbcode_parser_p, &z_bbcode_parser, -1, PHP_BBCODE_RES_NAME, le_bbcode);

	/* convert string for bbcode_parse_string usage */
	ret_string = bbcode_parse(parser, string, str_len, &ret_size);

	RETVAL_STRINGL(ret_string, ret_size , 1);
	free(ret_string);
}
/* }}} */

/* {{{ proto boolean bbcode_add_smiley(resource bbcode_container, string find, string replace)
   Adds a smiley to find and replace ruleset. */
static PHP_FUNCTION(bbcode_add_smiley)
{
	zval *z_bbcode_parser;
	char *search, *replace;
	int s_len, r_len;
	bbcode_parser_p parser = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rss", &z_bbcode_parser, &search, &s_len, &replace, &r_len) == FAILURE) {
		RETURN_NULL();
	}

	ZEND_FETCH_RESOURCE(parser, bbcode_parser_p, &z_bbcode_parser, -1, PHP_BBCODE_RES_NAME, le_bbcode);

	bbcode_parser_add_smiley(parser, search, s_len, replace, r_len);
	RETURN_BOOL(SUCCESS);
}
/* }}} */

/* {{{ proto boolean bbcode_set_flags(resource bbcode_container, long flag, long mode)
   Set flags on parser using BBCODE_SET_FLAGS_* constants. */
static PHP_FUNCTION(bbcode_set_flags)
{
	zval *z_bbcode_parser;
	long new_flags;
	long mode = 0;
	long flags;
	bbcode_parser_p parser=NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rl|l", &z_bbcode_parser, &new_flags, &mode) == FAILURE) {
		RETURN_NULL();
	}

	ZEND_FETCH_RESOURCE(parser, bbcode_parser_p, &z_bbcode_parser, -1, PHP_BBCODE_RES_NAME, le_bbcode);

	flags = bbcode_parser_get_flags(parser);

	switch (mode) {
		case BBCODE_SET_FLAGS_ADD:
			bbcode_parser_set_flags(parser, flags | new_flags);
			break;

		case BBCODE_SET_FLAGS_REMOVE:
			bbcode_parser_set_flags(parser, flags & (~new_flags));
			break;

		default:
		case BBCODE_SET_FLAGS_SET:
			bbcode_parser_set_flags(parser, new_flags);
			break;
	}

	RETURN_BOOL(SUCCESS);
}
/* }}} */

/* {{{ proto boolean bbcode_set_arg_parser(resource bbcode_container, resource bbcode_child)
   Defines a bbcode_argument_parser. */
static PHP_FUNCTION(bbcode_set_arg_parser)
{
	zval *z_bbcode_parser;
	zval *z_bbcode_parser_child;
	bbcode_parser_p parser = NULL;
	bbcode_parser_p arg_parser = NULL;

	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rr", &z_bbcode_parser, &z_bbcode_parser_child) == FAILURE) {
		RETURN_NULL();
	}

	ZEND_FETCH_RESOURCE(parser, bbcode_parser_p, &z_bbcode_parser, -1, PHP_BBCODE_RES_NAME, le_bbcode);
	ZEND_FETCH_RESOURCE(arg_parser, bbcode_parser_p, &z_bbcode_parser_child, -1, PHP_BBCODE_RES_NAME, le_bbcode);

	bbcode_parser_set_arg_parser(parser, arg_parser);

	RETURN_BOOL(SUCCESS);
}
/* }}} */

/* {{{ Module Info */
static PHP_MINFO_FUNCTION(bbcode)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "BBCode support", "enabled");
	php_info_print_table_row(2, "BBCode Extension Version", PHP_BBCODE_VERSION);
	php_info_print_table_row(2, "BBCode Library Version", BBCODE_LIB_VERSION);
	php_info_print_table_row(2, "BBCode Max Stack Size", "Dynamic");
	php_info_print_table_row(2, "BBCode Max Elements", "No Limit");
	php_info_print_table_end();
	php_info_print_box_start(0);
	PUTS("This extension makes use of Bstrlib available at http://bstrlib.sf.net");
	php_info_print_box_end();
}
/* }}} */

static function_entry bbcode_functions[] = { /* {{{ */
	PHP_FE(bbcode_create,         NULL)
	PHP_FE(bbcode_add_element,    NULL)
	PHP_FE(bbcode_destroy,        NULL)
	PHP_FE(bbcode_parse,          NULL)
	PHP_FE(bbcode_add_smiley,     NULL)
	PHP_FE(bbcode_set_flags,      NULL)
	PHP_FE(bbcode_set_arg_parser, NULL)
	{NULL, NULL, NULL}
};
/* }}} */

#if Void_0
/* Object Part, working on it */
zend_function_entry bbcode_funcs_object[] = {
	PHP_ME_MAPPING(__construct, bbcode_create,      NULL, 0)
	PHP_ME_MAPPING(addElement,  bbcode_add_element, NULL, 0)
	PHP_ME_MAPPING(destroy,     bbcode_destroy,     NULL, 0)
	PHP_ME_MAPPING(parse,       bbcode_parse,       NULL, 0)
	{NULL, NULL, NULL}
};
#endif

zend_module_entry bbcode_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	PHP_BBCODE_EXTNAME,
	bbcode_functions,
	PHP_MINIT(bbcode),
	NULL,
	NULL,
	NULL,
	PHP_MINFO(bbcode),
#if ZEND_MODULE_API_NO >= 20010901
	PHP_BBCODE_VERSION,
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_BBCODE
ZEND_GET_MODULE(bbcode)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
