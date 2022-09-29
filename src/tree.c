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
struct tree_t* tree_create(){
	struct tree_t* tree = malloc(sizeof(struct tree_t));
	if(tree == NULL){
		tree = NULL;
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
			tree->node = NULL;
		}
		if(tree->tree_left){
			tree_destroy(tree->tree_left);
			tree->tree_left = NULL;
		}
		if(tree->tree_right){
			tree_destroy(tree->tree_right);
			tree->tree_right = NULL;
		}
		//memset(tree, 0, sizeof(struct tree_t));
		free(tree);
		tree = NULL;
	}
	
}

struct tree_t* tree_dup(struct tree_t* tree){
	if(!tree){
		return NULL;
	}
	struct tree_t* tree_st = malloc(sizeof(struct tree_t));
	if(!tree_st){ //error on init
		tree_st = NULL;
		return NULL;
	}
	memset(tree_st, 0, sizeof(struct tree_t));

	if(tree->tree_left){
		tree_st->tree_left = tree_dup(tree->tree_left);
	}
	if(tree->tree_right){
		tree_st->tree_right = tree_dup(tree->tree_right);
	}
	if(tree->node){
		tree_st->node = entry_dup(tree->node);
	}

	return tree_st;
}

/**
 * @brief Find the tree object from given key
 * 
 * @param tree 
 * @param key 
 * @return struct entry_t* || NULL if not found 
 */
struct tree_t* get_tree(struct tree_t* tree, char* key){
	struct entry_t* entry = malloc(sizeof(struct entry_t));
	entry->key = strdup(key);

	struct tree_t* current_tree = tree;

	while(current_tree->node != NULL){
		int comp = entry_compare(entry, current_tree->node);
		entry_destroy(entry);

		if(comp == 0){	//found
			return current_tree;
		}else if(comp == -1 && current_tree->tree_left){
			return get_tree(current_tree->tree_left, key);
		}else if(comp == 1 && current_tree->tree_right){
			return get_tree(current_tree->tree_right, key);
		}else{			//notfound
			return NULL;
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
int tree_put(struct tree_t *tree, char *key, struct data_t *value){
	struct tree_t* current_tree = tree;
	struct entry_t* entry = malloc(sizeof(struct entry_t));
	entry->key = strdup(key);
	entry->value = data_dup(value);

	if(!current_tree->node){
		current_tree->node = entry_dup(entry);
		entry_destroy(entry);
		return 0;
	}

	while(current_tree->node){
		int comp = entry_compare(entry, current_tree->node);

		if(comp <= -1){
			if(!current_tree->tree_left){
				current_tree->tree_left = tree_create();
			}
			current_tree = current_tree->tree_left;
		}else if(comp >= 1){
			if(!current_tree->tree_right){
				current_tree->tree_right = tree_create();
			}
			current_tree = current_tree->tree_right;
		}else{				//replace
			entry_destroy(current_tree->node);		//clean bef put
			break;
		}
	}
	current_tree->node = entry_dup(entry);
	entry_destroy(entry);
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
	return (tree_st) ? data_dup(tree_st->node->value) : NULL ;
}

/* Função que devolve o número de elementos contidos na árvore.
 */
int tree_size(struct tree_t *tree){
	if(tree == NULL){
		return 0;
	}
	return tree_size(tree->tree_left) + tree_size(tree->tree_right) + (tree->node != NULL);
}

//ja esta a ignorar o primeiro da treeGone
struct entry_t* put_tree(struct tree_t* treeOG, struct tree_t* treeGone){
	//printf("here\n");
	struct entry_t* entry = malloc(sizeof(struct entry_t));
	
	printf("treeGone != NULL: %d\n", treeGone != NULL);
	if(treeGone){
		printf("%d:left\n", treeGone->tree_left != NULL);
		if(treeGone->tree_left){
			entry = entry_dup(put_tree(treeOG, treeGone->tree_left));
			printf("hi");
			tree_put(treeOG, entry->key, entry->value);
		} 
		printf("%d:right\n", treeGone->tree_right != NULL);
		if(treeGone->tree_right){
			entry = entry_dup(put_tree(treeOG, treeGone->tree_right));
			tree_put(treeOG, entry->key, entry->value);
		}
	}
	printf("return: %d\n", treeGone->node != NULL);
	return (treeGone) ? treeGone->node : NULL;
}

/* Função para remover um elemento da árvore, indicado pela chave key,
 * libertando toda a memória alocada na respetiva operação tree_put.
 * Retorna 0 (ok) ou -1 (key not found).
 */
int tree_del(struct tree_t *tree, char *key){
	struct tree_t* sub_tree = tree_dup(get_tree(tree, key));
	printf("%d\n", tree_size(get_tree(tree, key)));
	printf("%d\n", tree_size(tree_dup(get_tree(tree, key))));
	if(!sub_tree){
		printf("sub_tree NULL\n");
		return -1;
	}
	printf("2\n");
	tree_destroy(get_tree(tree, key));
	printf("2\n");
	put_tree(tree, sub_tree);
	printf("2\n");
	tree_destroy(sub_tree);
	return 0;
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
