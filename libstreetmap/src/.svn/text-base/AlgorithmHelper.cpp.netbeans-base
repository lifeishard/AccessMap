

#include "AlgorithmHelper.h"



//values used in setting up paths
//which don't correlate to real nodes or segment ID
const int64_t endOfPath = 88888888;
const int64_t turnPath = 7777777;
const double turnTime = 15;
const double noPathDist = 999999999;

vector<vector<pair<int64_t,int64_t> > > oldIntToNewInt;
vector<int64_t> useableStreetSegs; //1 is true
vector<int64_t> primeNodes; //1 is true
vector<int64_t> allPrimeNodes;
vector<int64_t> newIntToOldInt;
vector<int64_t> newIntToOldStreet;
int64_t newIntCounter;
int64_t tempCounter = 0;
vector<ANode> allANode;
vector<BNode> allBNode;
int64_t pathNodeCount;
vector<vector<pair<int,double> > >AstarNeighbour;
vector<pair<double,double> >AstarCord;

void AstarSetup()
{
    newIntCounter = 0;
    int64_t otherEnd;
    
    vector <int64_t> tempStreets;
    vector <int64_t> tempStreetSegs;

    int64_t otherEndANodeID;
    ANode blankANode;
    APath blankAPath;
    BNode blankBNode;
   
    DPath blankDPath;
    DPath tempDPath;
    vector<APath> tempAPath;
    

    
    priority_queue <DPath,vector<DPath>,DPathComparison> DPathQueue;   
    vector<int64_t>nodesVisitedToClear;
    vector<int64_t>nodesVisited;
    int64_t oldNodeID;
    int64_t oldStreetID;
    
    //create primenodes
    
    for(unsigned i = 0; i < allIntersects.size(); i++)
    {
        allBNode.push_back(blankBNode);
        oldIntToNewInt.push_back(vector<pair<int64_t,int64_t> >());
        nodesVisited.push_back(0);
        //check to see if its a primenode
        if(allIntersects[i].getStreets().size() < 2)
        {
            primeNodes.push_back(0);
            continue;
        }
        else
        {
            primeNodes.push_back(1);
            allPrimeNodes.push_back(i);
            tempStreets = allIntersects[i].getStreets();

            for(unsigned j = 0; j < tempStreets.size();j++)
            {
                
                oldIntToNewInt[i].push_back(make_pair(tempStreets[j],newIntCounter));
                newIntToOldInt.push_back(i);
                newIntToOldStreet.push_back(tempStreets[j]);
                allANode.push_back(blankANode);
                newIntCounter++;
            }
        }
    }
    
    
    
    // djstras through primenodes 
    //first time for only outgoing connections
    for(unsigned i = 0; i < newIntCounter; i++)
    {

        oldNodeID = newIntToOldInt[i];
        oldStreetID = newIntToOldStreet[i];
        tempStreetSegs = allIntersects[oldNodeID].getStreetSegs();
        
        
        for(unsigned j = 0; j < tempStreetSegs.size(); j++)
        {

            if(allStreetSegs[tempStreetSegs[j]].getStreet() == oldStreetID)
            {
               
                if(allStreetSegs[tempStreetSegs[j]].gettype()||allStreetSegs[tempStreetSegs[j]].getEnds().first == oldNodeID)
                {
                   
                    if(allStreetSegs[tempStreetSegs[j]].getEnds().first == oldNodeID)otherEnd = allStreetSegs[tempStreetSegs[j]].getEnds().second;
                    else otherEnd = allStreetSegs[tempStreetSegs[j]].getEnds().first;
                    blankAPath.Node = i;
                    blankAPath.StreetSegID = tempStreetSegs[j];
                    blankAPath.travelTime = allStreetSegs[tempStreetSegs[j]].getTime();
                    blankDPath.howToGetHere.clear();
                    blankDPath.howToGetHere.push_back(blankAPath);
                    blankDPath.curNode = otherEnd;

                    blankDPath.timeToInit = allStreetSegs[tempStreetSegs[j]].getTime();
                    blankDPath.prevStreetSeg = tempStreetSegs[j];
                    DPathQueue.push(blankDPath);
                }
                
            }
        }
        
        
        
        nodesVisited[oldNodeID] = 1;
        nodesVisitedToClear.push_back(oldNodeID);
         
        while(!DPathQueue.empty())
        {
            
            blankDPath = DPathQueue.top();
            
            DPathQueue.pop();



            if(!nodesVisited[blankDPath.curNode])
            {

                nodesVisited[blankDPath.curNode] = 1;
                nodesVisitedToClear.push_back(blankDPath.curNode);
                if(primeNodes[blankDPath.curNode])
                {

                    for(unsigned k = 0; k < oldIntToNewInt[blankDPath.curNode].size(); k++)
                    {
                        if(oldIntToNewInt[blankDPath.curNode][k].first == oldStreetID)
                        {
                            otherEndANodeID = oldIntToNewInt[blankDPath.curNode][k].second;
                            break;   
                        }
                 
                    }
                    blankAPath.Node = otherEndANodeID;
                    blankAPath.travelTime = endOfPath;
                    blankAPath.StreetSegID = endOfPath;

                    tempAPath = blankDPath.howToGetHere;
                    tempAPath.push_back(blankAPath);
                    allANode[i].outANode.push_back(tempAPath);
                
                }
                
                else
                {

                    blankAPath.Node = blankDPath.curNode;
                    blankAPath.travelTime = endOfPath;
                    blankAPath.StreetSegID = endOfPath;
                    tempAPath = blankDPath.howToGetHere;
                    tempAPath.push_back(blankAPath);
                    allBNode[blankDPath.curNode].ANodesIn.push_back(tempAPath);
                    
                    tempStreetSegs = allIntersects[blankDPath.curNode].getStreetSegs();


                     for(unsigned j = 0; j < tempStreetSegs.size(); j++)
                     {
                         if(tempStreetSegs[j] != blankDPath.prevStreetSeg)
                         {

                             if(allStreetSegs[tempStreetSegs[j]].gettype()||allStreetSegs[tempStreetSegs[j]].getEnds().first == blankDPath.curNode)
                             {
 
                                 if(allStreetSegs[tempStreetSegs[j]].getEnds().first == blankDPath.curNode)otherEnd = allStreetSegs[tempStreetSegs[j]].getEnds().second;
                                 else otherEnd = allStreetSegs[tempStreetSegs[j]].getEnds().first;
                                 
                                 tempDPath = blankDPath;
                                 
                                 blankAPath.StreetSegID = tempStreetSegs[j];
                                 blankAPath.travelTime = tempDPath.timeToInit+allStreetSegs[tempStreetSegs[j]].getTime();
                                 tempDPath.howToGetHere.push_back(blankAPath);
                                 tempDPath.curNode = otherEnd;

                                 tempDPath.timeToInit = tempDPath.timeToInit+allStreetSegs[tempStreetSegs[j]].getTime();
                                 tempDPath.prevStreetSeg = tempStreetSegs[j];
                                 DPathQueue.push(tempDPath);
                             }
                             
                         }
                     }
                
}
            }

        }
            
        for(unsigned j = 0 ; j < nodesVisitedToClear.size(); j++ )
        {
            nodesVisited[nodesVisitedToClear[j]] = 0;
        }
            nodesVisitedToClear.clear();
            
            
            oldNodeID = newIntToOldInt[i];
        oldStreetID = newIntToOldStreet[i];
        tempStreetSegs = allIntersects[oldNodeID].getStreetSegs();
        
        //djkstras second time for incoming connections
        
        for(unsigned j = 0; j < tempStreetSegs.size(); j++)
        {

            if(allStreetSegs[tempStreetSegs[j]].getStreet() == oldStreetID)
            {
               
                if(allStreetSegs[tempStreetSegs[j]].gettype()||allStreetSegs[tempStreetSegs[j]].getEnds().second == oldNodeID)
                {
                   
                    if(allStreetSegs[tempStreetSegs[j]].getEnds().first == oldNodeID)otherEnd = allStreetSegs[tempStreetSegs[j]].getEnds().second;
                    else otherEnd = allStreetSegs[tempStreetSegs[j]].getEnds().first;
                    blankAPath.Node = i;
                    blankAPath.StreetSegID = tempStreetSegs[j];
                    blankAPath.travelTime = allStreetSegs[tempStreetSegs[j]].getTime();
                    blankDPath.howToGetHere.clear();
                    blankDPath.howToGetHere.push_back(blankAPath);
                    blankDPath.curNode = otherEnd;

                    blankDPath.timeToInit = allStreetSegs[tempStreetSegs[j]].getTime();
                    blankDPath.prevStreetSeg = tempStreetSegs[j];
                    DPathQueue.push(blankDPath);
                }
                
            }
        }
        
        
        
        nodesVisited[oldNodeID] = 1;
        nodesVisitedToClear.push_back(oldNodeID);
         
        while(!DPathQueue.empty())
        {
            
            blankDPath = DPathQueue.top();
            
            DPathQueue.pop();



            if(!nodesVisited[blankDPath.curNode])
            {

                nodesVisited[blankDPath.curNode] = 1;
                nodesVisitedToClear.push_back(blankDPath.curNode);
                if(!primeNodes[blankDPath.curNode])
                {

                    blankAPath.Node = blankDPath.curNode;
                    blankAPath.travelTime = endOfPath;
                    blankAPath.StreetSegID = endOfPath;
                    tempAPath = blankDPath.howToGetHere;
                    tempAPath.push_back(blankAPath);
                    allBNode[blankDPath.curNode].ANodesOut.push_back(tempAPath);
                    
                    tempStreetSegs = allIntersects[blankDPath.curNode].getStreetSegs();


                     for(unsigned j = 0; j < tempStreetSegs.size(); j++)
                     {
                         if(tempStreetSegs[j] != blankDPath.prevStreetSeg)
                         {

                             if(allStreetSegs[tempStreetSegs[j]].gettype()||allStreetSegs[tempStreetSegs[j]].getEnds().second == blankDPath.curNode)
                             {
 
                                 if(allStreetSegs[tempStreetSegs[j]].getEnds().first == blankDPath.curNode)otherEnd = allStreetSegs[tempStreetSegs[j]].getEnds().second;
                                 else otherEnd = allStreetSegs[tempStreetSegs[j]].getEnds().first;
                                 
                                 tempDPath = blankDPath;
                                 
                                 blankAPath.StreetSegID = tempStreetSegs[j];
                                 blankAPath.travelTime = tempDPath.timeToInit+allStreetSegs[tempStreetSegs[j]].getTime();
                                 tempDPath.howToGetHere.push_back(blankAPath);
                                 tempDPath.curNode = otherEnd;

                                 tempDPath.timeToInit = tempDPath.timeToInit+allStreetSegs[tempStreetSegs[j]].getTime();
                                 tempDPath.prevStreetSeg = tempStreetSegs[j];
                                 DPathQueue.push(tempDPath);
                             }
                             
                         }
                     }
                
}
            }

        }
            
        for(unsigned j = 0 ; j < nodesVisitedToClear.size(); j++ )
        {
            nodesVisited[nodesVisitedToClear[j]] = 0;
        }
            nodesVisitedToClear.clear();

    }
    
    //creating the final reduced data structures used in algorithms
    //these ones only contain the ID, travel time and LatLon used for heuristic
    
        for(unsigned i = 0; i < newIntCounter; i++)
    {
        AstarNeighbour.push_back(vector<pair<int,double> >() );
        
        oldNodeID = newIntToOldInt[i];
        AstarCord.push_back(make_pair(allIntersects[oldNodeID].getLocation().getlat(),allIntersects[oldNodeID].getLocation().getlon()));
        for(unsigned j = 0 ; j < oldIntToNewInt[oldNodeID].size(); j++)
        {
            if(oldIntToNewInt[oldNodeID][j].second != i)
            {
                blankAPath.travelTime = turnTime;
                blankAPath.Node = i;
                blankAPath.StreetSegID = turnPath;
               tempAPath.clear();
                tempAPath.push_back(blankAPath);
                blankAPath.travelTime = endOfPath;
                blankAPath.Node = oldIntToNewInt[oldNodeID][j].second;
                blankAPath.StreetSegID = endOfPath;
                tempAPath.push_back(blankAPath);
                allANode[i].outANode.push_back(tempAPath);
            }
        }
        for(unsigned j = 0; j < allANode[i].outANode.size(); j++)
        {
            pathNodeCount = allANode[i].outANode[j].size();
            AstarNeighbour[i].push_back(make_pair(allANode[i].outANode[j][pathNodeCount-1].Node,allANode[i].outANode[j][pathNodeCount-2].travelTime));
        }   
    }
        
  
    cout << "INFO: Loaded Pathfinding Algorithm Data." << endl;
}

