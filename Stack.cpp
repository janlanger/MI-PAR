/* 
* File:   Stack.cpp
* Author: Honza
* 
* Created on 5. шнjen 2011, 16:06
*/
#include <locale.h>
#include "Stack.h"
#include "Item.h"

Stack::Stack() {
	this->top = NULL;
}

Stack::Stack(const Stack& orig) {
}

Stack::~Stack() {
}
void Stack::push(Item* item){
	item->setPrevious(this->top);
	this->top = item;
}

Item* Stack::pop(){
	Item* item = this->top;
	this->top = item->getPrevious();

	return item;
}

Item* Stack::head(){
	return this->top;
}

bool Stack::isEmpty(){
	return (NULL == this->top);
}

