/* 
 * File:   Street.cpp
 * Author: chenyu69
 * 
 * Created on January 26, 2015, 4:41 PM
 */

#include "Street.h"
#include <iostream>
using namespace std;

Street::Street() {}

Street::Street(const Street& orig) 
{
    ID = orig.ID;
    length = orig.length;
    name = orig.name;
    StreetSegs = orig.StreetSegs;
    Intersects = orig.Intersects;
    displayString = orig.displayString;
    boundary = orig.boundary;
}

Street::Street(int64_t _ID,string _name,double _length,
               vector<int64_t> _StreetSegs,vector<int64_t> _Intersects
                ,vector<string> _displayString)
{
    ID = _ID;
    name = _name;
    length = _length;
    StreetSegs = _StreetSegs;
    Intersects = _Intersects;
    displayString = _displayString;
}

Street::~Street() {}

//since these vectors won't change in size we can try to free up some memory
void Street::memCleanUp()
{
    StreetSegs.shrink_to_fit();
    Intersects.shrink_to_fit();
}

//used to debug unit tests
void Street::testoutput()
{
    cout << ID << endl;
    cout << name <<endl;
   // for(unsigned i = 0; i < Intersects.size(); i++)
   // cout << Intersects[i] << " ";
  //  cout << endl;
}

string Street::getName()
{

    return name;
}

vector<int64_t> Street::getSegs()
{
    return StreetSegs;
}

vector<int64_t> Street::getIntersects()
{
    return Intersects;
}

double Street::getLength()
{
    return length;
}

void Street::setboundary(t_bound_box _boundary)
{
    boundary = _boundary;
}

t_bound_box Street::getboundary()
{
    return boundary;
}

vector<string> Street::getdisplayString()
{
    return displayString;
}
        