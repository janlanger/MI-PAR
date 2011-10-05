/* 
 * File:   Disc.cpp
 * Author: Honza
 * 
 * Created on 5. øíjen 2011, 15:10
 */

#include "Disc.h"

Disc::Disc(int size) {
    this->size = size;
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
