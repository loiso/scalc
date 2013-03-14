#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "lex.h"
#include "mpl.h"
#include "stk.h"

int
stk_init(struct stack *sp)
{
	sp->n = 0;
	return OK;
}

int
push_item(struct stack *sp, number *buf) 
{
	if (sp->n < SIZE) {
		mpl_copy(&sp->stack[sp->n].value, &buf->value);
		sp->stack[sp->n].frac = buf->frac;
		sp->n++;
		return OK;
	} else {
		return OVERFLOW;
	}
}

int
pop_item(struct stack *sp, number *buf)
{
	if (sp->n >= SIZE)
		return OVERFLOW;
		
	sp->n -= 1;
	*buf = sp->stack[sp->n];
	
	return OK;
}

void
print_stk(struct stack *sp)
{
	char str[SIZE];
	int i, j;
	
	for (i = 0; i < sp->n; i++) {
		mpl_to_str(&sp->stack[i].value, str, 10, SIZE);
		for (j = 0; str[j] != '\0'; j++)
			printf("%c", str[j]);
		printf("\n");
	}
}
