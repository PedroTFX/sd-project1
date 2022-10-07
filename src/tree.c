
/* Módulo tree */
#include <stdlib.h>
#include <data.h>
#include <tree.h>
#include <tree-private.h>
#include <string.h>
#include <stdio.h>

/* Função para criar uma nova árvore tree vazia.
 * Em caso de erro retorna NULL.
 */
struct tree_t* tree_create() {
	struct tree_t* tree = (struct tree_t*) malloc(sizeof(struct tree_t));
	if(tree == NULL){
		return NULL;
	}
	memset(tree, 0, sizeof(struct tree_t));
	return tree;
}


/* Função para libertar toda a memória ocupada por uma árvore.
 */
void tree_destroy(struct tree_t *tree){
	if(tree){
		if(tree->node){
			entry_destroy(tree->node);
		}
		if(tree->tree_left){
			tree_destroy(tree->tree_left);
		}
		if(tree->tree_right){
			tree_destroy(tree->tree_right);
		}
		free(tree);
	}
}

/**
 * @brief Find the tree object from given key
 *
 * @param tree
 * @param key
 * @return struct entry_t* || NULL if not found
 */
struct tree_t* get_tree(struct tree_t* tree, char* key){
	while(tree->node != NULL) {
		int cmp = strcmp(key, tree->node->key);
		if(cmp < 0) {
			if(tree->tree_left == NULL) {
				break;
			}
			tree = tree->tree_left;
		} else if(cmp > 0) {
			if(tree->tree_right == NULL) {
				break;
			}
			tree = tree->tree_right;
		} else {
			return tree;
		}
	}
	return NULL;
}

/* Função para adicionar um par chave-valor à árvore.
 * Os dados de entrada desta função deverão ser copiados, ou seja, a
 * função vai *COPIAR* a key (string) e os dados para um novo espaço de
 * memória que tem de ser reservado. Se a key já existir na árvore,
 * a função tem de substituir a entrada existente pela nova, fazendo
 * a necessária gestão da memória para armazenar os novos dados.
 * Retorna 0 (ok) ou -1 em caso de erro.
 */
int tree_put(struct tree_t *tree, char *key, struct data_t *value) {
	if(tree->node == NULL) {
		struct entry_t* entry = entry_create(key, value);
		tree->node = entry_dup(entry);
		free(entry);
		return 0;
	}

	int cmp = strcmp(key, tree->node->key);
	if(cmp < 0) {
		if(tree->tree_left == NULL) {
			tree->tree_left = tree_create();
		}
		return tree_put(tree->tree_left, key, value);
	} else if(cmp > 0) {
		if(tree->tree_right == NULL) {
			tree->tree_right = tree_create();
		}
		return tree_put(tree->tree_right, key, value);
	} else if(cmp == 0) {
		entry_destroy(tree->node);
		tree->node = NULL;
		return tree_put(tree, key, value);
	}
	return 0;
}


/* Função para obter da árvore o valor associado à chave key.
 * A função deve devolver uma cópia dos dados que terão de ser
 * libertados no contexto da função que chamou tree_get, ou seja, a
 * função aloca memória para armazenar uma *CÓPIA* dos dados da árvore,
 * retorna o endereço desta memória com a cópia dos dados, assumindo-se
 * que esta memória será depois libertada pelo programa que chamou
 * a função. Devolve NULL em caso de erro.
 */
struct data_t* tree_get(struct tree_t *tree, char *key){
	struct tree_t* tree_st = get_tree(tree, key);
	return tree_st ? data_dup(tree_st->node->value) : NULL;
}

/* Função que devolve o número de elementos contidos na árvore. */
int tree_size(struct tree_t *tree){
	return tree == NULL ? 0 : (tree->node != NULL) + tree_size(tree->tree_left) + tree_size(tree->tree_right);
}


struct tree_t* tree_dup(struct tree_t* tree){
	struct tree_t* tree_st = tree_create();
	if(tree){
		if(tree->node){
			tree_st->node = entry_dup(tree->node);
		}
		if(tree->tree_left){
			tree_st->tree_left = tree_dup(tree->tree_left);
		}
		if(tree->tree_right){
			tree_st->tree_right = tree_dup(tree->tree_right);
		}
	}
	return tree_st;
}

void tree_replace(struct tree_t* dest, struct tree_t* src){
	struct tree_t* temp = tree_dup(src);
	tree_destroy(dest);
	dest = temp;
}

struct entry_t* min(struct tree_t* tree){
	struct tree_t* temp = tree;
	while(temp->tree_left) {
		temp = temp->tree_left;
	}
	return temp->node;
}

void print_tree(struct tree_t* tree){
	if(tree->tree_left){
		print_tree(tree->tree_left);
	}
	if(tree->node){
		printf("%s\n", tree->node->key);
	}

	if(tree->tree_right){
		print_tree(tree->tree_right);
	}

}

/* Função para remover um elemento da árvore, indicado pela chave key,
 * libertando toda a memória alocada na respetiva operação tree_put.
 * Retorna 0 (ok) ou -1 (key not found).
 */
int tree_del(struct tree_t *tree, char *key){

	struct tree_t* sub_tree = get_tree(tree, key);
	if(!sub_tree){
		return -1;
	}

	if(tree_size(sub_tree) == 1){ //works				//if the PD is a leaf
		sub_tree->node = NULL;
		sub_tree = NULL;
		return 0;

	}else if(sub_tree->tree_left && sub_tree->tree_right){		//if the PD is not a leaf and has two trees duplicate the next biggest value(tree_right)
		struct entry_t* tree_min = min(sub_tree->tree_right);
		entry_replace(sub_tree->node, tree_min->key, tree_min->value);
		tree_del(sub_tree->tree_right, tree_min->key);
	}else if(sub_tree->tree_left){
		entry_replace(sub_tree->node, sub_tree->tree_left->node->key, sub_tree->tree_left->node->value);
		tree_del(sub_tree->tree_left, sub_tree->tree_left->node->key);

	}else if(sub_tree->tree_right){
		entry_replace(sub_tree->node, sub_tree->tree_right->node->key, sub_tree->tree_right->node->value);
		tree_del(sub_tree->tree_right, sub_tree->tree_right->node->key);
	}

	return 0;
}

/* Função que devolve a altura da árvore.
 */
int tree_height(struct tree_t *tree){
	if(tree == NULL){
		return 0;
	}

	int left = tree_height(tree->tree_left) + 1;
	int right = tree_height(tree->tree_right) + 1;

	return (left > right) ? left : right;
}

/* Função que devolve um array de char* com a cópia de todas as keys da
 * árvore, colocando o último elemento do array com o valor NULL e
 * reservando toda a memória necessária. As keys devem vir ordenadas segundo a ordenação lexicográfica das mesmas.
 */
char **tree_get_keys(struct tree_t *tree) {
	int size = tree_size(tree) + 1;
	char **keyPtrs = malloc(size * sizeof(char*));
	if(!keyPtrs){
		return NULL;
	}

	tree_get_keys_aux(tree, keyPtrs, 0);
	keyPtrs[size - 1] = NULL;
	return keyPtrs;
}

int tree_get_keys_aux(struct tree_t *tree, char **keyPtrs, int index) {
	if (!tree || !tree->node){
		return index;
	}
	//a tree ja esta ordenada de forma lexicografica from TL to N to TR
	if(tree->tree_left){
		index = tree_get_keys_aux(tree->tree_left, keyPtrs, index);
	}

	keyPtrs[index] = malloc(strlen(tree->node->key) * sizeof(char));
	if(!keyPtrs[index]){ // error on init
		return -1;
	}
	strcpy(keyPtrs[index], tree->node->key);
		index++;

	if(tree->tree_right){
		index = tree_get_keys_aux(tree->tree_right, keyPtrs, index);
	}

	return index;
}

/* Função que devolve um array de void* com a cópia de todas os values da
 * árvore, colocando o último elemento do array com o valor NULL e
 * reservando toda a memória necessária.
 */
void ** tree_get_values(struct tree_t *tree) {

	int size = tree_size(tree) + 1;
	struct data_t **valuePtrs = malloc(size * sizeof(struct data_t));
  	if(!valuePtrs){
		return NULL;
	}

	tree_get_values_aux(tree, valuePtrs, 0);
	valuePtrs[size - 1] = NULL;
	return (void **) valuePtrs;
}


int tree_get_values_aux(struct tree_t *tree, struct data_t **valuePtrs, int index) {
	if (!tree || !tree->node){
		return -1;
	}

	//a tree ja esta ordenada de forma lexicografica from TL to N to TR
	if(tree->tree_left){
		index = tree_get_values_aux(tree->tree_left, valuePtrs, index);
	}

	valuePtrs[index] = malloc(sizeof(struct data_t));
	if(!valuePtrs[index]){ // error on init
		return -1;
	}
	valuePtrs[index] = data_dup(tree->node->value);
		index++;

	if(tree->tree_right){
		tree_get_values_aux(tree->tree_right, valuePtrs, index);
		index++;
	}

	return index;
}

void tree_free_keys(char **keys) {
	for (int i = 0; keys[i]; i++){
		free(keys[i]);
	}
	free(keys);
}

/* Função que liberta toda a memória alocada por tree_get_values().
 */

void tree_free_values(void **values) {
	for (int i = 0; values[i] != NULL; i++) {
		data_destroy(values[i]);
	}
}
