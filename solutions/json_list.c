#include "json_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

JObject* parse_json(const char *filename) {
	    json_error_t error;
	        json_t *root = json_load_file(filename, 0, &error);
		    if (!root) {
			            fprintf(stderr, "Error loading JSON: %s\n", error.text);
				            return NULL;
					        }
		        if (!json_is_array(root)) {
				        fprintf(stderr, "Error: root is not an array\n");
					        json_decref(root);
						        return NULL;
							    }

			    JObject *head = NULL;
			        JObject *tail = NULL;

				    size_t index;
				        json_t *value;
					    json_array_foreach(root, index, value) {
						            if (!json_is_object(value)) {
								                fprintf(stderr, "Warning: array element %zu is not an object\n", index);
										            continue;
											            }

							            json_t *jname = json_object_get(value, "name");
								            json_t *jage = json_object_get(value, "age");
									            if (!json_is_string(jname) || !json_is_integer(jage)) {
											                fprintf(stderr, "Warning: invalid or missing fields in element %zu\n", index);
													            continue;
														            }

										            JObject *node = malloc(sizeof(JObject));
											            if (!node) {
													                perror("malloc");
															            break;
																            }
												            const char *name_str = json_string_value(jname);
													            node->name = strdup(name_str);
														            node->age = (int)json_integer_value(jage);
															            node->next = NULL;

																            if (!head) {
																		                head = tail = node;
																				        } else {
																						            tail->next = node;
																							                tail = node;
																									        }
																	        }

					        json_decref(root);
						    return head;
}

void print_list(JObject *head) {
	    for (JObject *p = head; p != NULL; p = p->next) {
		            printf("Name: %s, Age: %d\n", p->name, p->age);
			        }
}

void free_list(JObject *head) {
	    while (head) {
		            JObject *tmp = head;
			            head = head->next;
				            free(tmp->name);
					            free(tmp);
						        }
}
