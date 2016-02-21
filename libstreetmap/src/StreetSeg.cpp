/* 
 * File:   StreetSeg.cpp
 * Author: chenyu69
 * 
 * Created on January 26, 2015, 4:44 PM
 */

#include "StreetSeg.h"
#include <iostream>
using namespace std;
StreetSeg::StreetSeg() {}

StreetSeg::StreetSeg(const StreetSeg& orig) 
{
    type = orig.type;
    ID = orig.ID;
    streetID = orig.streetID;
    speed = orig.speed;
    length = orig.length;
    ends = orig.ends;
    bendPoints = orig.bendPoints;
    drivingTime = orig.drivingTime;
    boundary = orig.boundary;
    hideHighwayLowZoom = orig.hideHighwayLowZoom;
    endsCord = orig.endsCord;
}

StreetSeg::~StreetSeg() {}

StreetSeg::StreetSeg(int64_t _ID)
{
    type = 0;
    ID = _ID;
    streetID = 0;
    speed = 0;
    length = 0;
    drivingTime = 0;
}

StreetSeg::StreetSeg(char _type, int64_t _ID, int64_t _streetID, double _speed,
        double _length, double _drivingTime, pair<int64_t,int64_t> _ends)
{
    type = _type;
    ID = _ID;
    streetID = _streetID;
    speed = _speed;
    length = _length;
    drivingTime = _drivingTime;
    ends = _ends;
}

//since the street segments doesn't change we can free up some space
void StreetSeg:: memCleanUp()
{
    bendPoints.shrink_to_fit();
}

void StreetSeg::testoutput()
{
    cout << ID << endl;
    cout << speed << endl;
}

char StreetSeg::gettype()
{
    return type;
}

int64_t StreetSeg::getID()
{
    return ID;
}

char StreetSeg::getType()
{
    return type;
}

int64_t StreetSeg::getStreet()
{
    return streetID;
}

double StreetSeg::getLength()
{
    return length;
}

double StreetSeg::getSpeed()
{
    return speed;
}

double StreetSeg::getTime()
{
    return drivingTime;
}

pair<int64_t,int64_t> StreetSeg::getEnds()
{
    return ends;
}

vector<Cord> StreetSeg::getBendPoints()
{
    return bendPoints;
}


void StreetSeg::setBendPoints(vector<Cord> _bendPoints)
{
    bendPoints = _bendPoints;
}

void StreetSeg::setboundary()
{
           
       double   xMaxCalc = -1 * XYBOUNDS;
       double     xMinCalc = XYBOUNDS;
           double yMaxCalc = -1 * XYBOUNDS;
          double  yMinCalc = XYBOUNDS;
           double tx;
           double ty;

        for(unsigned j = 0; j < bendPoints.size();j++)
        {

             if(bendPoints[j].getx() < xMinCalc)xMinCalc = bendPoints[j].getx();
             if(bendPoints[j].getx() > xMaxCalc)xMaxCalc = bendPoints[j].getx();
             if(bendPoints[j].gety() < yMinCalc)yMinCalc = bendPoints[j].gety();
            if(bendPoints[j].gety() > yMaxCalc)yMaxCalc = bendPoints[j].gety();
        
        }
 
           
          tx = endsCord.first.getx();
          ty = endsCord.first.gety();

               if(tx < xMinCalc)xMinCalc = tx;
             if(tx > xMaxCalc)xMaxCalc = tx;
             if(ty < yMinCalc)yMinCalc = ty;
            if(ty > yMaxCalc)yMaxCalc = ty;
          
          tx = endsCord.second.getx();
          ty = endsCord.second.gety();

                if(tx < xMinCalc)xMinCalc = tx;
             if(tx > xMaxCalc)xMaxCalc = tx;
             if(ty < yMinCalc)yMinCalc = ty;
            if(ty > yMaxCalc)yMaxCalc = ty;
           
             if(xMaxCalc - xMinCalc < 2)
   {
       xMaxCalc ++;
       xMinCalc --;
   }      
      if(yMaxCalc - yMinCalc < 2)
   {
       yMaxCalc ++;
       yMinCalc --;
   }      
          
           boundary = t_bound_box(xMinCalc,yMinCalc,xMaxCalc,yMaxCalc);
}
 t_bound_box StreetSeg::getboundary()
 {
     return boundary;
 }

char StreetSeg::gethideHighwayLowZoom()
{
    return hideHighwayLowZoom;
}
void StreetSeg::sethideHighwayLowZoom(char _hideHighwayLowZoom)
{
    hideHighwayLowZoom = _hideHighwayLowZoom;
}

void StreetSeg::setEndsCord(Cord a, Cord b)
{
    pair<Cord,Cord> _endsCord;
    _endsCord.first = a;
    _endsCord.second = b;
    endsCord = _endsCord;
}

pair<Cord,Cord> StreetSeg::getEndsCord()
{
    return endsCord;
}