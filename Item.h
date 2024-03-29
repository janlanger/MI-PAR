/*
 * File:   Item.h
 * Author: Honza
 *
 * Created on 5. říjen 2011, 14:57
 */

#ifndef ITEM_H
#define ITEM_H
#include <locale.h>
#include <vector>
#include "Pole.h"
#include <string>

using namespace std;

class Item {
public:

    Item(int noPoles, int noDiscs);
    Item(int noPoles, int noDiscs, string &serialized);
    Item(const Item& orig);
    virtual ~Item();
    Item* getPrevious();
    void setPrevious(Item* item);
    Pole* getPole(int id);
    void generateOptions();
    bool hasOption();
    int* popOption();
    void doStep(int* step);
    int getStepStartPole();
    int getStepEndPole();
    void incrementRecursionLevel();
    int getRecursionLevel();
    void setFinalPole(int poleNr);
    void addDiscOnPole(int pole, int discSize);
    string getSolution();
    string serialize();
    bool setPole(int pole, int score);

private:
    static Pole* allPoles;
    static int allPolesSize;
    void generateAllPoles(int noDiscs);

    short finalPole;
    Pole* getPoleWithScore(int score);
    double factorial(double a);
    void getCombinations(short* items, unsigned short itemsSize, unsigned short combinationLength, vector<short> &returned, unsigned short depth,unsigned short margin, vector<vector<short> > &combinations, int &combinationsSize);
    Item* previous;
    Pole** poles;
    bool** options;
    int noPoles;
    int noDiscs;
    int noOptions;
    int* executedStep;
    int recursionLevel;
    string solution;

};

#endif  /* ITEM_H */

