#include "../include/lexer.h"
#include "../include/tokenizer.h"

#include "../include/map.h"

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#define MAX_PUNCT_LEN 3


//! [Tokenizer init/destroy]
void tokenizer_init(struct tokenizer *t)
{
	/* string -> ID (size_t), identifier ID için kullanılacak */
	map_init(&t->ident_map, sizeof(size_t));

	t->last_id = 0;

	/* bunlara birazdan geleceğiz */
	map_init(&t->keywords, sizeof(size_t));
	map_init(&t->punctuations, sizeof(size_t));

	/* tokenizer'ın boş olduğunu belirtmek için mevcut işlenen lexemeyi
	 * LEXEME_EOF yapalım. */
	t->current_lexeme.kind = LEXEME_EOF;
}

void tokenizer_destroy(struct tokenizer *t)
{
	map_destroy(&t->ident_map);

	map_destroy(&t->keywords);
	map_destroy(&t->punctuations);
}
//! [Tokenizer init/destroy]

//! [Tokenizer'a keyword/operatör ekleme]
void tokenizer_add_keyword(struct tokenizer *t,
			   const char *keyword,
			   size_t id)
{
	map_insert(&t->keywords, keyword, &id);
}

void tokenizer_add_punctuation(struct tokenizer *t,
			       const char *punctuation,
			       size_t id)
{
	map_insert(&t->punctuations, punctuation, &id);
}
//! [Tokenizer'a keyword/operatör ekleme]

void tokenizer_feed(struct tokenizer *t, struct lexeme lexeme)
{
	assert(t->current_lexeme.kind == LEXEME_EOF &&
	       "Tokenizerda zaten bir lexeme mevcut. Önce tokenizer_next çağırın.");

	t->current_lexeme = lexeme;
}

static struct token consume_ident(struct tokenizer *t)
{
	t->current_lexeme.kind = LEXEME_EOF;

	size_t *keyword_id = map_get2(&t->keywords,
			       t->current_lexeme.seminfo,
			       t->current_lexeme.seminfo_len);

	if (keyword_id)
		return (struct token) { .id = *keyword_id };

	size_t *id_in_map = map_get2(&t->ident_map,
			       t->current_lexeme.seminfo,
			       t->current_lexeme.seminfo_len);

	size_t id;

	if (id_in_map) {
		id = *id_in_map;
	} else {
		id = t->last_id++;

		map_insert2(&t->ident_map,
			t->current_lexeme.seminfo,
			t->current_lexeme.seminfo_len, &id);
	}

	return (struct token) { .id = TK_IDENT, .seminfo.ident_id = id };
}

static struct token consume_num(struct tokenizer *t)
{
	bool is_int = t->current_lexeme.kind == LEXEME_INT;

	t->current_lexeme.kind = LEXEME_EOF;

	char num[t->current_lexeme.seminfo_len + 1];

	num[t->current_lexeme.seminfo_len] = '\0';
	memcpy(num, t->current_lexeme.seminfo, t->current_lexeme.seminfo_len);

	struct token tk;

	if (is_int) {
		tk.id = TK_INT;
		tk.seminfo.num_int = strtoimax(num, NULL, 10);
	} else {
		tk.id = TK_FLOAT;
		tk.seminfo.num_float = strtod(num, NULL);
	}

	return tk;
}

static struct token consume_punct(struct tokenizer *t)
{
	size_t *punct_id;
	size_t i;

	for (
	     i = MAX_PUNCT_LEN > t->current_lexeme.seminfo_len ?
		t->current_lexeme.seminfo_len : MAX_PUNCT_LEN;
	     i > 0;
	     i--) {
		punct_id = map_get2(&t->punctuations,
			     t->current_lexeme.seminfo, i);

		if (punct_id) {
			t->current_lexeme.seminfo += i;
			t->current_lexeme.seminfo_len -= i;

			break;
		}
	}

	assert(i && "Bilinmeyen sembol.");

	if (t->current_lexeme.seminfo_len == 0)
		t->current_lexeme.kind = LEXEME_EOF;

	return (struct token) { .id = *punct_id };
}

struct token tokenizer_next(struct tokenizer *t)
{
	switch (t->current_lexeme.kind) {
	case LEXEME_EOF:
		return (struct token) { .id = TK_NOTOKEN };

	case LEXEME_IDENT:
		return consume_ident(t);

	case LEXEME_PUNCT:
		return consume_punct(t);

	case LEXEME_INT:
	case LEXEME_FLOAT:
		return consume_num(t);
	}

	assert(0);  // GCOVR_EXCL_LINE: unreachable
}
