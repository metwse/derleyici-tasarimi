#include "../include/map.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


int main()
{
	srand(time(NULL));

	struct map m;

	for (int _fuzz = 0; _fuzz < 96; _fuzz++) {
		int elem_size = rand() % 16;

		map_init(&m, elem_size);

		char value[elem_size + 1];

		for (int i = 0; i <= elem_size; i++)
			value[i] = rand() % 255;

		int limit = rand() % 4096;
		char values[limit];

		size_t values_total = 0;

		for (int i = 0; i < limit; i++) {
			value[0] = rand() % 255;

			values[i] = value[0];

			// map does not contain the key
			assert(!map_get2(&m, &i, sizeof(int)));

			map_insert2(&m, &i, sizeof(int), value);

			// now it contain
			assert(map_get2(&m, &i, sizeof(int)));

			assert(!memcmp(map_get2(&m, &i, sizeof(int)),
					        value, elem_size));

			values_total += value[0];
		}

		if (elem_size) {
			size_t iter_values_total = 0;

			struct map_it it;

			map_iter(&m, &it);

			struct map_entry *e;
			while ((e = map_iter_next(&it)))
				iter_values_total += e->value[0];

			assert(iter_values_total == values_total);
		}

		int limit2 = limit % (rand() % 2024);
		// remove all even-numbered keys
		for (int i = 0; i < limit2; i += 2) {
			value[0] = values[i];

			// check element
			assert(!memcmp(map_get2(&m, &i, sizeof(int)),
					        value, elem_size));

			// now remove it
			assert(!memcmp(map_delete2(&m, &i, sizeof(int)),
					           value, elem_size));

			// the key should not present
			assert(!map_get2(&m, &i, sizeof(int)));
		}

		int limit3 = limit2 % (rand() % 2024);
		// now insert size_t keyed elements
		for (size_t i = 0; i < (size_t) limit3; i += 4)
			map_insert2(&m, &i, sizeof(size_t), &i);

		// check size_t keyed elements
		for (size_t i = 0; i < (size_t) limit3; i += 4)
			assert(!memcmp(map_get2(&m, &i, sizeof(size_t)),
					        &i, elem_size));

		map_insert(&m, "inserting string", "the string value");
		assert(map_get(&m, "inserting string"));
		assert(!memcmp(map_delete(&m, "inserting string"),
			       "the string value",
			       elem_size));
		assert(!map_delete(&m, "inserting string"));

		map_destroy(&m);
	}

	return EXIT_SUCCESS;
}
