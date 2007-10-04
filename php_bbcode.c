/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2007 The PHP Group                                |
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

  $Id$ 
*/
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_bbcode.h"
#include "lib/bbcode2.h"
#include "lib/stacklib.h"
#include "lib/bstrlib.h"

typedef struct _bbcode_object {
	zend_object     std;
	long 			rsrc_id;
} bbcode_object;

int le_bbcode;
static function_entry bbcode_functions[] = {
	PHP_FE(bbcode_create, NULL)
	PHP_FE(bbcode_add_element, NULL)
	PHP_FE(bbcode_destroy, NULL)
	PHP_FE(bbcode_parse, NULL)
	{NULL, NULL, NULL}
};

#if Void_0
/* Object Part, working on it */
zend_function_entry bbcode_funcs_object[] = {
	PHP_ME_MAPPING(__construct, bbcode_create, NULL,0)
	PHP_ME_MAPPING(addElement, bbcode_add_element, NULL,0)
	PHP_ME_MAPPING(destroy, bbcode_destroy, NULL,0)
	PHP_ME_MAPPING(parse, bbcode_parse, NULL,0)
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

/* {{{ _php_bbcode_handling_content */
/* Start user functions callback */
int _php_bbcode_handling_content(bstring content, bstring param, void *datas){
	zval *retval = NULL;
	zval ***zargs = NULL;
	zval funcname;
	int i, res;
	char *callable = NULL, *errbuf=NULL;
	funcname = *(zval *)datas;
	TSRMLS_FETCH();

	if (!zend_make_callable(&funcname, &callable TSRMLS_CC)) {
		zend_error(E_WARNING, "[BBCode] (_php_bbcode_handling_content) function `%s' is not a function name",funcname);
		efree(callable);
		zval_dtor(&funcname);
		return 0;
	}

	zargs = (zval ***)safe_emalloc((2), sizeof(zval **), 0);
	zargs[0] = emalloc(sizeof(zval *));
	MAKE_STD_ZVAL(*zargs[0]);
	ZVAL_STRING(*zargs[0], bdata(content), 1);
	zargs[1] = emalloc(sizeof(zval *));
	MAKE_STD_ZVAL(*zargs[1]);
	ZVAL_STRING(*zargs[1], bdata(param), 1);

	res = call_user_function_ex(EG(function_table), NULL, &funcname, &retval, 2, zargs, 1, NULL TSRMLS_CC);

	zval_dtor(&funcname);

	if (res == SUCCESS) {
		if (zargs) {
			for (i = 0; i < 2; i++) {
				zval_ptr_dtor(zargs[i]);
				efree(zargs[i]);
			}
			efree(zargs);
		}
	} else {
		zend_error(E_WARNING, "[BBCode] (_php_bbcode_handling_content) call_user_function_ex failed for function %s()",callable);
	}
	efree(callable);
	if (retval) {
		convert_to_string_ex(&retval);
		if(Z_STRLEN_P(retval)){
			bassigncstr(content,Z_STRVAL_P(retval));
		} else {
			bdelete(content,0,param->slen);
		}
		zval_ptr_dtor(&retval);
	}
	return 0;
}
/* }}} */
/* {{{ _php_bbcode_handling_param */
int _php_bbcode_handling_param(bstring content, bstring param, void *datas){
	zval *retval = NULL;
	zval ***zargs = NULL;
	zval funcname;
	int i, res;
	char *callable = NULL, *errbuf=NULL;
	TSRMLS_FETCH();
	funcname = *(zval *)datas;
	
	if (!zend_make_callable(&funcname, &callable TSRMLS_CC)) {
		zend_error(E_WARNING, "[BBCode] (_php_bbcode_handling_param) function `%s' is not a function name",callable);
		efree(callable);
		zval_dtor(&funcname);
		return 0;
	}

	zargs = (zval ***)safe_emalloc((2), sizeof(zval **), 0);
	zargs[0] = emalloc(sizeof(zval *));
	MAKE_STD_ZVAL(*zargs[0]);
	ZVAL_STRING(*zargs[0], bdata(content), 1);
	zargs[1] = emalloc(sizeof(zval *));
	MAKE_STD_ZVAL(*zargs[1]);
	ZVAL_STRING(*zargs[1], bdata(param), 1);

	res = call_user_function_ex(EG(function_table), NULL, &funcname, &retval, 2, zargs, 1, NULL TSRMLS_CC);

	zval_dtor(&funcname);

	if (res == SUCCESS) {
		if (zargs) {
			for (i = 0; i < 2; i++) {
				zval_ptr_dtor(zargs[i]);
				efree(zargs[i]);
			}
			efree(zargs);
		}
	} else {
		zend_error(E_WARNING, "[BBCode] (_php_bbcode_handling_param) call_user_function_ex failed for function %s()",callable);
	}
	efree(callable);
	if (retval) {
		convert_to_string_ex(&retval);
		if(Z_STRLEN_P(retval)){
			bassigncstr(param,Z_STRVAL_P(retval));
		} else {
			bdelete(param,0,param->slen);
		}
		zval_ptr_dtor(&retval);
	}
	return 0;
}
/* End User function Callback */
/* }}} */
/* {{{ _php_bbcode_add_element */
/* Fill a bbcode_container */
static void _php_bbcode_add_element(bbcode_parser_p parser, char *tag_name, zval *content) {
	zval **e;
	char type, flags=0;
	char empty[]="";
	char all[]="all";
	int (*content_handling_func)(bstring content, bstring param, void *func_data)=NULL;
	int (*param_handling_func)(bstring content, bstring param, void *func_data)=NULL;
    HashTable *ht;
	char *childs=all;
	int childs_len=3;
	char *parents=all;
	int parents_len=3;
	char *open_tag, *close_tag, *default_arg;
	int open_tag_len, close_tag_len, default_arg_len;
	open_tag_len = close_tag_len = default_arg_len = 0;
	zval *content_handling = NULL;
	zval *param_handling = NULL;
	
    if (Z_TYPE_P(content) == IS_ARRAY) {
        ht = HASH_OF(content);
    }
	if ((SUCCESS == zend_hash_find(ht, "flags", sizeof("flags"), (void *) &e))
			&& (Z_TYPE_PP(e) == IS_LONG)) {
			flags = Z_LVAL_PP(e);
	}
	if ((SUCCESS == zend_hash_find(ht, "type", sizeof("type"), (void *) &e))
			&& (Z_TYPE_PP(e) == IS_LONG)) {
		type = Z_LVAL_PP(e);
	} else {
		zend_error(E_WARNING, "[BBCode] (bbcode_add_element) No type specified for tag : [%s]",tag_name);
		return;
	}
	if ((SUCCESS == zend_hash_find(ht, "open_tag", sizeof("open_tag"), (void *) &e))
			&& (Z_TYPE_PP(e) == IS_STRING) && Z_STRLEN_PP(e)) {
		open_tag = Z_STRVAL_PP(e);
		open_tag_len=Z_STRLEN_PP(e);
	} else {
		open_tag=empty;
	}
	if ((SUCCESS == zend_hash_find(ht, "close_tag", sizeof("close_tag"), (void *) &e))
			&& (Z_TYPE_PP(e) == IS_STRING) && Z_STRLEN_PP(e)) {
		close_tag = Z_STRVAL_PP(e);
		close_tag_len=Z_STRLEN_PP(e);
	} else {
		close_tag=empty;
	}
	if ((SUCCESS == zend_hash_find(ht, "default_arg", sizeof("default_arg"), (void *) &e))
			&& (Z_TYPE_PP(e) == IS_STRING) && Z_STRLEN_PP(e)) {
		default_arg = Z_STRVAL_PP(e);
		default_arg_len = Z_STRLEN_PP(e);
	} else {
		default_arg = empty;
	}
	if ((SUCCESS == zend_hash_find(ht, "content_handling", sizeof("content_handling"), (void *) &e))
			&& ((Z_TYPE_PP(e) == IS_STRING && Z_STRLEN_PP(e)) || (Z_TYPE_PP(e)==IS_ARRAY))) {
		ZVAL_ADDREF(*e);
		content_handling = *e;
		content_handling_func= _php_bbcode_handling_content;
	} else {
		content_handling=NULL;
	}
	if ((SUCCESS == zend_hash_find(ht, "param_handling", sizeof("param_handling"), (void *) &e))
			&& ((Z_TYPE_PP(e) == IS_STRING && Z_STRLEN_PP(e)) || (Z_TYPE_PP(e)==IS_ARRAY))) {
		ZVAL_ADDREF(*e);
		param_handling = *e;
		param_handling_func= _php_bbcode_handling_param;
	}
	if ((SUCCESS == zend_hash_find(ht, "childs", sizeof("childs"), (void *) &e))
			&& (Z_TYPE_PP(e) == IS_STRING)) {
		childs = Z_STRVAL_PP(e);
		childs_len = Z_STRLEN_PP(e);
	}
	if ((SUCCESS == zend_hash_find(ht, "parents", sizeof("parents"), (void *) &e))
			&& (Z_TYPE_PP(e) == IS_STRING) && Z_STRLEN_PP(e)) {
		parents = Z_STRVAL_PP(e);
		parents_len = Z_STRLEN_PP(e);
	}
	bbcode_parser_add_ruleset(parser, type, flags,
			tag_name, strlen(tag_name),
			open_tag, open_tag_len,
			close_tag, close_tag_len, default_arg, default_arg_len, parents,
			parents_len, childs, childs_len, 
			param_handling_func,
			content_handling_func,
			(void *)param_handling,
			(void *)content_handling);
}
/* }}} */
/* Destructors */
ZEND_RSRC_DTOR_FUNC(php_bbcode_dtor)
{
    bbcode_parser_p parser = (bbcode_parser_p)rsrc->ptr;

    if (parser) {
    	bbcode_parser_free(parser);
    }
}
/* End destructors */
/* INIT / SHUTDOWN */
PHP_MINIT_FUNCTION(bbcode)
{
    le_bbcode = zend_register_list_destructors_ex(php_bbcode_dtor, NULL, PHP_BBCODE_RES_NAME, module_number);
	REGISTER_LONG_CONSTANT("BBCODE_TYPE_NOARG",					BBCODE_TYPE_NOARG, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_TYPE_SINGLE",				BBCODE_TYPE_SINGLE, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_TYPE_ARG",					BBCODE_TYPE_ARG, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_TYPE_OPTARG",				BBCODE_TYPE_OPTARG, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_TYPE_ROOT",					BBCODE_TYPE_ROOT, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_FLAGS_ARG_PARSING",			BBCODE_FLAGS_ARG_PARSING, CONST_CS|CONST_PERSISTENT);
	REGISTER_LONG_CONSTANT("BBCODE_FLAGS_CDATA_NOT_ALLOWED",	BBCODE_FLAGS_CDATA_NOT_ALLOWED, CONST_CS|CONST_PERSISTENT);
    return SUCCESS;
}
/* END INIT/SHUTDOWN */

#if Void_0
/* {{{
/* Working on it, not finished yet */
/* OBJECT START */
static void _php_bbcode_object_free_storage(void *object TSRMLS_DC){
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
/* END OBJECT */
/* }}} */
#endif
/* {{{ proto ressource bbcode_container bbcode_create([array initial_tags])
   create a new bbcode_container and add elements given in the array to the ressource
   initial_tags is an associative array with tag_name=>tag_definition
   (see bbcode_add_element to view tag_definition format) */
PHP_FUNCTION(bbcode_create)
{
	zval *bbcode_entry=NULL;
	bbcode_parser_p parser = NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "|a", &bbcode_entry) == FAILURE) {
        RETURN_NULL();
    }
    
    /* Container init */
    parser=bbcode_parser_create();
    if (parser==NULL){
		zend_error(E_ERROR, "[BBCode] (bbcode_create) Unable to allocate memory for tag_stack");
    }
    bbcode_parser_set_flags(parser, BBCODE_AUTO_CORRECT|BBCODE_ARG_DOUBLE_QUOTE|BBCODE_ARG_SINGLE_QUOTE);
    /* If array given initialisation */
    if(bbcode_entry!=NULL){
	    int i;
	    HashTable *myht;
	    
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
	            if (i == HASH_KEY_NON_EXISTANT)
	                break;
	
	            if (zend_hash_get_current_data_ex(myht, (void **) &data, &pos) == SUCCESS) {
	                tmp_ht = HASH_OF(*data);
	                if (tmp_ht) {
	                    tmp_ht->nApplyCount++;
	                }
                    if (i == HASH_KEY_IS_STRING) {
						/* Add Element to container */
                    	_php_bbcode_add_element(parser,key,*data);
	                }
	                if (tmp_ht) {
		                    tmp_ht->nApplyCount--;
	                }
	            }
	        }
	    }
    }
    /* Registering Resource & returning */
    ZEND_REGISTER_RESOURCE(return_value, parser, le_bbcode);
}
/* }}} */
/* {{{ proto boolean bbcode_add_element(ressource bbcode_container, string tag_name , array tag_definition)
   add a new tag to the given bbcode_container
   tag_definition is an array with those elements:
   needed:
    * 'type' => Type of the tag (view BBCODE_TYPE_* to view available tags)
   optionnals:
   	* 'flags' =0 => features of the tag (view BBCODE_FLAGS_* to view capacities)
    * 'open_tag' = '' => the replacement open_tag (1)
    * 'close_tag' = '' => the replacement close_tag (1)
    * 'default_argument' = '' => For Optionnal argument tags argument taken if not given (1)
    * 'content_handling' = '' => Function name called as callback for content ( string new_content content_function ( content, param ); )
    * 'param_handling' = '' => Function name called as callback for content ( string new_content content_function ( content, param ); )
    * 'childs' = 'all' => Comma separated list of accepted childs (use !list to make a list of non accepted childs, no space between tag_names)
    * 'parents' = 'all' => Comma separated list of tags of which this tag could be a child
    (1) note that the {CONTENT} string is automatically replaced by the content of the tag and {PARAM} by the parameter
    */
PHP_FUNCTION(bbcode_add_element)
{
	zval *z_bbcode_parser;
	zval *bbcode_entry=NULL;
	bbcode_parser_p parser=NULL;
	char *tag_name;
	int tag_len;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rsa", &z_bbcode_parser, &tag_name, &tag_len, &bbcode_entry) == FAILURE) {
        RETURN_NULL();
    }
	ZEND_FETCH_RESOURCE(parser, bbcode_parser_p, &z_bbcode_parser, -1, PHP_BBCODE_RES_NAME, le_bbcode);
	php_bbcode_add_element(parser, tag_name, bbcode_entry);
	RETURN_TRUE;
}
/* }}} */
/* {{{ proto boolean bbcode_destroy(ressource bbcode_container)
   free memory ressources of the given bbcode_container */
PHP_FUNCTION(bbcode_destroy)
{
	zval *z_bbcode_parser;
	bbcode_parser_p parser=NULL;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &z_bbcode_parser) == FAILURE) {
        RETURN_NULL();
    }
    
	ZEND_FETCH_RESOURCE(parser, bbcode_parser_p, &z_bbcode_parser, -1, PHP_BBCODE_RES_NAME, le_bbcode);
   	bbcode_parser_free(parser);
   	
	RETURN_BOOL(zend_list_delete(Z_LVAL_P(z_bbcode_parser)) == SUCCESS);
}
/* }}} */
/* {{{ proto string bbcode_parse(ressource bbcode_container, string to_parse)
   returns the bbcode parsed value of the to_parse string with definitions given by bbcode_container  */
PHP_FUNCTION(bbcode_parse)
{
	zval *z_bbcode_parser;
	bbcode_parser_p parser;
	bstring to_parse;
	char *string;
	int str_len;
	int ret_size;
	char *ret_string;
	
	if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &z_bbcode_parser, &string, &str_len) == FAILURE) {
        RETURN_NULL();
    }
	ZEND_FETCH_RESOURCE(parser, bbcode_parser_p, &z_bbcode_parser, -1, PHP_BBCODE_RES_NAME, le_bbcode);
	/* converting string for bbcode_parse_string usage */
	ret_string=bbcode_parse(parser, string, str_len, &ret_size);
	
	RETVAL_STRINGL(ret_string, ret_size ,0);
}
/* }}} */
/*** Module Infos ***/
PHP_MINFO_FUNCTION(bbcode)
{
	php_info_print_table_start();
	php_info_print_table_row(2, "BBCode support", "enabled");
	php_info_print_table_row(2, "BBCode Version Support", PHP_BBCODE_VERSION);
	php_info_print_table_row(2, "BBCode Library Version", BBCODE_LIB_VERSION);
	php_info_print_table_row(2, "BBCode Max Stack Size", "Dynamic");
	php_info_print_table_row(2, "BBCode Max Elements", "No Limit");
	php_info_print_table_header(1, "This extension makes use of Bstrlib available at http://bstrlib.sf.net");
	php_info_print_table_end();
}
