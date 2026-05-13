/**
 * @file tokenizer.h
 * @brief @ref tokenizer_page
 */

#ifndef TOKENIZER_H
#define TOKENIZER_H


#include "lexer.h"

#include "map.h"

#include <stddef.h>
#include <stdint.h>


/** @brief @ref tokenizer_feed() ile verilen lexemenin bittiğini belirtir. */
static const size_t TK_NOTOKEN = -1;

/** @brief Tam sayı token ID'si. */
static const size_t TK_INT = -2;

/** @brief Ondalık sayı token ID'si. */
static const size_t TK_FLOAT = -3;

/** @brief Identifier token ID'si. */
static const size_t TK_IDENT = -4;


/** @brief tokenizer. */
//! [Tokenizer tanımı]
struct tokenizer {
	/** @brief Identifier'a karşılık atanan sayısal ID. */
	struct map ident_map;
	/** @brief Identifier'lara eşsiz ID'ler ataybilmek için yardımcı sayaç. */
	size_t last_id;

	/** @brief Yazılım dilindeki keyword'ler. */
	struct map keywords;
	/** @brief Yazılım dilindeki punctuation'lar (sembol ve operatörler). */
	struct map punctuations;

	/** @brief İşlenmekte olan lexeme. */
	struct lexeme current_lexeme;
};
//! [Tokenizer tanımı]

/** @brief Token. */
//! [Token tanımı]
struct token {
	/** @brief Tokenlerin ekstra semantic bilgisi. */
	union {
		/** @brief TK_FLOAT tipi için seminfo. */
		double num_float;
		/** @brief TK_INT tipi için seminfo. */
		intmax_t num_int;
		/** @brief TK_IDENT tipi için seminfo. */
		size_t ident_id;
	} seminfo;

	/** @brief Token tipini ifade eden özgün ID. */
	size_t id;
};
//! [Token tanımı]


/** @brief Yeni bir tokenizer oluşturur. */
void tokenizer_init(struct tokenizer *tokenizer);

/** @brief Tokenizerın tahsis ettiği belleği temizler. */
void tokenizer_destroy(struct tokenizer *tokenizer);

/** @brief Tokenizera bir keyword kaydeder. */
void tokenizer_add_keyword(struct tokenizer *tokenizer,
			   const char *keyword, size_t id);

/** @brief Tokenizera bir punctuation kaydeder.. */
void tokenizer_add_punctuation(struct tokenizer *tokenizer,
			       const char *punctuation, size_t id);

/**
 * @brief Identifier'a ait ID'yi döner.
 *
 * Hâlihazırda bir ID atanmamışsa identifier için yeni bir ID üretir.
 */
size_t tokenizer_ident_id2(struct tokenizer *tokenizer,
			   const char *ident, size_t ident_len);

/** @brief Identifier'a ait ID'yi döner. */
size_t tokenizer_ident_id(struct tokenizer *tokenizer, const char *ident);

/** @brief Lexemeyi tokenizere gönderir. */
//! [Tokenizerın esas işlevleri]
void tokenizer_feed(struct tokenizer *tokenizer, struct lexeme lexeme);

/** @brief Sıradaki tokeni çek. */
struct token tokenizer_next(struct tokenizer *tokenzier);
//! [Tokenizerın esas işlevleri]


#endif
