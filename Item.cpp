/* 
 * File:   Item.cpp
 * Author: Honza
 * 
 * Created on 5. øíjen 2011, 14:57
 */

#include <locale.h>

#include "Item.h"
#include "Pole.h"
#include "Disc.h"

Item::Item(int noPoles, int noDiscs) {
    this->noPoles = noPoles;
	this->executedStep = NULL;
    this->noDiscs = noDiscs;
    this->poles = new Pole[noPoles];
	this->options = new bool *[noPoles];
	for(int i=0; i<noPoles; i++) 
		this->options[i] = new bool[noPoles];
	this->previous = NULL;
	this->previousStep = NULL;
}

Item::Item(const Item& orig) {
   // this->activePole = 0; //we need to always start from first pole //orig.activePole;
    this->noOptions = 0;// orig.noOptions;
    this->noPoles = orig.noPoles;
    this->noDiscs = orig.noDiscs;
	this->previousStep = NULL;
    this->options = new bool *[noPoles];
	for(int i=0; i<noPoles; i++) 
		this->options[i] = new bool[noPoles];
    this->poles = new Pole[this->noPoles];
	this->executedStep = NULL;
    
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

/*void Item::setActivePole(int id){
    this->activePole = id;
}*/

void Item::generateOptions(){
	this->noOptions = 0;
	for(int activePole = 0; activePole< noPoles; activePole++) {
		Disc* disc = this->poles[activePole].getLastDisc();
		if(disc == NULL) {

			for(int i = 0; i< this->noPoles; i++){
				this->options[activePole][i] = false;
			}
			continue;
		}

		if(this->poles[activePole].isFinal() && disc !=NULL)
		{
			if((1 + this->noDiscs - this->poles[activePole].getNoDiscs()) == disc->getSize()){
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
			if(this->poles[i].canAddDisc(disc)){
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

    /*int id;
    for( id = 0; id < this->noPoles; id++){
        if(this->options[id])
        {
            this->noOptions--;
            this->options[id] = false;
            break;
        }
    }
    
    return id;*/
}

void Item::doStep(int* step){
    this->executedStep = step;
    Disc* disc = this->poles[step[0]].popLastDisc();
    this->poles[step[1]].addDisc(disc);
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