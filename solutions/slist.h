#ifndef SLIST_H
#define SLIST_H

typedef struct SNode {
	    int value;
	        struct SNode *next;
} SNode;

SNode* add_node(SNode *head, int value);
int count_nodes(SNode *head);
void free_list(SNode *head);

#endif
