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
static size_t current_id(struct parser *p)
{
	return p->token.id;
}

static union seminfo current_seminfo(struct parser *p)
{
	return p->token.seminfo;
}

/* Sıradaki token'a geçer. */
static void consume(struct parser *p)
{
	p->token = tokenizer_next(&p->tokenizer);

	/* Tokenizer, lexeme'yi tüketmişse yeni lexeme çek. */
	if (current_id(p) == TK_NOTOKEN) {
		struct lexeme next_lexeme = lexer_next(&p->lexer);

		/* Lexeme'ler tükenmişse token TK_NOTOKEN olarak işaretle. */
		if (next_lexeme.kind == LEXEME_EOF) {
			p->token.id = TK_NOTOKEN;

			return;
		} else {
			/* Yeni lexeme gelmişse ona geçmek için bu fonksiyonu
			 * tekrar çağır. */
			tokenizer_feed(&p->tokenizer, next_lexeme);

			return consume(p);
		}
	}
}

/* Beklenen token geldiyse tüketir ve true döner. */
static bool match(struct parser *p, size_t tk_id)
{
	if (current_id(p) == tk_id) {
		consume(p);

		return true;
	}

	return false;
}
//! [consume/match]

void eval_print(struct parser *p);
void eval_asgn(struct parser *p);
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

		if (current_id(p) == TK_IDENT) {
			eval_asgn(p);
			continue;
		}

		assert(0 && "Beklenmeyen token! IDENT ya da PRINT gelmeliydi."); // GCOVR_EXCL_LINE
	} while (current_id(p) != TK_NOTOKEN);
}

/* Bütün production rule'ları fonksiyon olarak tanımlayacağız. */
void eval_print(struct parser *p);
void eval_asgn(struct parser *p);
double eval_expr(struct parser *p);
double eval_term(struct parser *p);
double eval_factor(struct parser *p);
double eval_atom(struct parser *p);
//! [eval]

//! [print/asgn]
void eval_print(struct parser *p)
{
	/* print'in hemen sonrasında bir expression olmalı. */
	double value = eval_expr(p);

	/* expression bitiminde ; olduğundan emin ol. */
	assert(match(p, TK_SEMI));

	printf("> %.2lf\n", value);
}

void eval_asgn(struct parser *p)
{
	size_t id = current_seminfo(p).ident_id;
	consume(p);

	/* eval fonksiyonunda id match etmiştik, ondan sonra = olmalı. */
	assert(match(p, TK_EQ));

	/* ='den sonraki expression'u evaluate et ve değişkene ata. */
	double new_value = eval_expr(p);

	assert(match(p, TK_SEMI));

	double *value = map_get2(&p->variables, &id, sizeof(size_t));

	if (value)
		*value = new_value;
	else
		map_insert2(&p->variables, &id, sizeof(size_t), &new_value);
}
//! [print/asgn]

//! [expr/term/factor]
double eval_expr(struct parser *p)
{
	double lhs = eval_term(p);

	/* expression, toplama ya da çıkarma içeriyorsa dallanmaya devam et. */
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

	/* term, expression'a benzer şekilde dallanır. */
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
//! [expr/term/factor]

//! [atom]
double eval_atom(struct parser *p)
{
	/* `match` fonksiyonu, bir sonraki tokene geçeceği için seminfo'yu
	 * kaydet. */
	union seminfo s = current_seminfo(p);

	if (match(p, TK_INT)) {
		return (double) s.num_int;
	} else if (match(p, TK_FLOAT)) {
		return s.num_float;
	} else if (match(p, TK_IDENT)) {
		double *value = map_get2(&p->variables,
					 &s.ident_id,
					 sizeof(size_t));

		return value ? *value : 0;
	} else if (match(p, TK_LPAREN)) {
		double expr = eval_expr(p);

		assert(match(p, TK_RPAREN));

		return expr;
	}

	assert(0 && "Syntax error.");  // GCOVR_EXCL_LINE
}
//! [atom]
