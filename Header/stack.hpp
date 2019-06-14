#ifndef _stack_hpp_
#define _stack_hpp_
#include "node.hpp"
#include <limits.h>
#include <stddef.h>

struct mstack
{
	mstack()
	{
		top = NULL;
	}
	bool isEmpty() const
	{
		return (top == NULL);
	}
	void push(int value)
	{
		node *n = new node(value); //createNode(value);
		n->next = top;
		top = n;
	}
	int pop()
	{
		if (isEmpty()) return INT_MIN;
		node *tmp = top;
		top = top->next;
		int res = tmp->value;
		delete tmp;
		return res;
	}
	int peek() const
	{
		if (isEmpty()) return INT_MIN;
		return top->value;
	}
	~mstack()
	{
		node *tmp;
		while (top != NULL)
		{
			tmp = top;
			top = top->next;
			delete tmp;
		}
	}
private:
	node *top;
};
#endif
