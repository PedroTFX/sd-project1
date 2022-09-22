#ifndef _TREE_PRIVATE_H
#define _TREE_PRIVATE_H

#include "tree.h"
#include "entry.h"
struct tree_t {
	struct entry_t *left_node;
	struct entry_t *right_node;
};
//pode-se mudar a struct


#endif
