/* 
 * File:   Pole.cpp
 * Author: Honza
 * 
 * Created on 5. øíjen 2011, 14:59
 */

#include "Pole.h"
#include "Disc.h"

Pole::Pole() {
}

Pole::Pole(const Pole& orig) {
}

Pole::~Pole() {
}

bool Pole::canAddDisc(Disc* disk) {
    return (this->last->getSize() > disk->getSize());
}

void Pole::addDisc(Disc* disc) {
    if (!this->canAddDisc(disc))
        throw "Is not possible to add bigger to the smaller disc.";
    disc->setPrevious(this->last);
    this->last = disc;
    this->noDiscs++;
}

Disc* Pole::getLastDisc(){
    return this->last;
}

void Pole::setFinal(bool final){
    this->final = final;
}

Disc* Pole::popLastDisc(){
    Disc* disc;
    disc = this->last;
    this->last = disc->getPrevious();
    this->noDiscs--;
    return disc;
}

int Pole::getNoDiscs(){
    return this->noDiscs;
}

bool Pole::isFinal(){
    return this->final;
}