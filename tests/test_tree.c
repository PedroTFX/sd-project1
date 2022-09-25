#define NDEBUG

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

#include "data.h"
#include "entry.h"
#include "tree.h"
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
	printf("here\n");
	for(i=0; i<1024; i++) {
		key[i] = (char*)malloc(16*sizeof(char));
		sprintf(key[i],"a/key/b-%d",i);
		data[i] = data_create2(strlen(key[i])+1,strdup(key[i]));
		tree_put(tree,key[i],data[i]);

	}
	printf("here\n");

	assert(tree_size(tree) == 1024);
	result = (tree_size(tree) == 1024);
	
	printf("here\n");
	
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

	tree_free_keys(keys);

	tree_destroy(tree);

	printf("tree - testGetKeys: %s\n",result?"passou":"não passou");
	return result;
}

/**************************************************************/

int mainmain()
{
	setbuf(stdout, NULL);
	printf("here\n");
	
	struct tree_t* tree = tree_create();
	printf("here\n");
	char* key1 = malloc(sizeof(char) * 5);
	char* key2 = malloc(sizeof(char) * 5);
	char* key3 = malloc(sizeof(char) * 5);
	char* key4 = malloc(sizeof(char) * 5);
	char* key5 = malloc(sizeof(char) * 5);
	printf("here\n");
	sprintf(key1,"a1");
	sprintf(key2,"a23");
	sprintf(key3,"a3");
	sprintf(key4,"a21");
	sprintf(key5,"a5");
	printf("here\n");
	struct data_t* data1 = data_create2(strlen(key1)+1,strdup(key1));
	struct data_t* data2 = data_create2(strlen(key2)+1,strdup(key2));
	struct data_t* data3 = data_create2(strlen(key3)+1,strdup(key3));
	struct data_t* data4 = data_create2(strlen(key4)+1,strdup(key4));
	struct data_t* data5 = data_create2(strlen(key5)+1,strdup(key5));
	printf("here\n");

	tree_put(tree, key1, data1);
	printf("1");
	tree_put(tree, key4, data4);
	printf("2");
	tree_put(tree, key3, data3);
	printf("3");
	tree_put(tree, key2, data2);
	printf("4");
	tree_put(tree, key5, data5);
	printf("5\n");

	free(key1);
	free(key2);
	free(key3);
	free(key4);
	free(key5);

	tree_destroy(tree);
	
	tree_destroy(tree);
	return 0;
}

int main() {
	int score = 0;
	setbuf(stdout, NULL);

	printf("iniciando teste tree bin\n");

	//mainmain();

	score += testTreeVazia();

	score += testPutInexistente();

	score += testPutExistente();

	//score += testDelInexistente();

	//score += testDelExistente();

	//score += testGetKeys();
	
	//aqui tmb pode ser adicionado um teste para o método tree_get_values

	printf("teste tree bin: %d/6\n",score);

    if (score == 6)
        return 0;
    else
        return -1;
}



