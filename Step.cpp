/* 
 * File:   Step.cpp
 * Author: Honza
 * 
 * Created on 5. ��jen 2011, 18:49
 */
#include <locale.h>
#include "Step.h"
#include "Disc.h"

Step::Step(int disc, int from, int to, Step* next) {
    this->disc = disc;
    this->from = from;
    this->to = to;
    this->next = next;
}

Step::Step(const Step& orig) {
}

Step::~Step() {
}

int Step::getDisc() {
    return this->disc;
}

int Step::getFrom() {
    return this->from;
}

int Step::getTo() {
    return this->to;
}

Step* Step::getNext() {
    return this->next;
}

bool Step::hasNext(){
    return (this->next != NULL );
}

