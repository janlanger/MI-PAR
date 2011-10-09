/* 
 * File:   Disc.cpp
 * Author: Honza
 * 
 * Created on 5. říjen 2011, 15:10
 */
#include <locale.h>
#include "Disc.h"

Disc::Disc(int size) {
    this->size = size;
	this->previous = NULL;
}
Disc::Disc(const Disc& orig) {
	this->size = orig.size;
	if(orig.previous != NULL)
	this->previous = new Disc(*orig.previous);
	else {
		this->previous = NULL;
	}
}


int Disc::getSize() {
    return this->size;
}

Disc::~Disc() {
}

Disc* Disc::getPrevious() {
    return this->previous;
}

void Disc::setPrevious(Disc* previous) {
    this->previous = previous;
}
