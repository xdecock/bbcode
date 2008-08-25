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

#ifndef PHP_BBCODE_H
#define PHP_BBCODE_H

#define PHP_BBCODE_VERSION "1.1.0-dev"
#define PHP_BBCODE_EXTNAME "bbcode"
#define PHP_BBCODE_RES_NAME "BBCode resource"

#define BBCODE_SET_FLAGS_SET	0
#define BBCODE_SET_FLAGS_ADD	1
#define BBCODE_SET_FLAGS_REMOVE 2

extern zend_module_entry bbcode_module_entry;
#define phpext_bbcode_ptr &bbcode_module_entry

#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
