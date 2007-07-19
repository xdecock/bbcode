#ifndef STACKLIB_H_
#define STACKLIB_H_
#include "bstrlib.h"

typedef struct _bbcode_stackLine {
    int tagId;
    bstring argument;
}bbcode_stackLine;
typedef struct _bbcode_stack {
	int size;
	int dimension;
	bbcode_stackLine* stack;
} bbcode_stack;
typedef struct _bbcode_int_stack {
	int size;
	int dimension;
	int *stack;
} bbcode_int_stack;


int bbcode_int_stack_init(bbcode_int_stack *stack);
int bbcode_int_stack_free(bbcode_int_stack *stack);
int bbcode_int_stack_grow(bbcode_int_stack *stack);
int bbcode_int_stack_shrink(bbcode_int_stack *stack);
int bbcode_int_stack_drop (bbcode_int_stack *stack, int i);
/** Stack Management **/
int bbcode_int_stack_push (bbcode_int_stack *stack, int data);
int bbcode_int_stack_pop(bbcode_int_stack *stack, int *data);
int bbcode_stack_init(bbcode_stack *stack);
int bbcode_stack_free(bbcode_stack *stack);
int bbcode_stack_grow(bbcode_stack *stack);
int bbcode_stack_shrink(bbcode_stack *stack);
/** Stack Management **/
int bbcode_stack_push (bbcode_stack *stack, bbcode_stackLine stackLine);
int bbcode_stack_pop(bbcode_stack *stack, bbcode_stackLine *stackLine);
#endif /*STACKLIB_H_*/
