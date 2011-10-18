/* 
* File:   Pole.cpp
* Author: Honza
* 
* Created on 5. říjen 2011, 14:59
*/

#include <locale.h>
#include <math.h>

#include "Pole.h"

Pole::Pole() {
    this->discs = NULL;
    this->discsOnPole = 0;
    this->score = 0;
}

Pole::Pole(const Pole& orig) {
    throw "Copy of pole is not allowed";
}

void Pole::init(short maxNoDiscs) {
    this->discs = new short[maxNoDiscs];
    for(int i=0; i<maxNoDiscs; i++) {
        this->discs[i] = -1;
    }
}



Pole::~Pole() {
    if(discs != NULL)
	    delete[] this->discs;
}

bool Pole::canAddDisc(short diskSize) {
    return (this->discsOnPole == 0 || this->discs[this->discsOnPole-1] > diskSize);
}

void Pole::addDisc(short diskSize) {
    if (!this->canAddDisc(diskSize))
		throw "Is not possible to add bigger to the smaller disc.";
    this->discs[this->discsOnPole++] = diskSize;
    this->recountScore();
}

short Pole::getLastDiscSize(){
    if(this->discsOnPole == 0) {
        return 0;
    } else {
        return this->discs[this->discsOnPole-1];
    }
}

int Pole::getNoDiscs(){
    return this->discsOnPole;
}

short* Pole::getDiscs() {
    return this->discs;
}

void Pole::recountScore() {
    this->score = 0;
    for(int i=0; i<this->discsOnPole; i++) {
        this->score += (int) ceil(pow((float)2, this->discs[i] -1));
    }
}

unsigned int Pole::getScore() {
    return this->score;
}
