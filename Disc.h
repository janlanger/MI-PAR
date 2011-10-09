/* 
 * File:   Disc.h
 * Author: Honza
 *
 * Created on 5. øíjen 2011, 15:10
 */

#ifndef DISC_H
#define	DISC_H

class Disc {
public:
    Disc(int size);
	Disc(const Disc& orig);
    virtual ~Disc();
    int getSize();
    Disc* getPrevious();
    void setPrevious(Disc* previous);

private:
    int size;
    Disc* previous;
};

#endif	/* DISC_H */

