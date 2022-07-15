/*
 * File: stack.h
 * -------------
 * This file provides an interface to a simple stack
 * abstraction.
 */

#ifndef _stack_h
#define _stack_h

#include "genlib.h"

#define MaxStackSize 100

/*
 * Type: stackADT
 * --------------
 * This line defines the abstract stack type as a pointer to
 * its concrete counterpart.  Clients have no access to the
 * underlying representation.
 */

struct stackCDT
{
	void *array[MaxStackSize];
	int top;
	
};

typedef struct stackCDT *stackADT;

/*
 * Function: NewStack
 * Usage: stack = NewStack();
 * --------------------------
 * This function allocates and returns an empty stack.
 */

stackADT NewStack(void);

/*
 * Function: FreeStack
 * Usage: FreeStack(stack);
 * ------------------------
 * This function frees the storage associated with stack.
 */

void FreeStack(stackADT stack);

/*
 * Function: Push
 * Usage: PushStack(stack, obj);
 * ---------------------------
 * This function adds obj to the top of the stack.
 */

void PushStack(stackADT stack, void *obj);

/*
 * Function: Popstack
 * Usage: obj = PopStack(stack);
 * ----------------------------
 * This function removes the data value at the top of the stack
 * and returns it to the client.  Popstack an empty stack is
 * an error.
 */

void *PopStack(stackADT stack);

/*
 * Function: IsemptyStack
 * Usage: bflag = IsemptyStack(stack);
 * ------------------------------
 * This function returns if the stack is empty.
 */

bool IsemptyStack(stackADT stack);

/*
 * Function: TopStack
 * Usage: obj = TopStack(stack);
 * ------------------------------
 * This function returns the top element of the stack.Topstack an empty stack is
 * an NULL pointer.
 */

void *TopStack(stackADT stack);

#endif
