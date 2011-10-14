/*
* File:   Item.cpp
* Author: Honza
*
* Created on 5. říjen 2011, 14:57
*/

#include <locale.h>
#include <iostream>

#include "Item.h"

Item::Item ( int noPoles, int noDiscs, int finalPole )
{
    this->recursionLevel = 0;
    this->noOptions = 0;
    this->noPoles = noPoles;
    this->executedStep = NULL;
    this->noDiscs = noDiscs;
    this->finalPole = finalPole;
    this->poles = new int*[this->noPoles];

    this->options = new bool*[noPoles];
    this->polesLastDiscIndex = new int[noPoles];

    for ( short i = 0; i < noPoles; i++ )
    {
        this->options[i] = new bool[noPoles];
        this->poles[i] = new int[noDiscs];
        this->polesLastDiscIndex[i] = -1;
        for ( short j = 0; j < noDiscs; j++ )
        {
                this->poles[i][j] = -1;
        }
        for (short j = 0; j < noPoles; j++)
        {
            this->options[i][j] = false;
        }
    }
    this->previous = NULL;
    this->previousStep = NULL;
}

Item::Item ( const Item& orig )
{
    this->noOptions = 0;
    this->noPoles = orig.noPoles;
    this->noDiscs = orig.noDiscs;
    this->finalPole = orig.finalPole;
    this->previousStep = NULL;
    this->recursionLevel = orig.recursionLevel;
    this->options = new bool*[orig.noPoles];
    this->poles = new int*[this->noPoles];
    this->polesLastDiscIndex = new int[noPoles];
    
    for ( short i = 0; i < orig.noPoles; i++ )
    {
        this->options[i] = new bool[orig.noPoles];
        this->poles[i] = new int[orig.noDiscs];
        this->polesLastDiscIndex[i] = orig.polesLastDiscIndex[i];
        for ( short j = 0; j < orig.noDiscs; j++ )
        {
                this->poles[i][j] = orig.poles[i][j];
        }
        for (short j = 0; j < orig.noPoles; j++)
        {
            this->options[i][j] = false;
        }
    }
}


Item::~Item()
{
    for ( short i = 0; i<this->noPoles; i++ )
    {
        delete[] this->options[i];
        delete[] this->poles[i];
    }
    
    delete[] this->options;
    delete[] this->poles;
    delete[] this->polesLastDiscIndex;
}

Item* Item::getPrevious()
{
    return this->previous;
}

void Item::setPrevious ( Item* item )
{
    this->previous = item;
}

int* Item::getPole ( int id ) const
{
    return poles[id];
}

void Item::generateOptions()
{
    this->noOptions = 0;
    for ( int activePole = 0; activePole < this->noPoles; activePole++ )
    {
        int lastDiscIndex = this->polesLastDiscIndex[activePole];

        if ( lastDiscIndex == -1 )
        {
            for ( int i = 0; i< this->noPoles; i++ )
            {
                this->options[activePole][i] = false;
            }
            continue;
        }

        if ( this->finalPole == activePole )
        {
            if ( ( 2 + this->noDiscs - this->polesLastDiscIndex[activePole] ) == this->poles[activePole][lastDiscIndex] )
            {
                //makes impossible to move discs in final position
                for ( short i = 0; i < this->noPoles; i++ )
                {
                    this->options[activePole][i] = false;
                }
                continue;
            }
        }
        for ( int i = 0; i < this->noPoles; i++ )
        {
            this->options[activePole][i] = false;
            if ( i == activePole )
            {
                continue;
            }
            if ( this->executedStep != NULL && activePole == this->executedStep[1] && i == executedStep[0] )
            {
                continue;
            }
            if (this->polesLastDiscIndex[i] == -1 || this->poles[i][(int)this->polesLastDiscIndex[i]] > this->poles[activePole][(int)lastDiscIndex] )
            {
                this->options[activePole][i] = true;
                this->noOptions++;
            }
        }
    }
}

bool Item::hasOption()
{
    return ( this->noOptions > 0 );
}

int* Item::popOption()
{
    if ( !this->hasOption() )
    {
        throw "No options left.";
    }
    int* option = new int[2];

    for ( int startPole=0; startPole < this->noPoles; startPole++ )
    {
        for ( int endPole = 0; endPole < this->noPoles; endPole++ )
        {
            if ( this->options[startPole][endPole] == true )
            {
                this->noOptions--;
                option[0] = startPole;
                option[1] = endPole;
                this->options[startPole][endPole] = false;
                goto finish;
            }
        }
    }
finish:
    return option;
}

void Item::doStep ( int* step )
{
    this->executedStep = step;
    this->poles[step[1]][++this->polesLastDiscIndex[step[1]]] = this->poles[step[0]][this->polesLastDiscIndex[step[0]]];
    this->poles[step[0]][this->polesLastDiscIndex[step[0]]--] = -1;
}

int Item::getStep()
{
    return this->executedStep[1];
}

int Item::getActivePole()
{
    return this->executedStep[0];
}

void Item::setPreviousStep ( Item* item )
{
    this->previousStep = item;
}

Item* Item::getPreviousStep()
{
    return this->previousStep;
}

int Item::getRecursionLevel()
{
    return recursionLevel;
}
void Item::incrementRecursionLevel()
{
    recursionLevel++;
}

void Item::addDisc ( int pole, int disc )
{
    this->poles[pole][(int)++this->polesLastDiscIndex[pole]] = disc;
}

int Item::getNoPoles() const
{
    return this->noPoles;
}

int Item::getNoDiscs() const
{
    return noDiscs;
}

int Item::getPoleNoDiscs ( char pole )
{
    return polesLastDiscIndex[(int)pole] + 1;
}

ostream& Item::print(ostream& stream)
{
    for ( unsigned char i = 0; i < this->noPoles; i++ )
    {
        stream << "Pole " << i+1;
        int* discs = this->getPole(i);
        if ( discs[0] == -1 )
        {
            stream << " has no discs " << endl;
        }
        else
        {
            stream << " has discs: ";
            for ( unsigned char j = 0; j < this->noDiscs; j++ )
            {
                if ( discs[j] > 0 )
                {
                    stream << discs[j] << " ";
                }
                else
                {
                    break;
                }
            }
            stream << endl;
        }
        
    }
    stream << endl;
    return stream;
}
