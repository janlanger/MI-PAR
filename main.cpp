/* 
 * File:   main.cpp
 * Author: Honza
 *
 * Created on 5. øíjen 2011, 14:55
 */

#include <cstdlib>
#include <iostream>
#include <time.h>

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
        Pole* pole = item->getPole(/*(time(NULL) % s)*/0);
        pole->addDisc(disc);
    }
    item->generateOptions();
    
    return item;
}

int getUpperBound(int n, int s) {
	//(2n/(s-2)-1)(2s-5)
	return (int) (pow((double)2,(n/(s-2)))-1)*(2*s-5);
}


/*
 * 
 */
int main(int argc, char** argv) {

    int n = 5;
    int s = 3;
    int f = 2;
    
    int limit = getUpperBound (n,s);
	cout << "Upper bound is: " << limit << "\r\n";
      
    Step* solution = NULL;
    Item* initial = generateInitState(n, s);
    initial->getPole(f)->setFinal(true);
    Item* next = NULL;
    Stack* stack = new Stack();
    stack->push(initial);
    Item* item = NULL;
    while(!stack->isEmpty())
    {
			
            item = stack->head();
            if(n == item->getPole(f)->getNoDiscs()){
                if(solution != NULL) {
					delete solution;
					solution = NULL;
				}
                int disc;
                int from;
                int to;
				limit = item->getRecursionLevel() - 1;
				cout << "Found solution on " << limit+1 << " moves, lowering upper bound to " << limit << "\n";
                while(NULL != item->getPreviousStep()){
                    disc = item->getPole(item->getStep())->getLastDisc()->getSize();
                    from = item->getActivePole();
                    to = item->getStep();
                    solution = new Step(disc, from, to, solution);
					item = item->getPreviousStep();
                    
                }
                
                
            }
			if(!item->hasOption()){
                delete stack->pop();
                continue;
            }
			if(item->getRecursionLevel() >= limit) {
				delete stack->pop();
				continue;
			}
            
            
                
                
                while(item->hasOption()){
                    int* step = item->popOption();
					next = new Item(*item);
					next->incrementRecursionLevel();
                    next->setPreviousStep(item);
                    next->doStep(step);
					next->generateOptions();
                    stack->push(next);
                }
    }
	cout << "\n\n-------------\n";
    cout << "I have solution! \n";
    cout <<"Number of moves: " << (limit+1) << "\r\n";
    while(solution != NULL){
        cout <<"Move disc " << solution->getDisc() << " from " << solution->getFrom() << " to " << solution->getTo()<< "\r\n";
        
        solution = solution->getNext();
    }
	getchar();
    return 0;
}


