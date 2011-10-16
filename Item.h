/* 
 * File:   Item.h
 * Author: Honza
 *
 * Created on 5. říjen 2011, 14:57
 */

#ifndef ITEM_H
#define	ITEM_H
#include <locale.h>
#include <vector>
#include "Pole.h"

using namespace std;

class Item {
public:

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
    void setFinalPole(int poleNr);
    void addDiscOnPole(int pole, int discSize);
    
private:
    static Pole* allPoles;
    static int allPolesSize;
    short finalPole;
    void generateAllPoles(int noDiscs);
    Pole* getPoleWithScore(unsigned int score);
    int noCombinations(int n, int k);
    int factorial(int a);
    void getCombinations(short* items, short itemsSize, unsigned short combinationLength, vector<short> &returned, unsigned short depth,unsigned short margin, vector<vector<short>> &combinations, int &combinationsSize);
    Item* previous;
	Item* previousStep;
    void* pp;
    std::vector<Pole*> poles;
    bool** options;
    int noPoles;
    int noDiscs;
    int noOptions;
	int* executedStep;
	int recursionLevel;
    
};

#endif	/* ITEM_H */

