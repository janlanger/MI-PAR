/* 
 * File:   Step.cpp
 * Author: Honza
 * 
 * Created on 5. říjen 2011, 18:49
 */
#include <locale.h>
#include "SolutionStep.h"

SolutionStep::SolutionStep(int disc, int from, int to, SolutionStep* next) {
    this->disc = disc;
    this->from = from;
    this->to = to;
    this->next = next;
}

SolutionStep::SolutionStep(const SolutionStep& orig) {
}

SolutionStep::~SolutionStep() {
	if(this->next != NULL)
		delete this->next;
}

int SolutionStep::getDisc() {
    return this->disc;
}

int SolutionStep::getFrom() {
    return this->from;
}

int SolutionStep::getTo() {
    return this->to;
}

SolutionStep* SolutionStep::getNext() {
    return this->next;
}

bool SolutionStep::hasNext(){
    return (this->next != NULL );
}

