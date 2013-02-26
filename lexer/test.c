#include <stdio.h>
#include <stdint.h>
#include "mpl.h"

#include "lex.h"

int
main(int argc, char **argv)
{	
	char str[128];
	int i;
	
	mpl_init(&token.value);
	
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
			printf("INTEGER ");
			mpl_to_str(&token.value, str, 10, 128);
			for (i = 0; str[i] != '\0'; i++)
				printf("%c", str[i]);
			printf("\n");
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
