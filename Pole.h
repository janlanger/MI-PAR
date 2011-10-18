/* 
 * File:   Pole.h
 * Author: Honza
 *
 * Created on 5. říjen 2011, 14:59
 */

#ifndef POLE_H
#define	POLE_H

class Pole {
public:
    Pole();
    Pole(const Pole& orig);
    void init(short maxNoDiscs);
    virtual ~Pole();
    bool canAddDisc(short diskSize);
    void addDisc(short diskSize);
    short getLastDiscSize();
    int getNoDiscs();
    short* getDiscs();
    unsigned int getScore();
private:
    void recountScore();
    short* discs;
    short discsOnPole;
    unsigned int score;
};

#endif	/* POLE_H */

