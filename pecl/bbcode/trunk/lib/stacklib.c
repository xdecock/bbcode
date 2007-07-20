#include <stdlib.h>
#include <malloc.h>
#include "stacklib.h"
#include "bstrlib.h"

int bbcode_int_stack_init(bbcode_int_stack *stack){
	stack->dimension=0;
	stack->size=0;
	stack->stack=malloc(sizeof(int)*32);
	return 0;
}
int bbcode_int_stack_free(bbcode_int_stack *stack){
	stack->dimension=0;
	stack->size=0;
	if (stack->stack!=NULL){ 
		free(stack->stack);
	}
	stack->stack=NULL;
	return 0;
}
int bbcode_int_stack_grow(bbcode_int_stack *stack){
	stack->stack=(int *)realloc(stack->stack,sizeof(int) * (stack->dimension+16));
	stack->dimension+=16;
	return 0;
}
int bbcode_int_stack_shrink(bbcode_int_stack *stack){
	stack->stack=(int *)realloc(stack->stack,sizeof(int)*(stack->size));
	stack->dimension=stack->size;
	return 0;
}
int bbcode_int_stack_drop (bbcode_int_stack *stack, int i){
	for (i=i;i<stack->size;i++){
		stack->stack[i-1]=stack->stack[i];
	}
	stack->size--;
}
/** Stack Management **/
int bbcode_int_stack_push (bbcode_int_stack *stack, int data){
	if (stack->size <= stack->dimension){
		bbcode_int_stack_grow(stack);
	    ++stack->size;
	    /* Insert Datas */
	    stack->stack[stack->size-1]=data;
	    if(stack->size<stack->dimension){
		    stack->stack[stack->size]=-1;
		}
	    return 0;
	} else {
		return 1;
	}
}
int bbcode_int_stack_pop(bbcode_int_stack *stack, int *data){
    if (stack->size>0){
	    --(stack->size);
	    *data=stack->stack[stack->size];
	    stack->stack[stack->size]=-1;
	    if (stack->size-16<stack->dimension){
	    	bbcode_int_stack_shrink(stack);
	    }
	    return 0;
    }else{
    	*data=0;
    }
    return 1;
}

int bbcode_stack_init(bbcode_stack *stack){
	stack->size=0;
	stack->dimension=0;
	stack->stack=malloc(sizeof(bbcode_stackLine)*32);
	return 0;
}
int bbcode_stack_free(bbcode_stack *stack){
	stack->size=0;
	stack->dimension=0;
	if (stack->stack!=NULL){ 
		free(stack->stack);
	}
	stack->stack=NULL;
	return 0;
}
int bbcode_stack_grow(bbcode_stack *stack){
	stack->stack=(bbcode_stackLine *)realloc(stack->stack,sizeof(bbcode_stackLine) * (stack->dimension+16));
	stack->dimension+=16;
	return 0;
}
int bbcode_stack_shrink(bbcode_stack *stack){
	stack->stack=(bbcode_stackLine *)realloc(stack->stack,sizeof(bbcode_stackLine)*(stack->size));
	stack->dimension=stack->size;
	return 0;
}
/** Stack Management **/
int bbcode_stack_push (bbcode_stack *stack, bbcode_stackLine stackLine){
	if (stack->size <= stack->dimension){
		bbcode_stack_grow(stack);
	    ++stack->size;
	    /* Insert Datas */
	    stack->stack[stack->size-1]=stackLine;
	    if(stack->size<stack->dimension){
		    stack->stack[stack->size].argument=NULL;
		    stack->stack[stack->size].tagId=-2;
		}
	    return 0;
	} else {
		return 1;
	}
}
int bbcode_stack_pop(bbcode_stack *stack, bbcode_stackLine *stackLine){
    if (stack->size>0){
	    --(stack->size);
	    *stackLine=stack->stack[stack->size];
	    stack->stack[stack->size].argument=NULL;
	    stack->stack[stack->size].tagId=-2;
	    if (stack->size-16<stack->dimension){
	    	bbcode_stack_shrink(stack);
	    }
	    return 0;
    }else{
    	stackLine->argument=NULL;
    	stackLine->tagId=-1;
    }
    return 1;
}
