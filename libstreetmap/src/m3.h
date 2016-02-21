#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Custapi.h"
#include "Point.h"
#include "Astar.h"
enum //used for convertTurn helper function
{
    CARDINAL = 0,
    NORAMP = 1,
    RAMP = 2,
};

// Returns a path (route) between the start intersection and the end 
// intersection, if one exists. If no path exists, this routine returns 
// an empty (size == 0) vector. If more than one path exists, the path 
// with the shortest travel time is returned. The path is returned as a vector 
// of street segment ids; traversing these street segments, in the given order,
// would take one from the start to the end intersection.
std::vector<unsigned> find_path_between_intersections(unsigned 
                   intersect_id_start, unsigned intersect_id_end);


// Returns the time required to travel along the path specified. The path
// is passed in as a vector of street segment ids, and this function can 
// assume the vector either forms a legal path or has size == 0.
// The travel time is the sum of the length/speed-limit of each street 
// segment, plus 15 seconds per turn implied by the path. A turn occurs
// when two consecutive street segments have different street names.
double compute_path_travel_time(const std::vector<unsigned>& path);


// Returns the shortest travel time path (vector of street segments) from 
// the start intersection to a point of interest with the specified name.
// If no such path exists, returns an empty (size == 0) vector.
std::vector<unsigned> find_path_to_point_of_interest (unsigned 
                   intersect_id_start, std::string point_of_interest_name);

double getPathTime(vector<unsigned> outputSeg);


// Outputs use friendly driving directions given a vector of streetseg IDs
vector<string> printDirections(vector<unsigned> path, string startPoint, string endPoint);

//returns the turn angle defined by 3 points in radians
//if the segment has curve points it also takes those into account
double findTurnAngle(StreetSeg seg1, StreetSeg seg2);

//converts an angle in radians into one of 3 things: a cardinal direction, a regular turn direction, and a ramped turn direction
//the threshold for "u-turns" and "straight" is pi/8 or 22.5 deg
string convertTurn(double angle, char turnType);

string convertTurnSymbol(double angle);

string convertLength(double length);

string convertTime(double time);

Intersect findNearestIntersectionFromPOI(Point thisPoint);

char isValidPath(vector<unsigned> outputSeg);