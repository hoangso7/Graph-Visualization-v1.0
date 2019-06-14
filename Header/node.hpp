#ifndef  _node_hpp_
#define _node_hpp_

struct node
{
	int value;
	node *next;
	node(int v) : value(v), next(NULL){}
};

#endif
