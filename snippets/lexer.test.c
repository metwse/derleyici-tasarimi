#include "../include/lexer.h"

#include "lexer.c"

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>


int main()
{
	struct lexer l;

	enum lexeme_kind kinds[] = {
		LEXEME_IDENT, LEXEME_INT, LEXEME_PUNCT, LEXEME_FLOAT,
		LEXEME_INT, LEXEME_FLOAT, LEXEME_PUNCT, LEXEME_PUNCT,
		LEXEME_PUNCT, LEXEME_PUNCT, LEXEME_EOF,
	};

	const char *seminfos[] = {
		"ident", "123", "-", "123.123", "12", "213.123", "->", "...",
		"(", ")",
	};

	lexer_init(&l, "ident 123   -123.123 12  213.123 -> ... ( )");

	for (size_t i = 0; i < sizeof(kinds) / sizeof(enum lexeme_kind); i++) {
		struct lexeme lexeme = lexer_next(&l);

		assert(lexeme.kind == kinds[i]);
		assert(memcmp(lexeme.seminfo, seminfos[i], lexeme.seminfo_len) == 0);
	}
}
