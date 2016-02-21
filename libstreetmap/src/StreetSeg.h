/* 
 * File:   StreetSeg.h
 * Author: chenyu69
 *
 * Created on January 26, 2015, 4:44 PM
 */

#ifndef STREETSEG_H
#define	STREETSEG_H
#include <vector>
#include "Cord.h"
#include "graphics.h"
using namespace std;

//used to declare street segment types
//can be expanded with more types later 
enum
{
    ONEWAY = 0,
    TWOWAY = 1
};

enum
{
    HIDDEN = 0,
    VISIBLE = 1
    
};

class StreetSeg 
{
    public:
        StreetSeg();
        StreetSeg(const StreetSeg& orig);
        StreetSeg(int64_t _ID);
        StreetSeg(char _type,int64_t _ID,int64_t _streetID, double _speed,
        double _length, double _drivingTime, pair<int64_t,int64_t> _ends);
        virtual ~StreetSeg();

        //debug and optimization functions
        void testoutput();
        void memCleanUp();

        //accessor functions
        int64_t getID();
        int64_t getStreet();
        double getLength();
        double getSpeed();
        double getTime();
       // double getbottom();
       // double gettop();
       // double getleft();
       // double getright();
        
        char getType();
        pair<int64_t,int64_t> getEnds();
        vector<Cord> getBendPoints();
        void setBendPoints(vector<Cord> _bendPoints);
        //void setCords( double _bottom, double _top,double _left,double _right);
        char gethideHighwayLowZoom();
        void sethideHighwayLowZoom(char _hideHighwayLowZoom);
        t_bound_box getboundary();
        void setboundary();
        void setEndsCord(Cord a, Cord b);
        pair<Cord,Cord> getEndsCord();
        char gettype();

    private:
        char type;//contains information about this segment like directionality
        //defined in enum above
        char hideHighwayLowZoom;
        double speed;//speed limit for this street segment
        double length;//length of street segment in meters
        double drivingTime;//length of shortest time to travel along street segment
        //theoretically through speed limit
        
        int64_t ID;
        int64_t streetID;
        vector<Cord> bendPoints;//vector of curved points on this street segment
        //double top,bottom,left,right;
        
        t_bound_box boundary;
         pair<int64_t,int64_t> ends;//IDs of the intersections at the end points
         
         pair<Cord, Cord> endsCord; // for minimap drawing, have intersection cords instead
};

#endif	/* STREETSEG_H */

