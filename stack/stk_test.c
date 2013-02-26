#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mpl.h>

#include "stk_mpl.h"

int
main()
{
  struct stack sp;
	
	stk_init(&sp);
	mpl_int a, b, c, d, e, f;
	char as[] = "12";
	char bs[] = "128";
	char cs[] = "5";
	
	char str[10];
	int i;
	
	mpl_init(&a);
	mpl_init(&b);
	mpl_init(&c);
	
	mpl_set_str(&a, as, 10);	
	mpl_set_str(&b, bs, 10);
	mpl_set_str(&c, cs, 10);
	
	push_item(&sp, &a);
	push_item(&sp, &b);
	push_item(&sp, &c);
	
	print_stk(&sp);
	
	pop_item(&sp, &d);
	pop_item(&sp, &e);
	pop_item(&sp, &f);
	
	mpl_add(&d, &f, &e);
	mpl_to_str(&d, str, 10, 10);
	for (i = 0; str[i] != '\0'; i++)
		printf("%c", str[i]);
	printf("\n");
	return 0;
}
