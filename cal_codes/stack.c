#include <stdio.h>

#include "genlib.h"
#include "simpio.h"
#include "stack.h"

#define MaxStackSize 100

stackADT NewStack(void)
{
	stackADT stack;
	stack = New(stackADT);
	int i;

	for(i=0; i<MaxStackSize; i++)
		stack->array[i] = NULL;
	stack->top = 0;

	return stack;
}

void FreeStack(stackADT stack)
{
    FreeBlock(stack);
}

void PushStack(stackADT stack, void *obj)
{
	if(stack->top == MaxStackSize)
		Error("Push on a full stack.");

	stack->array[stack->top] = obj;
	(stack->top) ++;

}

void *PopStack(stackADT stack)
{
	void *result;
	int i;

	if(stack->top == 0)
		Error("Pop of empty stack.");
	
	(stack->top) --;
	result = stack->array[stack->top];
	stack->array[stack->top] = NULL;
	
	return result;
}

bool IsemptyStack(stackADT stack)
{
	if(stack->top == 0)
		return 1;
	else
		return 0;
}

void *TopStack(stackADT stack)
{
	void *result;
	if(stack->top == 0)
		return NULL;
	
	result = stack->array[stack->top-1];
	return result;
}
