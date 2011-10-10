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
	this->size = 0;
}

Stack::Stack(const Stack& orig) {
}

Stack::~Stack() {
}
void Stack::push(Item* item){
	item->setPrevious(this->top);
	
	this->top = item;
	this->size++;
	std::cout << this->size << "\n";
}

Item* Stack::pop(){
	Item* item = this->top;
	this->top = item->getPrevious();
	this->size--;
	return item;
}

Item* Stack::head(){
	return this->top;
}

bool Stack::isEmpty(){
	return (NULL == this->top);
}

