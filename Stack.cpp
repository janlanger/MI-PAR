/* 
* File:   Stack.cpp
* Author: Honza
* 
* Created on 5. říjen 2011, 16:06
*/
#include <locale.h>
#include <iostream>
#include "Stack.h"
#include "Item.h"

Stack::Stack() {
	this->top = NULL;
    this->bottom = NULL;
	this->size = 0;
}

Stack::Stack(const Stack& orig) {
    throw "Copy of stack is not allowed!";
}

Stack::~Stack() {
	if(this->top != NULL) {
		delete this->top;
	}
}
void Stack::push(Item* item){
	item->setPrevious(this->top);
    if(this->top != NULL) {
        this->top->setNext(item);
    }
	
	this->top = item;
    if(this->bottom == NULL) {
        this->bottom = item;
    }
	this->size++;
	//std::cout << this->size << "\n";
}

Item* Stack::pop(){
	Item* item = this->top;
	this->top = item->getPrevious();
    if(this->top == NULL) {
        this->bottom = NULL;
    } else {
        this->top->setNext(NULL);
    }
	this->size--;
	return item;
}

Item* Stack::head(){
	return this->top;
}

bool Stack::isEmpty(){
	return (NULL == this->top);
}

Item* Stack::popBottom()
{
    if(this->bottom != NULL) {
        Item* b = this->bottom;
        this->bottom = this->bottom->getNext();
        this->size--;
        return b;
    }
}

