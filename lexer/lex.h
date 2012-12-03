/* 
 * lexical analyser header 
 */

#ifndef LEX_H
#define LEX_H

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

struct token {	
	token_t type;
	int value;
};

extern struct token token;

token_t get_next_token();

#endif /* LEX_H */

