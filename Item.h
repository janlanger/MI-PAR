/* 
 * File:   Item.h
 * Author: Honza
 *
 * Created on 5. říjen 2011, 14:57
 */

#ifndef ITEM_H
#define	ITEM_H
using namespace std;

class Item {
public:
    Item( int noPoles, int noDiscs, int finalPole);
    Item(const Item& orig);
    virtual ~Item();
    Item* getPrevious();
    void setPrevious(Item* item);
    int* getPole(int id) const;
    void generateOptions();
    bool hasOption();
    int* popOption();
    void doStep( int* step);
    int getStep();
    int getActivePole();
    void setPreviousStep(Item* item);
    Item* getPreviousStep();
    void incrementRecursionLevel();
    int getRecursionLevel();
    void addDisc(int pole, int disc);
    int getNoPoles() const;
    int getNoDiscs() const;
    int getPoleNoDiscs(char pole);
    ostream &print(ostream &stream);
    
private:
    Item* previous;
    Item* previousStep;
    int** poles;
    bool** options;
    int* polesLastDiscIndex;
    int noPoles;
    int noDiscs;
    int noOptions;
    int finalPole;
    int* executedStep;
    int recursionLevel;
};

#endif	/* ITEM_H */

