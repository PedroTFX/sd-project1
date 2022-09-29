#ifndef _TREE_PRIVATE_H
#define _TREE_PRIVATE_H

#include "tree.h"
#include "entry.h"

struct tree_t {
	struct entry_t* node;
	struct tree_t* tree_left;
	struct tree_t* tree_right;
};

struct entry_t* put_tree(struct tree_t* treeOG, struct tree_t* treeGone);

struct tree_t* get_tree(struct tree_t* tree, char* key);

struct tree_t* tree_dup(struct tree_t* tree);

#endif
