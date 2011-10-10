/* 
 * File:   Stack.h
 * Author: Honza
 *
 * Created on 5. říjen 2011, 16:06
 */

#ifndef STACK_H
#define	STACK_H
#include "Item.h"

class Stack {
public:
    Stack();
    Stack(const Stack& orig);
    virtual ~Stack();
    void push(Item* item);
    Item* pop();
    Item* head();
    bool isEmpty();
private:
    Item* top;
	int size;
};

#endif	/* STACK_H */

