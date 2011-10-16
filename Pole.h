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
    void init(short maxNoDiscs);
    virtual ~Pole();
    bool canAddDisc(short diskSize);
    void addDisc(short diskSize);
    short getLastDiscSize();
    void setFinal(bool isFinal);
    short popLastDisc();
    bool isFinal();
    int getNoDiscs();
    short* getDiscs();
private:
    short* discs;
    bool final;
    short discsOnPole;

};

#endif	/* POLE_H */

