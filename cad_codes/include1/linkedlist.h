/*
 * File: LinkedList.h
 * -------------
 * This file provides an interface to a simple linked list abstraction.
 *
 *typedef struct linkedlistCDT *linkedlistADT;
 *
 *linkedlistADT NewLinkedList(void);
 *void FreeLinkedList(linkedlistADT linkedlist);
 *void InsertNode(linkedlistADT linkedlist, linkedlistADT nodeptr, void *obj);
 *linkedlistADT DeleteNode(linkedlistADT linkedlist, void *obj,
                         bool (*equalfunptr)(void *obj1, void *obj2));
 *void TraverseLinkedList(linkedlistADT linkedlist,
 *                        void (*traversefunptr)(void *obj));
 *linkedlistADT NextNode(linkedlistADT head, linkedlistADT nodeptr);
 *void *NodeObj(linkedlistADT head, linkedlistADT nodeptr);
 */

#ifndef _LinkedList_h
#define _LinkedList_h

#include "genlib.h"

/*
 * Type: linkedlistADT
 * --------------
 * This line defines the abstract linked list type as a pointer to
 * its concrete counterpart.  Clients have no access to the
 * underlying representation.
 */

typedef struct linkedlistCDT *linkedlistADT;

/*
 * Function: NewLinkedList
 * Usage: linkedlist = NewLinkedList();
 * --------------------------
 * This function allocates and returns an empty linked list (including a dummy head
 * node only).
 */

linkedlistADT NewLinkedList(void);

/*
 * Function: FreeLinkedList
 * Usage: FreeLinkedList(linkedlist);
 * ------------------------
 * This function frees the storage associated with linked list.
 */

void FreeLinkedList(linkedlistADT linkedlist);

/*
 * Function: InsertNode
 * Usage: InsertNode(linkedlist, nodeptr, obj);
 * ----------------------------
 * This function ineserts the data value after the nodeptr node of the linked
 * list. if nodeptr is NULL, append the obj to the tail of the linked list.
 */

void InsertNode(linkedlistADT linkedlist, linkedlistADT nodeptr, void *obj);

/*
 * Function: DeleteNode
 * Usage: nodeptr = DeleteNode(linkedlist, obj, equalfunptr);
 * ------------------------------
 * This function deletes the node with the data obj of the linked list according to the function
 * equalfunptr. it returns the pointer of the node if found, NULL otherwise.
 */

linkedlistADT DeleteNode(linkedlistADT linkedlist, void *obj,
                         bool (*equalfunptr)(void *obj1, void *obj2));

/*
 * Function: TraverseList
 * Usage: TraverseList(linkedlist, void (*traversefunptr)(void *obj));
 * ------------------------------
 * This function traverses the linked list, and process the node value obj using
 * the function which traversefunptr points to.
 */

void TraverseLinkedList(linkedlistADT head, void (*traversefunptr)(void *obj));

void *TraverseChoose(linkedlistADT head, double x, double y, bool (*traversefunptr)(double mx, double my, void *obj));

/*
 * Function: NextNode
 * Usage: nextnodeptr = NextNode(linkedlist, nodeptr);
 * ------------------------------
 * This function return the pointer of the next node of the node nodeptr.
 */

linkedlistADT NextNode(linkedlistADT head, linkedlistADT nodeptr);

/*
 * Function: NodeObj
 * Usage: obj = NodeObj(linkedlist, nodeptr);
 * ------------------------------
 * This function return the dataptr of the node nodeptr points to.
 */

void *NodeObj(linkedlistADT head, linkedlistADT nodeptr);

#endif
