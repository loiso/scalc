#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "mpl.h"
#include "lex.h"
#include "syntax.h"
#include "stk_mpl.h"

#define SYN_ERR 1
#define MEM_ERR 2

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
	mpl_int val;
	int rv;
	
	rv = mpl_init(&val);
	if (rv != MPL_OK){
		err = MEM_ERR;
		goto out;
	}
	
	switch (token.type) {
	
	case TOKEN_LPARENTH:
		get_next_token();
		add_expr();
		
		if (err == SYN_ERR || err == MEM_ERR) {
			goto out;
		}
		if (token.type != TOKEN_RPARENTH) {
			err = SYN_ERR;
			goto out;
		} else {
			get_next_token();
		}
		goto out;
		
	case TOKEN_INTEGER:
		rv = mpl_copy(&val, &token.value);
		if (rv != MPL_OK)
			goto err;
			
		rv = push_item(&sp, &val);
		if (rv != OK)
			goto err;

		get_next_token();
		goto out;
		
	default:
		err = SYN_ERR;
		goto out;
	}
err:
	err = MEM_ERR;
out:
	return ;
}

static void
rest_mul_expr(void)
{
	int tmp;
	int rv;
	mpl_int op1, op2;
		
	while(token.type == TOKEN_ASTERISK || token.type == TOKEN_SLASH) {	
		tmp = token.type;
		get_next_token();
		pr_expr();
		
		if (err == SYN_ERR) {
			goto out;
		}
		rv = pop_item(&sp, &op2);
		if (rv != OK) {
			err = MEM_ERR;
			goto out;
		}
		rv = pop_item(&sp, &op1);
		if (rv != OK) {
			err = MEM_ERR;
			goto out;
		}
		if (tmp == TOKEN_ASTERISK) {
			rv = mpl_mul(&op1, &op1, &op2);
			if (rv != MPL_OK) {
				err = MEM_ERR; 
				goto out;
			}
		} else if (tmp == TOKEN_SLASH) {
			rv = mpl_div(&op1, NULL, &op1, &op2);
			if (rv != MPL_OK) {
				err = MEM_ERR;
				goto out;
			}
		}
		rv = push_item(&sp, &op1);
		if (rv != OK) {
			err = MEM_ERR;
			goto out;
		}
		mpl_clear(&op2);			
	}
	
	if (token.type != TOKEN_PLUS && token.type != TOKEN_MINUS &&
	    token.type != TOKEN_RPARENTH && token.type != TOKEN_EOL) {
			err = SYN_ERR;
			goto out;
	}
	
out:
	return ;
}

static void
mul_expr(void)
{
	switch(token.type) {		
	case TOKEN_INTEGER:
	case TOKEN_LPARENTH:
		pr_expr();
		if (err == SYN_ERR) {
			goto out;
		}
		rest_mul_expr();
		goto out;
	default:
		err = SYN_ERR;
		goto out;
	}
out:
	return ;
}

static void
rest_add_expr(void)
{
	mpl_int op1, op2;
	int tmp, rv;
		
	while (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS) {
		tmp = token.type;
		
		get_next_token();
		mul_expr();
		if (err == SYN_ERR) {
			goto out;
		}
		
		rv = pop_item(&sp, &op2);
		if (rv != OK) {
			err = MEM_ERR;
			goto out;
		}
		rv = pop_item(&sp, &op1);
		if (rv != OK) {
			err = MEM_ERR;
			goto out;
		}
		if (tmp == TOKEN_PLUS) {
			rv = mpl_add(&op1, &op1, &op2);
			if (rv != MPL_OK) {
				err = MEM_ERR;
				goto out;
			}
		} else if (tmp == TOKEN_MINUS) {
			rv = mpl_sub(&op1, &op1, &op2);
			if (rv != MPL_OK) {
				err = MEM_ERR;
				goto out;
			}
		}
		rv = push_item(&sp, &op1);
		if (rv != OK) {
			err = MEM_ERR;
			goto out;
		}
		mpl_clear(&op2);
	}
	
	if (token.type != TOKEN_EOL && token.type != TOKEN_RPARENTH) {
		err = SYN_ERR;
		goto out;
	}
out:
	return ;
}

static void
add_expr(void)
{	
	switch(token.type) {
	case TOKEN_INTEGER:
	case TOKEN_LPARENTH:
		mul_expr();
		if (err == SYN_ERR) {
			goto out;
		}
		rest_add_expr();
		goto out;		
	default:
		err = SYN_ERR;
		goto out;
	}
out:
	return ;
}

static void
list_expr(void)
{	
	char str[SIZE];
	mpl_int c;
		
start:
	while (token.type == TOKEN_INTEGER ||token.type == TOKEN_LPARENTH || token.type == TOKEN_EOL) {

		if (token.type == TOKEN_EOL) {
			get_next_token();
			continue;
		}
		if (token.type == TOKEN_EOF) {
			return ;
		}		
		add_expr();

		if (err == SYN_ERR || err == MEM_ERR) {
			if (err == SYN_ERR)
				printf("syntax error\n");
			if (err == MEM_ERR)
				printf("memory error\n");
			sync_stream();
			sp.n = 0;
			err = 0;
			goto start;
		}
		pop_item(&sp, &c);
		mpl_to_str(&c, str, 10, SIZE);
		printf("%s\n", str);
	}

	printf("syntax error\n");
	sync_stream();
	goto start;
}

void
parse(void)
{
	int rv;
	
	stk_init(&sp);
	err = 0;
	
	rv = mpl_init(&token.value);
	if (rv != MPL_OK) {
		printf("memory error\n");
		return ;
	}
	get_next_token();
	
	list_expr();
}

static void 
sync_stream(void) 
{
	while ((token.type != TOKEN_EOL) && (token.type != TOKEN_EOF)) {
		get_next_token();
	}
}
