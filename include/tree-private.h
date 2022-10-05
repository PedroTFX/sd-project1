#ifndef _TREE_PRIVATE_H
#define _TREE_PRIVATE_H

#include "tree.h"
#include "entry.h"

struct tree_t {
	struct entry_t* node;
	struct tree_t* tree_left;
	struct tree_t* tree_right;
};


char ** tree_get_keys_aux(struct tree_t *tree, char **key, int size);

void **tree_get_values_aux(struct tree_t *tree, void **values, int size);

void ordena(char **keys);

struct tree_t* get_tree(struct tree_t* tree, char* key);

struct tree_t *breadh_first(struct tree_t *);

#endif
