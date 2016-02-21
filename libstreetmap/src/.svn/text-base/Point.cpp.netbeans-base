/* 
 * File:   Point.cpp
 * Author: chenyu69
 * 
 * Created on January 26, 2015, 4:43 PM
 */

#include "Point.h"
#include <iostream>
using namespace std;
const int64_t notInitialized = 88888888;
Point::Point() {}

Point::Point(const Point& orig) 
{
    name = orig.name;
    ID = orig.ID;
    location = orig.location;
    displayString = orig.displayString;
    closestIntersect = orig.closestIntersect;
}

Point::~Point() {}

Point:: Point(int64_t _ID,string _name,vector<string> _displayString)
{
    ID = _ID;
    name = _name;
    displayString = _displayString;
    closestIntersect = notInitialized;
}

vector <string> Point::getdisplayString()
{
    return displayString;
}
void Point::testoutput()
{
    cout << ID << endl;
    cout << name << endl;
}

Cord Point::getLocation()
{
    return location;
}

string Point::getName()
{
    return name;
}

void Point::setLocation(Cord _location)
{
    location = _location;
}

int64_t Point::getClosestIntersect()
{
    return closestIntersect;
}

void Point::setClosestIntersect(int64_t _closestIntersect)
{
    closestIntersect = _closestIntersect;
}