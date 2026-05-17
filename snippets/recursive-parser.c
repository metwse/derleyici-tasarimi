#include "../include/lexer.h"
#include "../include/recursive-parser.h"
#include "../include/tokenizer.h"

#include "../include/map.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>


//! [Basmakalıp fonksiyonlar]
void parser_init(struct parser *p)
{
	tokenizer_init(&p->tokenizer);

	/* Değişkenlerin değerini map'te tutacağız. */
	map_init(&p->variables, sizeof(double));

	/* Eşleştirilmekte olan token. */
	p->token.id = TK_NOTOKEN;

	/* Dilimizdeki keyword ve punctuation'ları tokenizer'a bildirelim. */
	tokenizer_add_keyword(&p->tokenizer, "print", TK_PRINT);
	tokenizer_add_punctuation(&p->tokenizer, "(", TK_LPAREN);
	tokenizer_add_punctuation(&p->tokenizer, ")", TK_RPAREN);
	tokenizer_add_punctuation(&p->tokenizer, "+", TK_PLUS);
	tokenizer_add_punctuation(&p->tokenizer, "-", TK_MINUS);
	tokenizer_add_punctuation(&p->tokenizer, "*", TK_STAR);
	tokenizer_add_punctuation(&p->tokenizer, "/", TK_SLASH);
	tokenizer_add_punctuation(&p->tokenizer, "=", TK_EQ);
	tokenizer_add_punctuation(&p->tokenizer, ";", TK_SEMI);
}

/* Basmakalıp destroyer fonksiyonu. */
void parser_destroy(struct parser *p)
{
	map_destroy(&p->variables);

	tokenizer_destroy(&p->tokenizer);
}
//! [Basmakalıp fonksiyonlar]

//! [consume/match]
/* Sıradaki token'a geçer okur. */
static void consume(struct parser *p)
{
	p->token = tokenizer_next(&p->tokenizer);

	if (p->token.id == TK_NOTOKEN) {
		struct lexeme next_lexeme = lexer_next(&p->lexer);

		if (next_lexeme.kind == LEXEME_EOF) {
			p->token.id = TK_NOTOKEN;

			return;
		} else {
			tokenizer_feed(&p->tokenizer, next_lexeme);

			return consume(p);
		}
	}
}

/* Beklenen token geldiyse tüketir ve true döner. */
static bool match(struct parser *p, size_t tk_id)
{
	if (p->token.id == tk_id) {
		consume(p);

		return true;
	}

	return false;
}

static size_t current_id(struct parser *p)
{
	return p->token.id;
}

static union seminfo current_seminfo(struct parser *p)
{
	return p->token.seminfo;
}
//! [consume/match]

void eval_print(struct parser *p);
void eval_asgn(struct parser *p, size_t id);
//! [eval]
void parser_eval(struct parser *p, const char *text)
{
	lexer_init(&p->lexer, text);

	/* init fonksiyonundan kalma TK_NOTOKEN'ı consume et. */
	consume(p);

	/* Tokenler bitmediği sürece devam et. */
	do {
		if (match(p, TK_PRINT)) {
			eval_print(p);
			continue;
		}

		size_t id = current_seminfo(p).ident_id;
		if (match(p, TK_IDENT)) {
			eval_asgn(p, id);
			continue;
		}

		assert(0 && "Beklenmeyen token! IDENT ya da PRINT gelmeliydi."); // GCOVR_EXCL_LINE
	} while (current_id(p) != TK_NOTOKEN);
}

/* Bütün production rule'ları fonksiyon olarak tanımlayacağız. */
void eval_print(struct parser *p);
void eval_asgn(struct parser *p, size_t id);
double eval_expr(struct parser *p);
double eval_term(struct parser *p);
double eval_factor(struct parser *p);
double eval_atom(struct parser *p);
//! [eval]

void eval_print(struct parser *p)
{
	size_t id = current_seminfo(p).ident_id;

	assert(match(p, TK_IDENT));
	assert(match(p, TK_SEMI));

	double *value = map_get2(&p->variables, &id, sizeof(size_t));

	printf("> %.2lf\n", value ? *value : 0);
}

void eval_asgn(struct parser *p, size_t id)
{
	assert(match(p, TK_EQ));

	double new_value = eval_expr(p);

	assert(match(p, TK_SEMI));

	double *value = map_get2(&p->variables, &id, sizeof(size_t));

	if (value)
		*value = new_value;
	else
		map_insert2(&p->variables, &id, sizeof(size_t), &new_value);
}

double eval_expr(struct parser *p)
{
	double lhs = eval_term(p);

	if (match(p, TK_PLUS)) {
		double rhs = eval_expr(p);

		lhs += rhs;
	} else if (match(p, TK_MINUS)) {
		double rhs = eval_expr(p);

		lhs -= rhs;
	}

	return lhs;
}

double eval_term(struct parser *p)
{
	double lhs = eval_factor(p);

	if (match(p, TK_STAR)) {
		double rhs = eval_term(p);

		lhs *= rhs;
	} else if (match(p, TK_SLASH)) {
		double rhs = eval_term(p);

		lhs /= rhs;
	}

	return lhs;
}

double eval_factor(struct parser *p)
{
	if (match(p, TK_PLUS))
		return eval_atom(p);
	else if (match(p, TK_MINUS))
		return -eval_atom(p);
	else
		return eval_atom(p);

}

double eval_atom(struct parser *p)
{
	union seminfo s = p->token.seminfo;

	if (match(p, TK_INT)) {
		return (double) s.num_int;
	} else if (match(p, TK_FLOAT)) {
		return s.num_float;
	} else if (match(p, TK_IDENT)) {
		double *value = map_get2(&p->variables, &s.ident_id, sizeof(size_t));

		return value ? *value : 0;
	} else if (match(p, TK_LPAREN)) {
		double expr = eval_expr(p);

		assert(match(p, TK_RPAREN));

		return expr;
	}

	assert(0 && "Syntax error.");  // GCOVR_EXCL_LINE
}
