/* 
 * File:   Street.h
 * Author: chenyu69
 *
 * Created on January 26, 2015, 4:41 PM
 */

#ifndef STREET_H
#define	STREET_H
#include "Cord.h"
#include "graphics.h"
#include <vector>
using namespace std;

//class used to store data about individual streets
class Street 
{
    public:
        Street();
        Street(const Street& orig);
        Street(int64_t _ID,string _name,double _length,vector<int64_t> _StreetSegs,
        vector<int64_t> _Intersects,vector<string> _displayString);
        virtual ~Street();

        //debug and optimization methods
        void memCleanUp();
        void testoutput();

        //accessor methods
        string getName();
        vector<int64_t> getSegs();
        vector<int64_t> getIntersects();
        void setboundary(t_bound_box _boundary);
        t_bound_box getboundary();
        vector<string> getdisplayString();
        double getLength();

    private:
        int64_t ID;
        double length;
        string name;
        vector<int64_t> StreetSegs;//list of Street Segment IDs belonging to this 
        //street
        vector<int64_t> Intersects;//list of intersection IDs belonging to this
        //street
        vector<string> displayString;
        t_bound_box boundary;
        
};

#endif	/* STREET_H */

