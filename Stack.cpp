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

using namespace std;

Stack::Stack() {
	this->top = NULL;
	this->size = 0;
        //this->bottom2 = NULL;
}

Stack::Stack(const Stack& orig) {
}

Stack::~Stack() {
	if(this->top != NULL) {
		delete this->top;
	}
        //cout << "Stack is deleted" <<endl;
}
void Stack::push(Item* item){
	if(this->top != NULL) {
            item->setPrevious(this->top);
        }

        this->top = item;
        
	this->size++;
      //  cout << "I'm pushing to stack. New top is "<<this->top << ", new bottom " << /*this->bottom2 <<*/" size is "<<this->size << endl;
	//std::cout << this->size << "\n";
}

Item* Stack::pop(){
	Item* item = this->top;
	this->top = item->getPrevious();
        
	this->size--;
      //  cout << "Poping item "<<item<<" from stack. Top is now "<<this->top<<", size is " << this->size << endl;

	return item;
}

Item* Stack::shift() {
    Item* x = this->top;
    Item* x2 = this->top->getPrevious();
    while(x2->getPrevious() != NULL) {
        x = x->getPrevious();
        x2 = x2->getPrevious();
    }
//    cout << "Shifting from stack" <<endl;
    x->setPrevious(NULL);
    size--;
    return x2;
    
}

Item* Stack::head(){
	return this->top;
}

bool Stack::isEmpty(){
	return (NULL == this->top);
}

