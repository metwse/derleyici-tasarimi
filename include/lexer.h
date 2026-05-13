/**
 * @file lexer.h
 * @brief @ref lexer_page
 */

#ifndef LEXER_H
#define LEXER_H


#include <stddef.h>


/** @brief lexer. */
struct lexer {
	const char *start  /** Sıradaki tokeni toplamaya başlayacak pointer. */;
	const char *current  /** Token toplama esnasında yardımcı pointer. */;
};

/** @brief Ham lexer çıktısı. */
//! [Lexeme]
struct lexeme {
	/** Tokenin string karşılığı. */
	const char *seminfo;

	/** Seminfo uzunluğu. */
	size_t seminfo_len;

	/** Tokenin tipi. */
	enum lexeme_kind {
		/** Daha fazla lexeme yok. */
		LEXEME_EOF,
		/** Tam sayı. */
		LEXEME_INT,
		/** Ondalık sayı. */
		LEXEME_FLOAT,
		/** Identifier. */
		LEXEME_IDENT,
		/** Punctuation. */
		LEXEME_PUNCT,
	} kind/** _ */;
};
//! [Lexeme]


/** @brief Yeni bir lexer oluşturur. */
//! [Lexer fonksiyonları]
void lexer_init(struct lexer *lexer, const char *text);

/** @brief Sıradaki lexeme. */
struct lexeme lexer_next(struct lexer *lexer);
//! [Lexer fonksiyonları]


#endif
