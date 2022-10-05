#ifndef _TREE_PRIVATE_H
#define _TREE_PRIVATE_H

#include "entry.h"

struct tree_t {
	struct entry_t* node;
	struct tree_t* tree_left;
	struct tree_t* tree_right;
};

char ** tree_get_keys_aux(struct tree_t *, char **, int );
void tree_get_values_aux(struct tree_t *, struct data_t **, int);
void ordena(char **);
int found_str_in_array(char *, char **, int);
int index_of_string_in_array(char *, char **, int);
#endif
