#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "lex.h"

struct token token;
static int buf = ' ';

token_t
get_next_token()
{	
	while (isblank(buf))
		buf = getchar();
	
	switch(buf) {
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		token.type = TOKEN_INTEGER;
		token.value = 0;
		do {
			token.value = token.value * 10 + (buf - '0');
			buf = getchar();
		} while (isdigit(buf));
		goto out;
	case '+':
		token.type = TOKEN_PLUS;
		goto clean_out;
	case '-':
		token.type = TOKEN_MINUS;
		goto clean_out;
	case '*':
		token.type = TOKEN_ASTERISK;
		goto clean_out;
	case '/':
		token.type = TOKEN_SLASH;
		goto clean_out;
	case '(':
		token.type = TOKEN_LPARENTH;
		goto clean_out;
	case ')':
		token.type = TOKEN_RPARENTH;
		goto clean_out;
	case '\n':
		token.type = TOKEN_EOL;
		goto clean_out;
	case EOF:
		token.type = TOKEN_EOF;
		goto clean_out;
	}
	
	token.type = TOKEN_INVALID;
	token.value = buf;
	
clean_out:
	buf = ' ';	
out:
	return token.type;
}

