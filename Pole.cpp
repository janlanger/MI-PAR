/* 
* File:   Pole.cpp
* Author: Honza
* 
* Created on 5. říjen 2011, 14:59
*/

#include <locale.h>

#include "Pole.h"

Pole::Pole() {
    
    this->discsOnPole = 0;
	this->final = false;
}
void Pole::init(short maxNoDiscs) {
    this->discs = new short[maxNoDiscs];
    for(int i=0; i<maxNoDiscs; i++) {
        this->discs[i] = -1;
    }
}



Pole::~Pole() {
	delete[] this->discs;
}

bool Pole::canAddDisc(short diskSize) {
    return (this->discsOnPole == 0 || this->discs[this->discsOnPole-1] > diskSize);
}

void Pole::addDisc(short diskSize) {
    if (!this->canAddDisc(diskSize))
		throw "Is not possible to add bigger to the smaller disc.";
    this->discs[this->discsOnPole++] = diskSize;
}

short Pole::getLastDiscSize(){
    if(this->discsOnPole == 0) {
        return 0;
    } else {
        return this->discs[this->discsOnPole-1];
    }
}

void Pole::setFinal(bool final){
	this->final = final;
}

short Pole::popLastDisc(){
    return this->discs[this->discsOnPole-- -1];
}

int Pole::getNoDiscs(){
    return this->discsOnPole;
}

bool Pole::isFinal(){
	return this->final;
}

short* Pole::getDiscs() {
    return this->discs;
}
