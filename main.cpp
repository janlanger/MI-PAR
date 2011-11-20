/* 
* File:   main.cpp
* Author: Honza
*
* Created on 5. říjen 2011, 14:55
*/


#include <cstdlib>
#include <iostream>
#include <time.h>
#include <math.h>

#include "Item.h"
#include "Pole.h"
#include "Stack.h"
#include "SolutionStep.h"

using namespace std;

Item* generateInitState(int n, int s, int f, char* polesConf){
	Item* item = new Item(s, n);
    item->setFinalPole(f);
	/* initialize random seed: */
	/*srand ((unsigned int) time(NULL) );
	for( int i = 0 ; i<n; i++){
		item->addDiscOnPole(/*(rand() % s)/0, n-i);
	}*/
    int* poles = new int[s];
    char * it = strtok(polesConf,":");
    int charS = sizeof(polesConf)/sizeof(char);
    int sum = 0;
    for(int i=0; i<s; i++) {
        poles[i] = atoi(it);
        sum += poles[i];
        it = strtok(NULL,":");        
    }
    if(sum+1 != pow(2,(double)n)) {
       
        return NULL;
    }
    for(int i = 0; i<s; i++) {
        if(!item->setPole(i, poles[i])) {
            return NULL;
        }
    }
	item->generateOptions();

	return item;
}

int getUpperBound(double n, double s) {
	return (int) ceil((pow(2,(n/(s-2)))-1)*(2*s-5));
}


/*
* 
*/
int main(int argc, char** argv) {
	
	int n, s, f;

	if ( argc != 5 )
    {
        cout << "[ERROR]: Bad program call" << endl;
        cout << "Usage" << endl;
        cout << argv[0]<<" <n> <s> <f> <pole1:pole2...>" << endl;
        cout << " <n> - number of discs" << endl;
        cout << " <s> - number of poles" << endl;
        cout << " <f> - final pole" << endl;
        cout << " <pole1:pole2...> - score of poles (discs on them), separated by :" << endl;
        return 1;
    }

    n = atoi ( argv[1] );
    s = atoi ( argv[2] );
    f = atoi ( argv[3] );
	f--;
	clock_t runtime = clock();

	SolutionStep* solution = NULL;
	Item* initial = generateInitState(n, s, f, argv[4]);
    if(initial == NULL) {
        cout << "Invalid poles configuration. Abort." << endl;
        return 2;
    }
	// vypis zakladni konfigurace
	cout << "Initial configuration: \r\n";
	
	for(int i=0; i<s; i++) {
		cout << "Pole " << i+1;
        short* d = initial->getPole(i)->getDiscs();
        short noD = initial->getPole(i)->getNoDiscs();
		if(noD == 0) {
			cout << " has no discs \r\n";
			continue;
		}
		cout << " has discs: ";
		for(int i = 0; i<noD; i++) {
			cout << d[i] << " ";
		}
		cout << "\r\n";
	} 
	// -- // vypis zakladni konfigurace

	Item* next = NULL;	
	Item* item = NULL;

	int limit = getUpperBound (n,s);
	cout << "\r\nUpper bound is: " << limit << "\r\n\r\n";


	Stack* stack = new Stack();
	stack->push(initial);

	try{
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
            if(limit < 0) {
                cout << "Initial state is also final state. Abort." << endl;
            } else {
			    cout << "Found solution on " << limit+1 << " moves, lowering upper bound to " << limit << "\n";
            }
			while(NULL != item->getPreviousStep()){
                disc = item->getPole(item->getStepEndPole())->getLastDiscSize();
                from = item->getStepStartPole();
				to = item->getStepEndPole();
				solution = new SolutionStep(disc, from, to, solution);
				item = item->getPreviousStep();

			}


		}
		if(!item->hasOption()){
			delete stack->pop();
			continue;
		}
        if(item->getRecursionLevel() >= limit || ( item->getRecursionLevel() > limit - (n- item->getPole(f)->getNoDiscs()))) {
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
	} catch (const char* str) {
		cout << "Exception: " << str << endl;
	}
	cout << "RUN TIME: " << (double) (clock() - runtime) / CLOCKS_PER_SEC << endl;
	cout << "\n\n-------------\n";
	cout << "I have solution! \n";
	cout <<"Number of moves: " << (limit+1) << "\r\n";
	SolutionStep* tmp = solution; //stores top of solution stack for destruct call;
	while(solution != NULL){
		cout <<"Move disc " << solution->getDisc() << " from " << solution->getFrom()+1 << " to " << solution->getTo()+1 << "\r\n";
		solution = solution->getNext();
	} 
	delete tmp;
	delete stack;

	cin.clear();
	cin.ignore(2);
//	_CrtDumpMemoryLeaks();
	return 0;
}


