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
static void	sync_stream(void);

static unsigned int err;

struct stack sp;

static void
pr_expr(void)
{
	int *val;
	switch (token.type) {
	
	case TOKEN_LPARENTH:
		get_next_token();
		add_expr();
		
		if (err == 1) {
			goto err;
		}
		if (token.type != TOKEN_RPARENTH) {
			err = 1;
			goto err;
		} else {
			get_next_token();
		}
		goto out;
		
	case TOKEN_INTEGER:
		val = malloc(sizeof(*val));
		if (val == NULL)
			goto err;
		
		*val = token.value;		
		push_item(&sp, (void *)val);
		get_next_token();
		goto out;
		
	default:
		err = 1;
		goto err;
	}
err:
	printf("error\n");
out:
	return ;
}

static void
rest_mul_expr(void)
{
	int *op1, *op2;
	int tmp;
		
	while(token.type == TOKEN_ASTERISK || token.type == TOKEN_SLASH) {	
		tmp = token.type;
		get_next_token();
		pr_expr();
		if (err == 1) {
			goto err_out;
		}
		op2 = (int *) pop_item(&sp);
		op1 = (int *) pop_item(&sp);
		
		if (tmp == TOKEN_ASTERISK) {
			(*op1) = (*op1) * (*op2);
		} else if (tmp == TOKEN_SLASH) {
			(*op1) = (*op1) / (*op2);
		}
		push_item(&sp, (void *)op1);
		free(op2);
	}
	
	if (token.type != TOKEN_PLUS && token.type != TOKEN_MINUS &&
	    token.type != TOKEN_RPARENTH && token.type != TOKEN_EOL) {
			err = 1;
			goto err_out;
	}
err_out:
	return ;
}

static void
mul_expr(void)
{
		
	switch(token.type) {		
	case TOKEN_INTEGER:
	case TOKEN_LPARENTH:
		pr_expr();
		if (err == 1) {
			goto err;
		}
		rest_mul_expr();
		goto out;
	default:
		err = 1;
		goto err;
	}
err:
	printf("error\n");
out:
	return ;
}

static void
rest_add_expr(void)
{
	int *op1, *op2;
	int tmp;
		
	while (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS) {
		tmp = token.type;
		
		get_next_token();
		mul_expr();
		if (err == 1) {
			goto err_out;
		}
		op2 = (int *)pop_item(&sp);
		op1 = (int *)pop_item(&sp);
		
		if (tmp == TOKEN_PLUS) {
			(*op1) = (*op1) + (*op2);
		} else if (tmp == TOKEN_MINUS) {
			(*op1) = (*op1) - (*op2);
		}
		push_item(&sp, (void *)op1);
		free(op2);
	}
	
	if (token.type != TOKEN_EOL && token.type != TOKEN_RPARENTH) {
		err = 1;
		goto err_out;
	}
err_out:
	return ;
}

static void
add_expr(void)
{	
	switch(token.type) {
	case TOKEN_INTEGER:
	case TOKEN_LPARENTH:
		mul_expr();
		if (err == 1) {
			goto err;
		}
		rest_add_expr();
		goto out;		
	default:
		err = 1;
		goto err;
	}
err:
	printf("error\n");
out:
	return ;
}

static void
list_expr(void)
{	
	int *c;
	
	if (err == 1) {
		sync_stream();
		sp.n = 0;
		err = 0;
		printf("syntax error\n");
	}
again:
	while (token.type == TOKEN_INTEGER ||token.type == TOKEN_LPARENTH || token.type == TOKEN_EOL) {
		if (token.type == TOKEN_EOL) {
			get_next_token();
			continue;
		}
		if (token.type == TOKEN_EOF) {
			return ;
		}		
		add_expr();
		c = (int *)pop_item(&sp);
		printf("%i\n", (*c));		
	}
	
	sync_stream();
	goto again;
}

void
parse(void)
{
	stack_init(&sp);
	err = 0;
	
	get_next_token();
	list_expr();
	if (token.type == EOF) {
	}
}

static void 
sync_stream(void) 
{
	while (token.type != TOKEN_EOL || token.type != TOKEN_EOF)
		get_next_token();
}
