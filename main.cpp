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
#include "Stack.h"
#include "Step.h"

using namespace std;

Item* generateInitState ( int n, int s, int f )
{
    Item* item = new Item ( s, n, f );

    /* initialize random seed: */
    srand ( time ( NULL ) );
    for ( int i = 0 ; i < n; i++ )
    {
        item->addDisc(/*(rand() % s)*/0, n-i);
    }
    item->generateOptions();

    return item;
}

int getUpperBound ( double n, double s )
{
	return ceil ( ( pow ( 2, (n/ (s-2 ) ) )-1 ) * ( 2*s-5 ) );
}






/*
*
*/
int main ( int argc, char** argv )
{
    clock_t runtime = clock();
    int n, s, f;

    cout << "Enter number of discs: ";
    cin >> n;
    cout << "Enter number of poles [>=" << max ( 3, (int) n/4 ) << "]: ";
    cin >> s;
    if ( s < max ( 3, n/4 ) )
    {
        cout << "invalid pole number";
        return -1;
    }
    cout << "Enter number of final pole [1-" << s << "]: ";
    cin >> f;
    if ( f < 1 || f > s )
    {
        cout << "invalid final pole number";
        return -1;
    }
    f--;


    Step* solution = NULL;
    Item* initial = generateInitState ( (unsigned char)n, (unsigned char)s , (unsigned char)f);

    // vypis zakladni konfigurace
    cout << "Initial configuration: \r\n";

    initial->print(cout);
    // -- // vypis zakladni konfigurace

    Item* next = NULL;
    Item* item = NULL;

    int limit = getUpperBound ( n,s );
    cout << endl<< "Upper bound is: " << limit << endl << endl;


    Stack* stack = new Stack();
    stack->push ( initial );

    try
    {
        while ( !stack->isEmpty() )
        {
            item = stack->head();
            if ( n == item->getPoleNoDiscs(f))
            {
                if ( solution != NULL )
                {
                    delete solution;
                    solution = NULL;
                }
                int disc;
                int from;
                int to;
                limit = item->getRecursionLevel() - 1;
                cout << "Found solution on " << limit+1 << " moves, lowering upper bound to " << limit << "\n";
                while ( NULL != item->getPreviousStep() )
                {
                    disc = item->getPole(item->getStep())[item->getPoleNoDiscs(item->getStep())-1];
                    from = item->getActivePole();
                    to = item->getStep();
                    solution = new Step ( disc, from, to, solution );
                    item = item->getPreviousStep();

                }


            }
            if ( !item->hasOption() )
            {
                delete stack->pop();
                continue;
            }
            if ( item->getRecursionLevel() >= limit )
            {
                delete stack->pop();
                continue;
            }

            while ( item->hasOption() )
            {
                int* step = item->popOption();
                next = new Item ( *item );
                next->incrementRecursionLevel();
                next->setPreviousStep ( item );
                next->doStep ( step );
                next->generateOptions();
                stack->push ( next );
            }
        }
    }
    catch ( const char* str )
    {
        cout << "Exception: " << str << endl;
    }
    cout << "RUN TIME: " << ( double ) ( clock() - runtime ) / CLOCKS_PER_SEC << endl;
    cout << "\n\n-------------\n";
    cout << "I have solution! \n";
    cout <<"Number of moves: " << ( limit+1 ) << "\r\n";
    Step* tmp = solution; //stores top of solution stack for destruct call;
    while ( solution != NULL )
    {
        cout <<"Move disc " << solution->getDisc() << " from " << solution->getFrom() << " to " << solution->getTo() << "\r\n";
        solution = solution->getNext();
    }
    delete tmp;
    delete stack;

//  cin.clear();
//  cin.ignore(2);
//  _CrtDumpMemoryLeaks();
    return 0;
}


