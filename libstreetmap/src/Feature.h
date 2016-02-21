/* 
 * File:   Feature.h
 * Author: chenyu69
 *
 * Created on January 27, 2015, 11:27 PM
 */

#ifndef FEATURE_H
#define	FEATURE_H
#include "Cord.h"
#include "graphics.h"
#include "Color.h"
#include "StreetsDatabaseAPI.h"
#include <cstring>
#include <vector>
using namespace std;

//DASHED and SOLID are defined elsewhere


enum
{
    OPENFEATURE = 0,
    CLOSEDFEATURE = 1
};  

enum
{
    //naming should USAGE+LINE
    THINNESTLINE = 10000, // Using a function of current screen so this value should be good
    NORMALLINE = 20000,
    THICKLINE = 30000,
    THICKESTLINE = 40000
};
//class containing featurenews used for future milestones
//such as parks, lakes etc..
class Feature 
{
    public:
        Feature();
        Feature(const Feature& orig);
        Feature(int64_t _ID, char _shape, int64_t _nPoints);
        virtual ~Feature();
        void setdrawPoints(vector<Cord> & _drawPoints);
        vector<Cord> getdrawPoints();
        void testoutput();
        
        int64_t getID();
        bool isClosed();
        int64_t getnPoints();

        t_bound_box getboundary();
        int64_t getcolorID();
        string getfeatureType();
        int64_t getlineStyle();
        int64_t getlineWidth();
        t_point* getpolyPoints();
        t_point* getpolyMiniPoints();
        
        
        void updatepolyPoints();
        void updatepolyMiniPoints();
        void setboundary();
        //void setCords( double _bottom, double _top,double _left,double _right);
       // double getbottom();
       // double gettop();
       // double getleft();
       // double getright();
        

    private:
        char shape;
        int64_t ID;
        int64_t colorID;
        int64_t lineStyle;
        int64_t lineWidth;
        int64_t nPoints;
        string featureType;
        vector<Cord> drawPoints;
        t_bound_box boundary;
        t_point * polyPoints;
        t_point * polyMiniPoints;
        //double left,right,top,bottom;
        
        
        
};

#endif	/* FEATURE_H */

