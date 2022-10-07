#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "data.h"
#include "entry.h"
#include "tree.h"
#include "tree-private.h"
#include "serialization.h"
#include "serialization-private.h"

int testSerialization(){
	int result = 1;
	struct tree_t *tree = tree_create();
/*
	char *keys[5] = {"joaoa", "joaob", "joaoc", "joaod", "joaoe"};
	char *values[5] = {"santosa", "santosb", "santosc", "santosd", "santose"};

 	struct data_t *data0 = data_create2(strlen(values[0]) + 1, values[0]);
	struct data_t *data1 = data_create2(strlen(values[1]) + 1, values[1]);
	struct data_t *data2 = data_create2(strlen(values[2]) + 1, values[2]);
	struct data_t *data3 = data_create2(strlen(values[3]) + 1, values[3]);
	struct data_t *data4 = data_create2(strlen(values[4]) + 1, values[4]);

	tree_put(tree, keys[0], data0);
	tree_put(tree, keys[1], data1);
	tree_put(tree, keys[2], data2);
	tree_put(tree, keys[3], data3);
	tree_put(tree, keys[4], data4); */
	char * key;
    struct data_t* data;

    for(int i = 0; i < 100;i++){
        key = (char * )malloc(16 * sizeof(char));
        sprintf(key,"a/key/b-%d",i);
        data = data_create2(strlen(key)+1,key);
        tree_put(tree,key,data);
        data_destroy(data);
    }

	char** tree_keys= (char** )tree_get_keys(tree);
	char* buffer;
	int buffer_size = keyArray_to_buffer(tree_keys, &buffer);
	char **new_tree_keys = buffer_to_keyArray(buffer, buffer_size);

	for (int i = 0; i < 5; i++) {
		result = result && (strcmp(tree_keys[i], new_tree_keys[i]) == 0);
	}

	for (int i = 0; new_tree_keys[i]; i++) {
		free(new_tree_keys[i]);
	}
	for (int i = 0; tree_keys[i]; i++) {
		free(tree_keys[i]);
	}


	free(buffer);
	free(tree_keys);
	free(new_tree_keys);
	tree_destroy(tree);

	printf("tree - test_Serialization: %s\n", result ? "passou" : "não passou");
	return result;
}

int main(int argc, char const *argv[])
{
	int score = 0;

	printf("iniciando o teste serialization bin\n");

	score += testSerialization();

	printf("teste serialization bin: %d/1\n",score);

	return 0;
}
