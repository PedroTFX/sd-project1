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
struct tree_t *tree_create(){
	struct tree_t* tree = malloc(sizeof(struct tree_t));
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
	
	tree_destroy(tree->tree_left);
	tree_destroy(tree->tree_right);

	free(tree);
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

	struct entry_t *entry = malloc(sizeof (struct entry_t));
	if (entry == NULL) {
		return -1;
	}

	entry->key = strdup(key);
	entry->value = data_dup(value);

	struct tree_t* current = tree;

	while(current != NULL){
		
		if(current->node == NULL){ // found empt node
			current->node = entry;
			return 0;
		}

		int comp = entry_compare(entry, current->node);

		if(comp == -1){
			if(current->tree_left == NULL){		//empty branch
				current->tree_left = tree_create();
			}
			current = current->tree_left;
		}else if(comp == 1){
			if(current->tree_right == NULL){	//empty branch
				current->tree_right = tree_create();
			}
			current = current->tree_right;
		}else{									//replace
			entry_destroy(current->node);	
			current->node = entry;
			return 0;
		}
	}
	current->node = entry;
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
struct data_t *tree_get(struct tree_t *tree, char *key){
	struct entry_t *entry = malloc(sizeof(struct entry_t));
	if (entry == NULL) {
		return NULL;
	}
	
	entry->key =strdup(key);

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
	return 0;
}

/* Função que devolve o número de elementos contidos na árvore.
 */
int tree_size(struct tree_t *tree){
	if(tree == NULL){
		return 0;
	}
	return tree_size(tree->tree_left) + tree_size(tree->tree_right) + (tree->node != NULL);
}

/* Função que devolve a altura da árvore.
 */
int tree_height(struct tree_t *tree){
	if(tree == NULL){
		return 1;
	}

	int left = tree_height(tree->tree_left);
	int right = tree_height(tree->tree_right);

	if(tree->tree_left != NULL){
		left++;
	}

	if(tree->tree_right != NULL){
		right++;
	}


	return (left > right) ? left : right;
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
