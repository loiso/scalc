#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stk.h"

void
stack_init(struct stack *sp)
{
	if (sp != NULL) {
		sp->n = 0;
	}
}

int
push_item(struct stack *sp, void *val)
{
	if (sp == NULL)
		return NO_STACK;
	
	if (sp->n <= STACK_SIZE) {
		sp->arr[sp->n] = val;
		sp->n++;
		return OK;
	} else {
		return OVERFLOW;
	}
}
void *
pop_item(struct stack *sp)
{	
	if (sp != NULL) {
		sp->n--;	
		return sp->arr[sp->n];
	} else {
		return NO_STACK;
	}
}

void
print_stk(struct stack *sp) 
{	
	int *value;
	int len;
	
	len = sp->n;
	sp->n--;
	while (sp->n != -1) {
		value = (int *)sp->arr[sp->n];
		printf("stk %i, %i\n", (*value), sp->n);
		sp->n--;
	}
	sp->n = len;
}
