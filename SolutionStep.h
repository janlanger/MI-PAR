/* 
 * File:   Step.h
 * Author: Honza
 *
 * Created on 5. říjen 2011, 18:49
 */

#ifndef STEP_H
#define	STEP_H

class SolutionStep {
public:
    SolutionStep(int disc, int from, int to, SolutionStep* next);
    SolutionStep(const SolutionStep& orig);
    virtual ~SolutionStep();
    int getDisc();
    int getFrom();
    int getTo();
    SolutionStep* getNext();
    bool hasNext();
private:
    int disc;
    int from;
    int to;
    SolutionStep* next;

};

#endif	/* STEP_H */

