# PP11

## Goal

In this exercise you will:

* Design and implement a **doubly linked list** as a reusable library.
* Create and interact with a **singly linked list** via an interactive menu, using dynamic memory (`malloc`/`free`).
* Parse a JSON array from a file using **jansson**, store objects in a linked list, and manipulate them in C.

**Important:** Start a stopwatch when you begin and work uninterruptedly for **90 minutes**. When time is up, stop immediately and record where you paused.

---

## Workflow

1. **Fork** this repository on GitHub.
2. **Clone** your fork locally.
3. Create a `solutions/` directory at the project root:

   ```bash
   mkdir solutions
   ```
4. For each task, add the specified source files under `solutions/`.
5. **Commit** and **push** your changes to GitHub.
6. **Submit** your GitHub repo link for review.

---

## Prerequisites

* GNU C compiler (`gcc`).
* Understanding of pointers and dynamic memory (`malloc`, `free`).
* Jansson library installed (`libjansson-dev`).
* Linker flags (`-ljansson`).

---

## Tasks

### Task 1: Compile-Time Doubly Linked List

**Objective:** Implement and traverse a doubly linked list entirely with compile-time allocation.

1. Create `solutions/dlist_static.c` and include `dlist.h` (which defines `DNode` and `DList`).
2. Define a static array of five `DNode` elements:

   ```c
   #include <stdio.h>
   #include "dlist.h"  // defines DNode and DList

   // Static nodes for values 1–5
   DNode nodes[5];
   DList list;

   int main(void) {
       // Initialize static nodes
       for (int i = 0; i < 5; i++) {
           // Example: store node index as data
           nodes[i].data = (void*)(long)(i + 1);
           nodes[i].prev = (i > 0) ? &nodes[i - 1] : NULL;
           nodes[i].next = (i < 4) ? &nodes[i + 1] : NULL;
       }
       list.head = &nodes[0];
       list.tail = &nodes[4];

       // Traverse forward
       printf("Forward: ");
       for (DNode *p = list.head; p != NULL; p = p->next) {
           printf("%ld ", (long)p->data);
       }
       printf("\n");

       // Traverse backward
       printf("Backward: ");
       for (DNode *p = list.tail; p != NULL; p = p->prev) {
           printf("%ld ", (long)p->data);
       }
       printf("\n");

       return 0;
   }
   ```
3. Compile and run:

   ```bash
   gcc -o solutions/dlist_static solutions/dlist_static.c
   ./solutions/dlist_static
   ```

#### Reflection Questions

1. **How do you link `prev` and `next` pointers correctly using a static array?**

**Lösung zu Aufgabe 1:**
Indem man die Nachbaradressen im Array zuweist:
`nodes[i].prev = (i > 0) ? &nodes[i - 1] : NULL;`
`nodes[i].next = (i < N - 1) ? &nodes[i + 1] : NULL;`

2. **What are advantages and limitations of compile-time vs. dynamic allocation?**
**Lösung zu Aufgabe 2:**
**Statisch:**
+ Schnell, sicher, kein `malloc/free`
- Unflexibel, feste Größe.

**Dynamisch:**
+ Flexibel, größenunabhängig.
- Aufwändiger, fehleranfällig (Leaks, Pointer-Probleme).

3. **How would you extend this static list to include additional data fields?**
**Lösung zu Aufgabe 3:**
Einfach die `DNode`-Struktur erweitern (z. B. um `id`, `label`) oder `data` auf eine eigene Nutzdatenstruktur zeigen lassen.

---

### Task 2: Interactive Singly Linked List

**Objective:** Build a menu-driven program to manipulate a singly linked list at runtime.

1. Define `SNode` and function prototypes in `solutions/slist.h`:

   ```c
   typedef struct SNode {
       int value;
       struct SNode *next;
   } SNode;

   SNode* add_node(SNode *head, int value);
   int    count_nodes(SNode *head);
   void   free_list(SNode *head);
   ```
2. Implement `add_node`, `count_nodes`, and `free_list` in `solutions/slist.c` using `malloc` and `free`.
3. Create `solutions/slist_main.c`:

   ```c
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
                   scanf("%d", &val);
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
   ```
4. Compile and run:

   ```bash
   gcc -c solutions/slist.c -o solutions/slist.o
   gcc -o solutions/slist_main solutions/slist_main.c solutions/slist.o
   ./solutions/slist_main
   ```

#### Reflection Questions

1. **Why is `malloc` necessary when adding nodes dynamically?**
**Lösung zu Aufgabe 1:**
`malloc` reserviert zur Laufzeit Speicher auf dem Heap für jeden neuen Knoten.
Ohne `malloc` gäbe es keine dauerhafte Speicherstelle für neue Knoten – sie würden z. B. nur lokal in einer Funktion existieren und beim Verlassen ungültig werden.

2. **How can you traverse the list to print each node’s address and value?**
**Lösung zu Aufgabe 2:**
Mit einer einfachen Schleife:

`for (SNode *p = head; p != NULL; p = p->next)
    printf("Node at %p: %d\n", (void*)p, p->value);`

Dabei geht man vom Kopf aus durch die `next`-Zeiger bis zum Ende (`NULL`).

3. **What are the consequences of not freeing the list before exit?**
**Lösung zu Aufgabe 3:**
Wenn `free_list(head)` nicht aufgerufen wird:

- Bleibt der Speicher aller Knoten im Heap belegt (Memory Leak).
- Bei wiederholtem Programmstart oder in längeren Programmen kann das zu Speichererschöpfung führen.
- Tools wie Valgrind melden Speicherlecks (z. B. „still reachable“).

---

### Task 3: JSON Array to Linked List

**Objective:** Parse a JSON array from a file into a linked list of C structs, using `getopt` to accept the input filename.

1. Create `solutions/json_list.h`:

   ```c
   #include <jansson.h>
   typedef struct JObject {
       char *name;
       int age;
       struct JObject *next;
   } JObject;

   JObject* parse_json(const char *filename);
   void     print_list(JObject *head);
   void     free_list(JObject *head);
   ```

2. Implement `solutions/json_list.c`:

   * Use `json_load_file` to load an array of objects with fields `"name"` and `"age"`.
   * For each element, allocate a `JObject`, duplicate the `name`, store `age`, and link into a list.

3. Create `solutions/json_main.c` to use `getopt`:

   ```c
   #include "json_list.h"
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>

   void usage(const char *prog) {
       fprintf(stderr, "Usage: %s -i <file.json>
   ```

", prog);
exit(EXIT\_FAILURE);
}

int main(int argc, char \*argv\[]) {
int opt;
char \*filename = NULL;

```
   while ((opt = getopt(argc, argv, "i:")) != -1) {
       if (opt == 'i') {
           filename = optarg;
       } else {
           usage(argv[0]);
       }
   }
   if (!filename) {
       usage(argv[0]);
   }

   JObject *head = parse_json(filename);
   print_list(head);
   free_list(head);
   return 0;
```

}

````
4. Compile and run:
```bash
gcc -c solutions/json_list.c -o solutions/json_list.o -ljansson
gcc -o solutions/json_main solutions/json_main.c solutions/json_list.o -ljansson
./solutions/json_main -i data.json
````

#### Suggested Student Modifications

1. **Add** `void remove_by_age(JObject **head, int age)` to remove matching nodes.
2. **Extend** `JObject` with a new field (e.g., `char *email`) and update parsing accordingly.

#### Reflection Questions

1. **How does using `getopt` make the program more flexible than `argv[1]`?**
**Lösung zu Aufgabe 1:**
`getopt` ermöglicht das einfache Parsen von mehreren und optionalen Kommandozeilen-Optionen mit Flags (z.B. `-i filename`), unabhängig von der Reihenfolge.
`argv[1]` ist nur eine feste Position und erlaubt keine komfortable Erweiterung oder optionale Argumente.


2. **What happens if the user omits the `-i` option?**
**Lösung zu Aufgabe 2:**
Das Programm ruft die `usage`-Funktion auf und beendet sich mit einer Fehlermeldung, weil die Eingabedatei nicht angegeben wurde.



3. **How can you validate that the JSON file loaded is indeed an array?**
**Lösung zu Aufgabe 3:**

Nach dem Laden mit `json_load_file` prüft man mit `json_is_array(root)`nur wenn das `true` zurückgibt, ist das JSON-Root ein Array.



---

**Remember:** Stop after **90 minutes** and record where you stopped.
