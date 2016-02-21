/* 
 * File:   Astar.h
 * Author: chenyu69
 *
 * Created on March 16, 2015, 2:48 AM
 */

#ifndef ASTAR_H
#define	ASTAR_H
#include "AlgorithmHelper.h"
#define MAXNODES 300000


//highly optimized a star heuristic
// it is pretty basic standard consistent one
double AstarHeuristic1(pair<double,double> latlon1); 

typedef struct 
{
  
        bool operator() (const pair<int,double >& lhs, const pair<int,double >& rhs)
        {
            return  lhs.second > rhs.second;
        }
        
} AstarNodeComparison;
  
typedef struct
{
    double minDist;
    vector<int> ANodePathID;
}AstarReturn;
AstarReturn baseAstar2(int fromNode,int toNode);

#endif	/* ASTAR_H */

