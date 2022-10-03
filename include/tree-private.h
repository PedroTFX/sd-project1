#ifndef _TREE_PRIVATE_H
#define _TREE_PRIVATE_H

#include "tree.h"
#include "entry.h"

struct tree_t {
	struct entry_t* node;
	struct tree_t* tree_left;
	struct tree_t* tree_right;
};

char ** tree_get_keys_aux(struct tree_t *, char **, int );
void **tree_get_values_aux(struct tree_t *, void **, int);
void ordena(char **);
#endif
