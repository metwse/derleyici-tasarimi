#include "../include/rdesc-parser.h"

//! [Gramer tablosu]
#include <rdesc/rule_macros.h>

#include "../include/tokenizer.h" // IWYU pragma: keep, TK_INT için


struct rdesc_grammar_symbol
	grammar[NT_COUNT][MAX_ALT_COUNT + 1][MAX_ALT_SIZE + 1] = {
    /* ⟨Expr⟩ → ⟨Term⟩ ⟨Expr_Rest⟩ */
    r(
        NT(TERM), NT(EXPR_REST)
    ),

    /* ⟨Expr_Rest⟩ → - ⟨Term⟩ ⟨Expr_Rest⟩ | ε */
    r(
        TK(MINUS), NT(TERM), NT(EXPR_REST)
    alt EPSILON
    ),

    /* ⟨Term⟩ → ⟨Atom⟩ ⟨Term_Rest⟩ */
    r(
        NT(ATOM), NT(TERM_REST)
    ),

    /* ⟨Term_Rest⟩ → * ⟨Atom⟩ ⟨Term_Rest⟩ | ε */
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
