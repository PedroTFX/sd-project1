/* Módulo tree */
#include <stdlib.h>
#include "data.h"
#include "tree-private.h"
#include <string.h>
#include <stdio.h>

struct tree_t; /* A definir pelo grupo em tree-private.h */

/* Função para criar uma nova árvore tree vazia.
 * Em caso de erro retorna NULL.
 */
struct tree_t *tree_create(){
	struct tree_t *tree = (struct tree_t *)malloc(sizeof(struct tree_t));
	if(tree == NULL){
		return NULL;
	}
	
	return tree;
}

/* Função para libertar toda a memória ocupada por uma árvore.
 */
void tree_destroy(struct tree_t *tree){
	if(tree == NULL){
		return;
	}
	if(tree->node != NULL){
		entry_destroy(tree->node);
	}
	if(tree->tree_left != NULL){
		tree_destroy(tree->tree_left);
		free(tree->tree_left);
	}
	if(tree->tree_right != NULL){
		tree_destroy(tree->tree_right);
		free(tree->tree_right);
	}
}

/* Função para adicionar um par chave-valor à árvore.
 * Os dados de entrada desta função deverão ser copiados, ou seja, a
 * função vai *COPIAR* a key (string) e os dados para um novo espaço de
 * memória que tem de ser reservado. Se a key já existir na árvore,
 * a função tem de substituir a entrada existente pela nova, fazendo
 * a necessária gestão da memória para armazenar os novos dados.
 * Retorna 0 (ok) ou -1 em caso de erro.
 */
int tree_put(struct tree_t *tree, char *key, struct data_t *value){

	struct entry_t *entry = (struct entry_t *)malloc(sizeof(struct entry_t));

	if (entry == NULL) {
		return -1;
	}

	entry->key = (char *)malloc(strlen(key) + 1);
	strcpy(entry->key, key);

	entry->value = data_create(value->datasize);
	memcpy(entry->value->data, value->data, entry->value->datasize);

	//found empty node
	if(tree->node == NULL){
		tree->node = entry_dup(entry);
		return 0;
	}
	//		     node   		               	   tree->node
	//		    /   |		           tree->tree_left 	  tree->tree_right
	//   node_left  node_right    tree->tree_left->node	  tree->tree_right->node
	//
	//printf(" %d", entry_compare(entry, tree->node));
	if(entry_compare(entry, tree->node) == -1){
		tree->tree_left = tree_create();
		tree_put(tree->tree_left, key, value);
	}else if(entry_compare(entry, tree->node) == 1){
		tree->tree_right = tree_create();
		tree_put(tree->tree_right, key, value);
	}else{
		entry_replace(tree->node, key, value);
	}
	return -1;
}

/* Função para obter da árvore o valor associado à chave key.
 * A função deve devolver uma cópia dos dados que terão de ser
 * libertados no contexto da função que chamou tree_get, ou seja, a
 * função aloca memória para armazenar uma *CÓPIA* dos dados da árvore,
 * retorna o endereço desta memória com a cópia dos dados, assumindo-se
 * que esta memória será depois libertada pelo programa que chamou
 * a função. Devolve NULL em caso de erro.
 */
struct data_t *tree_get(struct tree_t *tree, char *key){
	struct entry_t *entry = (struct entry_t *)malloc(sizeof(struct entry_t));
	if (entry == NULL) {
		return NULL;
	}
	
	entry->key = (char *)malloc(strlen(key) + 1);
	strcpy(entry->key, key);

	if(entry_compare(entry, tree->node) == -1){
		return tree_get(tree->tree_left, key);
	}else if(entry_compare(entry, tree->node) == 1){
		return tree_get(tree->tree_right, key);
	}else{
		return tree->node->value;
	}
	return NULL;
}

/* Função para remover um elemento da árvore, indicado pela chave key,
 * libertando toda a memória alocada na respetiva operação tree_put.
 * Retorna 0 (ok) ou -1 (key not found).
 */
int tree_del(struct tree_t *tree, char *key){
	struct data_t *data_to_destroy = tree_get(tree, key);
	if (1 == 1/*data_to_destroy != NULL*/) {
		data_destroy(data_to_destroy);
		return 0;
	} else{
		return -1;
	}
}

/* Função que devolve o número de elementos contidos na árvore.
 */
int tree_size(struct tree_t *tree){
	return tree_size(tree->tree_left) + tree_size(tree->tree_right) + (tree->node != NULL) ? 1 : 0;
}

/* Função que devolve a altura da árvore.
 */
int tree_height(struct tree_t *tree){
	int left = tree_height(tree->tree_left);
	int right = tree_height(tree->tree_right);
	if(tree->node != NULL){
		left++;
		right++;
	}
	return (left < right) ? left : right;
}

/* Função que devolve um array de char* com a cópia de todas as keys da
 * árvore, colocando o último elemento do array com o valor NULL e
 * reservando toda a memória necessária. As keys devem vir ordenadas segundo a ordenação lexicográfica das mesmas.
 */
char **tree_get_keys(struct tree_t *tree){
	return NULL;
}

/* Função que devolve um array de void* com a cópia de todas os values da
 * árvore, colocando o último elemento do array com o valor NULL e
 * reservando toda a memória necessária.
 */
void **tree_get_values(struct tree_t *tree){
	return NULL;
}

/* Função que liberta toda a memória alocada por tree_get_keys().
 */
void tree_free_keys(char **keys){

}

/* Função que liberta toda a memória alocada por tree_get_values().
 */
void tree_free_values(void **values){

}
