/* 
* File:   Item.cpp
* Author: Honza
* 
* Created on 5. říjen 2011, 14:57
*/

#include <locale.h>
#include <iostream>
#include <vector>
#include "Item.h"
#include "Pole.h"

using namespace std;

Pole* Item::allPoles = NULL;
int Item::allPolesSize = 0;

Item::Item(int noPoles, int noDiscs) {
    if(this->allPoles == NULL) {
        this->generateAllPoles(noDiscs);
    }
    this->finalPole = NULL;
	this->recursionLevel = 0;
	this->noPoles = noPoles;
	this->executedStep = NULL;
	this->noDiscs = noDiscs;
    this->poles = vector<Pole*>(noDiscs, &this->allPoles[0]);
    this->options = new bool *[noPoles];
	for(int i=0; i<noPoles; i++) {
		this->options[i] = new bool[noPoles];
    }
	this->previous = NULL;
	this->previousStep = NULL;
}

Item::Item(const Item& orig) {
	this->noOptions = 0;
    this->finalPole = orig.finalPole;
	this->noPoles = orig.noPoles;
	this->noDiscs = orig.noDiscs;
	this->previousStep = NULL;
	this->recursionLevel = orig.recursionLevel;
	this->options = new bool *[noPoles];
	for(int i=0; i<noPoles; i++) 
		this->options[i] = new bool[noPoles];
	this->poles = vector<Pole*>(noPoles, NULL);
	this->executedStep = NULL;

	for(int i = 0; i<this->noPoles; i++){
		this->poles[i] = orig.poles[i];
	}


}


Item::~Item() {
	for(int i = 0; i<this->noPoles; i++) {
		delete[] this->options[i];
	}
	delete[] this->options;

	delete[] this->executedStep;
    if(this->previous == NULL) {
        delete[] this->allPoles;
    }
}

Item* Item::getPrevious(){
	return this->previous;
}

void Item::setPrevious(Item* item){
	this->previous = item;
}

Pole* Item::getPole(int id){
	return this->poles[id];
}

void Item::generateOptions(){
	this->noOptions = 0;
	for(int activePole = 0; activePole< noPoles; activePole++) {
        short disc = this->poles[activePole]->getLastDiscSize();
		if(disc == 0) {

			for(int i = 0; i< this->noPoles; i++){
				this->options[activePole][i] = false;
			}
			continue;
		}

        if(activePole == this->finalPole && disc !=0)
		{
			if((1 + this->noDiscs - this->poles[activePole]->getNoDiscs()) == disc){
				//makes impossible to move discs in final position
				for(int i = 0; i< this->noPoles; i++){
					this->options[activePole][i] = false;
				}
				continue;
			}
		}
		for(int i = 0; i< this->noPoles; i++){
			this->options[activePole][i] = false;
			if(i==activePole ){
				continue;
			}
			if(this->executedStep != NULL && activePole == this->executedStep[1] && i == executedStep[0]) {
				continue;
			}
			if(this->poles[i]->canAddDisc(disc)){
				this->options[activePole][i] = true;
				this->noOptions++;
			}
		}
	}    
}

bool Item::hasOption(){
	return (this->noOptions > 0);
}

int* Item::popOption(){
	if(!this->hasOption()){
		throw "No options left.";
	}
	int* option = new int[2];

	for(int startPole=0; startPole<this->noPoles; startPole++) {
		for(int endPole = 0; endPole < this->noPoles; endPole++) {
			if(this->options[startPole][endPole] == true) {
				this->noOptions--;
				option[0] = startPole;
				option[1] = endPole;
				this->options[startPole][endPole] = false;
				goto finish;
			}
		}
	}
finish:
	return option;
}

void Item::doStep(int* step){
	this->executedStep = step;
    int startScore = this->poles[step[0]]->getScore();
    int endScore = this->poles[step[1]]->getScore();

	this->poles[step[1]] = this->getPoleWithScore(endScore + pow((float)2, this->poles[step[0]]->getLastDiscSize()-1));
    this->poles[step[0]] = this->getPoleWithScore(startScore - pow((float)2, this->poles[step[0]]->getLastDiscSize()-1));
    

}

int Item::getStep(){
	return this->executedStep[1];
}

int Item::getActivePole(){
	return this->executedStep[0];
}

void Item::setPreviousStep(Item* item) {
	this->previousStep = item;
}

Item* Item::getPreviousStep(){
	return this->previousStep;
}

int Item::getRecursionLevel() {
	return recursionLevel;
}
void Item::incrementRecursionLevel() {
	recursionLevel++;
}

void Item::generateAllPoles(int noDiscs) {
    this->allPolesSize = noCombinations(noDiscs, noDiscs)+1;
    this->allPoles = new Pole[this->allPolesSize];
    this->allPoles[0].init(noDiscs);
    short* items = new short[noDiscs];
    for(int i=0; i<noDiscs; i++) {
        items[i] = i+1;
    }
    int pole = 1;
    for(int size = noDiscs; size > 0; size--) {
        vector<short> ix(size);
        vector<vector<short>> combinations(factorial(noDiscs)/(factorial(size)*(factorial(noDiscs-size))));
        int x = 0;
        getCombinations(items, noDiscs, size,ix,0,0, combinations, x);

        for(int i = 0; combinations.size() > i; i++) {
            this->allPoles[pole].init(noDiscs);
            for(int j = combinations[i].size()-1; j >= 0; j--) {
                this->allPoles[pole].addDisc(combinations[i][j]);
            }
            pole++;
        }
    }
    delete[] items;
}

int Item::noCombinations(int n, int k) {
    if(k > 1) {
        return factorial(n)/(factorial(k)*(factorial(n-k))) + noCombinations(n,k-1);
    } else {
        return n;
    }
}
int Item::factorial (int a)
{
  if (a > 1)
   return (a * factorial (a-1));
  else
   return (1);
}
void Item::getCombinations(short* items, short itemsSize, unsigned short combinationLength, vector<short> &returned,
    unsigned short depth,unsigned short margin, vector<vector<short>> &combinations, int &combinationsSize)
{
    // Have we selected the requested number of elements?
    if (depth >= combinationLength) {
        combinations[combinationsSize++] = returned;
        return;
    }

    // Are there enough remaining elements to be selected?
    // This test isn't required for the function to be
    // correct, but it saves futile calls.
    if ((itemsSize - margin) < (combinationLength - depth))
        return;

    // Try to select new elements to the right of the last
    // selected one.
    for (unsigned long ii = margin; ii < itemsSize; ++ii) {
        returned[depth] = ii+1;
        getCombinations(items, itemsSize, combinationLength, returned, depth + 1, ii + 1, combinations, combinationsSize);
    }
    return;
}
void Item::setFinalPole(int poleNr) {
    this->finalPole = poleNr;
}

void Item::addDiscOnPole(int pole, int discSize) {
    unsigned int score = this->poles[pole]->getScore();
    unsigned int newScore = score + pow((float)2, discSize-1);
    this->poles[pole] = this->getPoleWithScore(newScore);

}

Pole* Item::getPoleWithScore(unsigned int score) {
    int i=0;
    for(; i < this->allPolesSize; i++) {
        if(this->allPoles[i].getScore() == score) {
            break;
        }
    }
    return &this->allPoles[i];
}

