/* 
 * File:   kdnode.h
 * Author: chenyu69
 *
 * Created on February 4, 2015, 10:08 PM
 */

#ifndef KDNODE_H
#define	KDNODE_H
#include "LatLon.h"
using namespace std;


class kdnode {
public:
    kdnode();
    kdnode(const kdnode& orig);
    virtual ~kdnode();
private:
    int64_t ID;
    kdnode * left;
    kdnode * right;
    
};

#endif	/* KDNODE_H */

