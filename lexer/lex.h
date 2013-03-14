/* 
 * lexical analyser header 
 */

#ifndef LEX_H
#define LEX_H
 
#include <stdio.h>
#include <stdint.h>
#include "mpl.h"

#define SIZE 	128
#define NOMEM 	-1

typedef enum token_t {
	TOKEN_PLUS,
	TOKEN_MINUS,
	TOKEN_ASTERISK,
	TOKEN_SLASH,
	TOKEN_LPARENTH,
	TOKEN_RPARENTH,
	TOKEN_INTEGER,
	TOKEN_INVALID,
	TOKEN_EOL,
	TOKEN_EOF
} token_t;

typedef struct number {
	int frac;
	mpl_int value;
} number;

struct token {	
	token_t type;
	number num;
};

extern struct token token;

token_t get_next_token();

#endif /* LEX_H */
