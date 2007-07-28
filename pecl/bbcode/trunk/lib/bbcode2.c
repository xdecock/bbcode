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
#include <stdlib.h>
#include <malloc.h>
#include "bbcode2.h"
#include "bstrlib.h"


/*---------------------------
         Public API
---------------------------*/
/* Create and init a parser */
bbcode_parser *bbcode_parser_create(){
    bbcode_parser *parser;
    parser=(bbcode_parser *)malloc(sizeof(bbcode_parser));
    parser->options=0;
    parser->bbcodes=bbcode_list_create();
    parser->smileys=bbcode_smileys_list_create();
    parser->argument_parser=NULL;
    return parser;
}

/* Destroy a parser and associated ressources */
void bbcode_parser_free(bbcode_parser *parser){
    bbcode_list_free(parser->bbcodes);
    bbcode_smileys_list_free(parser->smileys);
    free(parser);
}

/* Set the argument Parser */
void bbcode_parser_set_arg_parser(bbcode_parser *parser, bbcode_parser *arg_parser){
    parser->argument_parser=arg_parser;
}

/* Constructs and add a bbcode_element to the parser */
void bbcode_parser_add_ruleset(bbcode_parser *parser, char type, char flags, char *open_tag,
            int open_tag_size, char *close_tag, int close_tag_size, char *default_arg,
            int default_arg_size, char *parent_list, int parent_list_size,
            char *child_list, int child_list_size,
            int (*param_handling_func)(bstring content, bstring param, void *func_data),
            int (*content_handling_func)(bstring content, bstring param, void *func_data),
            void *param_handling_func_data, void *content_handling_func_data)
{
    bbcode *entry=bbcode_entry_create();
    entry->type=type;
    entry->flags=flags;
    entry->open_tag=bfromcstralloc(open_tag_size,open_tag);
    entry->close_tag=bfromcstralloc(close_tag_size,close_tag);
    entry->default_arg=bfromcstralloc(default_arg_size,default_arg);
    entry->parent_list=bfromcstralloc(parent_list_size,parent_list);
    entry->child_list=bfromcstralloc(child_list_size,child_list);
    entry->parents=bbcode_allow_list_create();
    entry->childs=bbcode_allow_list_create();
    entry->param_handling_func_data=param_handling_func_data;
    entry->content_handling_func_data=content_handling_func_data;
    entry->param_handling_func=param_handling_func_data;
    entry->content_handling_func=content_handling_func_data;
    bbcode_list_add(parser->bbcodes, entry);
}

/* Construct and add a smiley to the parser */
void bbcode_parser_add_smiley(bbcode_parser *parser, char *smiley_search, int smiley_search_size,
            char *smiley_replace, int smiley_replace_size)
{
    bstring search;
    bstring replace;
    search=bfromcstralloc(smiley_search_size,smiley_search);
    replace=bfromcstralloc(smiley_replace_size, smiley_replace);
    bbcode_smileys_add(parser->smileys, search, replace);
}

/* Parse a BBCoded string to is treated equivalent */
char *bbcode_parse (bbcode_parser *parser, char *string, int string_size, int *result_size){
    bstring to_parse;
    bstring parsed;
    if (parser->options&BBCODE_DISABLE_TREE_BUILD){
        // No BBCode Parsing
        if (parser->options&BBCODE_FORCE_SMILEYS_OFF){
            // No Smiley Treatment
            *result_size=string_size;
            char *return_value=(char *)malloc(string_size * sizeof(char));
            return memcpy(return_value,string,string_size);
        }
        // Prepare Datas for smiley
        balloc(to_parse, string_size+5);
        to_parse->slen=string_size;
        to_parse->data=memcpy(to_parse->data,string, string_size);
        // Smiley Parsing
        bbcode_parse_smileys(to_parse);
        // Getting Treated Datas
        *result_size=to_parse->slen;
        char *return_value;
        return_value=(char *)malloc((*result_size*sizeof(char)));
        return_value=memcpy(return_value,to_parse->data,to_parse->slen);
        bdestroy(to_parse);
        // Returning Value
        return return_value;
    } else {
        // Prepare Datas for parsing
        balloc(to_parse, string_size+5);
        to_parse->slen=string_size;
        to_parse->data=memcpy(to_parse->data,string, string_size);
        // starting the tree
        bbcode_parse_tree *tree=bbcode_tree_create();
        // Preparing tag_list if needed
        bbcode_prepare_tag_list(parser);
        // Build the BBCode Tree from the string
        bbcode_build_tree(parser,to_parse,tree);
        // Correct Tree to match restrictions
        bbcode_correct_tree(parser,tree,-1,0);
        // Reset the working string
        bassigncstr(to_parse,"");
        // Apply the Output Rules
        bbcode_apply_rules(parser, tree, to_parse);
        // Getting the return string
        *result_size=to_parse->slen;
        char *return_value=(char *)malloc(*result_size * sizeof(char));
        return_value=memcpy(return_value,to_parse->data,to_parse->slen);
        bdestroy(to_parse);
        // Return Value
        return return_value;
    }
}

/* Get current options of the bbcode_parser */
int bbcode_parser_get_flags(bbcode_parser *parser){
    return parser->options;
}

/* Set options for the bbcode_parser */
void bbcode_parser_set_flags(bbcode_parser *parser, int flags){
    parser->options=flags;
}

/*---------------------------
        Internal API
---------------------------*/
/* Parse nesting rules and optimize datas */
void bbcode_prepare_tag_list(bbcode_parser *parser){
    bbcode_list *list;
    bbcode *bbcode;
	struct bstrList *bsplited;
    char accept_smileys;
    char default_smileys;
    if (parser->options & BBCODE_FORCE_SMILEYS_OFF){
        accept_smileys=0;
    }
    default_smileys=0;
    if (parser->options & BBCODE_DEFAULT_SMILEYS_ON){
        default_smileys=1;
    }
    int i,j;
    list=parser->bbcodes;
    for (i=0;i<list->size;i++){
        bbcode=&list->bbcodes[i];
        bbcode->speed_cache=0;
        if (bbcode->type == BBCODE_TYPE_ARG || bbcode->type == BBCODE_TYPE_OPTARG){
            bbcode->speed_cache +=BBCODE_CACHE_ACCEPT_ARG;
        }
        if (bbcode->type == BBCODE_TYPE_NOARG || bbcode->type == BBCODE_TYPE_SINGLE || bbcode->type == BBCODE_TYPE_OPTARG){
            bbcode->speed_cache +=BBCODE_CACHE_ACCEPT_NOARG;
        }
        if (bstrchr(bbcode->open_tag,'{')){
            bbcode->speed_cache +=BBCODE_CACHE_START_HAS_BRACKET_OPEN;
        }
        if (bstrchr(bbcode->close_tag,'{')){
            bbcode->speed_cache +=BBCODE_CACHE_END_HAS_BRACKET_OPEN;
        }
        if (accept_smileys && ((bbcode->flags & BBCODE_FLAGS_SMILEYS_ON) || ((default_smileys) && (bbcode->flags & BBCODE_FLAGS_SMILEYS_OFF==0)))) {
            bbcode->speed_cache +=BBCODE_CACHE_ACCEPT_SMILEYS;
        }
        bbcode->parents->size=0;
        bbcode->childs->size=0;
        if (blength(bbcode->parent_list)){
            if (biseqcstr(bbcode->parent_list,"all")){
                // All Accepted
                bbcode->parents->type=BBCODE_ALLOW_LIST_TYPE_ALL;
            } else {
                if (bchar(bbcode->parent_list,0)=='!'){
                    bbcode->parents->type=BBCODE_ALLOW_LIST_TYPE_EXCLUDE;
                } else {
                    bbcode->parents->type=BBCODE_ALLOW_LIST_TYPE_LISTED;
                }
                /* We add all entries */
				bsplited=bsplit ( bbcode->parent_list, ',' );
				int find, not_in_list;
			    bbcode_allow_list_check_size(bbcode->parents, bsplited->qty);
				for (j=0;j<bsplited->qty;j++){
					find = bbcode_get_tag_id (parser,bsplited->entry[j],-1);
					if (find>=0){
					    bbcode_allow_list_add(bbcode->parents,find);
					}
				}
				bstrListDestroy(bsplited);
            }
        } else {
            // None Accepted
            bbcode->parents->type=BBCODE_ALLOW_LIST_TYPE_NONE;
        }
    }
}

/* This reparse nesting rules and optimize datas */
void bbcode_build_tree (bbcode_parser *parser,bstring string, bbcode_parse_tree *tree){
    //INIT
    bstring end_quote, end_double, end_single, argument, tag;
    char no_quote;
    char quote_double=parser->options&BBCODE_ARG_DOUBLE_QUOTE;
    char quote_single=parser->options&BBCODE_ARG_SINGLE_QUOTE;
    char added=0;
    int offset,tag_id,end,next_equal,next_close;
    tag_id=end=next_equal=next_close=0;
    end_double=bfromcstr("\"]");
    end_single=bfromcstr("\']");
    // END INIT
    offset=bstrchr(string,'[');
    bbcode_tree_push_string_child(tree,bmidstr(string,0,offset));
    bbcode_parse_stack *work_stack, *close_stack;
    work_stack=bbcode_parse_stack_create();
    close_stack=bbcode_parse_stack_create();
    do {
        added=0;
        if (bchar(string,offset)=='['){
            if (bchar(string,offset+1)!='/'){
                //Open
                // Equal
                if (next_equal<=offset){
                    if (0>(next_equal=bstrchrp(string,'=',offset))){
                        next_equal=blength(string)+5;
                    }
                }
                if (next_close<=offset){
                    if (0>(next_close=bstrchrp(string,']',offset))){
                        next_close=blength(string)+5;
                    }
                }
                if (next_close<blength(string)){
                    // With Arg
                    if (next_equal<next_close){
                        tag = bmidstr(string,offset+1,next_equal-offset-1);
                        if (BBCODE_ERR==(tag_id=bbcode_get_tag_id(parser, tag, 1))){
                            if (quote_double || quote_single){
                                end=next_close;
                                no_quote=0;
                                if (quote_single && bchar(string,next_equal+1)=='\''){
                                    end_quote=end_single;
                                } else if (quote_double && bchar(string,next_equal+1)=='"') {
                                    end_quote=end_double;
                                } else {
                                    argument=bmidstr(string,next_equal+1,next_close-next_equal-1);
                                    no_quote=1;
                                }
                                if (!no_quote){
                                    if (0>(end=binchr(string,next_equal+1,end_quote))){
                                        argument=bmidstr(string,next_equal+2,end++ - next_equal - 2);
                                    } else {
                                        end=blenght(string)+5;
                                    }
                                }
                            } else {
                                argument=bmidstr(string,next_equal+1,next_close-next_equal-1);
                            }
                            if (argument!=NULL){
                                bbcode_tree_push_tree_child(parser,tree,work_stack,close_stack,bmidstr(string,offset,end-offset+1),tag_id, argument);
                                bdestroy(argument);
                                added=1;
                            }
                        } else {
                            end=next_close;
                        }
                        bdestroy(tag);
                    } else {
                        // Without Args
                        tag=bmidstr(string,offset+1,next_close-offset-1);
                        end=next_close;
                        if (BBCODE_ERR!=(tag_id=bbcode_get_tag_id(parser, tag, 0))){
                            bbcode_tree_push_tree_child(parser,tree,work_stack,close_stack,bmidstr(string,offset,end-offset+1),tag_id,NULL);
                            added=1;
                        }
                        bdestroy(tag);
                    }
                }
            } else {
                if (next_close<=offset){
                    if (0<(next_close=bstrchrp(string,']',offset))){
                        next_close=blength(string)+5;
                    }
                }
                //Close
                tag=bmidstr(string,offset+2,next_close-offset-2);
                end=next_close;
                if (BBCODE_ERR!=(tag_id=bbcode_get_tag_id(parser, tag, -1))){
                    bbcode_close_tag(parser, tree, work_stack, close_stack, tag_id, bmidstr(string,offset,end-offset+1),0);
                    added=1;
                }
                bdestroy(tag);
            }
        }
        if (!added){
            end=bstrchrp(string,'[',offset+1);
            if (end>0){
                end=blength(string);
            } else {
                --end;
            }
            bbcode_tree_push_string_child(tree, (bmidstr(string,offset,end-offset+1)));
        }
        offset=end+1;
    }while (offset<blength(string));
    // Freeing ressources
    bdestroy(end_double);
    bdestroy(end_single);
    bbcode_parse_stack_free(work_stack);
    bbcode_parse_stack_free(close_stack);
}

/* This closes an active tag */
void bbcode_close_tag (bbcode_parser *parser, bbcode_parse_tree *tree, bbcode_parse_stack *work, bbcode_parse_stack *close, int tag_id, bstring close_string, int true_close){
    int i;
    char in_close=0;
    // Check if on close List
    for (i=0;i<close->size;i++){
        if (close->element[i].tag_id==tag_id){
            in_close=1;
            break;
        }
    }
    if (in_close){
        // Mark Element as closed
        bbcode_tree_mark_element_closed(&(close->element[i]));
        // Tag Allready closed, droping silently
        bbcode_parse_drop_element_at(close, i);
    } else {
        // Check If Opened
        char opened=0;
        for (i=0;i<work->size;i++){
            if (work->element[i].tag_id==tag_id){
                opened=1;
                break;
            }
        }
        if (opened){
            /* It's allready opened */
            char searching=1;
            do {
                if (tree->currentNode->tag_id == tag_id){
                    /* It's the searched tag */
                    searching=0;
                    bbcode_tree_mark_element_closed(tree->currentNode);
                    tree->close_string=close_string;
                    if (!true_close){
                        bbcode_parse_stack_push_element(close,tree->currentNode);
                    }
                } else {
                    /* It's not the searched tag */
                    tree->currentNode->close_string=NULL;
                    if (parser->bbcodes->bbcodes[tree->currentNode->tag_id].flags & BBCODE_FLAGS_ONE_OPEN_PER_LEVEL){
                        bbcode_tree_mark_element_closed(tree->currentNode);
                    } else {
                        bbcode_parse_stack_push_element(close,tree->currentNode);
                    }
                }
                bbcode_parse_stack_pop_element_loose(work);
                tree->currentNode = &work->element[work->size-1];
            } while(searching);
            /* Reopening incorrectly nested & closed tags */
            if (parser->options & BBCODE_CORRECT_REOPEN_TAGS){
                bbcode_parse_tree *tmp_tree;
                bbcode_parse_tree *first;
                for (i=close->size-1; i>=0;--i){
                    if (!close->element[i].flags&BBCODE_TREE_FLAGS_MULTIPART){
                        if (close->element[i].flags&BBCODE_TREE_FLAGS_MULTIPART_FIRST_NODE){
                        } else {
                            first=close->element[i].multiparts;
                        }
                    }
                    tmp_tree=bbcode_tree_create();
                    tmp_tree->flags=BBCODE_TREE_FLAGS_MULTIPART;
                    tmp_tree->multiparts=&(close->element[i]);
                    tmp_tree->open_string=NULL;
                    tmp_tree->close_string=NULL;
                    tmp_tree->parent_node=tree->currentNode;
                    bbcode_tree_push_tree_raw(tree, tmp_tree, work);
                    tree->currentNode = &work->element[work->size-1];
                }
                close->size=0;
            }
        } else {
            bbcode_tree_push_string_child(tree, close_string);
        }
    }
}

/* This make some basic corrections to a given tree */
void bbcode_correct_tree (bbcode_parser *parser,bbcode_parse_tree *tree, int parent_id, char force_false){

}

/* This apply the BBCode rules to generate the final string */
void bbcode_apply_rules (bbcode_parser *parser,bbcode_parse_tree *tree, bstring parsed){

}

/* Search a tag_id from the string */
int bbcode_get_tag_id (bbcode_parser *parser, bstring value,  int has_arg){

}

/* Translate Smileys */
void bbcode_parse_smileys(bstring string){
}

/*---------------------------
   Smiley Manipulation API
---------------------------*/
/* Initialize a smiley list */
bbcode_smiley_list *bbcode_smileys_list_create (){
    bbcode_smiley_list *list;
    list=malloc(sizeof(bbcode_smiley_list));
    list->size=0;
    list->available_size=0;
    return list;
}

/* Free a smiley list */
void bbcode_smileys_list_free (bbcode_smiley_list *list){
    if (list->available_size==0){
        int i;
        for (i=0;i<list->size;i++) {
            bdestroy(list->smileys[list->size].search);
            bdestroy(list->smileys[list->size].replace);
        }
        free(list->smileys);
    }
    free(list);
}

/* Check if we can add an entry */
void bbcode_smiley_list_check_size (bbcode_smiley_list *list, int size){
    if (list->available_size==size){
        list->smileys=realloc(list->smileys, (size+BBCODE_BUFFER)*sizeof(bbcode_smiley));
    }
}

/* adds a smiley to the list */
void bbcode_smileys_add (bbcode_smiley_list *list, bstring search, bstring replace){
    bbcode_smiley_list_check_size(list, list->size+1);
    list->smileys[list->size].search=search;
    list->smileys[list->size].replace=replace;
}

/*---------------------------
 BBCode List Manipulation API
---------------------------*/
/* creates a BBcode list and init it */
bbcode_list *bbcode_list_create(){

}

/* free ressources for a BBCode list */
void bbcode_list_free(bbcode_list *list){

}

/* Check if there is room for a bbcode entry */
void bbcode_list_check_size(bbcode_list *list){

}

/* Insert the special entry "Root" */
void bbcode_list_set_root(bbcode_list *list, bbcode *root){

}

/* add a bbcode to a list */
void bbcode_list_add(bbcode_list *list, bbcode *to_add){

}

/*---------------------------
BBCode Entry Manipulation API
---------------------------*/
/* Malloc a bbcode entry and init it */
bbcode *bbcode_entry_create(){

}

/* Free a bbcode entry ressources */
void bbcode_entry_free(bbcode *entry){

}

/*---------------------------
BBCode Allow Manipulation API
---------------------------*/
/* Malloc a bbcode_allow_list and init it */
bbcode_allow_list *bbcode_allow_list_create(){

}

/* Free the ressources taken by an allow list */
void bbcode_allow_list_free(bbcode_allow_list *list){

}

/* Check for the size of an allow list */
void bbcode_allow_list_check_size(bbcode_allow_list *list, int size){

}

/* Add an element to the list */
void bbcode_allow_list_add(bbcode_allow_list *list, int element){

}

/*---------------------------
    Tree Manipulation API
---------------------------*/
/* Malloc and init a bbcode tree  */
bbcode_parse_tree *bbcode_tree_create(){

}

/* Free the ressources taken by a tree */
void bbcode_tree_free(bbcode_parse_tree *tree){

}

/* Check if there is sufficient space in child array */
void bbcode_tree_check_child_size(bbcode_parse_tree *tree, int size){

}

/* adds a child to the current list (sub_tree) */
void bbcode_tree_push_tree_child (bbcode_parser *parser, bbcode_parse_tree *tree, bbcode_parse_stack *work, bbcode_parse_stack *close,  bstring open_string, int tagId, bstring argument){

}

/* adds a child to the current list (string_leaf) */
void bbcode_tree_push_string_child (bbcode_parse_tree *tree, bstring string){

}

/* adds a tree to the current list (raw) */
void bbcode_tree_push_tree_raw (bbcode_parse_tree *tree, bbcode_parse_tree *tmp_tree, bbcode_parse_stack *work){

}

/* Get the last child and removes it from the list */
void bbcode_tree_pop_child(bbcode_parse_tree *tree, bbcode_parse_tree_child *bbcode_parse_tree_child){

}

/* Insert a given child on a given position */
void bbcode_tree_insert_child_at(bbcode_parse_tree *tree, bbcode_parse_tree_child *bbcode_parse_tree_child, int pos){

}

/* Mark an element closed, (and also multipart elements) */
void bbcode_tree_mark_element_closed(bbcode_parse_tree *tree){

}

/* Move a child set from a parent to another */
void bbcode_tree_move_childs(bbcode_parse_tree *from, bbcode_parse_tree *to, int offset_from, int count, int offset_to){

}

/*---------------------------
Parse Stack Manipulation API
---------------------------*/
/* Create a parse stack */
bbcode_parse_stack *bbcode_parse_stack_create(){

}

/* Free ressource used by a parse stack */
void bbcode_parse_stack_free(bbcode_parse_stack *stack){

}

/* Check if there is room for adding elements */
void bbcode_parse_stack_check_size(bbcode_parse_stack *stack, int size){

}

/* Add element to the stack */
void bbcode_parse_stack_push_element(bbcode_parse_stack *stack, bbcode_parse_tree *element){

}


/* Remove element from the stack  without giving it back */
void bbcode_parse_stack_pop_element_loose(bbcode_parse_stack *stack){

}

/* Remove element from the stack @ index */
void bbcode_parse_drop_element_at(bbcode_parse_stack *stack, int index){

}

