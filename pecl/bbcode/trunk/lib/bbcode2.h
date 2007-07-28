/*
 * This source file is part of the bbcode library.
 * Written and maintained by Xavier De Cock 2006-2007
 * Licensed under the BSD License Terms
 * Refer to the accompanying documentation for details on usage and license.
 * See also: Company Website: http://www.bmco.be/
 * See also: Hosted on pecl: http://pecl.php.net/
 * Leave this header As Is, add your name as maintainer, and please, contribute
 * enhancement back to the community
 * Revision : $Rev$
 */
#ifndef BBCODE_H_
#define BBCODE_H_
#include "bstrlib.h"
#define BBCODE_LIB_VERSION "2.0"

#define BBCODE_BUFFER 4

#define BBCODE_TYPE_NOARG               1
#define BBCODE_TYPE_SINGLE              2
#define BBCODE_TYPE_ARG                 3
#define BBCODE_TYPE_OPTARG              4
#define BBCODE_TYPE_ROOT                5

#define BBCODE_FLAGS_ARG_PARSING        0x1
#define BBCODE_FLAGS_CDATA_NOT_ALLOWED  0x2
#define BBCODE_FLAGS_SMILEYS_ON         0x4
#define BBCODE_FLAGS_SMILEYS_OFF        0x8
#define BBCODE_FLAGS_ONE_OPEN_PER_LEVEL 0x10

#define BBCODE_ARG_NO_QUOTE        0x0
#define BBCODE_ARG_DOUBLE_QUOTE    0x1
#define BBCODE_ARG_SINGLE_QUOTE    0x2
#define BBCODE_AUTO_CORRECT        0x4
#define BBCODE_CORRECT_REOPEN_TAGS 0x8
#define BBCODE_DEFAULT_SMILEYS_ON  0x10
#define BBCODE_DEFAULT_SMILEYS_OFF 0x20
#define BBCODE_FORCE_SMILEYS_OFF   0x40
#define BBCODE_DISABLE_TREE_BUILD  0x80

#define BBCODE_CACHE_ACCEPT_ARG             0x01
#define BBCODE_CACHE_ACCEPT_NOARG           0x02
#define BBCODE_CACHE_START_HAS_BRACKET_OPEN 0x04
#define BBCODE_CACHE_END_HAS_BRACKET_OPEN   0x08
#define BBCODE_CACHE_ACCEPT_SMILEYS         0x10

#define BBCODE_ALLOW_LIST_TYPE_NONE    0
#define BBCODE_ALLOW_LIST_TYPE_ALL     1
#define BBCODE_ALLOW_LIST_TYPE_LISTED  2
#define BBCODE_ALLOW_LIST_TYPE_EXCLUDE 3

#define BBCODE_TREE_CHILD_TYPE_TREE 0
#define BBCODE_TREE_CHILD_TYPE_STRING 1

#define BBCODE_LIST_IS_READY    1
#define BBCODE_LIST_HAS_ROOT    2

#define BBCODE_TREE_FLAGS_PAIRED 0x1
#define BBCODE_TREE_FLAGS_MULTIPART 0x2
#define BBCODE_TREE_FLAGS_MULTIPART_FIRST_NODE 0x4
#define BBCODE_TREE_FLAGS_MULTIPART_LAST_NODE 0x8
#define BBCODE_TREE_FLAGS_MULTIPART_DONE 0x10
#define BBCODE_TREE_FLAGS_ROOT 0x20

#define BBCODE_ERR -2

/* This represent a single smiley with search / replace */
typedef struct _bbcode_smiley{
    bstring search;
    bstring replace;
} bbcode_smiley;

/* This represent a smiley list */
typedef struct bbcode_smiley_list{
    bbcode_smiley* smileys;
    int size;
    int available_size;
} bbcode_smiley_list;

/* This represent a list of allowed tags */
typedef struct _bbcode_allow_list{
    int *id_list;
    char type;
    int size;
    int available_size;
}bbcode_allow_list;

/* This represent a BBCode Tag Rule Set */
typedef struct _bbcode{
    char type;
    char flags;
    char speed_cache;
    bstring open_tag;
    bstring close_tag;
    bstring default_arg;
    bstring parent_list;
    bstring child_list;
    bbcode_allow_list *parents;
    bbcode_allow_list *childs;
    void *param_handling_func_data;
    void *content_handling_func_data;
    int (*param_handling_func)(bstring content, bstring param, void *func_data);
    int (*content_handling_func)(bstring content, bstring param, void *func_data);
} bbcode;

/* This represent a complete BBCode Parsing Rule Set */
typedef struct _bbcode_list{
    bbcode* bbcodes;
    bbcode* root;
    int size;
    int available_size;
    char options;
} bbcode_list;

/* This is the bbcode parser */
typedef struct _bbcode_parser{
    struct _bbcode_parser *argument_parser;
    bbcode_smiley_list *smileys;
    bbcode_list *bbcodes;
    int options;
} bbcode_parser;

/* This is a single token found by parsing (in fact a token is often splitted with partial matches) */
typedef struct _bbcode_parse_tree_child{
    union {
        struct _bbcode_parse_tree* tree;
        bstring* string;
    };
    char type;
} bbcode_parse_tree_child;

/* This is the parse tree temporary data store */
typedef struct _bbcode_parse_tree{
    bbcode_parse_tree_child *childs;
    struct _bbcode_parse_tree* currentNode;
    struct _bbcode_parse_tree *multiparts;
    struct _bbcode_parse_tree *parent_node;
    int multiparts_size;
    int childs_size;
    int childs_available_size;
    int tag_id;
    int flags;
    bstring open_string;
    bstring close_string;
} bbcode_parse_tree;

/* This is the bbcode_parse_stack */
typedef struct _bbcode_parse_stack{
    bbcode_parse_tree *element;
    int size;
    int available_size;
}bbcode_parse_stack;

/*---------------------------
         Public API
---------------------------*/
/* Create and init a parser */
bbcode_parser *bbcode_parser_create();

/* Destroy a parser and associated ressources */
void bbcode_parser_free(bbcode_parser *parser);

/* Destroy a parser and associated ressources */
void bbcode_parser_set_arg_parser(bbcode_parser *parser, bbcode_parser *arg_parser);

/* Constructs and add a bbcode_element to the parser */
void bbcode_parser_add_ruleset(bbcode_parser *parser, char type, char flags, char *open_tag,
            int open_tag_size, char *close_tag, int close_tag_size, char *default_arg,
            int default_arg_size, char *parent_list, int parent_list_size,
            char *child_list, int child_list_size,
            int (*param_handling_func)(bstring content, bstring param, void *func_data),
            int (*content_handling_func)(bstring content, bstring param, void *func_data),
            void *param_handling_func_data, void *content_handling_func_data);

/* Construct and add a smiley to the parser */
void bbcode_parser_add_smiley(bbcode_parser *parser, char *smiley_search, int smiley_search_size,
            char *smiley_replace, int smiley_replace_size);

/* Parse a BBCoded string to is treated equivalent */
char *bbcode_parse (bbcode_parser *parser, char *string, int string_size, int *result_size);

/* Get current options of the bbcode_parser */
int bbcode_parser_get_flags(bbcode_parser *parser);

/* Set options for the bbcode_parser */
void bbcode_parser_set_flags(bbcode_parser *parser, int flags);

/*---------------------------
        Internal API
---------------------------*/
/* Parse nesting rules and optimize datas */
void bbcode_prepare_tag_list(bbcode_parser *parser);

/* This reparse nesting rules and optimize datas */
void bbcode_build_tree (bbcode_parser *parser, bstring string, bbcode_parse_tree *tree);

/* This closes an active tag */
void bbcode_close_tag (bbcode_parser *parser, bbcode_parse_tree *tree, bbcode_parse_stack *work, bbcode_parse_stack *close, int tag_id, bstring close_string, int true_close);

/* This make some basic corrections to a given tree */
void bbcode_correct_tree (bbcode_parser *parser, bbcode_parse_tree *tree, int parent_id, char force_false);

/* This apply the BBCode rules to generate the final string */
void bbcode_apply_rules (bbcode_parser *parser, bbcode_parse_tree *tree, bstring parsed);

/* Search a tag_id from the string */
int bbcode_get_tag_id (bbcode_parser *parser, bstring value, int has_arg);

/* Translate Smileys */
void bbcode_parse_smileys(bstring string);

/*---------------------------
   Smiley Manipulation API
---------------------------*/
/* Initialize a smiley list */
bbcode_smiley_list *bbcode_smileys_list_create ();

/* Free a smiley list */
void bbcode_smileys_list_free (bbcode_smiley_list *list);

/* Check if we can add an entry */
void bbcode_smiley_list_check_size (bbcode_smiley_list *list, int size);

/* adds a smiley to the list */
void bbcode_smileys_add (bbcode_smiley_list *list, bstring search, bstring replacement);

/*---------------------------
 BBCode List Manipulation API
---------------------------*/
/* creates a BBcode list and init it */
bbcode_list *bbcode_list_create();

/* free ressources for a BBCode list */
void bbcode_list_free(bbcode_list *list);

/* Check if there is room for a bbcode entry */
void bbcode_list_check_size(bbcode_list *list);

/* Insert the special entry "Root" */
void bbcode_list_set_root(bbcode_list *list, bbcode *root);

/* add a bbcode to a list */
void bbcode_list_add(bbcode_list *list, bbcode *to_add);

/*---------------------------
BBCode Entry Manipulation API
---------------------------*/
/* Malloc a bbcode entry and init it */
bbcode *bbcode_entry_create();

/* Free a bbcode entry ressources */
void bbcode_entry_free(bbcode *entry);

/*---------------------------
BBCode Allow Manipulation API
---------------------------*/
/* Malloc a bbcode_allow_list and init it */
bbcode_allow_list *bbcode_allow_list_create();

/* Free the ressources taken by an allow list */
void bbcode_allow_list_free(bbcode_allow_list *list);

/* Check for the size of an allow list */
void bbcode_allow_list_check_size(bbcode_allow_list *list, int size);

/* Add an element to the list */
void bbcode_allow_list_add(bbcode_allow_list *list, int element);

/*---------------------------
    Tree Manipulation API
---------------------------*/
/* Malloc and init a bbcode tree  */
bbcode_parse_tree *bbcode_tree_create();

/* Free the ressources taken by a tree */
void bbcode_tree_free(bbcode_parse_tree *tree);

/* Check if there is sufficient space in child array */
void bbcode_tree_check_child_size(bbcode_parse_tree *tree, int size);

/* adds a child to the current list (sub_tree) */
void bbcode_tree_push_tree_child (bbcode_parser *parser, bbcode_parse_tree *tree, bbcode_parse_stack *work, bbcode_parse_stack *close, bstring open_string, int tagId, bstring argument);

/* adds a child to the current list (string_leaf) */
void bbcode_tree_push_string_child (bbcode_parse_tree *tree, bstring string);

/* adds a tree to the current list (raw) */
void bbcode_tree_push_tree_raw (bbcode_parse_tree *tree, bbcode_parse_tree *tmp_tree, bbcode_parse_stack *work);

/* Get the last child and removes it from the list */
void bbcode_tree_pop_child(bbcode_parse_tree *tree, bbcode_parse_tree_child *bbcode_parse_tree_child);

/* Insert a given child on a given position */
void bbcode_tree_insert_child_at(bbcode_parse_tree *tree, bbcode_parse_tree_child *bbcode_parse_tree_child, int pos);

/* Mark an element closed, (and also multipart elements) */
void bbcode_tree_mark_element_closed(bbcode_parse_tree *tree);

/* Move a child set from a parent to another */
void bbcode_tree_move_childs(bbcode_parse_tree *from, bbcode_parse_tree *to, int offset_from, int count, int offset_to);

/*---------------------------
Parse Stack Manipulation API
---------------------------*/
/* Create a parse stack */
bbcode_parse_stack *bbcode_parse_stack_create();

/* Free ressource used by a parse stack */
void bbcode_parse_stack_free(bbcode_parse_stack *stack);

/* Check if there is room for adding elements */
void bbcode_parse_stack_check_size(bbcode_parse_stack *stack, int size);

/* Add element to the stack */
void bbcode_parse_stack_push_element(bbcode_parse_stack *stack, bbcode_parse_tree *element);

/* Remove element from the stack */
void bbcode_parse_stack_pop_element(bbcode_parse_stack *stack, bbcode_parse_tree *element);

/* Remove element from the stack  without giving it back */
void bbcode_parse_stack_pop_element_loose(bbcode_parse_stack *stack);

/* Remove element from the stack @ index */
void bbcode_parse_drop_element_at(bbcode_parse_stack *stack, int index);

#endif /*BBCODE_H_*/
