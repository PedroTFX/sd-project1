#ifndef _TREE_PRIVATE_H
#define _TREE_PRIVATE_H

#include "entry.h"

struct tree_t {
	struct entry_t* node;
	struct tree_t* tree_left;
	struct tree_t* tree_right;
};

struct tree_t* get_tree(struct tree_t* tree, char* key);



void tree_get_keys_aux(struct tree_t *tree, char **key, int index);

struct data_t **tree_get_values_aux(struct tree_t *tree, struct data_t **values, int index);

struct tree_t *breadh_first(struct tree_t *);

#endif
