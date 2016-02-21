#include "m1.h"
using namespace std;

//load the map
bool load_map(string map_name) 
{
    bool load_success = loadStreetDatabaseBIN(map_name);
    initializeCust(); //custom data structure creation for added speed
    AstarSetup();

    
    return load_success;
}

//close the map
void close_map() 
{
    closeStreetDatabase();
}

//NOTE: functions returning unsigned will return 4294967295 (max 32bit number)
//if given invalid input

// Case sensitive
unsigned find_intersection_id_from_name(string intersection_name)
{   
    // Returns using the unordered map IntersectNametoID
    return IntersectNametoID[intersection_name];
}

// Case sensitive
unsigned find_street_id_from_name(string street_name)
{
    // Returns street ID from the unordered map for streets
    return StreetNametoID[street_name];
}

// Find a vector containing ID of street segments from ID of intersection
vector<unsigned> find_intersection_street_segments(unsigned intersection_id)
{
    // The vector already exists in the Intersect class but we need to convert
    // to vector<unsigned>
    vector<int64_t> tempVec = allIntersects[intersection_id].getStreetSegs();
    vector<unsigned> outputVec;
    //converts each int64_t from 1st vector to an unsigned in 2nd vector
    for(unsigned i = 0; i < tempVec.size(); i++) { 
        outputVec.push_back(unsigned(tempVec[i]));
    }
    return outputVec;
}

// Case sensitive
vector<unsigned> find_intersection_street_segments(string intersection_name)
{
     //Calls the previous function after converting intersection name into ID
    return find_intersection_street_segments(
            find_intersection_id_from_name(intersection_name));
}

// Case sensitive
vector<string> find_intersection_street_names(unsigned intersection_id)
{
    // Gets vector of streets from the Intersect class
    vector<int64_t> tempVec = allIntersects[intersection_id].getStreets();
    vector<string> outputVec;
    //converts each int64_t from 1st vector to a string in 2nd vector
    for(unsigned i = 0; i < tempVec.size(); i++) { 
        outputVec.push_back(getStreetName(tempVec[i]));
    }
    return outputVec;
}

// Case sensitive
vector<string> find_intersection_street_names(string intersection_name)
{
    //Calls the previous function after converting intersection name into ID
    return find_intersection_street_names( 
            find_intersection_id_from_name(intersection_name));
}

// Case sensitive
bool are_directly_connected(string intersection_name1,string intersection_name2)
{
    // If both are the same then it's immediately true
    if(intersection_name1 == intersection_name2) {
        return true;
    }
    // Replies if you can get from intersect1 to intersect2 
    //(accounts for one way)
    Intersect Inter1 = allIntersects[find_intersection_id_from_name(
            intersection_name1)];
    unsigned ID2 = find_intersection_id_from_name(intersection_name2);
    // Creates vectors with neighbour intersects of first intersect
    // This vector is designed if we can go from intersect1 to intersect2 in one
    // segment, intersect2 will be within the vector of neighbours in intersect1
    vector<int64_t> Inter1Neighbours = Inter1.getNeighbours();
    
    // Loops through each neighbour of intersect1
    for (unsigned i = 0; i < Inter1Neighbours.size(); i++) {
        if (Inter1Neighbours[i] == int64_t(ID2)) {
            return true; // If it is within neighbour list, return true
        }
    } // After loop if it's not within the list, return false
    return false;
}

// Case sensitive, accounts for one-way
vector<unsigned> find_adjacent_intersections(string intersection_name)
{
    // Gets vector of neighbour intersections from the Intersect class
    unsigned intersectionID = find_intersection_id_from_name(intersection_name);
    // Converts first intersect into a class, second intersect into an ID
    vector<int64_t> tempVec = allIntersects[intersectionID].getNeighbours();
    vector<unsigned> outputVec;
    for (unsigned i = 0; i < tempVec.size(); i++) {
        outputVec.push_back(unsigned(tempVec[i]));
    }
    return outputVec;
}

// Case sensitive
vector<unsigned> find_street_street_segments(string street_name)
{    
    unsigned streetID = find_street_id_from_name(street_name);
    
    vector<int64_t> tempVec = allStreets[streetID].getSegs();
    vector<unsigned> outputVec;
    //converts each int64_t from 1st vector to an unsigned in 2nd vector
    for (unsigned i = 0; i < tempVec.size(); i++) { 
        outputVec.push_back(unsigned(tempVec[i]));
    }
    return outputVec;
}

// Case sensitive
vector<unsigned> find_all_street_intersections(string street_name)
{
    unsigned streetID = find_street_id_from_name(street_name);
    vector<int64_t> tempVec = allStreets[streetID].getIntersects();
    vector<unsigned> outputVec;
    
    //converts each int64_t from 1st vector to an unsigned in 2nd vector
    for(unsigned i = 0; i < tempVec.size(); i++) {
        outputVec.push_back(unsigned(tempVec[i]));
    }
    return outputVec;
}

//find distance between two coordinates
double find_distance_between_two_points(LatLon point1, LatLon point2)
{
    return distance(point1,point2); //actual implementation is in Custapi.cpp  
}

//find the length of a given street segments
//we assume that the order of the points is that the 1st point is closest to the 
//"from" endpoint and progress towards the "to" endpoint
double find_street_segment_length(unsigned street_segment_id)
{
    return allStreetSegs[street_segment_id].getLength();
              
}

//find the length of a whole street by adding its street segment lengths
double find_street_length(string street_name)
{    
    int64_t id = StreetNametoID[street_name];
    return allStreets[id].getLength();
    
}

//find the travel time to drive a street segment (time(minutes)  
// = distance(Km)/speed_limit(Km/h)*60)
double find_segment_travel_time(unsigned street_segment_id)
{    
    return allStreetSegs[street_segment_id].getTime();
}

//find the nearest point of interest (by name) to a given position
//returns NULL if no POI are found
string find_closest_point_of_interest(LatLon my_position)
{
    double mindist = INFINITY; //a flag for if there are 0 POIs on the map
    string name = "";
    for (unsigned i = 0; i < allPoints.size(); i++){
        LatLon featurePos;
        featurePos.lat= allPoints[i].getLocation().getlat();
        featurePos.lon= allPoints[i].getLocation().getlon();
        double dist = distance(my_position, featurePos);
        if (dist <= mindist){
            mindist = dist;
            name = getPointOfInterestName(i);
        }
    }
    
    if (mindist == INFINITY) return NULL;
    return name;
}

// NEW function to use Cord system
//find the nearest point of interest (by name) to a given position
//returns NULL if no POI are found
string find_closest_point_of_interest(Cord my_position)
{
    double mindist = INFINITY; //a flag for if there are 0 POIs on the map
    string name = "";
    for (unsigned i = 0; i < allPoints.size(); i++){
        Cord featurePos = allPoints[i].getLocation();
        double dist = cordDistance(my_position, featurePos);
        if (dist <= mindist) {
            mindist = dist;
            name = allPoints[i].getName();
        }
    }
    
    if (mindist == INFINITY) return NULL;
    return name;
}