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

/*---------------------------*
 *        Public API		 *
 *---------------------------*/
/* Create and init a parser  */
bbcode_parser_p bbcode_parser_create() {
	bbcode_parser_p parser;
	parser=(bbcode_parser_p)malloc(sizeof(bbcode_parser));
	parser->options=0;
	parser->bbcodes=bbcode_list_create();
	parser->smileys=bbcode_smileys_list_create();
	parser->argument_parser=NULL;
	return parser;
}

/* Destroy a parser and associated ressources */
void bbcode_parser_free(bbcode_parser_p parser) {
	bbcode_list_free(parser->bbcodes);
	bbcode_smileys_list_free(parser->smileys);
	free(parser);
}

/* Set the argument Parser */
void bbcode_parser_set_arg_parser(bbcode_parser_p parser,
		bbcode_parser_p arg_parser) {
	parser->argument_parser=arg_parser;
}

/* Constructs and add a bbcode_element to the parser */
void bbcode_parser_add_ruleset(bbcode_parser_p parser, char type, char flags,
		char *open_tag, int open_tag_size, char *close_tag, int close_tag_size,
		char *default_arg, int default_arg_size, char *parent_list,
		int parent_list_size, char *child_list, int child_list_size, int (*param_handling_func)(bstring content, bstring param, void *func_data), int (*content_handling_func)(bstring content, bstring param, void *func_data),
		void *param_handling_func_data, void *content_handling_func_data) {
	bbcode_p entry=bbcode_entry_create();
	entry->type=type;
	entry->flags=flags;
	entry->open_tag=bfromcstralloc(open_tag_size, open_tag);
	entry->close_tag=bfromcstralloc(close_tag_size, close_tag);
	entry->default_arg=bfromcstralloc(default_arg_size, default_arg);
	entry->parent_list=bfromcstralloc(parent_list_size, parent_list);
	entry->child_list=bfromcstralloc(child_list_size, child_list);
	entry->parents=bbcode_allow_list_create();
	entry->childs=bbcode_allow_list_create();
	entry->param_handling_func_data=param_handling_func_data;
	entry->content_handling_func_data=content_handling_func_data;
	entry->param_handling_func=param_handling_func_data;
	entry->content_handling_func=content_handling_func_data;
	bbcode_list_add(parser->bbcodes, entry);
}

/* Construct and add a smiley to the parser */
void bbcode_parser_add_smiley(bbcode_parser_p parser, char *smiley_search,
		int smiley_search_size, char *smiley_replace, int smiley_replace_size) {
	bstring search;
	bstring replace;
	search=bfromcstralloc(smiley_search_size, smiley_search);
	replace=bfromcstralloc(smiley_replace_size, smiley_replace);
	bbcode_smileys_add(parser->smileys, search, replace);
}

/* Parse a BBCoded string to is treated equivalent */
char *bbcode_parse(bbcode_parser_p parser, char *string, int string_size,
		int *result_size) {
	bstring to_parse;
	bstring parsed;
	if (parser->options&BBCODE_DISABLE_TREE_BUILD) {
		/* No BBCode Parsing */
		if (parser->options&BBCODE_FORCE_SMILEYS_OFF) {
			/* No Smiley Treatment */
			*result_size=string_size;
			char *return_value=(char *)malloc(string_size * sizeof(char));
			return memcpy(return_value, string, string_size);
		}
		/* Prepare Datas for smiley */
		balloc(to_parse, string_size+5);
		to_parse->slen=string_size;
		to_parse->data=memcpy(to_parse->data, string, string_size);
		/* Smiley Parsing */
		bbcode_parse_smileys(to_parse, parser->smileys);
		/* Getting Treated Datas */
		*result_size=to_parse->slen;
		char *return_value;
		return_value=(char *)malloc((*result_size*sizeof(char)));
		return_value=memcpy(return_value, to_parse->data, to_parse->slen);
		bdestroy(to_parse);
		/* Returning Value */
		return return_value;
	} else {
		/* Prepare Datas for parsing */
		balloc(to_parse, string_size+5);
		to_parse->slen=string_size;
		to_parse->data=memcpy(to_parse->data, string, string_size);
		/*  starting the tree */
		bbcode_parse_tree_p tree=bbcode_tree_create();
		/* Preparing tag_list if needed */
		bbcode_prepare_tag_list(parser);
		/* Build the BBCode Tree from the string */
		bbcode_build_tree(parser, to_parse, tree);
		/* Correct Tree to match restrictions */
		bbcode_correct_tree(parser, tree, -1, 0);
		/* Reset the working string */
		bassigncstr(to_parse, "");
		/* Apply the Output Rules */
		bbcode_apply_rules(parser, tree, to_parse);
		/* Getting the return string */
		*result_size=to_parse->slen;
		char *return_value=(char *)malloc(*result_size * sizeof(char));
		return_value=memcpy(return_value, to_parse->data, to_parse->slen);
		bdestroy(to_parse);
		/* Return Value */
		return return_value;
	}
}

/* Get current options of the bbcode_parser */
int bbcode_parser_get_flags(bbcode_parser_p parser) {
	return parser->options;
}

/* Set options for the bbcode_parser */
void bbcode_parser_set_flags(bbcode_parser_p parser, int flags) {
	parser->options=flags;
}

/* ----------------------------*
 * 		  Internal API         *
 * ----------------------------*/
/* Parse nesting rules and optimize datas */
void bbcode_prepare_tag_list(bbcode_parser_p parser) {
	bbcode_list_p list;
	bbcode_p bbcode;
	struct bstrList *bsplited;
	char accept_smileys;
	char default_smileys;
	if (parser->options & BBCODE_FORCE_SMILEYS_OFF) {
		accept_smileys=0;
	}
	default_smileys=0;
	if (parser->options & BBCODE_DEFAULT_SMILEYS_ON) {
		default_smileys=1;
	}
	int i, j, max;
	max=0;
	list=parser->bbcodes;
	for (i=0; i<bbcode_array_length(list->bbcodes); i++) {
		bbcode=bbcode_get_bbcode(parser, i);
		if (blength(bbcode->tag)>max) {
			max=blength(bbcode->tag);
		}
		bbcode->speed_cache=0;
		if (bbcode->type == BBCODE_TYPE_ARG || bbcode->type
				== BBCODE_TYPE_OPTARG) {
			bbcode->speed_cache +=BBCODE_CACHE_ACCEPT_ARG;
		}
		if (bbcode->type == BBCODE_TYPE_NOARG || bbcode->type
				== BBCODE_TYPE_SINGLE || bbcode->type == BBCODE_TYPE_OPTARG) {
			bbcode->speed_cache +=BBCODE_CACHE_ACCEPT_NOARG;
		}
		if (bstrchr(bbcode->open_tag, '{')) {
			bbcode->speed_cache +=BBCODE_CACHE_START_HAS_BRACKET_OPEN;
		}
		if (bstrchr(bbcode->close_tag, '{')) {
			bbcode->speed_cache +=BBCODE_CACHE_END_HAS_BRACKET_OPEN;
		}
		if (accept_smileys && ((bbcode->flags & BBCODE_FLAGS_SMILEYS_ON)
				|| ((default_smileys) && (bbcode->flags
						& BBCODE_FLAGS_SMILEYS_OFF==0)))) {
			bbcode->speed_cache +=BBCODE_CACHE_ACCEPT_SMILEYS;
		}
		bbcode->parents->size=0;
		bbcode->childs->size=0;
		if (blength(bbcode->parent_list)) {
			if (biseqcstr(bbcode->parent_list, "all")) {
				/* All Accepted */
				bbcode->parents->type=BBCODE_ALLOW_LIST_TYPE_ALL;
			} else {
				if (bchar(bbcode->parent_list, 0)=='!') {
					bbcode->parents->type=BBCODE_ALLOW_LIST_TYPE_EXCLUDE;
				} else {
					bbcode->parents->type=BBCODE_ALLOW_LIST_TYPE_LISTED;
				}
				/* We add all entries */
				bsplited=bsplit (bbcode->parent_list, ',');
				int find, not_in_list;
				bbcode_allow_list_check_size(bbcode->parents, bsplited->qty);
				for (j=0; j<bsplited->qty; j++) {
					find = bbcode_get_tag_id (parser, bsplited->entry[j], -1);
					if (find>=0) {
						bbcode_allow_list_add(bbcode->parents, find);
					}
				}
				bstrListDestroy(bsplited);
			}
		} else {
			/* None Accepted */
			bbcode->parents->type=BBCODE_ALLOW_LIST_TYPE_NONE;
		}
	}
	if (list->bbcode_max_size !=0) {
		for (i=0; i<list->bbcode_max_size; i++) {
			if (list->search_cache[i] !=NULL) {
				free(list->search_cache[i]);
				list->num_cache[i]=0;
			}
		}
		free(list->num_cache);
		list->num_cache=NULL;
		free(list->search_cache);
		list->search_cache=NULL;
	}
	list->bbcode_max_size=max;
	list->num_cache = (int*) malloc(sizeof(int)*max);
	list->search_cache = (bbcode_search_pp) malloc(sizeof(bbcode_search_p)*max);
	for (i=0; i<list->bbcode_max_size; i++) {
		list->num_cache[i]=0;
		list->search_cache[i]=NULL;
	}
	for (i=0; i<bbcode_array_length(list->bbcodes); i++) {
		bbcode=bbcode_get_bbcode(parser, i);
		int slen=blength(bbcode->tag);
		if (list->search_cache[slen]==NULL) {
			list->search_cache[slen]
					=(bbcode_search_p) malloc(sizeof(bbcode_search));
		} else {
			list->search_cache[slen]=(bbcode_search_p) realloc(
					list->search_cache[slen], sizeof(bbcode_search));
		}
		int pos=list->num_cache[slen];
		bbcode_search temp;
		temp.tag_name=bbcode->tag;
		temp.tag_id=i;
		list->search_cache[slen][list->num_cache[slen]]=temp;
		list->num_cache[slen]++;
	}
}

/* This reparse nesting rules and optimize datas */
void bbcode_build_tree(bbcode_parser_p parser, bstring string,
		bbcode_parse_tree_p tree) {
	/* INIT */
	bstring end_quote, end_double, end_single, end_html, html_quote, argument,
			tag;
	char no_quote;
	char quote_double=parser->options&BBCODE_ARG_DOUBLE_QUOTE;
	char quote_single=parser->options&BBCODE_ARG_SINGLE_QUOTE;
	char quote_html=parser->options&BBCODE_ARG_HTML_QUOTE;
	char added=0;
	int offset, tag_id, end, next_equal, next_close, string_length;
	string_length=blength(string);
	tag_id=end=next_equal=next_close=0;
	end_double=bfromcstr("\"]");
	end_single=bfromcstr("\']");
	end_html=bfromcstr("&quot;]");
	html_quote=bfromcstr("&quot;");
	/* END INIT */
	offset=bstrchr(string, '[');
	bbcode_tree_push_string_child(tree, bmidstr(string, 0, offset));
	bbcode_parse_tree_array_p work_stack, close_stack;
	work_stack=bbcode_parse_stack_create();
	close_stack=bbcode_parse_stack_create();
	do {
		added=0;
		if (bchar(string, offset)=='[') {
			if (bchar(string, offset+1)!='/') {
				/* Open */
				bbcode_find_next(next_equal, string, offset, '=');
				/* Equal */
				bbcode_find_next(next_close, string, offset, ']');
				if (next_close<string_length) {
					/* With Arg */
					if (next_equal<next_close) {
						tag = bmidstr(string, offset+1, next_equal-offset-1);
						if (BBCODE_ERR==(tag_id=bbcode_get_tag_id(parser, tag,
								1))) {
							if (quote_double || quote_single) {
								end=next_close;
								no_quote=0;
								if (quote_single && bchar(string, next_equal+1)
										=='\'') {
									end_quote=end_single;
								} else if (quote_double && bchar(string,
										next_equal+1)=='"') {
									end_quote=end_double;
								} else if (quote_html && binstrcaseless(string,
										next_equal+1, html_quote)) {
									end_quote=end_html;
								} else {
									argument=bmidstr(string, next_equal+1,
											next_close-next_equal-1);
									no_quote=1;
								}
								if (!no_quote) {
									if (0>(end=binstrcaseless(string,
											next_equal+1, end_quote))) {
										argument=bmidstr(string, next_equal+2,
												end++ - next_equal - 2);
									} else {
										end=string_length+5;
									}
								}
							} else {
								argument=bmidstr(string, next_equal+1,
										next_close-next_equal-1);
							}
							if (argument!=NULL) {
								if (bbcode_allow_list_no_child(bbcode_get_bbcode(parser,6)->childs)) {
									BBCODE_SPECIAL_CASE_NO_CHILD(argument)
								} else {
									bbcode_tree_push_tree_child(parser, tree,
											work_stack, close_stack, bmidstr(
													string, offset, end-offset
															+1), tag_id,
											argument);
									bdestroy(argument);
									added=1;
								}
							}
						} else {
							end=next_close;
						}
					} else {
						/* Without Args */
						tag=bmidstr(string, offset+1, next_close-offset-1);
						end=next_close;
						if (BBCODE_ERR!=(tag_id=bbcode_get_tag_id(parser, tag,
								0))) {
							if (bbcode_allow_list_no_child(bbcode_get_bbcode(parser,6)->childs)) {
								BBCODE_SPECIAL_CASE_NO_CHILD(NULL)
							} else {
								bbcode_tree_push_tree_child(parser, tree,
										work_stack, close_stack, bmidstr(
												string, offset, end-offset+1),
										tag_id, NULL);
								added=1;
							}
						}
					}
					bdestroy(tag);
				}
			} else {
				bbcode_find_next(next_close, string, offset, ']');
				/* Close */
				tag=bmidstr(string, offset+2, next_close-offset-2);
				end=next_close;
				if (BBCODE_ERR!=(tag_id=bbcode_get_tag_id(parser, tag, -1))) {
					bbcode_close_tag(parser, tree, work_stack, close_stack,
							tag_id, bmidstr(string, offset, end-offset+1), 0);
					added=1;
				}
				bdestroy(tag);
			}
		}
		if (!added) {
			end=bstrchrp(string, '[', offset+1);
			if (end>0) {
				end=string_length;
			} else {
				--end;
			}
			bbcode_tree_push_string_child(tree, (bmidstr(string, offset, end
					-offset+1)));
		}
		offset=end+1;
	} while (offset<string_length);
	/* Freeing ressources */
	bdestroy(end_html);
	bdestroy(end_double);
	bdestroy(end_single);
	bbcode_parse_stack_free(work_stack);
	bbcode_parse_stack_free(close_stack);
}

/* This closes an active tag */
void bbcode_close_tag(bbcode_parser_p parser, bbcode_parse_tree_p tree,
		bbcode_parse_tree_array_p work, bbcode_parse_tree_array_p close,
		int tag_id, bstring close_string, int true_close) {
	int i;
	char in_close=0;
	/* Check if on close List */
	for (i=0; i<bbcode_array_length(close); i++) {
		if (bbcode_array_element(close,i)->tag_id==tag_id) {
			in_close=1;
			break;
		}
	}
	if (in_close) {
		/* Mark Element as closed */
		bbcode_tree_mark_element_closed((close->element[i]));
		/* Tag Allready closed, droping silently */
		bbcode_parse_drop_element_at(close, i);
	} else {
		/* Check If Opened */
		char opened=0;
		for (i=0; i<work->size; i++) {
			if (bbcode_array_element(work,i)->tag_id==tag_id) {
				opened=1;
				break;
			}
		}
		if (opened) {
			/* It's allready opened */
			char searching=1;
			bbcode_parse_tree_array conditions;
			do {
				if (parser->current_node->tag_id == tag_id) {
					/* It's the searched tag */
					searching=0;
					bbcode_tree_mark_element_closed(parser->current_node);
					parser->current_node->close_string=close_string;
					if (!true_close) {
						bbcode_parse_stack_push_element(close,
								parser->current_node);
					}
				} else {
					/* It's not the searched tag */
					/* Is this tag putting conditions on tag_id nesting ? */
					if (bbcode_allow_list_check_access(bbcode_get_bbcode(parser, bbcode_get_cn(parser)->tag_id)->childs, tag_id)) {
						/* Fixme
						 * conditions[cond_size]=bbcode_get_cn(parser);
						 * cond_size++; */
					}
					parser->current_node->close_string=NULL;
					if (bbcode_get_bbcode(parser,bbcode_get_cn(parser)->tag_id)->flags
							& BBCODE_FLAGS_ONE_OPEN_PER_LEVEL) {
						bbcode_tree_mark_element_closed(parser->current_node);
					} else {
						bbcode_parse_stack_push_element(close,
								parser->current_node);
					}
				}
				bbcode_parse_stack_pop_element_loose(work);
				parser->current_node = bbcode_array_element(work,
						bbcode_array_length(work)-1);
			} while (searching);
			/* Reopening incorrectly nested & closed tags */
			if (parser->options & BBCODE_CORRECT_REOPEN_TAGS) {
				bbcode_parse_tree_p tmp_tree;
				bbcode_parse_tree_array_p first;
				for (i=close->size-1; i>=0; --i) {
					/* First Multipart Element */
					if (!bbcode_array_element(close,i)->flags&BBCODE_TREE_FLAGS_MULTIPART) {
						if (bbcode_array_element(close,i)->flags
								&BBCODE_TREE_FLAGS_MULTIPART_FIRST_NODE) {

						} else {
							/* The multipart Tree is common to all multipart elements of a set */
						}
					}
					tmp_tree=bbcode_tree_create();
					tmp_tree->flags=BBCODE_TREE_FLAGS_MULTIPART;
					/* tmp_tree->multiparts=*bbcode_array_element(close,i); */
					tmp_tree->open_string=NULL;
					tmp_tree->close_string=NULL;
					tmp_tree->parent_node=bbcode_get_cn(parser);
					bbcode_tree_push_tree_raw(tree, tmp_tree, work);
					bbcode_get_cn(parser) = bbcode_array_element(work,work->size-1);
				}
				close->size=0;
			}
		} else {
			bbcode_tree_push_string_child(tree, close_string);
		}
	}
}

/* This make some basic corrections to a given tree */
void bbcode_correct_tree(bbcode_parser_p parser, bbcode_parse_tree_p tree,
		int parent_id, char force_false) {
	char autocorrect;
	/* TODO: Implement */
}

/* This apply the BBCode rules to generate the final string */
void bbcode_apply_rules(bbcode_parser_p parser, bbcode_parse_tree_p tree,
		bstring parsed) {
	/* TODO: Implement */
}

/* Search a tag_id from the string */
int bbcode_get_tag_id(bbcode_parser_p parser, bstring value, int has_arg) {
	int taglen=blength(value);
	bbcode_list_p bbcode_list = parser->bbcodes;
	if (taglen < bbcode_list->bbcode_max_size) {
		if (bbcode_list->search_cache[taglen]==NULL) {
			/* Tag does not exists */
			return BBCODE_ERR;
		} else {
			/* tags of this size exists, start binary search
			 * First, get the count of elements & the elements */
			int count=bbcode_list->num_cache[taglen];
			bbcode_search_p list=bbcode_list->search_cache[taglen];
			if (count<5) {
				/* We use linear search, should be faster */
				int i;
				for (i=0; i<count; i++) {
					if (!bstricmp(value, list[i].tag_name)) {
						int pos=list[i].tag_id;
						if (has_arg) {
							if (bbcode_get_bbcode(parser,pos)->speed_cache
									& BBCODE_CACHE_ACCEPT_ARG) {
								return pos;
							}
						} else {
							if (bbcode_get_bbcode(parser,pos)->speed_cache
									& BBCODE_CACHE_ACCEPT_NOARG) {
								return pos;
							}
						}
					}
				}
			} else {
				bstring lower_tag;
				lower_tag=bstrcpy(value);
				btolower(lower_tag);
				/* We start true binary */
				int left=0;
				int right=count-1;
				int i=count/2;
				char found=0;
				int equal, pos;
				while (1) {
					equal=bstrcmp(lower_tag, list[i].tag_name);
					if (!equal) {
						/* We have found the entry */
						pos=list[i].tag_id;
						if (has_arg) {
							if (bbcode_get_bbcode(parser,pos)->speed_cache
									& BBCODE_CACHE_ACCEPT_ARG) {
								return pos;
							}
						} else {
							if (bbcode_get_bbcode(parser,pos)->speed_cache
									& BBCODE_CACHE_ACCEPT_NOARG) {
								return pos;
							}
						}
						return BBCODE_ERR;
					} else if (equal>0) {
						/* the searched entry is greater than this pos */
						left=i;
						i=((left+right+1)>>1);
						if (left==i) {
							break;
						}
					} else {
						/* the searched entry is smaller than this pos */
						right=i;
						i=((left+right)>>1);
						if (right==i) {
							break;
						}
					}
				}
			}
		}
	}
	return BBCODE_ERR;
}

/* Translate Smileys */
void bbcode_parse_smileys(bstring string, bbcode_smiley_list_p list) {
	int i;
	for (i=0; i<list->size; i++){
		bfindreplace(string, list->smileys[i].search, list->smileys[i].replace, 0);
	}
}

/*---------------------------
 Smiley Manipulation API
 ---------------------------*/
/* Initialize a smiley list */
bbcode_smiley_list_p bbcode_smileys_list_create() {
	bbcode_smiley_list_p list;
	list=malloc(sizeof(bbcode_smiley_list));
	list->size=0;
	list->msize=0;
	return list;
}

/* Free a smiley list */
void bbcode_smileys_list_free(bbcode_smiley_list_p list) {
	if (list->msize==0) {
		int i;
		for (i=0; i<list->size; i++) {
			bdestroy(list->smileys[i].search);
			bdestroy(list->smileys[i].replace);
		}
		free(list->smileys);
	}
	free(list);
}

/* Check if we can add an entry */
void bbcode_smiley_list_check_size(bbcode_smiley_list_p list, int size) {
	if (list->msize<=size) {
		list->smileys=realloc(list->smileys, (size+BBCODE_BUFFER)
				*sizeof(bbcode_smiley));
		list->msize=size+BBCODE_BUFFER;
	}
}

/* adds a smiley to the list */
void bbcode_smileys_add(bbcode_smiley_list_p list, bstring search,
		bstring replace) {
	bbcode_smiley_list_check_size(list, list->size+1);
	list->smileys[list->size].search=search;
	list->smileys[list->size].replace=replace;
}

/*---------------------------
 BBCode List Manipulation API
 ---------------------------*/
/* creates a BBcode list and init it */
bbcode_list_p bbcode_list_create() {
	bbcode_list_p list;
	list=malloc(sizeof(bbcode_list));
	list->root=NULL;
	list->bbcodes=bbcode_array_create();
	list->options=0;
	list->bbcode_max_size=0;
	list->num_cache=NULL;
	list->search_cache=NULL;
	return list;
}

/* free ressources for a BBCode list */
void bbcode_list_free(bbcode_list_p list) {
	if (list->root!=NULL) {
		free(list->root);
	}
	bbcode_array_free(list->bbcodes);
	free(list);
}

/* Check if there is room for a bbcode entry */
void bbcode_list_check_size(bbcode_list_p list, int size) {
	if (list->bbcodes->size<=size) {
		list->bbcodes=realloc(list->bbcodes, (size+BBCODE_BUFFER)
				*sizeof(bbcode_p));
		list->bbcodes->size=size+BBCODE_BUFFER;
	}
}

/* Insert the special entry "Root" */
void bbcode_list_set_root(bbcode_list_p list, bbcode_p root) {
	list->root=root;
	list->options |= BBCODE_LIST_HAS_ROOT;
}

/* add a bbcode to a list */
void bbcode_list_add(bbcode_list_p list, bbcode_p to_add) {
	bbcode_list_check_size(list, list->bbcodes->size+1);
	list->bbcodes->element[list->bbcodes->size-1]=to_add;
	list->options &= ~BBCODE_LIST_IS_READY;
}

/*---------------------------
 BBCode Array Manipulation API
 ---------------------------*/
/* creates a BBcode array and init it */
bbcode_array_p bbcode_array_create() {
	bbcode_array_p array;
	array=malloc(sizeof(bbcode_array));
	array->size=0;
	array->msize=0;
}

/* Free a BBCode array */
void bbcode_array_free(bbcode_array_p array) {
	if (array->msize==0) {
		int i;
		for (i=0; i<array->size; i++) {
			bbcode_entry_free(array->element[i]);
		}
		free(array->element);
	}
	free(array);
}

/* Check if we can add an entry */
void bbcode_array_check_size(bbcode_array_p array, int size) {
	if (array->msize<=size) {
		array->element=realloc(array->element, (size+BBCODE_BUFFER)
				*sizeof(bbcode_p));
		array->msize=size+BBCODE_BUFFER;
	}
}

/* adds a bbcode_rule to the list */
void bbcode_array_add(bbcode_array_p array, bbcode_p bbcode) {
	bbcode_array_check_size(array, array->size+1);
	array->element[array->size]=bbcode;
}

/*---------------------------
 BBCode Entry Manipulation API
 ---------------------------*/
/* Malloc a bbcode entry and init it */
bbcode_p bbcode_entry_create() {
	bbcode_p bbcode;
	bbcode=malloc(sizeof(bbcode));
	/* Init values */
	bbcode->type = bbcode->speed_cache = bbcode->flags = 0;
	bbcode->open_tag = bbcode->close_tag = bbcode->default_arg = bbcode->child_list = bbcode->parent_list = NULL;
	bbcode->param_handling_func_data = bbcode->content_handling_func_data = NULL;
	bbcode->param_handling_func = bbcode->content_handling_func = NULL;
	/* Starting Up allow_lists */
	bbcode->parents=bbcode_allow_list_create();
	bbcode->childs=bbcode_allow_list_create();
	return bbcode;
}

/* Free a bbcode entry ressources */
void bbcode_entry_free(bbcode *entry) {
	/* Freeing automaticaly started datas; */
	/* All other stored datas must be freed by user */
	bbcode_allow_list_free(entry->parents);
	bbcode_allow_list_free(entry->childs);
	free(entry);
}

/*---------------------------
 BBCode Allow Manipulation API
 ---------------------------*/
/* Malloc a bbcode_allow_list and init it */
bbcode_allow_list_p bbcode_allow_list_create() {
	bbcode_allow_list_p list;
	list=malloc(sizeof(bbcode_allow_list));
	list->size=list->msize=list->type=0;
	return list;
}

/* Free the ressources taken by an allow list */
void bbcode_allow_list_free(bbcode_allow_list_p list) {
	if (list->msize!=0) {
		free(list->id_list);
	}
	free(list);
}

/* Check for the size of an allow list */
void bbcode_allow_list_check_size(bbcode_allow_list_p list, int size) {
	if (list->msize<=size) {
		list->id_list=realloc(list->id_list, (size+BBCODE_BUFFER)*sizeof(int));
		list->msize=size+BBCODE_BUFFER;
	}

}

/* Add an element to the list */
void bbcode_allow_list_add(bbcode_allow_list_p list, int element) {
	bbcode_allow_list_check_size(list, list->size+1);
	list->id_list[list->size]=element;
}

/* Check if a given id is autorized */
int bbcode_allow_list_check_access(bbcode_allow_list_p list, int tag_id) {
	/* The easy ones */
	if (list->type==BBCODE_ALLOW_LIST_TYPE_ALL) {
		return 1;
	} else if (list->type==BBCODE_ALLOW_LIST_TYPE_NONE) {
		return 0;
	}
	/* And Now, We run */
	else {
		int i;
		for (i=0; i<list->size; i++) {
			if (list->id_list[i]==tag_id) {
				/* Found, we check if its a include or exclude list */
				if (list->type==BBCODE_ALLOW_LIST_TYPE_LISTED) {
					return 1;
				} else {
					return 0;
				}
			}
		}
		/* Not Found, checking if we're on include or exclude list */
		if (list->type==BBCODE_ALLOW_LIST_TYPE_LISTED) {
			return 0;
		} else {
			return 1;
		}
	}
}

/* return 1 if no_childs are accepted return 0 oterwise */
int bbcode_allow_list_no_child(bbcode_allow_list_p list) {
	/* Most of tags have no restrictions so exit ASAP */
	if (list->type==BBCODE_ALLOW_LIST_TYPE_ALL) {
		return 0;
	} else if (list->type==BBCODE_ALLOW_LIST_TYPE_NONE) { /* No childs */
		return 1;
	} else if (list->type==BBCODE_ALLOW_LIST_TYPE_LISTED) { /* Only Listed Childs and size=0 */
		if (list->size=0) {
			return 1;
		}
	}
	/* Undetermined Case: If exclude list with everything in it, (people using it must be considered insane -_-") */
	return 0;
}

/*---------------------------
 Tree Manipulation API
 ---------------------------*/
/* Malloc and init a bbcode tree  */
bbcode_parse_tree_p bbcode_tree_create() {
	bbcode_parse_tree_p tree;
	tree=malloc(sizeof(bbcode_parse_tree));
	tree->tag_id=BBCODE_TREE_ROOT_TAGID;
	tree->flags=0;
	tree->childs.size=0;
	tree->childs.msize=0;
	tree->multiparts = tree->conditions = NULL;
	tree->parent_node = NULL;
	tree->open_string = tree->close_string = NULL;
	return tree;
}

/* Free the ressources taken by a tree */
void bbcode_tree_free(bbcode_parse_tree_p tree) {
	int i;
	for (i=0; i < tree->childs.size; i++) {
		if (tree->childs.element[i]->type==BBCODE_TREE_CHILD_TYPE_TREE) {
			bbcode_tree_free(tree->childs.element[i]->tree);
		} else {
			bdestroy(tree->childs.element[i]->string);
		}
	}
	free(tree->childs.element);
	if (tree->open_string != NULL) {
		bdestroy(tree->open_string);
	}
	if (tree->close_string != NULL) {
		bdestroy(tree->close_string);
	}
}

/* Check if there is sufficient space in child array */
void bbcode_tree_check_child_size(bbcode_parse_tree_p tree, int size) {
	if (tree->childs.msize<size) {
		tree->childs.element=(bbcode_parse_tree_child_pp) realloc(
				tree->childs.element, sizeof(bbcode_parse_tree_child_p)*(size
						+BBCODE_BUFFER));
		tree->childs.msize=size+BBCODE_BUFFER;
	}
}

/* adds a child to the current list (sub_tree) */
void bbcode_tree_push_tree_child(bbcode_parser_p parser,
		bbcode_parse_tree_p tree, bbcode_parse_tree_array_p work,
		bbcode_parse_tree_array_p close, bstring open_string, int tagId,
		bstring argument) {
	/* TODO Implement */
}

/* adds a child to the current list (string_leaf) */
void bbcode_tree_push_string_child(bbcode_parse_tree_p tree, bstring string) {
	bbcode_tree_check_child_size(tree, tree->childs.size+1);
	tree->childs.element[tree->childs.size]->string=string;
	tree->childs.size++;
}

/* adds a tree to the current list (raw) */
void bbcode_tree_push_tree_raw(bbcode_parse_tree_p tree,
		bbcode_parse_tree_p tmp_tree, bbcode_parse_tree_array_p work) {
	bbcode_tree_check_child_size(tree, tree->childs.size+1);
	tree->childs.element[tree->childs.size]->tree=tmp_tree;
	bbcode_parse_stack_push_element(work, tmp_tree);
	tree->childs.size++;
}

/* Get the last child and removes it from the list */
void bbcode_tree_pop_child(bbcode_parse_tree_p tree,
		bbcode_parse_tree_child_p bbcode_parse_tree_child) {
	bbcode_parse_tree_child=tree->childs.element[tree->childs.size];
	tree->childs.size--;
}

/* Insert a given child on a given position */
void bbcode_tree_insert_child_at(bbcode_parse_tree_p tree,
		bbcode_parse_tree_child_p bbcode_parse_tree_child, int pos) {
	bbcode_tree_check_child_size(tree, tree->childs.size+1);
	int size=sizeof(bbcode_parse_tree_child_p);
	memmove(tree->childs.element+(size*(pos+1)), tree->childs.element
			+(size*pos), size*(tree->childs.size-pos));
	tree->childs.element[pos]=bbcode_parse_tree_child;
}

/* Mark an element closed, (and also multipart elements) */
void bbcode_tree_mark_element_closed(bbcode_parse_tree_p tree) {
	tree->flags |=BBCODE_TREE_FLAGS_PAIRED;
	if (tree->flags & BBCODE_TREE_FLAGS_MULTIPART) {
		/* TODO Multipart Code */
	}
}

/* Move a child set from a parent to another */
void bbcode_tree_move_childs(bbcode_parse_tree_p from, bbcode_parse_tree_p to,
		int offset_from, int count, int offset_to) {
	bbcode_parse_child_p start, stop;
	bbcode_tree_check_child_size(to, to->childs.size+count);
	/* We First Move the current childs to leave space */
	memmove(to->childs.element[offset_to+count], to->childs.element[offset_to],
			sizeof(bbcode_parse_child_p)*(to->childs.size-offset_to));
	/* Copying Childs From Old Position to new */
	memcpy(to->childs.element[offset_to], from->childs.element[offset_from],
			count * sizeof(bbcode_parse_child_p));
	/* Reducing Child Set In From Elements */
	memmove(from->childs.element[offset_from],
			from->childs.element[offset_from+count],
			sizeof(bbcode_parse_child_p) * (from->childs.size-offset_from));
	/* Setting the sizes to correct values */
	to->childs.size+=count;
	from->childs.size-=count;
}

/*---------------------------
 Parse Tree array Manipulation API
 ---------------------------*/
/* Create a Tree array */
bbcode_parse_tree_array_p bbcode_parse_stack_create() {
	bbcode_parse_tree_array_p array;
	array=malloc(sizeof(bbcode_parse_tree_array));
	array->size=0;
	array->msize=0;
	array->element=NULL;
	return array;
}

/* Free ressource used by a Tree array */
void bbcode_parse_stack_free(bbcode_parse_tree_array_p stack) {
	if (stack->msize) {
		free(stack->element);
	}
	free(stack);
}

/* Check if there is room for adding elements */
void bbcode_parse_stack_check_size(bbcode_parse_tree_array_p stack, int size) {
	if (stack->msize<size) {
		stack->element=(bbcode_parse_tree_pp) realloc(stack->element,
				(BBCODE_BUFFER+size)*sizeof(bbcode_parse_tree_p));
		stack->msize=(BBCODE_BUFFER+size);
	}
}

/* Add element to the Tree array */
void bbcode_parse_stack_push_element(bbcode_parse_tree_array_p stack,
		bbcode_parse_tree_p element) {
	bbcode_parse_stack_check_size(stack, stack->size+1);
	stack->element[stack->size]=element;
}

bbcode_parse_tree_p bbcode_parse_stack_pop_elemen(
		bbcode_parse_tree_array_p stack) {
	stack->size--;
	return stack->element[stack->size];
}

/* Remove element from the Tree array  without giving it back */
void bbcode_parse_stack_pop_element_loose(bbcode_parse_tree_array_p stack) {
	stack->size--;
}

/* Remove element from the Tree array @ index */
void bbcode_parse_drop_element_at(bbcode_parse_tree_array_p stack, int index) {
	if (index<stack->size) {
		stack->size--;
		memmove(stack->element[index-1], stack->element[index], (stack->size
				-index)*sizeof(bbcode_parse_tree_p));
	}
}

/*---------------------------
 Built-in callbacks
 ---------------------------*/
