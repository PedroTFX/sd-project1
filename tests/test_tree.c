
#define NDEBUG

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "data.h"
#include "entry.h"
#include "tree.h"
#include "tree-private.h"

/**************************************************************/
int testTreeVazia() {
	struct tree_t *tree = tree_create();

	int result = tree != NULL && tree_size(tree) == 0;
	tree_destroy(tree);

	printf("tree - testTreeVazia: %s\n",result?"passou":"não passou");
	return result;
}

/**************************************************************/

int testPutInexistente() {
	int result, i;
	struct tree_t *tree = tree_create();
	char *key[1024];
	struct data_t *data[1024], *d;
	for(i=0; i<1024; i++) {
		key[i] = (char*)malloc(16*sizeof(char));
		sprintf(key[i],"a/key/b-%d",i);
		data[i] = data_create2(strlen(key[i])+1,strdup(key[i]));

		tree_put(tree,key[i],data[i]);
	}

	assert(tree_size(tree) == 1024);
	result = (tree_size(tree) == 1024);
		for(i=0; i<1024; i++) {
		d = tree_get(tree,key[i]);

		assert(d->datasize == data[i]->datasize);
		assert(memcmp(d->data,data[i]->data,d->datasize) == 0);
		assert(d->data != data[i]->data);

		result = result && (d->datasize == data[i]->datasize &&
                           memcmp(d->data,data[i]->data,d->datasize) == 0 &&
                           d->data != data[i]->data);
		data_destroy(d);
	}
	for(i=0; i<1024; i++) {
		free(key[i]);
		data_destroy(data[i]);
	}
	tree_destroy(tree);

	printf("tree - testPutInexistente: %s\n",result?"passou":"não passou");
	return result;
}

/**************************************************************/

int testPutExistente() {
	int result, i;
	struct tree_t *tree = tree_create();
	char *key[1024];
	struct data_t *data[1024], *d;
	for(i=0; i<1024; i++) {
		key[i] = (char*)malloc(16*sizeof(char));
		sprintf(key[i],"a/key/b-%d",i);
		data[i] = data_create2(strlen(key[i])+1,strdup(key[i]));

		tree_put(tree,key[i],data[i]);
	}


	assert(tree_size(tree) == 1024);
	result = (tree_size(tree) == 1024);



	d = data_create2(strlen("256")+1,strdup("256"));
	tree_put(tree,key[256],d);
	data_destroy(d);

	assert(tree_size(tree) == 1024);
	result = result && (tree_size(tree) == 1024);

	for(i=0; i<1024; i++) {
		d = tree_get(tree,key[i]);

		if(i==256) {
			result = result && (d->datasize == strlen("256")+1 &&
        	                   memcmp(d->data,"256",d->datasize) == 0);
		} else {
			result = result && (d->datasize == data[i]->datasize &&
        	                   memcmp(d->data,data[i]->data,d->datasize) == 0 &&
        	                   d->data != data[i]->data);
		}

		data_destroy(d);
	}

	for(i=0; i<1024; i++) {
		free(key[i]);
		data_destroy(data[i]);
	}

	tree_destroy(tree);

	printf("tree - testPutExistente: %s\n",result?"passou":"não passou");
	return result;
}

/**************************************************************/

int testDelInexistente() {
	int result, i;
	struct tree_t *tree = tree_create();
	char *key;
	struct data_t *data;

	for(i=0; i<1024; i++) {
		key = (char*)malloc(16*sizeof(char));
		sprintf(key,"a/key/b-%d",i);
		data = data_create2(strlen(key)+1,key);

		tree_put(tree,key,data);

		data_destroy(data);
	}

	assert(tree_size(tree) == 1024);
	result = (tree_size(tree) == 1024);

	result = result && (tree_get(tree,"a/key/b-1024") == NULL) &&
			   (tree_get(tree,"abc") == NULL);

	result = result && (tree_del(tree,"a/key/b-1024") != 0) &&
			   (tree_del(tree,"abc") != 0);

	result = result && (tree_get(tree,"a/key/b-1024") == NULL) &&
			   (tree_get(tree,"abc") == NULL);

	assert(tree_size(tree) == 1024);
	result = result && (tree_size(tree) == 1024);

	tree_destroy(tree);

	printf("tree - testDelInexistente: %s\n",result?"passou":"não passou");
	return result;
}

/**************************************************************/

int testDelExistente() {
	int result, i;
	struct tree_t *tree = tree_create();
	char *key;
	struct data_t *data, *data2 = NULL;

	for(i=0; i<1024; i++) {
		key = (char*)malloc(16*sizeof(char));
		sprintf(key,"a/key/b-%d",i);
		data = data_create2(strlen(key)+1,key);

		tree_put(tree,key,data);

		data_destroy(data);
	}

	assert(tree_size(tree) == 1024);
	result = (tree_size(tree) == 1024);

	result = result && ((data = tree_get(tree,"a/key/b-1023")) != NULL) &&
			   ((data2 = tree_get(tree,"a/key/b-45")) != NULL);

	data_destroy(data);
	data_destroy(data2);

	result = result && (tree_del(tree,"a/key/b-1023") == 0) &&
			   (tree_del(tree,"a/key/b-45") == 0);

	result = result && (tree_get(tree,"a/key/b-1023") == NULL) &&
			   (tree_get(tree,"a/key/b-45") == NULL);

	assert(tree_size(tree) == 1022);
	result = result && (tree_size(tree) == 1022);

	tree_destroy(tree);

	printf("tree - testDelExistente: %s\n",result?"passou":"não passou");
	return result;
}

/**************************************************************/
//este teste não verifica se as keys são retornadas por ordem lexicografica
//no entanto, pode ser facilmente modificado para testar isso
int testGetKeys() {
	int result = 1,i,j,achou;
	struct tree_t *tree = tree_create();
	char **keys;
	char *k[4] = {"abc","bcd","cde","def"};
	struct data_t *d = data_create(5);

	tree_put(tree,k[3],d);
	tree_put(tree,k[2],d);
	tree_put(tree,k[1],d);
	tree_put(tree,k[0],d);

	data_destroy(d);

	keys = tree_get_keys(tree);

	for(i=0; keys[i] != NULL; i++) {
		achou = 0;
		for(j=0; j<4; j++) {
			achou = (achou || (strcmp(keys[i],k[j]) == 0));
		}
		result = (result && achou);
	}

	result = result && (tree_size(tree) == i);

	ordena(keys);

 	for (i = 1; keys[i] != NULL; i++) {
		result = result && (strcmp(keys[i - 1], keys[i]) <= 0);
	}

	tree_free_keys(keys);

	tree_destroy(tree);

	printf("tree - testGetKeys: %s\n",result?"passou":"não passou");
	return result;
}

/**************************************************************/
int testGetValues() {
	int result = 1;
	struct tree_t *tree = tree_create();

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
	tree_put(tree, keys[4], data4);

	struct data_t **tree_values = (struct data_t **) tree_get_values(tree);

	for (int i = 0; tree_values[i] != NULL; i++) {
		int size = sizeof(values) / sizeof(values[0]);
		result = result && found_str_in_array(tree_values[i]->data, values, size);
	}

	tree_free_values((void **)tree_values);

	tree_destroy(tree);

	printf("tree - testGetValues: %s\n", result ? "passou" : "não passou");

	return result;
}

int found_str_in_array(char* string, char** array, int size) {
	return index_of_string_in_array(string, array, size) > -1;
}

int index_of_string_in_array(char* string, char** array, int size) {
	for (int i = 0; i < size; i++) {
		if(strcmp(string, array[i]) == 0) {
			return i;
		}
	}
	return -1;
}

/***********************************************************/
int main() {
	int score = 0;
	setbuf(stdout, NULL);

	printf("iniciando teste tree bin\n");

	score += testTreeVazia();

	score += testPutInexistente();

	score += testPutExistente();

	score += testDelInexistente();

	score += testDelExistente();

	score += testGetKeys();

	score += testGetValues();

	printf("teste tree bin: %d/7\n", score);

    if (score == 7)
        return 0;
    else
        return -1;
}

