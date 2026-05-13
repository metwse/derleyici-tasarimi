#include "../include/lexer.h"
#include "../include/tokenizer.h"

#include "../snippets/lexer.c"
#include "../snippets/tokenizer.c"

#include <assert.h>
#include <stddef.h>


//! [Token ID enum]
enum token_id {
	TK_IF,
	TK_ELSE,
	TK_TY_INT,
	TK_PLUS,
	TK_MINUS,
	// ...
};
//! [Token ID enum]

static size_t TK_RARROW = (TK_MINUS + 1);
static size_t TK_GT = (TK_MINUS + 2);


int main()
{
	struct tokenizer t;
	struct lexer l;

	tokenizer_init(&t);

//! [Tokenizer'a keyword ve punctuation kaydı]
tokenizer_add_keyword(&t, "if", TK_IF);
tokenizer_add_keyword(&t, "else", TK_ELSE);
tokenizer_add_keyword(&t, "int", TK_TY_INT);

tokenizer_add_punctuation(&t, "+", TK_PLUS);
tokenizer_add_punctuation(&t, "-", TK_MINUS);
//! [Tokenizer'a keyword ve punctuation kaydı]

	tokenizer_add_punctuation(&t, "->", TK_RARROW);
	tokenizer_add_punctuation(&t, ">", TK_GT);

	size_t ids[] = {
		TK_IF, TK_IDENT, TK_ELSE, TK_IDENT, TK_IDENT, TK_INT,
		TK_MINUS, TK_FLOAT, TK_RARROW, TK_MINUS, TK_TY_INT, TK_PLUS,
		TK_GT, TK_IDENT, TK_NOTOKEN
	};

	lexer_init(&l, "if test else test test2 123-1.2-> -int+ > ifx");

	for (size_t i = 0; i < sizeof(ids) / sizeof(size_t); i++) {
		tokenizer_feed(&t, lexer_next(&l));

		struct token tk = tokenizer_next(&t);

		assert(tk.id == ids[i]);

		if (tk.id == TK_INT)
			assert(tk.seminfo.num_int == 123);

		if (tk.id == TK_FLOAT)
			assert(tk.seminfo.num_float == 1.2);

		if (tk.id == TK_IDENT)
			assert((tk.seminfo.ident_id == tokenizer_ident_id(&t, "test") &&
					(i == 1 || i == 3)) ||
			       (tk.seminfo.ident_id == tokenizer_ident_id(&t, "test2") &&
					i == 4) ||
			       (tk.seminfo.ident_id == tokenizer_ident_id(&t, "ifx") &&
					i == 13));
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
