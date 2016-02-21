#include <set>

#include "m3.h"


using namespace std;
    
    int DcameFrom[MAXNODES];
    double DcostSoFar[MAXNODES];
char nodesVisited [MAXNODES];
char potentialPOI [MAXNODES];
vector<unsigned> find_path_between_intersections(unsigned 
                   intersect_id_start, unsigned intersect_id_end)
{
    if(intersect_id_start == intersect_id_end)return vector<unsigned>();

    vector<int> APathEndAdded;

    
    AstarNeighbour.push_back(vector<pair<int,double> > ());
    AstarNeighbour.push_back(vector<pair<int,double> > ());
    AstarCord.push_back(make_pair(allIntersects[intersect_id_start].getLocation().getlat(),allIntersects[intersect_id_start].getLocation().getlon()));
    AstarCord.push_back(make_pair(allIntersects[intersect_id_end].getLocation().getlat(),allIntersects[intersect_id_end].getLocation().getlon()));
    int startNewANode = newIntCounter;
    int endNewANode = newIntCounter + 1;
    int shareNeighbour = 0;
    
    
    //setting up initial conditions possible
    
    if(primeNodes[intersect_id_start])
    {
        
        for(unsigned j = 0; j < oldIntToNewInt[intersect_id_start].size(); j++)
        { 
            AstarNeighbour[startNewANode].push_back(make_pair(oldIntToNewInt[intersect_id_start][j].second,0));

        }
        
    }
    else
    {
        for(unsigned j = 0; j < allBNode[intersect_id_start].ANodesOut.size(); j++)
        { 
            AstarNeighbour[startNewANode].push_back(make_pair(allBNode[intersect_id_start].ANodesOut[j][0].Node,allBNode[intersect_id_start].ANodesOut[j][allBNode[intersect_id_start].ANodesOut[j].size()-2].travelTime));
        } 
    }
    
        if(primeNodes[intersect_id_end])
    {
        for(unsigned j = 0; j < oldIntToNewInt[intersect_id_end].size(); j++)
        { 
            AstarNeighbour[oldIntToNewInt[intersect_id_end][j].second].push_back(make_pair(endNewANode,0));
            APathEndAdded.push_back(oldIntToNewInt[intersect_id_end][j].second);

        }
    }
    else
    {
        for(unsigned j = 0; j < allBNode[intersect_id_end].ANodesIn.size(); j++)
        {
             AstarNeighbour[allBNode[intersect_id_end].ANodesIn[j][0].Node].push_back(make_pair(endNewANode,allBNode[intersect_id_end].ANodesIn[j][allBNode[intersect_id_end].ANodesIn[j].size()-2].travelTime));
             APathEndAdded.push_back(allBNode[intersect_id_end].ANodesIn[j][0].Node);
        } 
    }
        
            //run actual astar
    AstarReturn baseAstarResult = baseAstar2(startNewANode,endNewANode);

    AstarNeighbour.pop_back();
    AstarNeighbour.pop_back();
    AstarCord.pop_back();
    AstarCord.pop_back();
    for(unsigned i = 0; i < APathEndAdded.size() ; i++)
    {
        AstarNeighbour[APathEndAdded[i]].pop_back();
    }
    vector<APath> tempAPath;
    int64_t foundPath = 0;

    
    //edge case if two nodes are too close to each other
    //due to graph reduction
    
    
    if(!primeNodes[intersect_id_start]&&!primeNodes[intersect_id_end])
    {
 
        set<int> ANode1NeighbourSet;

        
        for(unsigned j = 0; j < allBNode[intersect_id_start].ANodesOut.size(); j++)
        {
            ANode1NeighbourSet.insert(allBNode[intersect_id_start].ANodesOut[j][0].Node);
    
        }
        for(unsigned j = 0; j < allBNode[intersect_id_start].ANodesIn.size(); j++)
        {
            ANode1NeighbourSet.insert(allBNode[intersect_id_start].ANodesIn[j][0].Node);
          
        }
  
        
        
        for(unsigned j = 0; j < allBNode[intersect_id_end].ANodesOut.size(); j++)
        {
          
            if(ANode1NeighbourSet.find(allBNode[intersect_id_end].ANodesOut[j][0].Node)!= ANode1NeighbourSet.end())
            {
                shareNeighbour = 1;
                break;
            }
            
        }
        if(!shareNeighbour)
        {
            for(unsigned j = 0; j < allBNode[intersect_id_end].ANodesIn.size(); j++)
            {
               
                if(ANode1NeighbourSet.find(allBNode[intersect_id_end].ANodesIn[j][0].Node)!= ANode1NeighbourSet.end())
                {
                    shareNeighbour = 1;
                    break;
                }
            }
        }
        
        
        
        if(shareNeighbour)
        {
           

            int64_t otherEnd;
          
            vector <int64_t> tempStreets;
            vector <int64_t> tempStreetSegs;
          
            ANode blankANode;
            APath blankAPath;
            BNode blankBNode;

            DPath blankDPath;
            DPath tempDPath;
            
            priority_queue <DPath,vector<DPath>,DPathComparison> DPathQueue;   
            
            tempStreetSegs = allIntersects[intersect_id_start].getStreetSegs();
            blankAPath.Node = intersect_id_start;
                
        for(unsigned j = 0; j < tempStreetSegs.size(); j++)
        {

                if(allStreetSegs[tempStreetSegs[j]].gettype()||allStreetSegs[tempStreetSegs[j]].getEnds().first == intersect_id_start)
                {
                   
                    if(allStreetSegs[tempStreetSegs[j]].getEnds().first == intersect_id_start)otherEnd = allStreetSegs[tempStreetSegs[j]].getEnds().second;
                    else otherEnd = allStreetSegs[tempStreetSegs[j]].getEnds().first;
                    
                    if(!primeNodes[otherEnd]){
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
        
        
            memset(nodesVisited,0,MAXNODES);
        nodesVisited[intersect_id_start] = 1;
       
         
        while(!DPathQueue.empty())
        {
            
            blankDPath = DPathQueue.top();
            
            DPathQueue.pop();



            if(!nodesVisited[blankDPath.curNode])
            {

                nodesVisited[blankDPath.curNode] = 1;

                if(!primeNodes[blankDPath.curNode])
                {
                    if(blankDPath.curNode == intersect_id_end)
                    {
                        tempAPath = blankDPath.howToGetHere;
                        tempAPath.push_back(blankAPath);
                        foundPath = 1;
                        break;
                    }
                    
                    
                    blankAPath.Node = blankDPath.curNode;
                    blankAPath.travelTime = endOfPath;
                    blankAPath.StreetSegID = endOfPath;
                   
                    
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

        }

    }

    vector<unsigned> resultPathVector;
    int64_t backTrackStartANode;
    int64_t toNode;
    int64_t fromNode;

    //generate valid path
    if(foundPath && tempAPath[tempAPath.size()-2].travelTime < baseAstarResult.minDist)
    {
  
        for(unsigned i = 0 ; i < tempAPath.size()-1 ;i++)
        {
            resultPathVector.push_back(unsigned(tempAPath[i].StreetSegID));
        }
        
    }
    
    else if(baseAstarResult.minDist != noPathDist)
    {   
        
       if(!primeNodes[intersect_id_start])
       {
            backTrackStartANode = baseAstarResult.ANodePathID[baseAstarResult.ANodePathID.size()-2];
         
           for(unsigned i = 0; i < allBNode[intersect_id_start].ANodesOut.size(); i++)
           {
               if(allBNode[intersect_id_start].ANodesOut[i][0].Node == backTrackStartANode)
               {
               
                           
                   for(int j = allBNode[intersect_id_start].ANodesOut[i].size()-2; j >-1; j--)
                   {
                       resultPathVector.push_back(unsigned(allBNode[intersect_id_start].ANodesOut[i][j].StreetSegID));
                   }
                   break;
               }
           }
       }
    
       for(int i = baseAstarResult.ANodePathID.size()-2; i > 1; i-- )
       {
           fromNode = baseAstarResult.ANodePathID[i];
           toNode = baseAstarResult.ANodePathID[i-1];
           for(unsigned j = 0; j < allANode[fromNode].outANode.size(); j++)
           {
               if(allANode[fromNode].outANode[j][allANode[fromNode].outANode[j].size()-1].Node == toNode)
               {
                   for(unsigned k = 0; k < allANode[fromNode].outANode[j].size()-1; k++)
                   {
                       if(allANode[fromNode].outANode[j][k].StreetSegID != turnPath)resultPathVector.push_back(unsigned(allANode[fromNode].outANode[j][k].StreetSegID));
                   }
               }
           }
       }
              if(!primeNodes[intersect_id_end])
       {
                 
            backTrackStartANode = baseAstarResult.ANodePathID[1];
           for(unsigned i = 0; i < allBNode[intersect_id_end].ANodesIn.size(); i++)
           {
               if(allBNode[intersect_id_end].ANodesIn[i][0].Node == backTrackStartANode)
               {
                   for(unsigned j = 0; j< allBNode[intersect_id_end].ANodesIn[i].size()-1; j++)
                   {
                   
                       resultPathVector.push_back(unsigned(allBNode[intersect_id_end].ANodesIn[i][j].StreetSegID));
                   }
                   break;
               }
           }
            
       }
           
    }
 
    
    
    
 
    
 
    return resultPathVector;
  
}




double compute_path_travel_time(const std::vector<unsigned>& path)
{
   
    return getPathTime(path)/60.0;
}


int trc = 0;
// Returns the shortest travel time path (vector of street segments) from 
// the start intersection to a point of interest with the specified name.
// If no such path exists, returns an empty (size == 0) vector.
vector<unsigned> find_path_to_point_of_interest (unsigned 
                   intersect_id_start, std::string point_of_interest_name)
{
memset(potentialPOI,0,MAXNODES);
    vector<int64_t> samePOIIntersects;
    for(unsigned i = 0; i < allPoints.size(); i++)
    {
        if(allPoints[i].getName() == point_of_interest_name)
        {
            if(allPoints[i].getClosestIntersect() == notInitialized)
            {
                findClosestPOIIntersection(i);

            }
                samePOIIntersects.push_back(allPoints[i].getClosestIntersect());
                potentialPOI[allPoints[i].getClosestIntersect()] = 1;
               
        }
    }

    //setup initial condition
    if(samePOIIntersects.empty())return vector<unsigned>();
    if(potentialPOI[intersect_id_start])return vector<unsigned>();
    vector<int> APathEndAdded;
    int intersect_id_end; 
    int endNewANode; 
        int startNewANode = newIntCounter;
         AstarNeighbour.push_back(vector<pair<int,double> > ());
        
    if(primeNodes[intersect_id_start])
    {
        
        for(unsigned j = 0; j < oldIntToNewInt[intersect_id_start].size(); j++)
        { 
            AstarNeighbour[startNewANode].push_back(make_pair(oldIntToNewInt[intersect_id_start][j].second,0));
          
        }
        
    }
    else
    {
        for(unsigned j = 0; j < allBNode[intersect_id_start].ANodesOut.size(); j++)
        { 
            AstarNeighbour[startNewANode].push_back(make_pair(allBNode[intersect_id_start].ANodesOut[j][0].Node,allBNode[intersect_id_start].ANodesOut[j][allBNode[intersect_id_start].ANodesOut[j].size()-2].travelTime));
        } 
    }
    
    for(unsigned i = 0; i < samePOIIntersects.size(); i++){
        endNewANode = newIntCounter + i +1;
        intersect_id_end = samePOIIntersects[i];
        AstarNeighbour.push_back(vector<pair<int,double> > ());
       
       
        
        
        
        
               if(primeNodes[intersect_id_end])
    {
        for(unsigned j = 0; j < oldIntToNewInt[intersect_id_end].size(); j++)
        { 
            AstarNeighbour[oldIntToNewInt[intersect_id_end][j].second].push_back(make_pair(endNewANode,0));
            APathEndAdded.push_back(oldIntToNewInt[intersect_id_end][j].second);
         
        }
    }
    else
    {
        for(unsigned j = 0; j < allBNode[intersect_id_end].ANodesIn.size(); j++)
        {
             AstarNeighbour[allBNode[intersect_id_end].ANodesIn[j][0].Node].push_back(make_pair(endNewANode,allBNode[intersect_id_end].ANodesIn[j][allBNode[intersect_id_end].ANodesIn[j].size()-2].travelTime));
             APathEndAdded.push_back(allBNode[intersect_id_end].ANodesIn[j][0].Node);
        } 
    }
    }
    
    

 //djstras search for first POI intersection
       
    priority_queue <pair<int,double> ,vector<pair<int,double> >,AstarNodeComparison > openQueue;
    openQueue.push(make_pair(startNewANode,0));
 
    int currentNode;
    int prevNode;
    int neighbourNode;
    int reachFlag = 0;
    double newCost;
    memset(nodesVisited,0,MAXNODES);
    nodesVisited[startNewANode] = 1;
    DcameFrom[startNewANode] = 8008135;
    DcostSoFar[startNewANode] = 0;
    
    AstarReturn tr;
     while(!openQueue.empty())
     {
         currentNode = openQueue.top().first;
        openQueue.pop();
        if(currentNode > newIntCounter)
        {
            reachFlag = 1;
            tr.minDist = DcostSoFar[currentNode];
            prevNode = currentNode;
            tr.ANodePathID.push_back(currentNode);
            break;
        }
        for(unsigned j = 0; j < AstarNeighbour[currentNode].size(); j++)
        {
            neighbourNode = AstarNeighbour[currentNode][j].first;

                newCost = DcostSoFar[currentNode] + AstarNeighbour[currentNode][j].second;
                if(!nodesVisited[neighbourNode] || newCost < DcostSoFar[neighbourNode])
                {  
                    DcostSoFar[neighbourNode] = newCost;
                    nodesVisited[neighbourNode] = 1;
                    
                   
                   
                    openQueue.push(make_pair(neighbourNode, newCost));
                    DcameFrom[neighbourNode] = currentNode;
                
            }
        }
        
        
     }
        
    
    if(reachFlag)
    {  
        
        while(prevNode != startNewANode)
        {
            prevNode = DcameFrom[prevNode];
            tr.ANodePathID.push_back(prevNode);
        }
    }
    else
    {
        tr.minDist = noPathDist;
        tr.ANodePathID = vector<int>();
    }
AstarReturn baseAstarResult = tr;
    
        
        
       
        
        
        AstarNeighbour.pop_back();
           for(unsigned i = 0; i < samePOIIntersects.size(); i++){
       AstarNeighbour.pop_back();

    }

    for(unsigned i = 0; i < APathEndAdded.size() ; i++)
    {

        AstarNeighbour[APathEndAdded[i]].pop_back();
    }
        
        
        
        
        
        
        
        
        
        
        
        
        //small search edge case for nearby subnodes POI intersection
       
    vector<APath> tempAPath;
    int64_t foundPath = 0;
            int64_t otherEnd;
          
            vector <int64_t> tempStreets;
            vector <int64_t> tempStreetSegs;
           
            ANode blankANode;
            APath blankAPath;
            BNode blankBNode;

            DPath blankDPath;
            DPath tempDPath;
            
            priority_queue <DPath,vector<DPath>,DPathComparison> DPathQueue;   
            
            tempStreetSegs = allIntersects[intersect_id_start].getStreetSegs();
            blankAPath.Node = intersect_id_start;
                
        for(unsigned j = 0; j < tempStreetSegs.size(); j++)
        {

                if(allStreetSegs[tempStreetSegs[j]].gettype()||allStreetSegs[tempStreetSegs[j]].getEnds().first == intersect_id_start)
                {
                   
                    if(allStreetSegs[tempStreetSegs[j]].getEnds().first == intersect_id_start)otherEnd = allStreetSegs[tempStreetSegs[j]].getEnds().second;
                    else otherEnd = allStreetSegs[tempStreetSegs[j]].getEnds().first;
                    
                    if(!primeNodes[otherEnd]){
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
        
        
            memset(nodesVisited,0,MAXNODES);
        nodesVisited[intersect_id_start] = 1;
     
         
        while(!DPathQueue.empty())
        {
            
            blankDPath = DPathQueue.top();
            
            DPathQueue.pop();



            if(!nodesVisited[blankDPath.curNode])
            {

                nodesVisited[blankDPath.curNode] = 1;

                if(!primeNodes[blankDPath.curNode])
                {
                    
                    if(potentialPOI[blankDPath.curNode])
                    {
                        tempAPath = blankDPath.howToGetHere;
                        tempAPath.push_back(blankAPath);
                        foundPath = 1;
                        break;
                    }
                    
                    
                    blankAPath.Node = blankDPath.curNode;
                    blankAPath.travelTime = endOfPath;
                    blankAPath.StreetSegID = endOfPath;
           
                    
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

        
    
    
    
    
    
    
    
    //generate actual path
     vector<unsigned> resultPathVector;
    int64_t backTrackStartANode;
    int64_t toNode;
    int64_t fromNode;
   
    if(baseAstarResult.minDist != noPathDist)intersect_id_end = samePOIIntersects[baseAstarResult.ANodePathID[0]-newIntCounter -1];
    if(foundPath && tempAPath[tempAPath.size()-2].travelTime < baseAstarResult.minDist)
    {
        for(unsigned i = 0 ; i < tempAPath.size()-1 ;i++)
        {
            resultPathVector.push_back(unsigned(tempAPath[i].StreetSegID));
        }
        
    }
    
    else if(baseAstarResult.minDist != noPathDist)
    {   
        
       if(!primeNodes[intersect_id_start])
       {
            backTrackStartANode = baseAstarResult.ANodePathID[baseAstarResult.ANodePathID.size()-2];
        
           for(unsigned i = 0; i < allBNode[intersect_id_start].ANodesOut.size(); i++)
           {
               if(allBNode[intersect_id_start].ANodesOut[i][0].Node == backTrackStartANode)
               {
             
                           
                   for(int j = allBNode[intersect_id_start].ANodesOut[i].size()-2; j >-1; j--)
                   {
                       resultPathVector.push_back(unsigned(allBNode[intersect_id_start].ANodesOut[i][j].StreetSegID));
                   }
                   break;
               }
           }
       }
   
       for(int i = baseAstarResult.ANodePathID.size()-2; i > 1; i-- )
       {
           fromNode = baseAstarResult.ANodePathID[i];
           toNode = baseAstarResult.ANodePathID[i-1];
           for(unsigned j = 0; j < allANode[fromNode].outANode.size(); j++)
           {
               if(allANode[fromNode].outANode[j][allANode[fromNode].outANode[j].size()-1].Node == toNode)
               {
                   for(unsigned k = 0; k < allANode[fromNode].outANode[j].size()-1; k++)
                   {
                       if(allANode[fromNode].outANode[j][k].StreetSegID != turnPath)resultPathVector.push_back(unsigned(allANode[fromNode].outANode[j][k].StreetSegID));
                   }
               }
           }
       }
              if(!primeNodes[intersect_id_end])
       {
                  
            backTrackStartANode = baseAstarResult.ANodePathID[1];
           for(unsigned i = 0; i < allBNode[intersect_id_end].ANodesIn.size(); i++)
           {
               if(allBNode[intersect_id_end].ANodesIn[i][0].Node == backTrackStartANode)
               {
                   for(unsigned j = 0; j< allBNode[intersect_id_end].ANodesIn[i].size()-1; j++)
                   {
                      
                       resultPathVector.push_back(unsigned(allBNode[intersect_id_end].ANodesIn[i][j].StreetSegID));
                   }
                   break;
               }
           }
            
       }
           
    }
    
    
    
    
    

    return resultPathVector;
}

vector<string> printDirections(vector<unsigned> path, int64_t startID, int64_t endID, char arePOI, char same)
{
    vector<string> directionsList;
    directionsList.push_back("");
    directionsList.push_back("From:");
    if(arePOI == 1 || arePOI == 3)
    {
        for(unsigned i = 0; i < allPoints[startID].getdisplayString().size(); i++)
        {
            directionsList.push_back(allPoints[startID].getdisplayString()[i]);
        }
    } else {
        for(unsigned i = 0; i < allIntersects[startID].getdisplayString().size(); i++)
        {
            directionsList.push_back(allIntersects[startID].getdisplayString()[i]);
        }
    }
    directionsList.push_back("to:");
    if(arePOI == 2 || arePOI == 3)
    {
        for(unsigned i = 0; i < allPoints[endID].getdisplayString().size()-1; i++)
        {
            directionsList.push_back(allPoints[endID].getdisplayString()[i]);
        }
        directionsList.push_back(allPoints[endID].getdisplayString()[allPoints[endID].getdisplayString().size()-1] + ".");
    } else {
        for(unsigned i = 0; i < allIntersects[endID].getdisplayString().size()-1; i++)
        {
            directionsList.push_back(allIntersects[endID].getdisplayString()[i]);
        }
        directionsList.push_back(allIntersects[endID].getdisplayString()[allIntersects[endID].getdisplayString().size()-1] + ".");
    }
    directionsList.push_back("");
    
    // Same intersection
    if (path.size() == 0 && same == 1){
        directionsList.push_back("[HE] You are already");
        directionsList.push_back("at your destination.");
        return directionsList;
    }
    
    // No path
    if (path.size() == 0 && same == 0){
        directionsList.push_back("[HE] Unfortunately there are");
        directionsList.push_back("no valid paths between your");
        directionsList.push_back("selected intersections.");
        directionsList.push_back("");
        directionsList.push_back("Please try again with different inputs.");
        return directionsList;
    }
    
    StreetSeg startSeg = allStreetSegs[path[0]];
    StreetSeg endSeg = allStreetSegs[path[path.size() - 1]];
    
    //special case for a 1 segment path because direction is unknown
    if (path.size() == 1){
        directionsList.push_back("[ST] 1. Drive for " + convertLength(startSeg.getLength()) + " on");
        directionsList.push_back(allStreets[endSeg.getStreet()].getName()+".");
        directionsList.push_back("");
        directionsList.push_back("Estimated travel time:");
        directionsList.push_back(convertTime(getPathTime(path)));
        return directionsList;
    }
    
    //do a little bit of math to figure out the initial heading
    double initialAngle;
    Cord start, end;
    StreetSeg secondSeg = allStreetSegs[path[1]];
    
    if (startSeg.getEnds().second == secondSeg.getEnds().first || startSeg.getEnds().second == secondSeg.getEnds().second){
        start = startSeg.getEndsCord().first;
        end = (startSeg.getBendPoints().size() > 0) ? (startSeg.getBendPoints()[0]) : (startSeg.getEndsCord().second);
    } else { //since we already checked for a valid path, we can assume they are connected
        start = startSeg.getEndsCord().second;
        end = (startSeg.getBendPoints().size() > 0) ? (startSeg.getBendPoints()[startSeg.getBendPoints().size() - 1]) : 
                                             (startSeg.getEndsCord().first);
    }
    
    initialAngle = atan2(end.gety() - start.gety(), end.getx() - start.getx());
    
    directionsList.push_back("[ST] 1. Head " + convertTurn(initialAngle, CARDINAL) + " on");
    directionsList.push_back(allStreets[startSeg.getStreet()].getName()+".");
    directionsList.push_back("");
    
    double streetLength = 0;
    int stepNum = 2; //because we already took care of step 1
    
    for (unsigned i = 0; i < path.size() - 1; i ++){
        StreetSeg thisSeg = allStreetSegs[path[i]];
        StreetSeg nextSeg = allStreetSegs[path[i + 1]];
        if(allStreets[thisSeg.getStreet()].getName() == allStreets[nextSeg.getStreet()].getName()){ //same street, no turn
            streetLength += thisSeg.getLength();
        } else {
            streetLength += thisSeg.getLength();
            if(allStreets[nextSeg.getStreet()].getName().find("Ramp ") == string::npos &&
               allStreets[thisSeg.getStreet()].getName().find("Ramp ") == string::npos)
            { // No ramps
                directionsList.push_back(convertTurnSymbol(findTurnAngle(thisSeg, nextSeg)) + to_string(stepNum) + ". In " + convertLength(streetLength) + ",");
                directionsList.push_back(convertTurn(findTurnAngle(thisSeg, nextSeg), NORAMP));
                directionsList.push_back(allStreets[nextSeg.getStreet()].getName() + ".");
                directionsList.push_back("");
                stepNum ++;
                streetLength = 0;
            } else if(allStreets[thisSeg.getStreet()].getName().find("Ramp ") == string::npos)
            { // Next seg is a ramp
                directionsList.push_back("[RM] " + to_string(stepNum) + ". In " + convertLength(streetLength) + ",");
                directionsList.push_back("Take ramp " + convertTurn(findTurnAngle(thisSeg, nextSeg), RAMP)); 
                directionsList.push_back(allStreets[nextSeg.getStreet()].getName() + ".");
                directionsList.push_back("");
                stepNum ++;
                streetLength = 0;
            } else if(allStreets[nextSeg.getStreet()].getName().find("Ramp ") == string::npos &&
                      findTurnAngle(thisSeg, nextSeg) > 13*PI/16 && findTurnAngle(thisSeg, nextSeg) < 19*PI/16)
            { // angle is not steep enough for a turn, must be a merge
                directionsList.push_back("[MG] " + to_string(stepNum) + ". In " + convertLength(streetLength) + ",");
                directionsList.push_back("Merge " + convertTurn(findTurnAngle(thisSeg, nextSeg), RAMP)); 
                directionsList.push_back(allStreets[nextSeg.getStreet()].getName() + ".");
                directionsList.push_back("");
                stepNum ++;
                streetLength = 0;
            } else { // is a turning ramp, do normal turning
                directionsList.push_back(convertTurnSymbol(findTurnAngle(thisSeg, nextSeg)) + to_string(stepNum) + ". In " + convertLength(streetLength) + ",");
                directionsList.push_back(convertTurn(findTurnAngle(thisSeg, nextSeg), NORAMP));
                directionsList.push_back(allStreets[nextSeg.getStreet()].getName() + ".");
                directionsList.push_back("");
                stepNum ++;
                streetLength = 0;
            }
        }
    }
    
    streetLength += endSeg.getLength();
    directionsList.push_back("[ST] " + to_string(stepNum) + ". Arrive at destination in ");
    directionsList.push_back(convertLength(streetLength) + " on");
    directionsList.push_back(allStreets[endSeg.getStreet()].getName());
    directionsList.push_back("");
    directionsList.push_back("Estimated travel time:");
    directionsList.push_back(convertTime(getPathTime(path))+".");
    
    return directionsList;
}

double getPathTime(vector<unsigned> outputSeg)
{
    double totalTime = 0;
    if(outputSeg.size() == 0) return totalTime;
    for(unsigned i = 0; i < outputSeg.size()-1; i++)
    {
        totalTime += allStreetSegs[outputSeg[i]].getTime();
        if(allStreetSegs[outputSeg[i]].getStreet() != allStreetSegs[outputSeg[i+1]].getStreet())
        {
            totalTime += 15;
        }
    }
    totalTime += allStreetSegs[outputSeg[outputSeg.size()-1]].getTime();
    return totalTime;
}

double findTurnAngle(StreetSeg seg1, StreetSeg seg2)
{
    Cord head, turn, tail;
    double angle;
    
    if(seg1.getEnds().first == seg2.getEnds().first){
        turn = seg1.getEndsCord().first;
        head = (seg1.getBendPoints().size() > 0) ? seg1.getBendPoints()[0] : seg1.getEndsCord().second;
        tail = (seg2.getBendPoints().size() > 0) ? seg2.getBendPoints()[0] : seg2.getEndsCord().second;
    } else if(seg1.getEnds().first == seg2.getEnds().second){
        turn = seg1.getEndsCord().first;
        head = (seg1.getBendPoints().size() > 0) ? seg1.getBendPoints()[0] : seg1.getEndsCord().second;
        tail = (seg2.getBendPoints().size() > 0) ? seg2.getBendPoints()[seg2.getBendPoints().size() - 1] :
                                                   seg2.getEndsCord().first;
    } else if(seg1.getEnds().second == seg2.getEnds().first){
        turn = seg1.getEndsCord().second;
        head = (seg1.getBendPoints().size() > 0) ? seg1.getBendPoints()[seg1.getBendPoints().size() - 1] :
                                                   seg1.getEndsCord().first;
        tail = (seg2.getBendPoints().size() > 0) ? seg2.getBendPoints()[0] : seg2.getEndsCord().second;
    } else { //already checked for valid path
        turn = seg1.getEndsCord().second;
        head = (seg1.getBendPoints().size() > 0) ? seg1.getBendPoints()[seg1.getBendPoints().size() - 1] :
                                                   seg1.getEndsCord().first;
        tail = (seg2.getBendPoints().size() > 0) ? seg2.getBendPoints()[seg2.getBendPoints().size() - 1] :
                                                   seg2.getEndsCord().first;
    }
    
    angle = atan2(tail.gety()-turn.gety(), tail.getx()-turn.getx()) - atan2(head.gety()-turn.gety(), head.getx()-turn.getx());
    return angle;    
}

string convertTurn(double angle, char turnType)
{   
    if (turnType == CARDINAL) {
        while (angle >= 19*PI/8) angle -= 2*PI; //by converting all angle to pi/4 -> 9pi/4 I save an or
        while (angle < 3*PI/8) angle += 2*PI;
        if (angle >= 3*PI/8 && angle < 5*PI/8) return "north";
        if (angle >= 5*PI/8 && angle < 7*PI/8) return "north-west";
        if (angle >= 7*PI/8 && angle < 9*PI/8) return "west";
        if (angle >= 9*PI/8 && angle < 11*PI/8) return "south-west";
        if (angle >= 11*PI/8 && angle < 13*PI/8) return "south";
        if (angle >= 13*PI/8 && angle < 15*PI/8) return "south-east";
        if (angle >= 15*PI/8 && angle < 17*PI/8) return "east";
        if (angle >= 17*PI/8 && angle < 19*PI/8) return "north-east";
    } else if (turnType == RAMP) {
        while (angle >= 2*PI) angle -= 2*PI;
        while (angle < 0) angle += 2*PI;
        if (angle >= 0 && angle < (7*PI)/8) return "on right, onto ";
        if (angle >= (7*PI)/8 && angle < (9*PI)/8) return "straight ahead, onto ";
        if (angle >= (9*PI)/8 && angle < 2*PI) return "on left, onto ";
    } else {
        while (angle >= (17*PI)/8) angle -= 2*PI;
        while (angle < PI/8) angle += 2*PI;
        if (angle >= PI/8 && angle < (7*PI)/8) return "Turn right onto ";
        if (angle >= (7*PI)/8 && angle < (9*PI)/8) return "Continue onto ";
        if (angle >= (9*PI)/8 && angle < (15*PI)/8) return "Turn left onto ";
        if (angle >= (15*PI)/8 && angle < (17*PI)/8) return "Make a U-turn onto ";
    }
    return "";
}

string convertTurnSymbol(double angle)
{   
    while (angle >= (17*PI)/8) angle -= 2*PI;
    while (angle < PI/8) angle += 2*PI;
    if (angle >= PI/8 && angle < (7*PI)/8) return "[RT] ";
    if (angle >= (7*PI)/8 && angle < (9*PI)/8) return "[ST] ";
    if (angle >= (9*PI)/8 && angle < (15*PI)/8) return "[LF] ";
    if (angle >= (15*PI)/8 && angle < (17*PI)/8) return "[UT] ";
    return "END OF FUNCTION";
}

//this assumes length is in metres????
string convertLength(double length)
{
    string len = "";
    string lenfrac = "";
    
    if(length < 1000) {
        len = to_string((int)round(length));
        return len + " m";
    } else {
        len = to_string((int)round(length/1000));
        lenfrac = to_string((int)round(fmod((length*10)/1000,10)));
        return len + "." + lenfrac + " km";
    }
    
    return "END OF FUNCTION";
}
    
string convertTime(double time)
{
    string sec = "";
    string min = "";
    string hour = "";
   
    if(time < 60) {
        sec = to_string((int)round(time));
        return sec + " seconds";
        
    } else if(time < 3600) {
        min = to_string((int)round(time/60));
        return min + " minutes";
    
    } else {
        hour = to_string((int)round(time/3600));
        min = to_string((int)round((fmod(time,3600))/60));
        return hour + " hours, " + min + " minutes";
    }
    
    return "END OF FUNCTION";
}

Intersect findNearestIntersectionFromPOI(Point thisPoint)
{
    Intersect nearestIntersect;
    double minDist = INFINITY;
    
    for (unsigned i = 0; i < allIntersects.size(); i++){
        double tempDist = cordDistance(thisPoint.getLocation(), allIntersects[i].getLocation());
        if (tempDist < minDist){
            minDist = tempDist;
            nearestIntersect = allIntersects[i];
        }
    }
    
    thisPoint.setClosestIntersect(nearestIntersect.getID());
    
    return nearestIntersect;
        
}

char isValidPath(vector<unsigned> outputSeg)
{
    if(outputSeg.size() == 0) return 1;
    for(unsigned i = 0; i < outputSeg.size()-1; i++)
    {
        if(allStreetSegs[outputSeg[i]].getEnds().first == allStreetSegs[outputSeg[i+1]].getEnds().first)
        {
            if(allStreetSegs[outputSeg[i]].getType() == TWOWAY) continue;
        } else if(allStreetSegs[outputSeg[i]].getEnds().first == allStreetSegs[outputSeg[i+1]].getEnds().second)
        {
            if(allStreetSegs[outputSeg[i]].getType() == TWOWAY && allStreetSegs[outputSeg[i+1]].getType() == TWOWAY) continue;
        } else if(allStreetSegs[outputSeg[i]].getEnds().second == allStreetSegs[outputSeg[i+1]].getEnds().first)
        {
            continue;
        } else if(allStreetSegs[outputSeg[i]].getEnds().second == allStreetSegs[outputSeg[i+1]].getEnds().second)
        {
            if(allStreetSegs[outputSeg[i+1]].getType() == TWOWAY) continue;
        }
        return 0;
    }
    return 1;
}
