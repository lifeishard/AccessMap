/* 
 * File:   AlgorithmHelper.h
 * Author: chenyu69
 *
 * Created on March 12, 2015, 4:22 PM
 */

#ifndef ALGORITHMHELPER_H
#define	ALGORITHMHELPER_H

#include "Custapi.h"
#include <queue>


//used to help setup data structures for Astar
void AstarSetup();




//structs for fast access
//instead of classes
typedef struct
{
  int64_t StreetSegID;
  int64_t Node;
  double travelTime;
}APath;
//first node is the beginning node
//last node is the end node
//first road connect first node to second node;
typedef struct
{
    
  vector<vector<APath> >outANode;// which ANode can this reach and the path Nodes in between


}ANode;

//A node is same as prime node
// big intersections with more than 1 street connected


typedef struct
{
  vector<vector<APath> >ANodesIn;
  vector<vector<APath> >ANodesOut;    
}BNode;

//A node is same as sub node
//small intersection connected to 1 street only
//this separation is used to reduce graph nodes
//to speed up the search
typedef struct{
    int64_t prevStreetSeg;
    double timeToInit;
    int64_t curNode;
    vector<APath> howToGetHere;
} DPath;
//comparison function for priority queue
typedef struct 
{
  
        bool operator() (const DPath& lhs, const DPath& rhs)
        {
            return  lhs.timeToInit > rhs.timeToInit;
        }
        
} DPathComparison;

//actual datastructures used in pathfinding algorithms
extern const int64_t endOfPath;
extern const int64_t turnPath;
extern const double turnTime;
extern const double noPathDist;
extern int64_t newIntCounter;

extern vector<vector<pair<int64_t,int64_t> > > oldIntToNewInt;
extern vector<int64_t> useableStreetSegs; //1 is true
extern vector<int64_t> primeNodes; //1 is true
extern vector<int64_t> allPrimeNodes;
extern vector<int64_t> newIntToOldInt;
extern vector<int64_t> newIntToOldStreet;
extern vector<ANode> allANode;
extern vector<BNode> allBNode;
extern vector<vector<pair<int,double> > >AstarNeighbour;
extern vector<pair<double,double> >AstarCord;



#endif	/* ALGORITHMHELPER_H */

