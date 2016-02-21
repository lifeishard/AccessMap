//protects against multiple inclusions of this header file
#ifndef M1_H
#define	M1_H
#include <iostream> // Just so cout is implemented and can be used to test functions
#include <string>
#include <vector>
#include <cmath>
#include "StreetsDatabaseAPI.h"
#include "Custapi.h"
#include "AlgorithmHelper.h"
#include "Astar.h"
//function to load bin or osm map
bool load_map(std::string map_name);

//close the loaded map
void close_map();

//function to return intersection id for an intersection name
unsigned find_intersection_id_from_name(std::string intersection_name);

//function to return street id for an intersection name
unsigned find_street_id_from_name(std::string street_name);

//function to return the street segments for a given intersection 
std::vector<unsigned> find_intersection_street_segments(std::string intersection_name);
std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id);

//function to return street names at an intersection
std::vector<std::string> find_intersection_street_names(std::string intersection_name);
std::vector<std::string> find_intersection_street_names(unsigned intersection_id);

//can you get from intersection1 to intersection2 using a single street segment (hint: check for 1-way streets too)
bool are_directly_connected(std::string intersection_name1, std::string intersection_name2);

//find all intersections connected by one street segment from given intersection (hint: check for 1-way streets too)
std::vector<unsigned> find_adjacent_intersections(std::string intersection_name);

//for a given street, return all the street segments
std::vector<unsigned> find_street_street_segments(std::string street_name);

//for a given street, find all the intersections
std::vector<unsigned> find_all_street_intersections(std::string street_name);

//find distance between two coordinates
double find_distance_between_two_points(LatLon point1, LatLon point2);

//find the length of a given street segments
double find_street_segment_length(unsigned street_segment_id);

//find the length of a whole street
double find_street_length(std::string street_name);

//find the travel time to drive a street segment (time(minutes) = distance(Km)/speed_limit(Km/h)*60)
double find_segment_travel_time(unsigned street_segment_id);

//find the nearest point of interest (by name) to a given position
std::string find_closest_point_of_interest(LatLon my_position);
std::string find_closest_point_of_interest(Cord my_position); // Same function but with Cord

// Distance between two cords
double cordDistance(Cord a, Cord b);
#endif