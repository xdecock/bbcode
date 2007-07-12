/*
 * This source file is part bbcode PHP extension.
 * Written and maintained by Xavier De Cock 2006-2007
 * Licensed under the PHP License Terms
 */
#ifndef PHP_BBCODE_H
#define PHP_BBCODE_H 1

#define PHP_BBCODE_VERSION "1.0"
#define PHP_BBCODE_EXTNAME "bbcode"

PHP_MINIT_FUNCTION(bbcode);
PHP_MINFO_FUNCTION(bbcode);
PHP_FUNCTION(bbcode_create);
PHP_FUNCTION(bbcode_add_element);
PHP_FUNCTION(bbcode_destroy);
PHP_FUNCTION(bbcode_parse);

extern zend_module_entry bbcode_module_entry;
#define phpext_bbcode_ptr &bbcode_module_entry;

#define PHP_BBCODE_RES_NAME "BBCode ressource"

#endif
