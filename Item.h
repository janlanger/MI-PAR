/* 
 * File:   Item.h
 * Author: Honza
 *
 * Created on 5. říjen 2011, 14:57
 */

#ifndef ITEM_H
#define	ITEM_H

class Item {
public:
    Item( const unsigned char noPoles, const unsigned char noDiscs, const unsigned char finalPole);
    Item(const Item& orig);
    virtual ~Item();
    Item* getPrevious();
    void setPrevious(Item* item);
    char* getPole( unsigned char id) const;
    void generateOptions();
    bool hasOption();
    int* popOption();
    void doStep( char* step);
    int getStep();
    int getActivePole();
    void setPreviousStep(Item* item);
    Item* getPreviousStep();
    void incrementRecursionLevel();
    int getRecursionLevel();
    void addDisc(unsigned char pole, unsigned char disc);
    unsigned char getNoPoles() const;
    unsigned char getNoDiscs() const;
    unsigned char getPoleNoDiscs(char pole);
    
private:
    Item* previous;
    Item* previousStep;
    char** poles;
    bool** options;
    char* polesLastDiscIndex;
    unsigned char noPoles;
    unsigned char noDiscs;
    unsigned char noOptions;
    unsigned char finalPole;
    char* executedStep;
    unsigned char recursionLevel;
};

#endif	/* ITEM_H */

