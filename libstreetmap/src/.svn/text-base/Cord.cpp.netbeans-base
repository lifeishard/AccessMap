/* 
 * File:   Cord.cpp
 * Author: chenyu69
 * 
 * Created on February 22, 2015, 6:03 PM
 */

#include "Cord.h"
#include <iostream>
using namespace std;
//declaring static variables
 double Cord::rotateAngle = 0;
 double Cord::baseCenterx = 0;
 double Cord::baseCentery = 0;
 double Cord::basemaxx = 0;
 double Cord::basemaxy = 0;
 double Cord::baseminx = 0;
 double Cord::baseminy = 0;
 double Cord::maxx = 0;
 double Cord::maxy = 0;
 double Cord::minx = 0;
 double Cord::miny = 0;
  double Cord::yOffset = 0;
  double Cord::xOffset = 0;
  double Cord::yBound = 0;
  double Cord::xBound = 0;
  double Cord::totalRotation = 0;
  double Cord::height = 0;
  double Cord::width = 0;
 vector <double> Cord::allRotations;
    vector <pair<double,double> > Cord::rotationCenters ;
    vector <pair<double,double> > Cord::rotationOffsets ;
Cord::Cord() {
}

Cord::Cord(const Cord& orig) {
    x = orig.x;
    y = orig.y;
    basex = orig.basex;
    basey = orig.basey;
    lat = orig.lat;
    lon = orig.lon;
    
}

Cord::Cord(double _lat, double _lon, double _x, double _y)
{
    lat = _lat;
    lon = _lon;
    x = _x;
    y = _y;
   
}

//xedge is the offset used to move edge of map data away from draw window
   void  Cord :: calcOffset(double xEdge, double yEdge)
{
    xOffset = -1 * minx + xEdge;
   yOffset = -1 * miny + yEdge;
   xBound = maxx-minx + 2*xEdge;
   yBound = maxy-miny + 2*yEdge;
   height = maxy-miny;
   width = maxx-minx;
}
   
void  Cord::setupRotate(double _rotation)
{
    //initialize to extreme values
    //negative longitude is west of meridian
    //negative latitude is south of equator
    maxx = -1 * XYBOUNDS;
    maxy = -1 * XYBOUNDS;
    minx =  XYBOUNDS;
    miny =  XYBOUNDS;
    _rotation *= DEG_TO_RAD;
    Cord::rotateAngle = _rotation;
    totalRotation += _rotation;
    if(totalRotation < 0) totalRotation += 2*PI;
    else if(totalRotation >= 2*PI) totalRotation -= 2*PI;
}

Cord::~Cord()
{
}

void  Cord::rotationCenter(double oldx,double oldy)
{
    baseCenterx = oldx;
    baseCentery = oldy;
}


void Cord::rotateMapCord() 
{
    //using general 2d rotation matrix to rotate every point
    double tx = baseCenterx + (x - baseCenterx) * cos(rotateAngle)
    - (y - baseCentery) * sin(rotateAngle);
    double ty = baseCentery + (x - baseCenterx) * sin(rotateAngle)
    + (y - baseCentery) * cos(rotateAngle);
    x = tx;
    y = ty;
    
    //records extreme value to adjust for border offset later
    if(x > maxx) maxx = x;
    if(y > maxy) maxy = y;
    if(x < minx) minx = x;
    if(y < miny) miny = y;
}

double Cord::getlat()
{
    return lat;
}

double Cord::getlon()
{
    return lon;
}

double Cord::getx()
{
    return x;
}

double Cord::gety()
{
    return y;
}

void Cord::setxy()
{
    basex = x;
    basey = y;
}

double Cord::getbasex()
{
    return basex;
}

double Cord::getbasey()
{
    return basey;
}

void Cord::drawOffset()
{
    x += xOffset;
    y += yOffset;
    //shifts the map's bottom left corner to (0,0) 
}

void Cord::recordRotation()
{
    allRotations.push_back(rotateAngle);
    rotationCenters.push_back(make_pair(baseCenterx,baseCentery));   
    rotationOffsets.push_back(make_pair(xOffset,yOffset));
}


pair<double,double> Cord::newToOld(double _x,double _y)
{
    double tx = _x;
    double ty = _y;
    
    for(int64_t i = allRotations.size()-1; i > -1 ; i--)
    {
        _x = tx;
        _y = ty;
        _x -= rotationOffsets[i].first;
        _y -= rotationOffsets[i].second;
     tx = rotationCenters[i].first + (_x - rotationCenters[i].first ) * cos(allRotations[i])
    + (_y - rotationCenters[i].second) * sin( allRotations[i]);
    ty = rotationCenters[i].second - (_x - rotationCenters[i].first) * sin(allRotations[i])
    + (_y - rotationCenters[i].second) * cos(allRotations[i]);
    }
    
    return make_pair(tx,ty);
}

pair<double,double> Cord::oldToNew(double _x,double _y)
{
    double tx = _x;
    double ty = _y;
    for(unsigned i = 0; i < allRotations.size(); i++)
    {
    _x = tx;
    _y = ty;
    tx = rotationCenters[i].first + (_x - rotationCenters[i].first ) * cos(allRotations[i])
    - (_y - rotationCenters[i].second) * sin(allRotations[i]) + rotationOffsets[i].first;
    ty = rotationCenters[i].second + (_x - rotationCenters[i].first) * sin(allRotations[i])
    + (_y - rotationCenters[i].second) * cos(allRotations[i]) + rotationOffsets[i].second;
    }
    return make_pair(tx,ty);
    
}