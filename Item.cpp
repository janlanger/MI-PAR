/* 
 * File:   Item.cpp
 * Author: Honza
 * 
 * Created on 5. øíjen 2011, 14:57
 */

#include "Item.h"
#include "Pole.h"
#include "Disc.h"

Item::Item(int noPoles, int noDiscs) {
    this->noPoles = noPoles;
    this->noDiscs = noDiscs;
    this->poles = new Pole[noPoles];
    this->options = new bool[noPoles];
}

Item::Item(const Item& orig) {
    this->activePole = orig.activePole;
    this->noOptions = orig.noOptions;
    this->noPoles = orig.noPoles;
    this->noDiscs = orig.noDiscs;
    this->options = orig.options;
    this->poles = new Pole[this->noPoles];
    
    for(int i = 0; i<this->noPoles; i++){
        Pole pole = orig.poles[i];
        this->poles[i] = pole;
    }
    

}


Item::~Item() {
}

Item* Item::getPrevious(){
    return this->previous;
}

void Item::setPrevious(Item* item){
    this->previous = item;
}

Pole* Item::getPole(int id){
    return &this->poles[id];
}

void Item::setActivePole(int id){
    this->activePole = id;
}

void Item::generateOptions(){
    Disc* disc;
    disc = this->poles[this->activePole].getLastDisc();
    this->noOptions = 0;
    if(this->poles[this->activePole].isFinal())
    {
        if((1 + this->noDiscs - this->poles[this->activePole].getNoDiscs()) == disc->getSize()){
            for(int i = 0; i< this->noPoles; i++){
                this->options[i] = false;
                return;
            }
        }
    }
    for(int i = 0; i< this->noPoles; i++){
        this->options[i] = false;
        if(i==this->activePole ){
            continue;
        }
        if(this->poles[i].canAddDisc(disc)){
            this->options[i] = true;
            this->noOptions++;
        }
    }
}

bool Item::hasOption(){
    return (this->noOptions > 0);
}

int Item::popOption(){
    if(!this->hasOption()){
        throw "No options left.";
    }
    int id;
    for( id = 0; id < this->noPoles; id++){
        if(this->options[id])
        {
            this->noOptions--;
            this->options[id] = false;
            break;
        }
    }
    
    return id;
}

void Item::doStep(int step){
    this->step = step;
    Disc* disc = this->poles[this->activePole].popLastDisc();
    this->poles[step].addDisc(disc);
}

int Item::getStep(){
    return this->step;
}

int Item::getActivePole(){
    return this->activePole;
}