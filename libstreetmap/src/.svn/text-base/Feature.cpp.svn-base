/* 
 * File:   Feature.cpp
 * Author: chenyu69
 * 
 * Created on January 27, 2015, 11:27 PM
 */

#include "Feature.h"
#include <iostream>
using namespace std;

double miniX, miniY;
double miniMultiplierX, miniMultiplierY;

Feature::Feature(int64_t _ID, char _shape, int64_t _nPoints)
{
    ID = _ID;
    shape = _shape;
    nPoints = _nPoints;
    
    if(nPoints > 0) {
        polyPoints = new t_point[nPoints];
        polyMiniPoints = new t_point[nPoints];
    } else {
        polyPoints = NULL;
        polyMiniPoints = NULL;
    }
    
    colorID = DEFAULTC; //set default color to test for missing features
    
    //closed features are polygons
    if (_shape) {
        lineStyle = SOLID;
        lineWidth = THINNESTLINE;
    //converts vector of cords to array of t_points for fillpoly
    //naming convention for type is "firststring-secondstring"
    //LAKEBLUE is an example doesn't actually exist
    //TESTSCHEME = 0,
    //BORDERC = 1,
    //STREETC = 2,
    //HIGHWAYC = 3,
    //WATERC = 4,
    //SANDC = 5,
    //GRASSC = 6,
    //POIC = 7
        
        //water features
        if(getFeatureAttribute(_ID, "water") == "pond") {
            featureType = "water-pond";
            colorID = LIGHTBLUEC;
        } else if(getFeatureAttribute(_ID, "water") == "lagoon") {
            featureType = "water-lagoon";
            colorID = DARKBLUEC;
        } else if(getFeatureAttribute(_ID, "water") == "reservoir") {
            featureType = "water-reservoir";
            colorID = DARKBLUEC;
        } else if(getFeatureAttribute(_ID, "water") == "lake") {
            featureType = "water-lake";
            colorID = BLUEC;
        } else if(getFeatureAttribute(_ID, "natural") == "water" ) {
            featureType = "natural-water";
            colorID = BLUEC;
        } else if(getFeatureAttribute(_ID, "landuse") == "reservoir" ) {
            featureType = "landuse-reservoir";
            colorID = DARKBLUEC;
        } else if(getFeatureAttribute(_ID, "man_made") == "breakwater" ) {
            featureType = "man_made-breakwater";
            colorID = BORDERC;
        } else if(getFeatureAttribute(_ID, "waterway") != "" ) {
            featureType = "waterway-"; // drain, weir, wier, dam, breakwater, rapids,
            colorID = LIGHTBLUEC; // ditch, yes
        } else if(getFeatureAttribute(_ID, "water") == "wetland") {
            featureType = "water-wetland";
            colorID = DARKBLUEC;
        } else if(getFeatureAttribute(_ID, "natural") == "wetland") {
            featureType = "natural-wetland";
            colorID = DARKBLUEC;
        } else if(getFeatureAttribute(_ID, "water") == "river") {
            featureType = "water-river";
            colorID = BLUEC;
        } else if(getFeatureAttribute(_ID, "natural") == "coastline") {
            featureType = "natural-coastline";
            colorID = LIGHTBLUEC;
        }
        
        //sand features
        else if(getFeatureAttribute(_ID, "natural") == "sand" ) {
            featureType = "natural-sand";
            colorID = TANC;
        } else if(getFeatureAttribute(_ID, "natural") == "beach" ) {
            featureType = "natural-beach";
            colorID = TANC;
        } else if(getFeatureAttribute(_ID, "land") == "island") {
            featureType = "land-island";
            colorID = TANC;
        } else if(getFeatureAttribute(_ID, "place") == "island") {
            featureType = "place-island";
            colorID = TANC;
        } else if(getFeatureAttribute(_ID, "building") == "yes") {
            featureType = "building-yes";
            colorID = TANC;
        } else if(getFeatureAttribute(_ID, "historical") == "memorial") {
            featureType = "historical-memorial";
            colorID = TANC;
        } else if(getFeatureAttribute(_ID, "tourism") == "attraction") {
            featureType = "tourism-attraction";
            colorID = PINKC;
        }
        
        //leafy features
        else if(getFeatureAttribute(_ID, "natural") == "wood" ) {
            featureType = "natural-wood";
            colorID = GREENC;
        } else if(getFeatureAttribute(_ID, "natural") == "scrub" ) {
            featureType = "natural-scrub";
            colorID = DARKGREENC;
        } else if(getFeatureAttribute(_ID, "natural") == "heath" ) {
            featureType = "natural-heath";
            colorID = GREENC;
        } else if(getFeatureAttribute(_ID, "natural") == "grassland" ) {
            featureType = "natural-grassland";
            colorID = LIGHTGREENC;
        } else if(getFeatureAttribute(_ID, "leisure") == "park" ) {
            featureType = "leisure-park";
            colorID = LIGHTGREENC;
        } else if(getFeatureAttribute(_ID, "landuse") == "park" ) {
            featureType = "landuse-park";
            colorID = LIGHTGREENC;
        } else if(getFeatureAttribute(_ID, "landuse") == "recreation_ground" ) {
            featureType = "landuse-recreation_ground";
            colorID = GREENC;
        } else if(getFeatureAttribute(_ID, "landuse") == "forest" ) {
            featureType = "landuse-forest";
            colorID = GREENC;
        } else if(getFeatureAttribute(_ID, "landuse") == "conservation" ) {
            featureType = "landuse-conservation";
            colorID = DARKGREENC;
        } else if(getFeatureAttribute(_ID, "landuse") == "grass" ) {
            featureType = "landuse-grass";
            colorID = GREENC;
        } else if(getFeatureAttribute(_ID, "leisure") == "nature_reserve" ) {
            featureType = "leisure-nature_reserve";
            colorID = DARKGREENC;
        } else if(getFeatureAttribute(_ID, "natural") == "tree" ) {
            featureType = "natural-tree";
            colorID = GREENC;
        } else if(getFeatureAttribute(_ID, "natural") == "land" ) {
            featureType = "natural-land";
            colorID = GREENC;
        } else if(getFeatureAttribute(_ID, "natural") == "cliff" ) {
            featureType = "natural-cliff";
            colorID = DARKGREENC;
        } else if(getFeatureAttribute(_ID, "natural") == "peak" ) {
            featureType = "natural-peak";
            colorID = DARKGREENC;
        } else if(getFeatureAttribute(_ID, "sport") != "" ) {
            featureType = "sport-"; // soccer, basketball, hockey, skating,
            colorID = GREENC; // baseball, softball, tennis, football, skateboard
        } else if(getFeatureAttribute(_ID, "natural") == "tree_row" ) {
            featureType = "natural-tree_row";
            colorID = GREENC;
        } else if(getFeatureAttribute(_ID, "natural") == "meadow" ) {
            featureType = "natural-meadow";
            colorID = LIGHTGREENC;
        } else if(getFeatureAttribute(_ID, "natural") == "ravine" ) {
            featureType = "natural-ravine";
            colorID = DARKGREENC;
        } else if(getFeatureAttribute(_ID, "natural") == "grassland" ) {
            featureType = "natural-grassland";
            colorID = LIGHTGREENC;
        } else if(getFeatureAttribute(_ID, "natural") == "grass" ) {
            featureType = "natural-grass";
            colorID = GREENC;
        }
        
        // misc features
        else if(getFeatureAttribute(_ID, "barrier") == "hedge" ) {
            featureType = "barrier-hedge"; 
            colorID = LIGHTGREENC;
        } else if(getFeatureAttribute(_ID, "landuse") == "construction" ) {
            featureType = "landuse-construction";
            colorID = TANC;
        } else if(getFeatureAttribute(_ID, "natural") == "fell" ) {
            featureType = "natural-fell"; 
            colorID = BROWNC;
        } else if(getFeatureAttribute(_ID, "natural") == "mud" ) {
            featureType = "natural-mud"; 
            colorID = CAMOC;
        } else if(getFeatureAttribute(_ID, "natural") == "slope" ) {
            featureType = "natural-slope";
            colorID = BROWNC;
        } else if(getFeatureAttribute(_ID, "natural") == "yes" ) {
            featureType = "natural-yes";
            colorID = GREENC;
        } else if(getFeatureAttribute(_ID, "natural") == "stone" ) {
            featureType = "natural-stone";
            colorID = GREYC;
        }
        
        else
        {
            cout << "1. " << getFeatureAttribute(_ID, "natural") <<
                    "2. " << getFeatureAttribute(_ID, "water") <<
                    "3. " << getFeatureAttribute(_ID, "waterway");
            cout << "MISSED CLOSE FEATURE" << endl;
        }

         
    //open features   
    } else { 
        
        //green features
        if(getFeatureAttribute(_ID, "natural") == "tree_row") {
            featureType = "natural-tree_row";
            colorID = GREENC;
            lineStyle = DASHED;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "barrier") == "fence") {
            featureType = "barrier-fence";
            colorID = BORDERC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "leisure") == "park") {
            featureType = "leisure-park";
            colorID = LIGHTGREENC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "leisure") == "nature_reserve") {
            featureType = "leisure-nature_reserve";
            colorID = DARKGREENC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "barrier") == "retaining_wall") {
            featureType = "barrier-retaining_wall";
            colorID = TANC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "barrier") == "hedge") {
            featureType = "barrier-hedge";
            colorID = LIGHTGREENC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "boundary") == "protected_area") {
            featureType = "boundary-protected_area";
            colorID = DARKGREENC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "boundary") == "national_park") {
            featureType = "boundary-national_park";
            colorID = GREENC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "boundary") == "administrative") {
            featureType = "boundary-administrative";
            colorID = TANC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "natural") == "tree") {
            featureType = "natural-tree";
            colorID = GREENC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "natural") == "cliff") {
            featureType = "natural-cliff";
            colorID = DARKGREENC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "natural") == "water") {
            featureType = "natural-tree";
            colorID = BLUEC;
            lineStyle = SOLID;
            lineWidth = NORMALLINE;
        } else if(getFeatureAttribute(_ID, "natural") == "wood") {
            featureType = "natural-tree";
            colorID = GREENC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "natural") == "scrub") {
            featureType = "natural-scrub";
            colorID = DARKGREENC;
            lineStyle = SOLID;
            lineWidth = NORMALLINE;
        } else if(getFeatureAttribute(_ID, "natural") == "grassland") {
            featureType = "natural-grassland";
            colorID = LIGHTGREENC;
            lineStyle = SOLID;
            lineWidth = NORMALLINE;
        } else if(getFeatureAttribute(_ID, "natural") == "landform") {
            featureType = "natural-landform";
            colorID = BROWNC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } 
        
        //blue features
        else if(getFeatureAttribute(_ID, "waterway") == "stream" ) {
            featureType = "waterway-stream";
            colorID = LIGHTBLUEC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "waterway") == "steram" ) { // data has spelling error
            featureType = "waterway-stream";
            colorID = LIGHTBLUEC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "waterway") == "rapids" ) {
            featureType = "waterway-rapids";
            colorID = LIGHTBLUEC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "waterway") == "river" ) {
            featureType = "waterway-river";
            colorID = BLUEC;
            lineStyle = SOLID;
            lineWidth = THICKLINE;
        } else if(getFeatureAttribute(_ID, "waterway") == "drain" ) {
            featureType = "waterway-drain";
            colorID = DARKBLUEC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "waterway") == "canal" ) {
            featureType = "waterway-canal";
            colorID = BLUEC;
            lineStyle = SOLID;
            lineWidth = NORMALLINE;
        } else if(getFeatureAttribute(_ID, "waterway") == "ditch" ) {
            featureType = "waterway-ditch";
            colorID = DARKBLUEC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "waterway") == "wadi" ) {
            featureType = "waterway-wadi";
            colorID = DARKBLUEC;
            lineStyle = SOLID;
            lineWidth = THICKESTLINE;
        } else if(getFeatureAttribute(_ID, "waterway") == "dam" ) {
            featureType = "waterway-dam";
            colorID = BLUEC;
            lineStyle = SOLID;
            lineWidth = NORMALLINE;
        } else if(getFeatureAttribute(_ID, "waterway") == "aquaduct" ) { // misspelt
            featureType = "waterway-aquaduct";
            colorID = LIGHTBLUEC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "waterway") == "riverbank" ) {
            featureType = "waterway-riverbank";
            colorID = LIGHTBLUEC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "waterway") == "weir" ) {
            featureType = "waterway-weir";
            colorID = GREYC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "waterway") == "wier" ) { // more spelling errors
            featureType = "waterway-weir";
            colorID = GREYC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "waterway") == "lock_gate" ) {
            featureType = "waterway-lock_gate";
            colorID = GREYC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "waterway") == "breakwater" ) {
            featureType = "waterway-breakwater";
            colorID = GREYC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "waterway") == "<different>" ) {
            featureType = "waterway-<different>";
            colorID = BLUEC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "natural") == "wetland") {
            featureType = "natural-wetland";
            colorID = DARKBLUEC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } else if(getFeatureAttribute(_ID, "water") == "river") {
            featureType = "water-river";
            colorID = BLUEC;
            lineStyle = SOLID;
            lineWidth = THICKLINE;
        } else if(getFeatureAttribute(_ID, "water") == "lake") {
            featureType = "water-lake";
            colorID = BLUEC;
            lineStyle = SOLID;
            lineWidth = THICKESTLINE;
        } else if(getFeatureAttribute(_ID, "natural") == "coastline") { // DO SOMETHING ELSE HERE
            featureType = "natural-coastline"; // this is saint helena coastline
            colorID = LIGHTBLUEC;
            lineStyle = SOLID;
            lineWidth = THICKESTLINE;
        }
        
        // misc features
        else if(getFeatureAttribute(_ID, "natural") == "slope" ) {
            featureType = "natural-slope"; 
            colorID = BROWNC;
            lineStyle = SOLID;
            lineWidth = THINNESTLINE;
        } 
        else
        {
            cout << "1. " << getFeatureAttribute(_ID, "natural") <<
                    "2. " << getFeatureAttribute(_ID, "water") <<
                    "3. " << getFeatureAttribute(_ID, "waterway");
            cout << "MISSED OPEN FEATURE" << endl;
        }

    }

}

Feature::Feature() 
{
    polyPoints = NULL;
    polyMiniPoints = NULL;
}

Feature::Feature(const Feature& orig) 
{
    ID = orig.ID;
    shape = orig.shape;
    drawPoints = orig.drawPoints;
    boundary = orig.boundary;
    colorID = orig.colorID;
    featureType = orig.featureType;
    lineStyle = orig.lineStyle;
    lineWidth = orig.lineWidth;
    nPoints = orig.nPoints;
    
    if(nPoints > 0)
    {
    polyPoints = new t_point[nPoints];
    polyMiniPoints = new t_point[nPoints];
    /*
    for(unsigned i = 0; i < nPoints; i++)
    {
        cout<<i<<endl;
        polyPoints[i] = orig.polyPoints[i];
        polyMiniPoints[i] = orig.polyMiniPoints[i];
    }*/
    memcpy(polyPoints,orig.polyPoints,nPoints * sizeof(t_point));
 //   cout<<"donecopy1"<<endl;
    memcpy(polyMiniPoints,orig.polyMiniPoints,nPoints * sizeof(t_point));
     //cout<<"donecopy"<<endl;
    }
    else 
    {
        polyPoints = NULL;
        polyMiniPoints = NULL;
    }
       // cout<<"donecopy"<<endl;
}
int64_t Feature::getnPoints()
{
    return nPoints;
}

t_bound_box Feature::getboundary()
{
    return boundary;
}

int64_t Feature::getcolorID()
{
    return colorID;
}

string Feature::getfeatureType()
{
    return featureType;
}

int64_t Feature::getlineStyle()
{
    return lineStyle;
}

int64_t Feature::getlineWidth()
{
    return lineWidth;
}

t_point* Feature::getpolyPoints()
{
    return polyPoints;
}

t_point* Feature::getpolyMiniPoints()
{
    return polyMiniPoints;
}

Feature::~Feature() 
{
    if(polyPoints != NULL)
    {
        delete [] polyPoints;
    }
    if(polyMiniPoints != NULL)
    {
        delete [] polyMiniPoints;
    }
          //  cout<<"donedelete"<<endl;
}

void Feature::testoutput()
{
    cout << ID << endl;
    cout << drawPoints.size() << endl;
    cout << int(shape) << endl;
}

void Feature::setdrawPoints(vector<Cord> & _drawPoints)
{
    drawPoints = _drawPoints;
}

void Feature::updatepolyPoints()
{
    for(unsigned i = 0; i < nPoints; i++)
    {
        polyPoints[i] = t_point(drawPoints[i].getx(),drawPoints[i].gety());
    }
}

void Feature::updatepolyMiniPoints()
{
    for(unsigned i = 0; i < nPoints; i++)
    {
        polyPoints[i] = t_point(drawPoints[i].getx()*miniMultiplierX+miniX,drawPoints[i].gety()*miniMultiplierY+miniY);
    }
}

vector<Cord> Feature::getdrawPoints()
{
    return drawPoints;
}

int64_t Feature::getID()
{
    return ID;
}

bool Feature::isClosed()
{
    return shape;
}

void Feature::setboundary()
{        
   double xMaxCalc = -1 * XYBOUNDS;
   double xMinCalc = XYBOUNDS;
   double yMaxCalc = -1 * XYBOUNDS;
   double yMinCalc = XYBOUNDS;
           
        for(unsigned j = 0; j < drawPoints.size();j++)
        {
           // tempFeatures[i][j].drawOffset();
            if(drawPoints[j].getx() < xMinCalc)xMinCalc = drawPoints[j].getx();
             if(drawPoints[j].getx() > xMaxCalc)xMaxCalc = drawPoints[j].getx();
             if(drawPoints[j].gety() < yMinCalc)yMinCalc = drawPoints[j].gety();
            if(drawPoints[j].gety() > yMaxCalc)yMaxCalc = drawPoints[j].gety();
        }
   
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
           boundary =  t_bound_box(xMinCalc, yMinCalc, xMaxCalc, yMaxCalc);
          // if(shape == 0 )cout << boundary.area()<<endl;
          //store the rectangular boundary cords for the feature
}
