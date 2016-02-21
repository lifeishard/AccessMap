#include "Astar.h"

    char visited[MAXNODES];
    int cameFrom[MAXNODES];
    double costSoFar[MAXNODES];


double lat2;
double lon2;//in m per sec
//constants used to speed up calculation
double halfDEGTORAD = DEG_TO_RAD/2.0;
double DEGproductEARTHRADIUS = DEG_TO_RAD*EARTH_RADIUS_IN_METERS;

double AstarHeuristic1(pair<double,double> latlon1) 
{
    double lat1 = latlon1.first;
    double lon1 = latlon1.second;
    double xConversion = cos(halfDEGTORAD*(lat1+lat2));
    double xDiff = xConversion * (lon1-lon2);
    double yDiff = lat1-lat2;
    return sqrt(xDiff*xDiff + yDiff*yDiff)/distSpeedVar;

}

AstarReturn baseAstar2(int fromNode,int toNode)
{
    lat2 = AstarCord[toNode].first;
    lon2 = AstarCord[toNode].second;

    priority_queue <pair<int,double> ,vector<pair<int,double> >,AstarNodeComparison > openQueue;
    openQueue.push(make_pair(fromNode,0));
    double priority;
    int currentNode;
    int prevNode;
    int neighbourNode;
    int reachFlag = 0;
    double newCost;
    memset(visited,0,MAXNODES);
    visited[fromNode] = 1;
    cameFrom[fromNode] = 8008135;
    costSoFar[fromNode] = 0;
    
    
     while(!openQueue.empty())
     {
         currentNode = openQueue.top().first;
        openQueue.pop();
        if(currentNode == toNode)
        {
            reachFlag = 1;
            break;
        }
        for(unsigned j = 0; j < AstarNeighbour[currentNode].size(); j++)
        {
            neighbourNode = AstarNeighbour[currentNode][j].first;

                newCost = costSoFar[currentNode] + AstarNeighbour[currentNode][j].second;
                if(!visited[neighbourNode] || newCost < costSoFar[neighbourNode])
                {  
                    costSoFar[neighbourNode] = newCost;
                    visited[neighbourNode] = 1;
                    
                   
                    priority = newCost + AstarHeuristic1(AstarCord[neighbourNode]);
                    openQueue.push(make_pair(neighbourNode, priority));
                    cameFrom[neighbourNode] = currentNode;
                
            }
        }
        
        
     }
        AstarReturn tr;
    
    if(reachFlag)
    {  
        tr.minDist = costSoFar[toNode];
        prevNode = toNode;
        tr.ANodePathID.push_back(toNode);
        
        while(prevNode != fromNode)
        {
            prevNode = cameFrom[prevNode];
            tr.ANodePathID.push_back(prevNode);
        }
    }
    else
    {
        tr.minDist = noPathDist;
        tr.ANodePathID = vector<int>();
    }
    return tr;
    
}

