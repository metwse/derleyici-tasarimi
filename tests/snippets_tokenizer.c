#include "../include/lexer.h"
#include "../include/tokenizer.h"

#include "../snippets/lexer.c"
#include "../snippets/tokenizer.c"

#include <assert.h>
#include <stddef.h>


int main()
{
	struct tokenizer t;
	struct lexer l;

	tokenizer_init(&t);

	tokenizer_add_keyword(&t, "if", 0);
	tokenizer_add_keyword(&t, "else", 1);
	tokenizer_add_keyword(&t, "int", 2);

	tokenizer_add_punctuation(&t, "+", 3);
	tokenizer_add_punctuation(&t, "-", 4);
	tokenizer_add_punctuation(&t, "->", 5);
	tokenizer_add_punctuation(&t, ">", 6);

	size_t ids[] = {
		0, TK_IDENT, 1, TK_IDENT, TK_IDENT, TK_INT, TK_FLOAT,
		5, 4, 2, 3, 6, TK_IDENT, TK_NOTOKEN
	};

	lexer_init(&l, "if test else test test2 123-1.2-> -int+ > ifx");

	for (size_t i = 0; i < sizeof(ids) / sizeof(size_t); i++) {
		tokenizer_feed(&t, lexer_next(&l));

		struct token tk = tokenizer_next(&t);

		assert(tk.id == ids[i]);

		if (tk.id == TK_INT)
			assert(tk.seminfo.num_int == 123);

		if (tk.id == TK_FLOAT)
			assert(tk.seminfo.num_float == -1.2);

		if (tk.id == TK_IDENT)
			assert((tk.seminfo.ident_id == 0 && (i == 1 || i == 3)) ||
			       (tk.seminfo.ident_id == 1 && i == 4) ||
			       (tk.seminfo.ident_id == 2 && i == 12));
	}

	lexer_init(&l, "-->->>->");

	tokenizer_feed(&t, lexer_next(&l));

	assert(tokenizer_next(&t).id == 4);
	assert(tokenizer_next(&t).id == 5);
	assert(tokenizer_next(&t).id == 5);
	assert(tokenizer_next(&t).id == 6);
	assert(tokenizer_next(&t).id == 5);
	assert(tokenizer_next(&t).id == TK_NOTOKEN);

	tokenizer_destroy(&t);
}
