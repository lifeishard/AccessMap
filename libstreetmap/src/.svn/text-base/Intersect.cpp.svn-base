/* 
 * File:   Intersect.cpp
 * Author: chenyu69
 * 
 * Created on January 26, 2015, 4:44 PM
 */

#include "Intersect.h"
#include <iostream>
using namespace std;

Intersect::Intersect() {}

Intersect::Intersect(const Intersect& orig) 
{
    ID = orig.ID;
    name = orig.name;
    location = orig.location;
    StreetSegs = orig.StreetSegs;
    Neighbours = orig.Neighbours;
    Streets = orig.Streets;
    displayString = orig.displayString;
}

Intersect::~Intersect() {}

Intersect::Intersect(int64_t _ID, string _name,
        vector<int64_t> _StreetSegs, vector<int64_t> _Neighbours,
        vector<int64_t> _Streets, vector<string> _displayString)
{
    ID = _ID;
    name = _name;
    StreetSegs = _StreetSegs;
    Neighbours = _Neighbours;
    Streets = _Streets;
    displayString = _displayString;
}

//since these vectors won't change in size we can try to free up some memory
void Intersect:: memCleanUp(){
    Neighbours.shrink_to_fit();
    StreetSegs.shrink_to_fit();
    Streets.shrink_to_fit();
}

//used to debug unit test
void Intersect::testoutput()
{
    cout << ID << endl;
    cout << name << endl;
    for(unsigned i = 0; i < StreetSegs.size(); i++)
    {  cout << StreetSegs[i] << " ";
    cout << endl;
    }
}

int64_t Intersect::getID()
{
    return ID;
}

string Intersect::getName()
{
    return name;
}

vector<int64_t> Intersect::getStreetSegs()
{
    return StreetSegs;
}

vector<int64_t> Intersect::getNeighbours()
{
    return Neighbours;
}

vector<int64_t> Intersect::getStreets()
{
    return Streets;
}

Cord Intersect::getLocation()
{
    return location;
}

void Intersect::setLocation(Cord _location)
{
    location = _location;
}

double Intersect::getSpeed()
{
    return speed;
}

void Intersect::setSpeed(double _speed)
{
    speed = _speed;
}

double Intersect::getLength()
{
    return length;
}

void Intersect::setLength(double _length)
{
    length = _length;
}

vector<string> Intersect::getdisplayString()
{
    return displayString;
}