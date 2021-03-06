#include "Custapi.h"

using namespace std;

//Gobal Data structures 

vector<StreetSeg> allStreetSegs;
vector<StreetSeg> allLargeStreetSegs;
vector<Intersect> allIntersects;
vector<Point> allPoints;
vector<Street> allStreets;
vector<Feature> allFeatures;
vector<Feature> allLargeFeatures;
vector<int64_t> allHighwayStreetSegs;
vector<int64_t> allHighwayIntersects;
vector<pair<double,double> > allDrawInter;
unordered_map<string, int64_t> IntersectNametoID;
unordered_map<string, int64_t> StreetNametoID;
unordered_map<string, int64_t> PointNametoID;

double meanLat;//average latitude of city used to convert all cordinates
double rotateAngle;
double xBound,yBound;
double totalLat;
double distSpeedVar = 0;
//double xMaxCalc,xMinCalc,yMaxCalc,yMinCalc;
char colorMode;
const int64_t streetNameBreakLength = 15;


vector<string> lineBreaker(string orig,unsigned spaceLen)
{
    vector<string> displayString;
    string spaceStr;
    unsigned prev = 0;
    //looks to replace spaces past a certain length with newline characters
    //to make long names displayed on maps easier to read
    for(unsigned i = 0; i < orig.length(); i++){
        if(isspace(orig[i]) && (i - prev) > spaceLen){
                prev = i;
                //spaceStr += '\r';
                displayString.push_back(spaceStr);
                spaceStr = "";
                //spaceStr += '\n';
            }
       
            else{
                spaceStr += orig[i];
            }
        }
    if(spaceStr != "")displayString.push_back(spaceStr);
    return displayString;
}

//overloaded operator for LatLon
bool operator == (LatLon point1, LatLon point2)
{
    return (point1.lat == point2.lat)&&(point1.lon == point2.lon);
}

bool operator != (LatLon point1, LatLon point2)
{
    return !(point1 == point2);
}



//DO NOT USE THIS FUNCTION FOR NUMERICAL DISTANCE CALCULATIONS!!!
//IT WILL NOT GIVE CORRECT DISTANCES!!!
//sqrt is removed since if a > b, a^2 > b^2
//as well as linear constant EARTH_RADIUS_IN_METERS
double latLonDistance(double lat1, double lon1, double lat2, double lon2) 
{
    double latavg = (lat1 + lat2) / 2.0;
    double dx = (lon2 * cos(DEG_TO_RAD * latavg)) - (lon1 * cos(DEG_TO_RAD * latavg));
    double dy = lat2 - lat1;
    return (dx*dx) + (dy*dy);
}


//find distance between two coordinates and return distance in meters
double distance(LatLon point1, LatLon point2) 
{
    double latavg = (point1.lat + point2.lat) / 2.0;
    double x1 = ( point1.lon * cos(DEG_TO_RAD * latavg));
    double x2 = ( point2.lon * cos(DEG_TO_RAD * latavg));
    double y1 =  point1.lat;
    double y2 =  point2.lat;
    return DEG_TO_RAD *EARTH_RADIUS_IN_METERS * hypot(y2 - y1, x2 - x1);
}



double Olat2;
double Olon2;//in m per sec
double OhalfDEGTORAD = DEG_TO_RAD/2.0;
double ODEGproductEARTHRADIUS = DEG_TO_RAD*EARTH_RADIUS_IN_METERS;
double latLonOptimize(double lat1,double lon1) 
{
   // double lat1 = latlon1.first;
    //double lon1 = latlon1.second;
    double xConversion = cos(OhalfDEGTORAD*(lat1+Olat2));
    double xDiff = xConversion * (lon1-Olon2);
    double yDiff = lat1-Olat2;
    return xDiff*xDiff + yDiff*yDiff;
    //return 0;
}
void findClosestPOIIntersection(int64_t pointID)
{
        double minDist;
        double tempDist;
        int64_t tempIntersectID;

            minDist = 200000000000000;
            Olat2 = allPoints[pointID].getLocation().getlat();
            Olon2 = allPoints[pointID].getLocation().getlon();
            for(unsigned j = 0; j < allIntersects.size(); j++)
            {
                tempDist = latLonOptimize(allIntersects[j].getLocation().getlat(),allIntersects[j].getLocation().getlon());
                if(tempDist < minDist)
                {
                    minDist = tempDist;
                    tempIntersectID = j;
                }
            }
            allPoints[pointID].setClosestIntersect(tempIntersectID);
            
        
}




// Distance between two Cords
double cordDistance(Cord a, Cord b)
{
    return pow(pow(a.getx()-b.getx(),2)+pow(a.gety()-b.gety(),2),0.5);
}

// Distance between pair<double,double> and Cord
double cordDistance(pair<double,double> a, Cord b)
{
    return pow(pow(a.first-b.getx(),2)+pow(a.second-b.gety(),2),0.5);
}

//convert coordinates to x,y system in meters
pair<double, double> realCord(LatLon point1) 
{
    return make_pair(EARTH_RADIUS_IN_METERS * DEG_TO_RAD * point1.lon 
            * cos(DEG_TO_RAD * meanLat), DEG_TO_RAD * EARTH_RADIUS_IN_METERS 
            * point1.lat);
}

//converts a LatLon into Cord object
Cord drawCord(LatLon point1)
{
    return Cord(point1.lat, point1.lon, EARTH_RADIUS_IN_METERS * DEG_TO_RAD 
            * point1.lon * cos(DEG_TO_RAD * meanLat), DEG_TO_RAD 
            * EARTH_RADIUS_IN_METERS  * point1.lat);
}

//removes potential duplicate IDs from a vector
//by sorting, uniquing it and resize it afterwards;
vector<int64_t> removeIDDuplicates(vector<int64_t> orig)
{
        sort(orig.begin(), orig.end());
        vector<int64_t>::iterator uniqueit = unique(orig.begin(), orig.end());
        orig.resize(uniqueit - orig.begin());
        return orig;
}

//main function to initialize the global data structures
void initializeCust() 
{
    
    //reduce load map time by reducing need of vector/hash map resizing
    allStreetSegs.reserve(getNumberOfStreetSegments());
    allStreets.reserve(getNumberOfStreets());
    allIntersects.reserve(getNumberOfIntersections());
    allPoints.reserve(getNumberOfPointsOfInterest());
    allFeatures.reserve(getFeatureCount());
    IntersectNametoID.reserve(getNumberOfIntersections());
    StreetNametoID.reserve(getNumberOfStreets());
    PointNametoID.reserve(getNumberOfPointsOfInterest());
    
    vector<vector<int64_t> >tempStreetSegs;
    vector<vector<int64_t> >tempIntersectSegs;
    vector<vector<int64_t> >tempNeighbours;
    vector<vector<int64_t> >tempStreetIntersects;
    vector<vector<int64_t> >tempStreets;
    vector<vector<Cord> >tempCurvePoints;
    
    vector<int64_t>::iterator uniqueit;
    vector<double> tempStreetLength;
    double tempLength;
    double tempTime;
    double totalLat;
    char tempType;
    char tempFlag;
    Cord tempLoc;

    pair<int64_t,int64_t> tempEnds;
    LatLon tempStart, tempEnd;
    pair<double, double> tempCord;
    pair<double, double> tempCord1;
    
    const double hideHighwaySegDistance = 100;
    const double hideHighwaySegSpeed = 70;
    
      double xTotal,yTotal;
    vector<pair<double,double> > meanSegxy;
    vector<int64_t> hideHighwayVisibleID;
double    xMaxCalc = -1 * XYBOUNDS;
double    xMinCalc = XYBOUNDS;
double    yMaxCalc = -1 * XYBOUNDS;
double    yMinCalc = XYBOUNDS;

    totalLat = 0;
    //create empty 2D vectors used later for each intersection
    for (unsigned i = 0; i < getNumberOfIntersections(); i++) {
        tempIntersectSegs.push_back(vector<int64_t>());
        tempNeighbours.push_back(vector<int64_t>());
        tempStreets.push_back(vector<int64_t>());
        totalLat += getIntersectionPosition(i).lat;
    }
    meanLat = totalLat/getNumberOfIntersections();
    //calculating average Latitude by taking mean lat of all the intersections
   
    //create empty 2D vectors used later for each Street
    for (unsigned i = 0; i < getNumberOfStreets(); i++) {
        tempStreetSegs.push_back(vector<int64_t>());
        tempStreetIntersects.push_back(vector<int64_t>());
        tempStreetLength.push_back(0);
    }

    //initialize StreetSegments and store them in the global vector
    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++) {
        tempCurvePoints.push_back(vector<Cord>());
        //grab list of curve points from streetsdatabase api
        tempLength = 0;
        tempStart = getIntersectionPosition(getStreetSegmentEnds(i).from);
           
        //compute each segment's length by adding the length of curve points
        for (unsigned j = 0; j < getStreetSegmentCurvePointCount(i); j++) {
            //also convert the latlon into Cord
            
            tempCurvePoints[i].push_back(drawCord(getStreetSegmentCurvePoint(i, j)));
       
            tempEnd = getStreetSegmentCurvePoint(i, j);
            tempLength += distance(tempStart, tempEnd);
            tempStart = tempEnd;
        }
        
        tempLength += distance(tempStart, 
                getIntersectionPosition(getStreetSegmentEnds(i).to));
        //computes total length of a street by adding up its segments
        tempStreetLength[getStreetSegmentStreetID(i)] += tempLength;

        //which street has this segment
        tempStreetSegs[getStreetSegmentStreetID(i)].push_back(i);
        //which intersection is connected to this segment
        tempStreetIntersects[getStreetSegmentStreetID(i)].push_back(
        getStreetSegmentEnds(i).to);
        tempStreetIntersects[getStreetSegmentStreetID(i)].push_back(
        getStreetSegmentEnds(i).from);
        
        //which street is connected to this intersection
        tempStreets[getStreetSegmentEnds(i).from].push_back(
        getStreetSegmentStreetID(i));
        //which segment is connected to this street
        tempIntersectSegs[getStreetSegmentEnds(i).from].push_back(i);

        //self-loop detection
        if (getStreetSegmentEnds(i).from != getStreetSegmentEnds(i).to) {
            //adds the other segments and streets if not self loop
            tempStreets[getStreetSegmentEnds(i).to].push_back(
            getStreetSegmentStreetID(i));
            tempIntersectSegs[getStreetSegmentEnds(i).to].push_back(i);
            //neighbour basically means which intersection you can reach from 
            //current intersection
            //to is a neighbour of from iff they aren't the same intersection
            tempNeighbours[getStreetSegmentEnds(i).from].push_back(
            getStreetSegmentEnds(i).to);
            //from is a neighbour of to iff it isn't a one way street
            if (!getStreetSegmentOneWay(i))tempNeighbours[
            getStreetSegmentEnds(i).to].push_back(getStreetSegmentEnds(i).from);
        }
        
        //creating temporary variables and pass it to the constructor
       // tempTime = (0.06 * tempLength) / (1.0 * getStreetSegmentSpeedLimit(i));
        tempTime = tempLength * 3.6 / (1.0 *getStreetSegmentSpeedLimit(i));
        tempEnds = make_pair(getStreetSegmentEnds(i).from, 
                getStreetSegmentEnds(i).to);
        allStreetSegs.push_back(StreetSeg(getStreetSegmentOneWay(i) ? ONEWAY 
        : TWOWAY, i, getStreetSegmentStreetID(i), getStreetSegmentSpeedLimit(i),
                tempLength, tempTime,tempEnds));
        if( getStreetSegmentSpeedLimit(i) > distSpeedVar) distSpeedVar = getStreetSegmentSpeedLimit(i);
        allStreetSegs[i].setBendPoints(tempCurvePoints[i]);
        
        if(allStreetSegs[i].getSpeed() >= streetLargeThreshold)
        {
            allHighwayStreetSegs.push_back(i);
            allLargeStreetSegs.push_back(StreetSeg(i));
        }
       

        allStreetSegs[i].memCleanUp(); //removes reserved extra memory
       
    }
    distSpeedVar /= 3.6; 
    distSpeedVar /= ODEGproductEARTHRADIUS;
    cout << "INFO: Loaded " << allStreetSegs.size() << " street segments" 
            << endl;
   
    tempLength = 0;
    for (unsigned i = 0; i < getNumberOfIntersections(); i++) { 
        //removes self loops from occurring multiple times
        //removes multiple same streetsegments for each intersection
        tempIntersectSegs[i] = removeIDDuplicates(tempIntersectSegs[i]);
        //removes multiple same neighbours for each intersection
        tempNeighbours[i] = removeIDDuplicates(tempNeighbours[i]);
        //removes multiple same streets for each intersection
        tempStreets[i] = removeIDDuplicates(tempStreets[i]);
        
        allIntersects.push_back(Intersect(i, getIntersectionName(i),
                tempIntersectSegs[i], tempNeighbours[i], tempStreets[i], lineBreaker(getIntersectionName(i),streetNameBreakLength)));
        allIntersects[i].setLocation(drawCord(getIntersectionPosition(i)));    
        double currentSpeed = 0;
        double currentSize = 0;
        for(unsigned j = 0; j < tempIntersectSegs[i].size(); j++)
        {
            if(allStreetSegs[tempIntersectSegs[i][j]].getSpeed() > currentSpeed 
                    && getStreetName(allStreetSegs[tempIntersectSegs[i][j]].getStreet()) != "(unknown)")
            {
                currentSpeed = allStreetSegs[tempIntersectSegs[i][j]].getSpeed();
            }
            if(tempStreetLength[allStreetSegs[tempIntersectSegs[i][j]].getStreet()] > currentSize 
                    && getStreetName(allStreetSegs[tempIntersectSegs[i][j]].getStreet()) != "(unknown)")
            {
                currentSize = tempStreetLength[allStreetSegs[tempIntersectSegs[i][j]].getStreet()];
            }
        }
        allIntersects[i].setSpeed(currentSpeed);
        allIntersects[i].setLength(currentSize);

        //fast hash map for quick access time;
        IntersectNametoID[getIntersectionName(i)] = i;
        
        for(unsigned j = 0; j < tempIntersectSegs[i].size(); j++)
        {
            if(allStreetSegs[tempIntersectSegs[i][j]].getSpeed() >= streetLargeThreshold)
            {
                allHighwayIntersects.push_back(i);
                j = tempIntersectSegs[i].size();
            }
        }

        allIntersects[i].memCleanUp(); //removes reserved extra memory
    }
    cout << "INFO: Loaded " << allIntersects.size() << " intersects" << endl;
    
    for (unsigned i = 0; i < getNumberOfStreets(); i++) {
        tempStreetIntersects[i] = removeIDDuplicates( tempStreetIntersects[i]);
        
        allStreets.push_back(Street(i, getStreetName(i), 
        tempStreetLength[i], tempStreetSegs[i], tempStreetIntersects[i],
                lineBreaker(getStreetName(i),streetNameBreakLength)));
 
        //fast hash map for quick access time
      
        StreetNametoID[getStreetName(i)] = i;
        
        allStreets[i].memCleanUp(); //removes reserved extra memory
    }
    cout << "INFO: Loaded " << allStreets.size() << " streets" << endl;

    for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++) {
        allPoints.push_back(Point(i,getPointOfInterestName(i),
                lineBreaker(getPointOfInterestName(i),streetNameBreakLength))); 
        allPoints[i].setLocation(drawCord(getPointOfInterestPosition(i)));
       // Stores each point of interest into vector sorted by ID
   
        //fast hash map for quick access time

        PointNametoID[getPointOfInterestName(i)] = i;
    }
    
  for (unsigned i = 0; i < getFeatureCount(); i++) { 
        // Initializes all natural features
        // Stores each feature object into vector sorted by ID
    
      vector<Cord> tempFeatures;
      tempStart = getFeaturePoint(i,0);
      
      for(unsigned j = 0; j < getFeaturePointCount(i)-1; j++){
          tempEnd = getFeaturePoint(i,j);
          tempFeatures.push_back(drawCord(tempEnd));
         
      }
      tempEnd = getFeaturePoint(i,getFeaturePointCount(i)-1);
      if(tempStart!= tempEnd || getFeaturePointCount(i) < 4){
          //initialize the features depending on weather its a drawn like a 
          //closed polygon or line shape
        tempFeatures.push_back(drawCord(getFeaturePoint(i,
                getFeaturePointCount(i)-1)));  
        tempType = OPENFEATURE;
      }
      else {
          tempType = CLOSEDFEATURE;
      }
        allFeatures.push_back(Feature(i,tempType,tempFeatures.size()));
        allFeatures[i].setdrawPoints(tempFeatures);
    }
      
    setupColor();
    cout << "INFO: Loaded " << colorScheme.size() << " colors" << endl;
    

    
    Cord::baseCenterx = 0;
    Cord::baseCentery = 0;
    Cord::totalRotation = 0;
    
   // Cord::rotateAngle = 0*DEG_TO_RAD;
    Cord::setupRotate(0);
    vector<vector<Cord> >tempFeatures;
    
    
    //setup and rotate all the feature cordinates
    for (unsigned i = 0; i < getFeatureCount(); i++) { 
        tempFeatures.push_back(allFeatures[i].getdrawPoints());
        for(unsigned j = 0; j < tempFeatures[i].size();j++)
        {
            //tempFeatures[i][j].setxy();
            tempFeatures[i][j].rotateMapCord();
        }
    }
    
    //setup and rotate all the point of interests cordinates
    vector<Cord> tempPoints;
     for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++) {
         tempPoints.push_back(allPoints[i].getLocation());
       // tempPoints[i].setxy();
         tempPoints[i].rotateMapCord();
     }
    
    //setup and rotate all the intersection cordinates
      vector<Cord> tempInter;
    for (unsigned i = 0; i < getNumberOfIntersections(); i++) { 
         tempInter.push_back(allIntersects[i].getLocation());
          
          //tempInter[i].setxy();
         tempInter[i].rotateMapCord();
    }
      
      //setup and rotate all the street segments cordinates
        vector<vector<Cord> >tempSegs;
    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++) { 
        tempSegs.push_back(allStreetSegs[i].getBendPoints());
        for(unsigned j = 0; j < tempSegs[i].size();j++)
        {
           // tempSegs[i][j].setxy();
            tempSegs[i][j].rotateMapCord();
        }
    }
        
        
        //offset all the feature cordinates and calculate the boundaries of the 
        //feature, used later to test if the feature is in visible screen and
        //needs to be drawn
        Cord::calcOffset(0,0);
        
        
        
          for (unsigned i = 0; i < getFeatureCount(); i++) { 
              
           
        for(unsigned j = 0; j < tempFeatures[i].size();j++)
        {
            tempFeatures[i][j].drawOffset();
            tempFeatures[i][j].setxy();
         
        }
           
        allFeatures[i].setdrawPoints(tempFeatures[i]);
        allFeatures[i].setboundary();
        allFeatures[i].updatepolyPoints();
          //store the rectangular boundary cords for the feature
        
        // Getting large features into a vector of their own to be drawn later
        if(allFeatures[i].getboundary().area()/(Cord::xBound*Cord::yBound) >= largeFeatureThreshold)
        {
            allLargeFeatures.push_back(Feature(i,allFeatures[i].isClosed(),allFeatures[i].getnPoints()));
            allLargeFeatures[allLargeFeatures.size()-1].setdrawPoints(tempFeatures[i]);
            allLargeFeatures[allLargeFeatures.size()-1].updatepolyPoints();
        }
    }
        
   
        //offset points of interests
     for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++) {
         
         tempPoints[i].drawOffset();
         tempPoints[i].setxy();
         allPoints[i].setLocation(tempPoints[i]);
     }
       //offset intersections
        
    for (unsigned i = 0; i < getNumberOfIntersections(); i++) { 
         
         tempInter[i].drawOffset();
          tempInter[i].setxy();
         allIntersects[i].setLocation(tempInter[i]);
    }
       
//offset all the streetsegment cordinates and calculate the boundaries of the 
//street segment, used later to test if the street segment is in visible screen and
//needs to be drawn
          
    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++) { 
        

          xTotal = 0;
          yTotal = 0;
        for(unsigned j = 0; j < tempSegs[i].size();j++)
        {
              xTotal += tempSegs[i][j].getx();
              yTotal += tempSegs[i][j].gety();
            tempSegs[i][j].drawOffset();
            tempSegs[i][j].setxy();

        
        }
           double tx;
           double ty;
           
          tx = allIntersects[allStreetSegs[i].getEnds().first].getLocation().getx();
          ty = allIntersects[allStreetSegs[i].getEnds().first].getLocation().gety();
          xTotal += tx;
              yTotal += ty;

          
          tx = allIntersects[allStreetSegs[i].getEnds().second].getLocation().getx();
          ty = allIntersects[allStreetSegs[i].getEnds().second].getLocation().gety();
                    xTotal += tx;
              yTotal += ty;
              xTotal /= (2+tempSegs[i].size());
              yTotal /= (2+tempSegs[i].size());

           
           
         //  allStreetSegs[i].setCords( yMinCalc,  yMaxCalc, xMinCalc,xMaxCalc);
        allStreetSegs[i].setBendPoints(tempSegs[i]);
        allStreetSegs[i].setEndsCord(allIntersects[allStreetSegs[i].getEnds().first].getLocation(),
                allIntersects[allStreetSegs[i].getEnds().second].getLocation());
        allStreetSegs[i].setboundary();
        
        
        
        
        
 
       allStreetSegs[i].sethideHighwayLowZoom(VISIBLE);
        if(allStreetSegs[i].getSpeed() >= hideHighwaySegSpeed)
        {
             tempFlag = 1;
            for(unsigned j = 0; j < hideHighwayVisibleID.size(); j++)
            {
                if(hypot(xTotal - meanSegxy[j].first,yTotal - meanSegxy[j].second) < hideHighwaySegDistance)
                {
                    tempFlag = 0;
                    break;
                }
            }
             if(tempFlag)
             {
                 hideHighwayVisibleID.push_back(i);
                 meanSegxy.push_back(make_pair(xTotal,yTotal));
                 allStreetSegs[i].sethideHighwayLowZoom(VISIBLE);
             }
             else
             {
                 allStreetSegs[i].sethideHighwayLowZoom(HIDDEN);
             }
        }
        
        
          //store the rectangular boundary cords for the streetsegment
    }
        vector<int64_t> tempStreetSeg;
        for(unsigned i = 0; i < allStreets.size(); i++)
        {
              xMaxCalc = -1 * XYBOUNDS;
              xMinCalc = XYBOUNDS;
          yMaxCalc = -1 * XYBOUNDS;
              yMinCalc = XYBOUNDS;
              tempStreetSeg.clear();
            for(unsigned j = 0; j < allStreets[i].getSegs().size(); j++)
            {
                tempStreetSeg = allStreets[i].getSegs();
                if (allStreetSegs[tempStreetSeg[j]].getboundary().left() < xMinCalc)
                    xMinCalc = allStreetSegs[tempStreetSeg[j]].getboundary().left ();
                if (allStreetSegs[tempStreetSeg[j]].getboundary().right() > xMaxCalc)
                    xMaxCalc = allStreetSegs[tempStreetSeg[j]].getboundary().right();
                if (allStreetSegs[tempStreetSeg[j]].getboundary().bottom() < yMinCalc)
                    yMinCalc = allStreetSegs[tempStreetSeg[j]].getboundary().bottom();
                if (allStreetSegs[tempStreetSeg[j]].getboundary().top() > yMaxCalc)
                    yMaxCalc = allStreetSegs[tempStreetSeg[j]].getboundary().top();
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
           allStreets[i].setboundary(  t_bound_box(xMinCalc, yMinCalc, xMaxCalc, yMaxCalc));
               
        }
            
        
        
    // Sets highways points to be drawn on the minimap
    for (unsigned i = 0; i < allLargeStreetSegs.size(); i++)
    {
        allLargeStreetSegs[i].setBendPoints(tempSegs[allLargeStreetSegs[i].getID()]);
        allLargeStreetSegs[i].setEndsCord(tempInter[allStreetSegs[allLargeStreetSegs[i].getID()].getEnds().first],
                tempInter[allStreetSegs[allLargeStreetSegs[i].getID()].getEnds().second]);
    }
        
        cout << "Prepare for a long wait! Preloading N^2 data." <<  endl;
    //finds and sets closest intersections for each POI

        double minDist;
        double tempDist;
        int64_t tempIntersectID;
        for(unsigned i = 0; i < allPoints.size(); i++)
        {
            minDist = 200000000000000;
            Olat2 = allPoints[i].getLocation().getlat();
            Olon2 = allPoints[i].getLocation().getlon();
            for(unsigned j = 0; j < allIntersects.size(); j++)
            {
                tempDist = latLonOptimize(allIntersects[j].getLocation().getlat(),allIntersects[j].getLocation().getlon());
                if(tempDist < minDist)
                {
                    minDist = tempDist;
                    tempIntersectID = j;
                }
            }
            allPoints[i].setClosestIntersect(tempIntersectID);
            
        }
   
    
    
    cout << "INFO: Loaded " << allPoints.size() << " points of interest" << endl;
       
        
    Cord::basemaxx = Cord::width;
    Cord::basemaxy = Cord::height;
        
    
    //removing extra unnecessary memory that is reserved
    //test for error in initialization due to corrupt bin data
    allStreetSegs.shrink_to_fit();
    assert(allStreetSegs.size());
    allIntersects.shrink_to_fit();
    assert(allIntersects.size());
    allPoints.shrink_to_fit();
    assert(allPoints.size());
    allStreets.shrink_to_fit();
    assert(allStreets.size());
    allFeatures.shrink_to_fit();
    assert(allFeatures.size());
    
    //for future milestones
    //space saved for raw space module used for milestone 3/4 etc
}
