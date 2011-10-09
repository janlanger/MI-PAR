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

	/* initialize random seed: */
	srand ( time(NULL) );
	for( int i = 0 ; i<n; i++){
		Disc* disc = new Disc(n-i);
		Pole* pole = item->getPole((rand() % s));
		pole->addDisc(disc);
	}
	item->generateOptions();

	return item;
}

int getUpperBound(int n, int s) {
	return (int) (pow((double)2,(n/(s-2)))-1)*(2*s-5);
}


/*
* 
*/
int main(int argc, char** argv) {

	int n, s, f;

	cout << "Enter number of discs: ";
	cin >> n;
	cout << "Enter number of poles [>=" << max(3, n/4) << "]: ";
	cin >> s;
	if(s < max(3, n/4)) {
		cout << "invalid pole number";
		return -1;
	}
	cout << "Enter number of final pole [1-" << s << "]: ";
	cin >> f;
	if(f < 1 || f > s) {
		cout << "invalid final pole number";
		return -1;
	}
	f--;
	

	Step* solution = NULL;
	Item* initial = generateInitState(n, s);
	initial->getPole(f)->setFinal(true);

	// vypis zakladni konfigurace
	cout << "Initial configuration: \r\n";
	
	for(int i=0; i<s; i++) {
		cout << "Pole " << i+1;
		Disc* d = initial->getPole(i)->getLastDisc();
		if(d == NULL) {
			cout << " has no discs \r\n";
			continue;
		}
		cout << " has discs: ";
		while(d != NULL) {
			cout << d->getSize() << " ";
			d = d->getPrevious();
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
	//TODO - better
	fflush(stdin);
	cin.ignore(1);
	return 0;
}


