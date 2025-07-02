#include <stdio.h>
#include "slist.h"

int main(void) {
	    SNode *head = NULL;
	        int choice, val;

		    do {
			            printf("1) Count nodes\n2) Add node\n3) Exit\n> ");
				            if (scanf("%d", &choice) != 1) break;
					            switch (choice) {
							                case 1:
										                printf("Count: %d\n", count_nodes(head));
												                for (SNode *p = head; p; p = p->next)
															                    printf("Node at %p: %d\n", (void*)p, p->value);
														                break;
																            case 2:
																                printf("Value: ");
																		                if (scanf("%d", &val) == 1)
																					                    head = add_node(head, val);
																				                break;
																						            case 3:
																						                free_list(head);
																								                break;
																										            default:
																										                printf("Invalid option\n");
																												        }
						        } while (choice != 3);

		        return 0;
}
