#ifndef STK_H
#define STK_H

#include <stdio.h>
#include <stdint.h>

#include "lex.h"
#include "mpl.h"

#define OK		0
#define OVERFLOW	1
#define SIZE		128

struct stack {
	number stack[SIZE];
	unsigned int n;
};

int	stk_init(struct stack *sp);
int	push_item(struct stack *sp, number *buf);
int	pop_item(struct stack *sp, number *buf);
void	print_stk(struct stack *sp);

#endif /* STK_H */

