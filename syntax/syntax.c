#include <stdio.h>
#include <stdlib.h>

#include "lex.h"
#include "syntax.h"
#include "stk.h"

static void	pr_expr(void);
static void	rest_mul_expr(void);
static void	mul_expr(void);
static void	rest_add_expr(void);
static void	add_expr(void);
static void	list_expr(void);

struct stack sp;

static void
pr_expr(void)
{
	int *val;
	
	switch (token.type) {
	
	case TOKEN_LPARENTH:
		get_next_token();
		add_expr();
		if (token.type != TOKEN_RPARENTH) {
			printf("syntax error\n");
		} else {
			get_next_token();
		}
		return ;
		
	case TOKEN_INTEGER:
		val = malloc(sizeof(*val));
		if (val == NULL)
			return;
		*val = token.value;		
		push_item(&sp, (void *)val);
		get_next_token();
		return ;
		
	default:
		printf("syntax error\n");
		return ;
	}
}

static void
rest_mul_expr(void)
{
	int *op1, *op2;
	
	while(token.type == TOKEN_ASTERISK || token.type == TOKEN_SLASH) {	
		switch(token.type) {

		case TOKEN_ASTERISK:
			get_next_token();
			pr_expr();
			op1 = (int *) pop_item(&sp);
			op2 = (int *) pop_item(&sp);
			(*op1) = (*op1) * (*op2);
			push_item(&sp, (void *)op1);
			free(op2);
			break;
			
		case TOKEN_SLASH:
			get_next_token();
			pr_expr();			
			op2 = (int *)pop_item(&sp);
			op1 = (int *)pop_item(&sp);
			(*op1) = (*op1) / (*op2);
			push_item(&sp, (void *)op1);
			free(op2);
			break;
		/* case TOKEN_PLUS, case TOKEN_MINUS, case TOKEN_RPARENTH, case TOKEN_EOL - empty cases */
		}
	}
}

static void
mul_expr(void)
{
	switch(token.type) {		
	case TOKEN_INTEGER:
	case TOKEN_LPARENTH:
		pr_expr();
		rest_mul_expr();
		break;
	default:
		return ; 
	}
}

static void
rest_add_expr(void)
{
	int *op1, *op2;
	
	while (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS) {
		
		switch (token.type) {
		case TOKEN_PLUS:
			get_next_token();
			mul_expr();
			op1 = (int *)pop_item(&sp);
			op2 = (int *)pop_item(&sp);
			(*op1) = (*op1) + (*op2);
			push_item(&sp, (void *)op1);
			free(op2);
			break;
			
		case TOKEN_MINUS:
			get_next_token();
			mul_expr();
			op2 = (int *)pop_item(&sp);
			op1 = (int *)pop_item(&sp);
			(*op1) = (*op1) - (*op2);
			push_item(&sp, (void *)op1);
			free(op2);
			break;
		/* case TOKEN_EOL, case TOKEN_RPARENTH - empty cases */
		}
	}
}

static void
add_expr(void)
{
	switch(token.type) {
	case TOKEN_INTEGER:
	case TOKEN_LPARENTH:
		mul_expr();
		rest_add_expr();
		break;
		
	default:
		printf("syntax error2\n");
		return ;
	}
}

static void
list_expr(void)
{	
	int *c;
	
	while (token.type == TOKEN_INTEGER ||token.type == TOKEN_LPARENTH || token.type == TOKEN_EOL) {
			if (token.type == TOKEN_EOL) {
				get_next_token();
				break;
			}
			if (token.type == TOKEN_EOF) {
				return ;
			}		
			add_expr();
			c = (int *)pop_item(&sp);
			printf("%i\n", (*c));
			if (token.type != TOKEN_EOL) {
				/* syntax error */
				printf("syntax error1\n");
			}
	}
}


void
parse(void)
{
	stack_init(&sp);
	get_next_token();
	list_expr();
	
	if (token.type == EOF) {
	}
}
