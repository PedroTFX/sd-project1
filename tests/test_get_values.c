#include <stdio.h>
#include "tree.h"
#include "tree-private.h"
#include "data.h"
#include "entry.h"

int main(int argc, char const *argv[]) {
	struct data_t* data = data_create2(10, "joao");
	struct data_t *data1 = data_create2(9, "joaob");
	struct data_t *data2 = data_create2(8, "joaoc");
	struct data_t* data3 = data_create2(7, "joaod");
	struct data_t *data4 = data_create2(6, "joaoe");

	struct tree_t* tree = tree_create();

	tree_put(tree, "joao", data);
	tree_put(tree, "joaob", data1);
	tree_put(tree, "joaoc", data2);
	tree_put(tree, "joaod", data3);
	tree_put(tree, "joaoe", data4);


	void ** values = tree_get_values(tree);

	for (int i = 0; values[i] != NULL; i++) {
		printf(values[i]);
	}



	return 0;
}

