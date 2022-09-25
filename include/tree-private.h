#ifndef _TREE_PRIVATE_H
#define _TREE_PRIVATE_H

#include "tree.h"
#include "entry.h"

struct tree_t {
	struct entry_t *node;
	struct tree_t *tree_left;
	struct tree_t *tree_right;
};

#endif
