/**
 * @file recursive-parser.h
 * @brief @ref td_parser_page
 */

#ifndef PARSER_H
#define PARSER_H


#include "lexer.h"
#include "map.h"
#include "tokenizer.h"

#include <stddef.h>


/** @brief Parser. */
//! [Parser tanımı]
struct parser {
	/** @brief Keyword ve punctuation'ımızı kaydettiğimiz tokenizer. */
	struct tokenizer tokenizer;

	/** @brief Tanımlı değişkenler. */
	struct map variables;

	/** @brief Eşleştirilen token. */
	struct token token;
	/** @brief Metin girdisini saran lexer. */
	struct lexer lexer;
};
//! [Parser tanımı]

/** @brief Token ID'leri. */
//! [Token ID'leri]
enum token_id {
	TK_PRINT,
	TK_LPAREN, TK_RPAREN,
	TK_PLUS, TK_MINUS,
	TK_STAR, TK_SLASH,
	TK_EQ,
	TK_SEMI,
};
//! [Token ID'leri]


/** @brief Yeni bir parser oluşturur.*/
//! [Parser'ın işlevleri]
void parser_init(struct parser *parser);

/** @brief Metin girdisini *evaluate* eder.*/
void parser_eval(struct parser *parser, const char *text);
//! [Parser'ın işlevleri]

/** @brief Parser'ın kullandığı belleği temizler.*/
void parser_destroy(struct parser *parser);


#endif
