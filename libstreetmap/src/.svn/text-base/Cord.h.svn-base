/* 
 * File:   Cord.h
 * Author: chenyu69
 *
 * Created on February 22, 2015, 6:03 PM
 */

#ifndef CORD_H
#define	CORD_H

#include <cmath>
#include <cstdint>
#include <string>
#include <vector>
#define XYBOUNDS 100000000
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define EARTH_RADIUS_IN_METERS 6372797.560856
#define PI 3.14159265359
using namespace std;
//x,y system is setup in meters, in theory it should never approach this large
//number

//class used to graphically represent location interms of x,y and latlon
//mainly used to aid the rotation feature


//extern int64_t mapArea;



class Cord {

public:
    static double totalRotation;
    static double rotateAngle;
    static double basemaxx,basemaxy,baseminx,baseminy;
    static double height,width;
    static double maxx,maxy,miny,minx;
    //rotation axis in terms of drawn x,y cordinate
    static double baseCenterx,baseCentery;
    //offset to make the bottom left corner (0,0)
    static double xOffset;
    static double yOffset;
    //bounds for drawing map
    static double xBound;
    static double yBound;
    static vector <double> allRotations;
    static vector <pair<double,double> > rotationCenters;
    static vector <pair<double,double> > rotationOffsets;
    Cord();
    Cord(double _lat, double _lon, double _x, double _y);
    Cord(const Cord& orig);
    virtual ~Cord();
    void rotateMapCord();
    double getx();
    double gety();
    double getlat(); //CAPITALIZE PROPERLY JESUS
    double getlon();
    
    double getbasex();
    double getbasey();
    
    void drawOffset();//computers the offset and adjust cordinates accordingly
    
    void setxy();//helper method, like an extension of a constructor
    
    static void calcOffset(double xEdge, double yEdge);

    static void setupRotate(double _rotation);

    static void rotationCenter(double oldx,double oldy);
    static void recordRotation();
    //these timerotation features assume the point is within the original map
    static pair<double,double> newToOld(double _x,double _y);
    static pair<double,double> oldToNew(double _x,double _y);
    
private:
    double x,y;
    double lat,lon;
    double basex,basey;
};

#endif	/* CORD_H */

