/* 
 * File:   Pole.h
 * Author: Honza
 *
 * Created on 5. říjen 2011, 14:59
 */

#ifndef POLE_H
#define	POLE_H
#include "Disc.h"

class Pole {
public:
    Pole();
    Pole(const Pole& orig);
    virtual ~Pole();
    bool canAddDisc(Disc* disk);
    void addDisc(Disc* disk);
    Disc* getLastDisc();
    void setFinal(bool isFinal);
    Disc* popLastDisc();
    bool isFinal();
    int getNoDiscs();
private:
    Disc* last;
    bool final;
    int noDiscs;

};

#endif	/* POLE_H */

