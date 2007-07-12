/*
 * This source file is part of the bbcode library.
 * Written and maintained by Xavier De Cock 2006-2007
 * Licensed under the BSD License Terms
 * Refer to the accompanying documentation for details on usage and license.
 */
#ifndef BBCODE_H_
#define BBCODE_H_
#include "bstrlib.h"


#define BBCODE_TYPE_NOARG 1
#define BBCODE_TYPE_SINGLE 2
#define BBCODE_TYPE_ARG 3
#define BBCODE_TYPE_OPTARG 4
#define BBCODE_TYPE_ROOT 10
#define BUFFER_BLOCK_SIZE 8192
#define BBCODE_MAX_CODES 64

#define BBCODE_FLAGS_ARG_PARSING 1
#define BBCODE_FLAGS_CDATA_NOT_ALLOWED 2
#define STACK_MAX_SIZE 512
#include "bstrlib.h"
typedef struct _bbcode_stackLine {
    int tagId;
    bstring argument;
}bbcode_stackLine;
typedef struct _bbcode_stack {
	int size;
	bbcode_stackLine stack[STACK_MAX_SIZE];
} bbcode_stack;
typedef struct _bbcode {
    char type;
    char flags;
    
    bstring tag;
    bstring open_tag;
    bstring close_tag;
    bstring default_arg;
    bstring childs;
    bstring parents;
    bstring content_handling_u;
    bstring param_handling_u;
    int *child_list;
    int child_list_size;
    int *parent_list;
    int parent_list_size;
    
    int (*content_handling_func)	(bstring content, bstring param, bstring user_func);
    int (*param_handling_func)	(bstring content, bstring param, bstring user_func);
} bbcode;
typedef struct _bbcode_container{
	int size;
	bbcode array[BBCODE_MAX_CODES+1];
	char is_ready;
	char has_root;
} bbcode_container;

extern int bbcode_add_element(char *tag, char type, char flags, char *open_tag, char *close_tag, char *default_arg, char *content_handling_u, char *param_handling_u, char *childs, char *parents, int (*content_handling_func)(bstring content, bstring param, bstring user_func), int (*param_handling_func)(bstring content, bstring param, bstring user_func), bbcode_container *tag_list);
extern void bbcode_destroy_tag_stack(bbcode_container *tag_list);
extern int bbcode_parseString(bstring string, bbcode_container *tag_list) ;

int bbcode_getTagId(const_bstring tagLabel, bbcode_container *array);
static int bbcode_close_tag(bbcode_stack *stack, bbcode_container *tag_list);

static int bbcode_stack_push	(bbcode_stack *stack, bbcode_stackLine stackLine);
static int bbcode_stack_pop	(bbcode_stack *stack, bbcode_stackLine *stackLine);

#endif /*BBCODE_H_*/
