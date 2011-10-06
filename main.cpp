/* 
 * File:   main.cpp
 * Author: Honza
 *
 * Created on 5. øíjen 2011, 14:55
 */

#include <cstdlib>
#include <iostream>

#include "Item.h"
#include "Disc.h"
#include "Pole.h"
#include "Stack.h"
#include "Step.h"

using namespace std;

Item* generateInitState(int n, int s){
    Item* item = new Item(s, n);
    for( int i = 0 ; i<n; i++){
        Disc* disc = new Disc(n-i);
        Pole* pole = item->getPole((time(NULL) % s));
        pole->addDisc(disc);
    }
    item->generateOptions();
    
    return item;
}

/*
 * 
 */
int main(int argc, char** argv) {

    int n = 3;
    int s = 3;
    int f = 2;
    
    int limit = 100;
    int counter = 0;  
    Step* solution = NULL;
    Item* item = generateInitState(n, s);
    item->getPole(f)->setFinal(true);
    
    Stack* stack = new Stack();
    stack->push(item);
    
    while(!stack->isEmpty())
    {
        while(counter < limit){
            counter++;
            Item* item = stack->head();
            if(n == item->getPole(f)->getNoDiscs()){
                // volat destrukci predchoziho reseni
                int disc;
                int from;
                int to;
                while(NULL != item->getPrevious()){
                    disc = item->getPole(item->getStep())->getLastDisc()->getSize();
                    from = item->getActivePole();
                    to = item->getStep();
                    solution = new Step(disc, from, to, solution);
                    item = item->getPrevious();
                    
                }
                
                limit == counter - 1;
            }
            if(!item->hasOption()){
                stack->pop();
                counter--;
                continue;
            }
            for(int i = 0; i<s; i++){
                item->setActivePole(i);
                while(item->hasOption()){
                    int step = item->popOption();
                    Item next = *item;
                    next.setPrevious(item);
                    next.doStep(step);
                    next.generateOptions();
                    stack->push(&next);
                }
            }
        }
    }
    
    cout <<"Number of moves: " << (limit+1) << "\r\n";
    while(solution->hasNext()){
        cout <<"Disk: " << solution->getDisc() << " Z: " << solution->getFrom() << " Na: " << solution->getTo()<< "\r\n";
        
        solution = solution->getNext();
    }
    return 0;
}
