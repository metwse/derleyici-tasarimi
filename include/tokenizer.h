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


/** @brief tokenizer. */
struct tokenizer {
	/** @brief Identifiere karşılık atanan sayısal ID. */
	struct map ident_map;
	/** @brief Identifierlere eşsiz ID'ler ataybilmek için yardımcı sayaç. */
	size_t last_id;

	/** @brief Yazılım dilindeki keywordler. */
	struct map keywords;
	/** @brief Yazılım dilindeki sembol ve operatörler. */
	struct map punctuations;

	/** @brief İşlenmekte olan lexeme. */
	struct lexeme current_lexeme;
};

/** @brief Token. */
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


/** Herhangi bir token bulunamadı. */
static const size_t TK_NOTOKEN = -1;

/** Tam sayı token ID'si. */
static const size_t TK_INT = -2;

/** Ondalık token ID'si. */
static const size_t TK_FLOAT = -3;

/** Identifier token ID'si. */
static const size_t TK_IDENT = -4;


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

/** @brief Lexemeyi tokenizere gönderir. */
void tokenizer_feed(struct tokenizer *tokenizer, struct lexeme lexeme);

/** @brief Sıradaki tokeni çek. */
struct token tokenizer_next(struct tokenizer *tokenzier);


#endif
