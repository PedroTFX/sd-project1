#ifndef _TREE_PRIVATE_H
#define _TREE_PRIVATE_H

#include "tree.h"
#include "entry.h"

struct tree_t {
	struct entry_t* node;
	struct tree_t* tree_left;
	struct tree_t* tree_right;
};

struct entry_t* get_entry(struct tree_t* tree, struct entry_t* entrys);

struct entry_t* get_tree(struct tree_t* tree, char* key);

#endif
