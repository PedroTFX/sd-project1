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
struct tree_t *tree_create()
{
	struct tree_t *tree = malloc(sizeof(struct tree_t));
	if (tree == NULL)
	{
		free(tree);
		tree = NULL;
		return NULL;
	}
	memset(tree, 0, sizeof(struct tree_t));
	return tree;
}

/* Função para libertar toda a memória ocupada por uma árvore.
 */
void tree_destroy(struct tree_t *tree)
{
	if (tree)
	{
		if (tree->node)
		{
			entry_destroy(tree->node);
			tree->node = NULL;
		}
		if (tree->tree_left)
		{
			tree_destroy(tree->tree_left);
			tree->tree_left = NULL;
		}
		if (tree->tree_right)
		{
			tree_destroy(tree->tree_right);
			tree->tree_right = NULL;
		}

		free(tree);
		tree = NULL;
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
int tree_put(struct tree_t *tree, char *key, struct data_t *value)
{
	struct tree_t *current_tree = tree;
	struct entry_t *entry = malloc(sizeof(struct entry_t));
	entry->key = strdup(key);
	entry->value = data_dup(value);

	if (!current_tree->node)
	{
		current_tree->node = entry_dup(entry);
		entry_destroy(entry);
		return 0;
	}

	while (current_tree->node)
	{
		int comp = entry_compare(entry, current_tree->node);

		if (comp <= -1)
		{
			if (!current_tree->tree_left)
			{
				current_tree->tree_left = tree_create();
			}
			current_tree = current_tree->tree_left;
		}
		else if (comp >= 1)
		{
			if (!current_tree->tree_right)
			{
				current_tree->tree_right = tree_create();
			}
			current_tree = current_tree->tree_right;
		}
		else
		{									   // replace
			entry_destroy(current_tree->node); // clean bef put
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
struct data_t *tree_get(struct tree_t *tree, char *key)
{
	struct entry_t *entry = malloc(sizeof(struct entry_t));
	entry->key = strdup(key);

	struct tree_t *current_tree = tree;

	while (current_tree->node != NULL)
	{
		int comp = entry_compare(entry, current_tree->node);
		entry_destroy(entry);

		if (comp == 0)
		{
			return data_dup(current_tree->node->value);
		}
		else if (comp == -1 && current_tree->tree_left)
		{
			return tree_get(current_tree->tree_left, key);
		}
		else if (comp == 1 && current_tree->tree_right)
		{
			return tree_get(current_tree->tree_right, key);
		}
		else
		{
			return NULL;
		}
	}

	return NULL;
}

/* Função para remover um elemento da árvore, indicado pela chave key,
 * libertando toda a memória alocada na respetiva operação tree_put.
 * Retorna 0 (ok) ou -1 (key not found).
 */
int tree_del(struct tree_t *tree, char *key)
{
	return -2;
}

/* Função que devolve o número de elementos contidos na árvore.
 */
int tree_size(struct tree_t *tree)
{
	if (tree == NULL)
	{
		return 0;
	}
	return tree_size(tree->tree_left) + tree_size(tree->tree_right) + (tree->node != NULL);
}

/* Função que devolve a altura da árvore.
 */
int tree_height(struct tree_t *tree)
{
	if (tree == NULL)
	{
		return 1;
	}

	int left = tree_height(tree->tree_left);
	int right = tree_height(tree->tree_right);

	if (tree->tree_left != NULL)
	{
		left++;
	}

	if (tree->tree_right != NULL)
	{
		right++;
	}
	return (left > right) ? left : right;
}

/* Função que devolve um array de char* com a cópia de todas as keys da
 * árvore, colocando o último elemento do array com o valor NULL e
 * reservando toda a memória necessária. As keys devem vir ordenadas segundo a ordenação lexicográfica das mesmas.
 */
char **tree_get_keys(struct tree_t *tree)
{
	int size = tree_size(tree) + 1;
	char **keyPtrs = (char **)malloc(size * sizeof(char *));
	keyPtrs = tree_get_keys_aux(tree, keyPtrs, 0);
	keyPtrs[size - 1] = NULL;
	return keyPtrs;
}

char **tree_get_keys_aux(struct tree_t *node, char **keyPtrs, int index) {
	if (node == NULL) {
		return keyPtrs;
	}

	keyPtrs[index] = (char *)malloc(strlen(node->node->key) + 1);
	strcpy(keyPtrs[index], node->node->key);

	if (node->tree_left != NULL) {
		tree_get_keys_aux(node->tree_left, keyPtrs, ++index);
	}

	if (node->tree_right != NULL) {
		tree_get_keys_aux(node->tree_right, keyPtrs, ++index);
	}

	return keyPtrs;
}

/* Função que devolve um array de void* com a cópia de todas os values da
 * árvore, colocando o último elemento do array com o valor NULL e
 * reservando toda a memória necessária.
 */
void ** tree_get_values(struct tree_t *tree) {
	int size = tree_size(tree);
	struct data_t **valuePtrs = (struct data_t **) malloc((size+1) * sizeof(struct data_t *));
	tree_get_values_aux(tree, valuePtrs, 0);
	valuePtrs[size - 1] = NULL;
	return (void **) valuePtrs;
}

void tree_get_values_aux(struct tree_t *node, struct data_t **valuePtrs, int index) {
	if (node == NULL) {
		return;
	}

	valuePtrs[index] = data_dup(node->node->value);

	if (node->tree_left != NULL) {
		tree_get_values_aux(node->tree_left, valuePtrs, ++index);
	}

	if (node->tree_right != NULL) {
		tree_get_values_aux(node->tree_right, valuePtrs, ++index);
	}
}

void tree_free_keys(char **keys)
{
	for (int i = 0; keys[i] != NULL; i++)
	{
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
	free(values);
}

void ordena(char **keyPtrs) {
/* 	for (int i = 0; keyPtrs[i] != NULL; i++) {
		printf(keyPtrs[i]);
		printf("\n");
	} */

	for (int i = 0; keyPtrs[i] != NULL ; i++) {
		for (int j = i; keyPtrs[j] != NULL; j++) {
			if (strcmp(keyPtrs[j],keyPtrs[i]) < 0) {
				char* temp = keyPtrs[i];
				keyPtrs[i] = keyPtrs[j];
				keyPtrs[j] = temp;
			}
		}
	}

/* 	for (int i = 0; keyPtrs[i] != NULL; i++) {
		printf(keyPtrs[i]);
		printf("\n");
	} */
}
