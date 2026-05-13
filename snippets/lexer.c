#include "../include/lexer.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>


//! [lexer_init]
/* lexer struct'ı içinde start ve current şeklinde iki pointer tutuyordu */
void lexer_init(struct lexer *lexer, const char *text)
{
	lexer->start = text;
	lexer->current = text;
}
//! [lexer_init]


//! [Yardımcı fonksiyonlar]
static struct lexeme lexeme(struct lexer *l, enum lexeme_kind kind)
{
	return (struct lexeme) {
		.seminfo = l->start,
		.seminfo_len = (size_t) (l->current - l->start),
		.kind = kind
	};
}

/* Lexer'da okunmamış karakter kaldı mı? */
static bool is_at_end(struct lexer *l)
{
	return *l->current == '\0';
}

/* Mevcut karakter. */
static char peek(struct lexer *l)
{
	return l->current[0];
}

/* Varsa, bir sonraki karakter. */
static char peek_next(struct lexer *l)
{
	return is_at_end(l) ? '\0' : l->current[1];
}

/* Mevcut karakteri consume ederek bir sonraki karaktere geç. */
static void advance(struct lexer *l)
{
	l->current++;
}
//! [Yardımcı fonksiyonlar]


//! [Sayı toplama]
static struct lexeme collect_number(struct lexer *lexer)
{
	enum lexeme_kind kind = LEXEME_INT;

	/* Tam sayı kısmı topla. */
	while (isdigit(peek(lexer)))
		advance(lexer);

	/* Ondalık kısım varsa, onu da topla. */
	if (peek(lexer) == '.' && isdigit(peek_next(lexer))) {
		kind = LEXEME_FLOAT;
		advance(lexer);  /* Noktayı topla */

		/* Geri kalan sayı kısmı da topla. */
		while (isdigit(peek(lexer)))
			advance(lexer);
	}

	return lexeme(lexer, kind);
}
//! [Sayı toplama]

//! [Identifier ve punctuation toplama]
static struct lexeme collect_identifier(struct lexer *lexer)
{
	/* a-z|A-Z|_|0-9 */
	while (isalnum(peek(lexer)) || peek(lexer) == '_')
		advance(lexer);

	return lexeme(lexer, LEXEME_IDENT);
}

static struct lexeme collect_punctuation(struct lexer *lexer)
{
	/* Lexer, birden fazla karakterden oluşan punctuation'ları da
	 * toplayabilmek adına punctuation'ları gruplar. */
	while (ispunct(peek(lexer)))
		advance(lexer);

	return lexeme(lexer, LEXEME_PUNCT);
}
//! [Identifier ve punctuation toplama]


//! [lexer_next]
struct lexeme lexer_next(struct lexer *lexer)
{
	/* Boşlukları görmezden gelip bir sonraki karaktere atlıyoruz. */
	while (isspace(peek(lexer)))
		advance(lexer);

	lexer->start = lexer->current;

	if (is_at_end(lexer))
		return lexeme(lexer, LEXEME_EOF);

	char c = peek(lexer);

	/* a-z|A-Z|_ */
	if (isalpha(c) || c == '_')
		return collect_identifier(lexer);

	/* 0-9 */
	if (isdigit(c)) {
		return collect_number(lexer);
	}

	/* punctuation */
	if (ispunct(c))
		return collect_punctuation(lexer);

	assert(0 && "Tanınmayan karakter.");  // GCOVR_EXCL_LINE: not tested
}
//! [lexer_next]
