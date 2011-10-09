/* 
 * File:   Step.h
 * Author: Honza
 *
 * Created on 5. říjen 2011, 18:49
 */

#ifndef STEP_H
#define	STEP_H

class Step {
public:
    Step(int disc, int from, int to, Step* next);
    Step(const Step& orig);
    virtual ~Step();
    int getDisc();
    int getFrom();
    int getTo();
    Step* getNext();
    bool hasNext();
private:
    int disc;
    int from;
    int to;
    Step* next;

};

#endif	/* STEP_H */

