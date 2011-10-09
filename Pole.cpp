/* 
* File:   Pole.cpp
* Author: Honza
* 
* Created on 5. шнjen 2011, 14:59
*/

#include <locale.h>

#include "Pole.h"
#include "Disc.h"

Pole::Pole() {
	this->last = NULL;
	this->noDiscs = 0;
	this->final = false;
}

Pole::Pole(const Pole& orig) {
	this->final=orig.final;
	if(orig.last != NULL)
		this->last = new Disc(*orig.last);
	else {
		this->last = NULL;
	}

	this->noDiscs = orig.noDiscs;
}

Pole::~Pole() {
}

bool Pole::canAddDisc(Disc* disk) {
	return (this->last == NULL || disk == NULL || this->last->getSize() > disk->getSize());
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
