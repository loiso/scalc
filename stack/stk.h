#ifndef STK_H
#define STK_H

#define	STACK_SIZE	1024

#define OK 0
#define NO_STACK 1
#define OVERFLOW 2

struct stack {
	void *arr[STACK_SIZE];
	unsigned int n;
};

void	stack_init(struct stack *sp);
int 	push_item(struct stack *sp, void *val);
void	*pop_item(struct stack *sp);
void	print_stk(struct stack *sp);

#endif /* STK_H */
