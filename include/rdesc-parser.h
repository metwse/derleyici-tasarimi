/**
 * @file rdesc-parser.h
 * @brief @ref rdesc_page
 */

#ifndef RDESC_PARSER_H
#define RDESC_PARSER_H


//! [Gramer declaration]
#include <rdesc/grammar.h>

/**_*/
/* toplam nonterminal sayısı */
#define NT_COUNT 5
/**_*/
/* bir nonterminal'deki maksimum alternatif sayısı */
#define MAX_ALT_COUNT 2
/**_*/
/* bir alternatifdeki maksimum sembol sayısı */
#define MAX_ALT_SIZE 3

/** @brief Token ID. */
enum tk_id {
	TK_MINUS,
	TK_STAR
	/* TK_INT'i tokenizer tanımlıyor. */
};

/** @brief Nonterminal ID. */
enum nt_id {
	NT_EXPR, NT_EXPR_REST,
	NT_TERM, NT_TERM_REST,
	NT_ATOM
};

/**_*/
/* Gramer tanımını tutacak struct'ı declare ediyoruz.*/
extern struct rdesc_grammar_symbol grammar
	[NT_COUNT][MAX_ALT_COUNT + 1][MAX_ALT_SIZE + 1];
//! [Gramer declaration]


#endif
