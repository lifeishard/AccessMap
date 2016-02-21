/* 
 * File:   Point.h
 * Author: chenyu69
 *
 * Created on January 26, 2015, 4:43 PM
 */

#ifndef POINT_H
#define	POINT_H
#include "Cord.h"
#include <vector>
using namespace std;
extern const int64_t notInitialized;
//class used to store points of interest data
class Point 
{
    public:
        Point();
        Point(int64_t _ID,string _name, vector<string> _displayString);
        Point(const Point& orig);
        virtual ~Point();


        void testoutput();
        vector <string> getdisplayString();
        Cord getLocation();
        string getName();
        int64_t getClosestIntersect();
        void setClosestIntersect(int64_t _closestIntersect);
        
        void setLocation(Cord _location);
    private:
        int64_t ID;
        int64_t closestIntersect;
        Cord location;
        string name;
        vector <string> displayString;
};

#endif	/* POINT_H */

