/*
* File:   Item.cpp
* Author: Honza
*
* Created on 5. říjen 2011, 14:57
*/

#include <locale.h>
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <sstream>
#include <math.h>
#include "Item.h"
#include "Pole.h"

using namespace std;

Pole* Item::allPoles = NULL;
int Item::allPolesSize = 0;

Item::Item(int noPoles, int noDiscs) {
    if (this->allPoles == NULL) {
        this->generateAllPoles(noDiscs);
    }
    this->finalPole = 0;
    this->recursionLevel = 0;
    this->noPoles = noPoles;
    this->executedStep = NULL;
    this->noDiscs = noDiscs;
    this->poles = new Pole*[noPoles];
    this->options = new bool *[noPoles];
    for (int i=0; i<noPoles; i++) {
        this->options[i] = new bool[noPoles];
        this->poles[i] = &this->allPoles[0];
    }
    this->previous = NULL;
    this->next = NULL;
}

Item::Item(int noPoles, int noDiscs, string &serialized) {
    if (this->allPoles == NULL) {
        this->generateAllPoles(noDiscs);
    }
    this->finalPole = 0;
    this->recursionLevel = 0;
    this->noPoles = noPoles;
    this->executedStep = new int[2];
    this->noDiscs = noDiscs;
    this->poles = new Pole*[noPoles];
    this->options = new bool *[noPoles];

    int from = 0;
    int activePole = 0;
    int part = 0;
    for (unsigned int i = 1; i < serialized.length(); i++) {
        if ( serialized[i] == ':')
        {
            
            switch (part){
                case 0:
                    this->poles[activePole++] = this->getPoleWithScore(atoi(serialized.substr(from,i-from).c_str()));
                    if(activePole == noPoles){
                        part++;
                    }
                    break;
                case 1:
                    this->recursionLevel = atoi(serialized.substr(from,i-from).c_str());
                    part++;
                    break;
                case 2:
                    this->solution = serialized.substr(from,i-from).c_str();
                    part++;
                    break;
                case 3:
                    this->executedStep[0] = atoi(serialized.substr(from,i-from).c_str());
                    part++;
                    break;
                case 4:
                    this->executedStep[1] = atoi(serialized.substr(from,i-from).c_str());
                    part++;
                    break;
            }
            from = i+1;
        }
    }

    for (int i=0; i<noPoles; i++) {
        this->options[i] = new bool[noPoles];
    }
    this->previous = NULL;
    this->next = NULL;
}

Item::Item(const Item& orig) {
    this->next = NULL;
    this->noOptions = 0;
    this->finalPole = orig.finalPole;
    this->noPoles = orig.noPoles;
    this->noDiscs = orig.noDiscs;
    this->recursionLevel = orig.recursionLevel;
    this->options = new bool *[noPoles];
    this->solution = orig.solution;
    for (int i=0; i<noPoles; i++)
        this->options[i] = new bool[noPoles];
    this->poles = new Pole*[noPoles];
    this->executedStep = NULL;

    for (int i = 0; i<this->noPoles; i++) {
        this->poles[i] = orig.poles[i];
    }
    if (this->allPoles == NULL) {
        this->generateAllPoles(orig.noDiscs);
    }
}


Item::~Item() {
//     cout<< "Destruction of item ";
    for (int i = 0; i<this->noPoles; i++) {
        delete[] this->options[i];
    }
    delete[] this->options;
    try {
        delete[] this->poles;
    } catch (const char* str) {}
    delete[] this->executedStep;
    if (this->previous == NULL) {
//         delete[] this->allPoles;   // HERE IS THE BULLSHIT. This seems to not work correctly
    }
//     cout<< "DONE"<<endl;
}

 Item* Item::getPrevious() {
     return this->previous;
 }

void Item::setPrevious(Item* item) {
    this->previous = item;
}

Item* Item::getNext() {
    return this->next;
}

void Item::setNext(Item* item) {
    this->next = item;
}


Pole* Item::getPole(int id) {
    return this->poles[id];
}

void Item::generateOptions() {
    this->noOptions = 0;
    for (int activePole = 0; activePole< noPoles; activePole++) {
        short disc = this->poles[activePole]->getLastDiscSize();
        if (disc == 0) {

            for (int i = 0; i< this->noPoles; i++) {
                this->options[activePole][i] = false;
            }
            continue;
        }

        if (activePole == this->finalPole && disc !=0)
        {
            if ((1 + this->noDiscs - this->poles[activePole]->getNoDiscs()) == disc) {
                //makes impossible to move discs in final position
                for (int i = 0; i< this->noPoles; i++) {
                    this->options[activePole][i] = false;
                }
                continue;
            }
        }
        for (int i = 0; i< this->noPoles; i++) {
            this->options[activePole][i] = false;
            if (i==activePole ) {
                continue;
            }
            if (this->executedStep != NULL && activePole == this->executedStep[1] && i == executedStep[0]) {
                continue;
            }
            if (this->poles[i]->canAddDisc(disc)) {
                this->options[activePole][i] = true;
                this->noOptions++;
            }
        }
    }
}

bool Item::hasOption() {
    return (this->noOptions > 0);
}

int* Item::popOption() {
    if (!this->hasOption()) {
        throw "No options left.";
    }
    int* option = new int[2];

    for (int startPole=0; startPole<this->noPoles; startPole++) {
        for (int endPole = 0; endPole < this->noPoles; endPole++) {
            if (this->options[startPole][endPole] == true) {
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

void Item::doStep(int* step) {
    this->executedStep = step;
    int startScore = this->poles[step[0]]->getScore();
    int endScore = this->poles[step[1]]->getScore();

    this->poles[step[1]] = this->getPoleWithScore(endScore + (int) ceil(pow((float)2, this->poles[step[0]]->getLastDiscSize()-1)));
    this->poles[step[0]] = this->getPoleWithScore(startScore - (int) ceil(pow((float)2, this->poles[step[0]]->getLastDiscSize()-1)));
    std::stringstream out;

    out << '(' << this->poles[step[1]]->getLastDiscSize() << ") ";
    out <<  step[0] + 1 ;
    out << " > ";
    out <<  step[1] + 1 ;
    out << ";" << endl;
    this->solution += out.str();
}

int Item::getStepEndPole() {
    return this->executedStep[1];
}

int Item::getStepStartPole() {
    return this->executedStep[0];
}

int Item::getRecursionLevel() {
    return recursionLevel;
}
void Item::incrementRecursionLevel() {
    recursionLevel++;
}

void Item::generateAllPoles(int noDiscs) {
    //this->allPolesSize = noCombinations(noDiscs, noDiscs)+1;
    this->allPolesSize = (int) pow((float) 2,noDiscs);
    this->allPoles = new Pole[this->allPolesSize];
    this->allPoles[0].init(noDiscs);
    short* items = new short[noDiscs];
    for (int i=0; i<noDiscs; i++) {
        items[i] = i+1;
    }

    for (int size = noDiscs; size > 0; size--) {
        vector<short> ix(size);
        vector<vector<short> > combinations((long)(factorial(noDiscs)/(factorial(size)*(factorial(noDiscs-size)))));
        int x = 0;
        getCombinations(items, noDiscs, size,ix,0,0, combinations, x);

        for (unsigned int i = 0; combinations.size() > i; i++) {
            int pole = 0;
            for (unsigned int x=0; x < combinations[i].size(); x++) {
                pole +=(int) ceil(pow((float)2, combinations[i][x] -1));
            }
            this->allPoles[pole].init(noDiscs);
            for (int j = combinations[i].size()-1; j >= 0; j--) {
                this->allPoles[pole].addDisc(combinations[i][j]);
            }
        }
    }
    delete[] items;
}

double Item::factorial (double a)
{
    if (a > 1)
        return (a * factorial (a-1));
    else
        return (1);
}
void Item::getCombinations(short* items, unsigned short itemsSize, unsigned short combinationLength, vector<short> &returned,
                           unsigned short depth,unsigned short margin, vector<vector<short> > &combinations, int &combinationsSize)
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
    unsigned int newScore = score + (int) pow((float)2, discSize-1);
    this->poles[pole] = this->getPoleWithScore(newScore);

}

Pole* Item::getPoleWithScore(int score) {
    return &this->allPoles[score];
}

string Item::getSolution() {
    return solution;
}

/**
 * Co je potreba serializovat
 * 
 *  * pole scoru        - DONE
 *  * recursionLevel 
 *  * solution
 *  * executedStep 
 */
string Item::serialize()
{
    std::stringstream out;
    for (int i = 0; i < noPoles; i++)
    {
        out << this->poles[i]->getScore()<<':';
    }
    out << this->recursionLevel << ':';
    out << this->solution << ':';
    if(this->executedStep == NULL){
        out << "0:0:";
    }else{
        out << this->executedStep[0] << ':';
        out << this->executedStep[1] << ':';
    }

    return out.str();
}

bool Item::setPole(int pole, int score) {
    if(score != 0) {
        for(int i=0; i<pole; i++) {
            if(this->poles[i]->getScore() == score) {
                return false;
            }
        }
    }
    this->poles[pole] = this->getPoleWithScore(score);
    return true;
}