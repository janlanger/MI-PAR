/* 
 * File:   Item.h
 * Author: Honza
 *
 * Created on 5. øíjen 2011, 14:57
 */

#ifndef ITEM_H
#define	ITEM_H
#include "Pole.h"

class Item {
public:
    Item(int noPoles, int noDiscs);
    Item(const Item& orig);
    virtual ~Item();
    Item* getPrevious();
    void setPrevious(Item* item);
    Pole* getPole(int id);
 //   void setActivePole(int id);
    void generateOptions();
    bool hasOption();
    int* popOption();
    void doStep(int* step);
    int getStep();
    int getActivePole();
	void setPreviousStep(Item* item);
    
private:
    Item* previous;
	Item* previousStep;
    Pole* poles;
 //   int activePole;
    bool** options;
    int noPoles;
    int noDiscs;
    int noOptions;
//    int step;
	int* executedStep;
};

#endif	/* ITEM_H */

