#ifndef DLIST_H
#define DLIST_H

typedef struct DNode {
	    void *data;
	        struct DNode *prev;
		    struct DNode *next;
} DNode;

typedef struct {
	    DNode *head;
	        DNode *tail;
} DList;

#endif
