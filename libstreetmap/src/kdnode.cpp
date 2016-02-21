/* 
 * File:   kdnode.cpp
 * Author: chenyu69
 * 
 * Created on February 4, 2015, 10:08 PM
 */

#include "kdnode.h"

kdnode::kdnode() {
}

kdnode::kdnode(const kdnode& orig) {
}

kdnode::~kdnode() {
}
//commented code used in testing kd purposes, I don't want to branch project
    /*
 cout << setprecision(15);
 tempTime = 0;
    for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++) {
        allPoints.push_back(Point(i, getPointOfInterestName(i), 
                getPointOfInterestPosition(i))); 
       // Stores each point of interest into vector sorted by ID
         tempCord1 = tempCord;
        tempCord = realCord( getPointOfInterestPosition(i));
        //cout  <<tempCord.first << "  "<<tempCord.second<< endl;
       
        //fast hash map for quick access time
        if(i > 0 && i< 6950)
        {
            tempLength = abs(distance(getPointOfInterestPosition(i),getPointOfInterestPosition(i-1))-hypot(tempCord.first-tempCord1.first,tempCord.second-tempCord1.second));
            //cout << tempLength <<endl;
            //tempLength = distance(getPointOfInterestPosition(i),getPointOfInterestPosition(i-1));
            if (tempLength > tempTime)
            {
                tempID = i-1;
                tempTime = tempLength;
            }
            
        }
        PointNametoID[getPointOfInterestName(i)] = i;
    }
 
 //cout << dist
 cout<< tempTime << endl;
 cout<< tempID << endl;

  tempCord = realCord(getPointOfInterestPosition(tempID));
    tempCord1 = realCord(getPointOfInterestPosition(tempID+1));
    cout << tempCord.first<<" "<<tempCord.second<<endl;
        cout << tempCord1.first<<" "<<tempCord1.second<<endl;
        cout <<getPointOfInterestPosition(tempID).lat<<" "<<getPointOfInterestPosition(tempID).lon<<endl;
  cout <<getPointOfInterestPosition(tempID+1).lat<<" "<<getPointOfInterestPosition(tempID+1).lon<<endl;
  cout <<distance (getPointOfInterestPosition(tempID+1),getPointOfInterestPosition(tempID))<< " " << hypot(tempCord.first-tempCord1.first,tempCord.second-tempCord1.second)<<endl;
   
  
  
  LatLon point1,point2;
  point1=getPointOfInterestPosition(tempID);
  point2=getPointOfInterestPosition(tempID+1);
  
      double latavg = (point1.lat + point2.lat) / 2.0;
    double x1 = ( point1.lon * cos(DEG_TO_RAD * latavg));
    double x2 = ( point2.lon * cos(DEG_TO_RAD * latavg));
    double y1 =  point1.lat;
    double y2 =  point2.lat;
    cout<<y2 - y1<<" "<< x2 - x1<<endl;

    cout <<point2.lat-point1.lat<<" "<<(point2.lon * cos(DEG_TO_RAD * point2.lat))-(point1.lon * cos(DEG_TO_RAD * point1.lat))<<endl;
//convert coordinates to x,y system in meters
    point2.lat = point1.lat-0.5;
    point2.lon = point1.lon;
    tempTime = 888888;
    for(int i=0;i<1000;i++)
    {
        point2.lat+=0.0001;
        tempLength = distance(point1,point2);
         if (tempLength < tempTime)
            {
                tempID = i-1;
                tempTime = tempLength;
            }
    }
    cout << tempTime<<" "<<tempID<<endl;
  */