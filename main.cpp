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
#include "mpi.h"

using namespace std;
#define MASTER_CPU 0

/* TAGS*/
#define TAG_NEED_MORE_WORK 1
#define TAG_NO_MORE_WORK 2
#define TAG_NEW_WORK 3
#define TAG_SIZE_OF_NEW_WORK 4
#define TAG_SOLUTION_NEW_LIMIT 5
#define TAG_SOLUTION_SIZE 6
#define TAG_SOLUTION_SOLUTION 7
#define TAG_FINALIZE 100  // pesek
#define TAG_TERMINATE 500 // konec

#define COLOR_W 0
#define COLOR_B 1
// #define DEBUG 1

Item* generateInitState ( int n, int s, int f )
{
    Item* item = new Item ( s, n );
    item->setFinalPole ( f );
    /* initialize random seed: */
    srand ( ( unsigned int ) time ( NULL ) );
    for ( int i = 0 ; i<n; i++ )
    {
        item->addDiscOnPole ( /*(rand() % s)*/0, n-i );
    }
    item->generateOptions();

    return item;
}

int getUpperBound ( double n, double s )
{
    return ( int ) ceil ( ( pow ( 2, ( n/ ( s-2 ) ) )-1 ) * ( 2*s-5 ) );
}

void moreWorkHandle() {

}


/*
*
*/
int main ( int argc, char** argv )
{
    /* Number of discs */
    int n;
    /* Number of poles */
    int s;
    /* Final pole */
    int f;

    /* Upper bound */
    int limit;

    /* Solution */
    string solution;

    /* Rank of CPU */
    int myRank;

    /* Numer of CPU's */
    int p;

    /* Flag used for async communication*/
    int flag;

    /* Status */
    MPI_Status status;

    /* Run time */
    clock_t runtime;

    /* Stack */
    Stack* stack = new Stack();

    /* Pointer to item  */
    Item* next = NULL;

    /* Pointer to item */
    Item* item = NULL;

    /* Specifying if the CPU has anything to do */
    int myColor;

    /* Rank of ancestor CPU */
    int ancestor;

    /* Rank of succesor CPU */
    int successor;

    MPI_Request request;

    if ( argc != 4 )
    {
        cout << "[ERROR]: Bad program call" << endl;
        cout << "Usage" << endl;
        cout << argv[0]<<" <n> <s> <f>" << endl;
        cout << " <n> - number of discs" << endl;
        cout << " <s> - number of poles" << endl;
        cout << " <f> - final pole" << endl;
        return 1;
    }

    n = atoi ( argv[1] );
    s = atoi ( argv[2] );
    f = atoi ( argv[3] );
    limit = getUpperBound ( n,s );

    f--;


    MPI_Init ( &argc, &argv );
    MPI_Comm_rank ( MPI_COMM_WORLD, &myRank );
    MPI_Comm_size ( MPI_COMM_WORLD, &p );

    successor = (myRank + 1) % p;
    ancestor= (myRank + (p-1)) % p;
    cout << "Hi! I'm P" << myRank << ". My ancestor id P" << ancestor << " and successor is P"<< successor << endl;

    if ( myRank == MASTER_CPU )
    {

        if ( s < max ( 3, n/4 ) )
        {
            cout << "[ERROR]: Invalid number of poles. The minimum is "<< max ( 3, n/4 ) << '.' << endl;
            MPI_Finalize();
            return 2;
        }

        if ( f < 0 || f > s )
        {
            cout << "[ERROR]: Invalid final pole. It has to be from interval [1," << s << "]" << endl;
            MPI_Finalize();
            return 3;
        }
        cout << "SETUP" << endl;
        cout << "Number of discs: " << n << endl;
        cout << "Number of poles: " << s << endl;
        cout << "Final pole: " << f+1 << endl << endl;

        myColor = COLOR_W;
        runtime = clock();
        Item* initial = generateInitState ( n, s, f );

        // vypis zakladni konfigurace
        cout << "Initial configuration: \r\n";

        for ( int i=0; i<s; i++ )
        {
            cout << "Pole " << i+1;
            short* d = initial->getPole ( i )->getDiscs();
            short noD = initial->getPole ( i )->getNoDiscs();
            if ( noD == 0 )
            {
                cout << " has no discs \r\n";
                continue;
            }
            cout << " has discs: ";
            for ( int i = 0; i<noD; i++ )
            {
                cout << d[i] << " ";
            }
            cout << "\r\n";
        }
        // -- // vypis zakladni konfigurace

        cout << endl <<"Upper bound is: " << limit << endl<<endl;



        stack->push ( initial );

    } else {
        // konfigurace pro slave procesory
        myColor = COLOR_B;
        int message = 1;
        MPI_Isend (&message, 1, MPI_INT, MASTER_CPU, TAG_NEED_MORE_WORK, MPI_COMM_WORLD, &request);
        int size;
        MPI_Recv(&size, 1, MPI_INT, MASTER_CPU, TAG_SIZE_OF_NEW_WORK, MPI_COMM_WORLD, &status);
#ifdef DEBUG
        cout<< "[P"<<myRank<<"] Master has work for me. Size is " << size << '.' << endl << endl;
#endif        
        char resp[size];
        MPI_Recv(&resp, size+1, MPI_CHAR, MASTER_CPU, TAG_NEW_WORK, MPI_COMM_WORLD, &status);
#ifdef DEBUG
        cout<< "[P"<<myRank<<"] The work from master is " << '|' << resp << '|' << endl << endl;
#endif        
        string sp = string(resp);
#ifdef DEBUG        
        cout << "[P"<< myRank<< "] Creating new item" << endl;
#endif        
        item = new Item(n, s, sp);
#ifdef DEBUG        
        cout << "Item created"<<endl;
#endif        
        item->setFinalPole(f);
#ifdef DEBUG        
        cout << "Final pole seted up"<<endl;
#endif        
        item->generateOptions();
#ifdef DEBUG
        cout << "Generated options"<< endl;
#endif        
        stack->push(item);
#ifdef DEBUG
        cout<< "[P"<<myRank<<"] finaly get some work" << endl << endl;
#endif        
        myColor = COLOR_W;
        //free(resp);
    }


    try
    {
        bool superEnd = false;
        while (!superEnd)
        {
            while ( !stack->isEmpty() )
            {
//                 if(myRank != MASTER_CPU)
//                 cout << "[P" << myRank << "] is solving something"<<endl;
                item = stack->head();

                if ( n == item->getPole ( f )->getNoDiscs() )
                {
                    limit = item->getRecursionLevel() - 1;
                    if ( limit < 0 )
                    {
                        cout << "Initial state is also final state. Abort." << endl;
                    }
                    else
                    {
#ifdef DEBUG
                        cout << "[P"<<myRank<<"] Found solution on " << limit+1 << " moves, lowering upper bound to " << limit << "\n";
#endif
                        solution = item->getSolution();

                        // nasel jsem reseni
                        // musim ho poslat MASTERU vcetne limitu
                        // Master pak musi vsem poslat novy limit

                        if (myRank != MASTER_CPU) {
                            MPI_Send(&limit, 1, MPI_INT, MASTER_CPU, TAG_SOLUTION_NEW_LIMIT, MPI_COMM_WORLD);
                            int length = solution.size()+1;
                            MPI_Send( &length, 1, MPI_INT, MASTER_CPU, TAG_SOLUTION_SIZE, MPI_COMM_WORLD);

                            char message[solution.size()+1];
                            strcpy(message, solution.c_str());
                            MPI_Send(message, length, MPI_CHAR, MASTER_CPU, TAG_SOLUTION_SOLUTION, MPI_COMM_WORLD);
                            //free(message);
                        } else {
                            for (int i = 1; i < p; i++) {
                                MPI_Isend(&limit, 1, MPI_INT, i, TAG_SOLUTION_NEW_LIMIT, MPI_COMM_WORLD, &request);
                            }
                        }
                    }
                }
                if (myRank == MASTER_CPU) {
                    MPI_Iprobe ( MPI_ANY_SOURCE, TAG_SOLUTION_NEW_LIMIT, MPI_COMM_WORLD, &flag, &status );
                    if (flag) {
#ifdef DEBUG
                        cout << "[P"<<myRank<<"] P"<< status.MPI_SOURCE << " found solution"<<endl;
#endif                        
                        int newLimit;
                        int size;


                        MPI_Recv(&newLimit, 1, MPI_INT, MPI_ANY_SOURCE, TAG_SOLUTION_NEW_LIMIT, MPI_COMM_WORLD, &status);
#ifdef DEBUG                        
                        cout << "[P"<<myRank<<"] Previous limit was "<< limit<< " new limit is " << newLimit <<endl;
#endif                        
                        MPI_Recv(&size, 1, MPI_INT, status.MPI_SOURCE, TAG_SOLUTION_SIZE, MPI_COMM_WORLD, &status);
                        char newSolution[size];
                        MPI_Recv(newSolution, size, MPI_INT, status.MPI_SOURCE, TAG_SOLUTION_SOLUTION, MPI_COMM_WORLD, &status);
#ifdef DEBUG
                        cout << "[P"<<myRank<<"] Solution is :"<< endl << newSolution<<endl<<endl;
#endif                        
                        if ( newLimit < limit) {
                            solution.assign(newSolution);
                            for (int i = 1; i < p; i++) {
                                if (i != status.MPI_SOURCE) {
                                    MPI_Isend(&limit, 1, MPI_INT, i, TAG_SOLUTION_NEW_LIMIT, MPI_COMM_WORLD, &request);
                                }
                            }
                        }
//                         free(newSolution);

                    }
                } else {
                    MPI_Iprobe ( MASTER_CPU, TAG_SOLUTION_NEW_LIMIT, MPI_COMM_WORLD, &flag, &status );
                    if (flag) {
                        MPI_Recv(&limit, 1, MPI_INT, MASTER_CPU, TAG_SOLUTION_NEW_LIMIT, MPI_COMM_WORLD, &status);
#ifdef DEBUG                      
                        cout << "[P" << myRank << "] Master send me new limit= " << limit;
#endif                        
                    }
                }
                if ( !item->hasOption() )
                {
                    delete stack->pop();
                    continue;
                }

                if ( item->getRecursionLevel() >= limit || ( item->getRecursionLevel() > limit - ( n- item->getPole ( f )->getNoDiscs() ) ) )
                {
                    delete stack->pop();
                    continue;
                }

                while ( item->hasOption() )
                {
                    int* step = item->popOption();
                    next = new Item ( *item );
                    next->incrementRecursionLevel();
                    next->doStep ( step );
                    next->generateOptions();
                    stack->push ( next );
                }
                MPI_Iprobe ( MPI_ANY_SOURCE, TAG_NEED_MORE_WORK, MPI_COMM_WORLD, &flag, &status );
                if ( flag )
                {
#ifdef DEBUG                
                    cout<< "[P" << myRank << "] someone need work"<<endl;
#endif                    
                    int message;
                    MPI_Recv(&message, 1, MPI_INT, MPI_ANY_SOURCE, TAG_NEED_MORE_WORK, MPI_COMM_WORLD, &status);
//                     MPI_Get_count(&status, MPI_CHAR, &length);
#ifdef DEBUG                    
                    cout<< "[P" << myRank << "] it is [P" << status.MPI_SOURCE << "]" << endl;
#endif                    
                    //tady bych se mel podivat jestli po me nekdo nechce praci
                    if ( !stack->isEmpty() )
                    {
                        string serializedItem = item->serialize();
                        char message[serializedItem.size()+1];
                        strcpy(message, serializedItem.c_str());
                        int length = strlen(message);
                        item = stack->pop();
                        // Posilam velikosti budouci nove prace
                        MPI_Send(&length, 1, MPI_INT, status.MPI_SOURCE, TAG_SIZE_OF_NEW_WORK, MPI_COMM_WORLD);
                        // Posilam samotnou novou praci
#ifdef DEBUG                        
                        cout << "[P"<<myRank << "] i'm sending work "<< message << endl << endl;
#endif                        
                        MPI_Send (message, length+1, MPI_CHAR, status.MPI_SOURCE, TAG_NEW_WORK, MPI_COMM_WORLD);
//                         free(message);
                    }
                    else
                    {
                        int message = 1;
                        MPI_Isend(&message, 1, MPI_INT, status.MPI_SOURCE, TAG_NO_MORE_WORK, MPI_COMM_WORLD, &request);
                    }
                }
                // Musim kontrolovat zda uz neko neposila peska
                MPI_Iprobe ( ancestor, TAG_FINALIZE, MPI_COMM_WORLD, &flag, &status );
                if (flag) {
                    int pesekColor;
                    MPI_Recv(&pesekColor, 1, MPI_INT, ancestor, TAG_FINALIZE, MPI_COMM_WORLD, &status);
                    pesekColor = myColor && pesekColor;
                    MPI_Isend(&pesekColor, 1, MPI_INT, successor, TAG_FINALIZE, MPI_COMM_WORLD, &request);
                }
            }
            if (1 == p) {
                superEnd = true;
                continue;
            }
            myColor = COLOR_B;
#ifdef DEBUG            
            cout<< 'P'<<myRank<<" has no more work" << endl << endl;
#endif            
//             cout << clock() << endl;
            int dest = 0;
            for (int i = 0; i<=p; i++) {
                int source = (dest + i) % p;
                if (source == myRank) {
                    continue;
                }
                int message = 1;
#ifdef DEBUG                
                cout << "[P" << myRank << "] is asking P" << source << " for new work." << endl;
#endif                
                MPI_Isend (&message,  1, MPI_INT, source, TAG_NEED_MORE_WORK, MPI_COMM_WORLD, &request);
                bool end = false;
                while (!end) {
                    // Nova prace
                    MPI_Iprobe ( source, TAG_SIZE_OF_NEW_WORK, MPI_COMM_WORLD, &flag, &status );
                    if (flag) {
                        int size;
                        MPI_Recv(&size, 1, MPI_INT, source, TAG_SIZE_OF_NEW_WORK, MPI_COMM_WORLD, &status);
#ifdef DEBUG                        
                        cout<< "[P"<<myRank<<"] P"<<source<<" has work for me. Size is " << size << '.' << endl << endl;
#endif                        
                        char message[size];
                        MPI_Recv(message, size+1, MPI_CHAR, source, TAG_NEW_WORK, MPI_COMM_WORLD, &status);
#ifdef DEBUG                        
                        cout<< "[P"<<myRank<<"] The work from P" << source << " is " << '|' << message << '|' << endl << endl;
#endif                        
                        string serialized = string(message);
                        Item* item = new Item(n, s, serialized);
                        item->setFinalPole(f);
                        item->generateOptions();
                        stack->push(item);
                        myColor = COLOR_W;
                        end = true;
//                         free(message);
                    }
                    // Pesek
                    // Ja jsem neposilal, takze ho jenom prepocitam a predam dal
                    MPI_Iprobe ( ancestor, TAG_FINALIZE, MPI_COMM_WORLD, &flag, &status );
                    if (flag) {
#ifdef DEBUG                        
                        cout << "[P" << myRank << "] P" << status.MPI_SOURCE << " has started with pesek." << endl;
#endif                        
                        int pesekColor;
                        MPI_Recv(&pesekColor, 1, MPI_INT, ancestor, TAG_FINALIZE, MPI_COMM_WORLD, &status);
                        pesekColor = myColor && pesekColor;
                        MPI_Isend(&pesekColor, 1, MPI_INT, successor, TAG_FINALIZE, MPI_COMM_WORLD, &request);
                    }
                    // Ten koho jsem zadal o praci hlasi ze uz praci nema
                    MPI_Iprobe ( source, TAG_NO_MORE_WORK, MPI_COMM_WORLD, &flag, &status );
                    if (flag) {
#ifdef DEBUG                        
                        cout << "[P" << myRank << "] Damn! P" << status.MPI_SOURCE << " has no more work for me." << endl;
#endif                        
                        int message;
                        MPI_Recv(&message, 1, MPI_INT, source, TAG_NO_MORE_WORK, MPI_COMM_WORLD, &status);
                        end = true;
                    }
                    // Nekdo me zada o praci
                    MPI_Iprobe ( MPI_ANY_SOURCE, TAG_NEED_MORE_WORK, MPI_COMM_WORLD, &flag, &status );
                    if (flag) {
#ifdef DEBUG                        
                        cout << "[P" << myRank << "] P" << status.MPI_SOURCE << " is asking me for new work but I have no more work even for myself." << endl;
#endif                        
                        int message;
                        MPI_Recv(&message, 1, MPI_INT, status.MPI_SOURCE, TAG_NEED_MORE_WORK, MPI_COMM_WORLD, &status);
                        message = 1;
                        MPI_Isend(&message, 1, MPI_INT, status.MPI_SOURCE, TAG_NO_MORE_WORK, MPI_COMM_WORLD, &request);
                    }

                    //  Konec aplikace
                    MPI_Iprobe ( ancestor, TAG_TERMINATE, MPI_COMM_WORLD, &flag, &status );
                    if (flag) {
                        // Predchozi mi poslal ukonceni vypoctu. Koncim se vsim
#ifdef DEBUG                        
                        cout << "[P" << myRank << "] P" << status.MPI_SOURCE << " is terminating program." << endl;
#endif                        
                        int message;
                        MPI_Recv(&message, 1, MPI_INT, ancestor, TAG_TERMINATE, MPI_COMM_WORLD, &status);
                        MPI_Isend(&myColor, 1, MPI_INT, successor, TAG_TERMINATE, MPI_COMM_WORLD, &request);
                        if ( myRank == MASTER_CPU )
                        {
                            cout << "RUN TIME: " << ( double ) ( clock() - runtime ) / CLOCKS_PER_SEC << endl << endl;
                            cout << "-------------" << endl;
                            cout << "We have solution!" << endl;
                            cout << "Number of moves: " << ( limit+1 ) << endl;
                            cout << solution << endl;
                        }
                        MPI_Finalize();
                        return 0;
                    }
                }
            }
#ifdef DEBUG            
            cout << "[P" << myRank << "] New work handeling ends. My state is " << myColor << endl;
#endif            
            // Zeptal jsem se vsech a nikdo mi nedal praci
            // Musim zacit posilat peska
            if (myColor == COLOR_B) {
#ifdef DEBUG                
                cout << "[P" << myRank << "] No more work left so lets start with pesek."<<endl;
#endif                
                MPI_Send(&myColor, 1, MPI_INT, successor, TAG_FINALIZE, MPI_COMM_WORLD);
                int pesek;
                // cekam dokud se mi pesek nevrati
                MPI_Recv(&pesek, 1, MPI_INT, ancestor, TAG_FINALIZE, MPI_COMM_WORLD, &status);
#ifdef DEBUG                
                cout << "[P" << myRank << "] Pesek has returned. Its color is "<<pesek<<endl;
#endif                
                if (pesek == COLOR_B) {
#ifdef DEBUG                    
                    cout << "[P" << myRank << "] So lets terminate application."<<endl;
#endif                    
                    // pesek se vratil a uz nikdo nema praci => VSECHNY VYPNU
                    MPI_Send(&myColor, 1, MPI_INT, successor, TAG_TERMINATE, MPI_COMM_WORLD);
                    int message;
                    MPI_Recv(&message, 1, MPI_INT, ancestor, TAG_TERMINATE, MPI_COMM_WORLD, &status);
                    if ( myRank == MASTER_CPU )
                    {
                        cout << "RUN TIME: " << ( double ) ( clock() - runtime ) / CLOCKS_PER_SEC << endl << endl;
                        cout << "-------------" << endl;
                        cout << "We have solution!" << endl;
                        cout << "Number of moves: " << ( limit+1 ) << endl;
                        cout << solution << endl;
                    }
                    MPI_Finalize();
                    return 0;
                } else {
                    // pesek se vratil ale prace jeste je
                }
            }
        }
    }
    catch ( const char* str )
    {
        cout << "[p-" << myRank << "] Exception: " << str << endl;
    }


//  _CrtDumpMemoryLeaks();
    MPI_Finalize();
    return 0;
}




