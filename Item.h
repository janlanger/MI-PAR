/* 
 * File:   Item.h
 * Author: Honza
 *
 * Created on 5. říjen 2011, 14:57
 */

#ifndef ITEM_H
#define	ITEM_H
#include "Pole.h"

class Item {
public:
    Pole* allPoles;
    Item(int noPoles, int noDiscs);
    Item(const Item& orig);
    virtual ~Item();
    Item* getPrevious();
    void setPrevious(Item* item);
    Pole* getPole(int id);
    void generateOptions();
    bool hasOption();
    int* popOption();
    void doStep(int* step);
    int getStep();
    int getActivePole();
	void setPreviousStep(Item* item);
	Item* getPreviousStep();
	void incrementRecursionLevel();
	int getRecursionLevel();
    
private:
    void generateAllPoles(int noDiscs);
    int noCombinations(int n, int k);
    int factorial(int a);
    Item* previous;
	Item* previousStep;
    Pole* poles;
    bool** options;
    int noPoles;
    int noDiscs;
    int noOptions;
	int* executedStep;
	int recursionLevel;
};

#endif	/* ITEM_H */

