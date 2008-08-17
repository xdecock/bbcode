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
#ifndef PHP_BBCODE_H
#define PHP_BBCODE_H 1

#define PHP_BBCODE_VERSION "1.0.1"
#define PHP_BBCODE_EXTNAME "bbcode"

#define BBCODE_SET_FLAGS_SET	0
#define BBCODE_SET_FLAGS_ADD	1
#define BBCODE_SET_FLAGS_REMOVE 2

PHP_MINIT_FUNCTION(bbcode);
PHP_MINFO_FUNCTION(bbcode);
PHP_FUNCTION(bbcode_create);
PHP_FUNCTION(bbcode_add_element);
PHP_FUNCTION(bbcode_destroy);
PHP_FUNCTION(bbcode_parse);
PHP_FUNCTION(bbcode_add_smiley);
PHP_FUNCTION(bbcode_set_flags);
PHP_FUNCTION(bbcode_set_arg_parser);

extern zend_module_entry bbcode_module_entry;
#define phpext_bbcode_ptr &bbcode_module_entry

#define PHP_BBCODE_RES_NAME "BBCode ressource"

#endif
