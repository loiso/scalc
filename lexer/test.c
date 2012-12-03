#include <stdio.h>

#include "lex.h"

int
main (int argc, char **argv)
{	
	do {
		get_next_token();
		switch(token.type) {
		case TOKEN_PLUS:
			printf("+\n");
			break;
		case TOKEN_MINUS:
			printf("-\n");
			break;
		case TOKEN_ASTERISK:
			printf("*\n");
			break;
		case TOKEN_SLASH:
			printf("\\\n");
			break;
		case TOKEN_LPARENTH:
			printf("(\n");
			break;
		case TOKEN_RPARENTH:
			printf(")\n");
			break;
		case TOKEN_INTEGER:
			printf("INTEGER: %d\n", token.value);
			break;
		case TOKEN_INVALID:
			printf("INVALID\n");
			break;
		case TOKEN_EOL:
			printf("EOL\n");
			break;
		case TOKEN_EOF:
			printf("EOF\n");
			break;
		
		}	
	} while (token.type != TOKEN_EOF);
	
	return 0;
}

