#include "../include/rdesc-parser.h"
#include "../include/tokenizer.h"

#include <stdio.h>

//! [Gramer tablosu]
#include <rdesc/rule_macros.h>
#include <string.h>

#include "../include/tokenizer.h" // IWYU pragma: keep, TK_INT için


struct rdesc_grammar_symbol
	production_rules[NT_COUNT][MAX_ALT_COUNT + 1][MAX_ALT_SIZE + 1] = {
    /* ⟨S⟩ → ⟨Expr⟩ ; */
    r(
        NT(EXPR), TK(SEMI)
    ),

    /* ⟨Expr⟩ → ⟨Term⟩ ⟨ExprRest⟩ */
    r(
        NT(TERM), NT(EXPR_REST)
    ),

    /* ⟨ExprRest⟩ → - ⟨Term⟩ ⟨ExprRest⟩ | ε */
    r(
        TK(MINUS), NT(TERM), NT(EXPR_REST)
    alt EPSILON
    ),

    /* ⟨Term⟩ → ⟨Atom⟩ ⟨TermRest⟩ */
    r(
        NT(ATOM), NT(TERM_REST)
    ),

    /* ⟨TermRest⟩ → * ⟨Atom⟩ ⟨TermRest⟩ | ε */
    r(
        TK(STAR), NT(ATOM), NT(TERM_REST)
    alt EPSILON
    ),

    /* ⟨Atom⟩ → num */
    r(
        TK(INT)
    )
};
//! [Gramer tablosu]

#include <rdesc/cst_macros.h>


//! [Node print]
const char *tk_names[] = {
	"-", "*", ";",
};

const char *nt_names[] = {
	"stmt",
	"expr", "expr_rest",
	"term", "term_rest",
	"atom",
};

void node_printer(FILE *out, struct rdesc_node node)
{
	if (rtype(node) == RDESC_TOKEN) {
		if (rid(node) == TK_INT) {
			union seminfo s;

			memcpy(&s, rseminfo(node), sizeof(union seminfo));

			fprintf(out, "[shape=box,label=<%lld>]", (long long int) s.num_int);
		} else {
			fprintf(out, "[shape=box,label=<%s>]", tk_names[rid(node)]);
		}
	} else {
		fprintf(out, "[label=\"%s\"]", nt_names[rid(node)]);
	}
}
//! [Node print]
