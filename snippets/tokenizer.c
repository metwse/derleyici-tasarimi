#include "../include/lexer.h"
#include "../include/tokenizer.h"

#include "../include/map.h"

#include <assert.h>
#include <inttypes.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#define MAX_PUNCT_LEN 3


static void clear_lexeme(struct tokenizer *t)
{
	t->current_lexeme.kind = LEXEME_EOF;
}


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
	 * LEXEME_EOF yapalım.
	 *
	 * clear_lexeme'nin tek yaptığı
	 * 	t->clear_lexeme.kind = LEXEME_EOF
	 */
	clear_lexeme(t);
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

//! [tokenizer_feed]
void tokenizer_feed(struct tokenizer *t, struct lexeme lexeme)
{
	assert(t->current_lexeme.kind == LEXEME_EOF &&
	       "Tokenizerda zaten bir lexeme mevcut. Önce tokenizer_next çağırın.");

	t->current_lexeme = lexeme;
}
//! [tokenizer_feed]

//! [tokenizer_ident_id]
/* Null-terminated stringler için kısayol. */
size_t tokenizer_ident_id(struct tokenizer *t, const char *ident)
{
	return tokenizer_ident_id2(t, ident, strlen(ident));
}

size_t tokenizer_ident_id2(struct tokenizer *t,
			  const char *ident, size_t ident_len)
{
	size_t *id_in_map = map_get2(&t->ident_map, ident, ident_len);

	/* Daha önce ID atanmışsa onu dön ya da yenisini ata. */
	if (id_in_map) {
		return *id_in_map;
	} else {
		size_t id = t->last_id++;

		map_insert2(&t->ident_map, ident, ident_len, &id);

		return id;
	}
}
//! [tokenizer_ident_id]

//! [consume_ident]
static struct token consume_ident(struct tokenizer *t)
{
	/* Identifier'ın keyword olup olmadığına bakıyoruz. */
	size_t *keyword_id = map_get2(&t->keywords,
			       t->current_lexeme.seminfo,
			       t->current_lexeme.seminfo_len);

	clear_lexeme(t);
	if (keyword_id) {
		return (struct token) { .id = *keyword_id };
	} else {
		size_t ident_id = tokenizer_ident_id2(t,
				       t->current_lexeme.seminfo,
				       t->current_lexeme.seminfo_len);

		return (struct token) { .id = TK_IDENT, .seminfo.ident_id = ident_id};
	}
}
//! [consume_ident]

//! [consume_punct]
static struct token consume_punct(struct tokenizer *t)
{
	size_t *punct_id;
	size_t i;

	/* Uzun punctuation'lardan başlayarak tek tek punctuation map'ten ID
	 * arar. Örneğin hem `*` hem de `**` operatörü varsa `1 ** 2` ifadesini
	 * uygun şekilde tokenize etmek için uzun sembollerden başlamak
	 * gerekir. */
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

	/* i'in 0'a kadar inmesi, hiçbir punctuation bulunamamıştır demektir. */
	assert(i && "Bilinmeyen punctuation.");

	/* punctuation lexemenin tamamı tüketilmişse current_lexemeyi temizle. */
	if (t->current_lexeme.seminfo_len == 0)
		clear_lexeme(t);

	return (struct token) { .id = *punct_id };
}
//! [consume_punct]

//! [consume_num]
static struct token consume_num(struct tokenizer *t)
{
	char num[t->current_lexeme.seminfo_len + 1];

	/* Lexemeden gelen seminfo, null terminated değildir, devamında diğer
	 * tokenleri içerebilir. Onu bir char[]'a kopyalayarak stringden sayı
	 * dönüşümünü yapıyoruz. */
	num[t->current_lexeme.seminfo_len] = '\0';
	memcpy(num, t->current_lexeme.seminfo, t->current_lexeme.seminfo_len);

	struct token tk;

	if (t->current_lexeme.kind == LEXEME_INT) {
		tk.id = TK_INT;
		tk.seminfo.num_int = strtoimax(num, NULL, 10);
	} else {
		tk.id = TK_FLOAT;
		tk.seminfo.num_float = strtod(num, NULL);
	}

	clear_lexeme(t);
	return tk;
}
//! [consume_num]

//! [tokenizer_next]
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
//! [tokenizer_next]
