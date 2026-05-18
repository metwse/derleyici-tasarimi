#include "../include/rdesc-parser.h"
#include "../include/tokenizer.h"

#include "rdesc-parser.c"

#include <assert.h>

#include <rdesc/grammar.h>
#include <rdesc/rdesc.h>
#include <rdesc/util.h>


int main()
{
//! [rdesc init]
struct rdesc_grammar grammar;
struct rdesc parser;

assert(rdesc_grammar_init_checked(&grammar,
	NT_COUNT, MAX_ALT_COUNT, MAX_ALT_SIZE, production_rules) == 0);

assert(rdesc_init(&parser, &grammar, sizeof(union seminfo), NULL) == 0);
//! [rdesc init]

//! [rdesc pump]
assert(rdesc_start(&parser, NT_STMT) == 0);

union seminfo s;

s.num_int = 10;
assert(rdesc_pump(&parser, TK_INT, &s) == RDESC_CONTINUE);

assert(rdesc_pump(&parser, TK_MINUS, NULL) == RDESC_CONTINUE);

s.num_int = 2;
assert(rdesc_pump(&parser, TK_INT, &s) == RDESC_CONTINUE);

assert(rdesc_pump(&parser, TK_MINUS, NULL) == RDESC_CONTINUE);

s.num_int = 3;
assert(rdesc_pump(&parser, TK_INT, &s) == RDESC_CONTINUE);

assert(rdesc_pump(&parser, TK_SEMI, NULL) == RDESC_READY);
//! [rdesc pump]

//! [rdesc print]
rdesc_dump_cst(stdout, rdesc_get_root(&parser), node_printer);
//! [rdesc print]

//! [rdesc flip&print]
rdesc_flip_left(rdesc_get_root(&parser), 0);

rdesc_dump_cst(stdout, rdesc_get_root(&parser), node_printer);
//! [rdesc flip&print]

	rdesc_destroy(&parser);
	rdesc_grammar_destroy(&grammar);
}
