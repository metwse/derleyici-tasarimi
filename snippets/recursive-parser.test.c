#include "../src/map.c"  // required-by: tokenizer.c

#include "lexer.c"
#include "recursive-parser.c"
#include "tokenizer.c"


int main()
{
	struct parser p;

	parser_init(&p);

	parser_eval(&p, "a = 1;"
			"b = +2.3 + -3 / (4 * 2) * b + undefined;"
			"a = 3 - 2;"
			"print a;"
			"print d;");

	parser_destroy(&p);
}
