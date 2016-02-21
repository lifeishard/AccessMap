/* 
 * File:   DrawStreet.h
 * Author: chenyu69
 *
 * Created on March 7, 2015, 3:25 PM
 */

#ifndef DRAWSTREET_H
#define	DRAWSTREET_H

#include <string>
#include <iostream>
#include <cfloat> // for FLT_MAX
#include <chrono>
#include <thread>
#include <cstdlib>
#include <vector>
#include "graphics.h"
#include "Custapi.h"
#include "Screen.h"

using namespace std;

char willDraw(double speed, double size);
double getRadius(double speed);
double getStartAngle(int64_t seg1, int64_t seg2);
double getAngle(int64_t seg1, int64_t seg2);
string getDirection(double angle);
string getTurn(double angle);
string getSmartLength(double length);
string getSmartTime(double time);
double getPathTime(vector<unsigned> outputSeg);

void drawStreetSeg(unsigned i, double radius, char isBorder);
void drawIntersect(unsigned i, double currentSpeed, double currentSize, char isBorder);
void drawFunctionals();
void drawOneWay();

void highlightPath(vector<unsigned> segs);
vector<string> getDirectionsString(vector<unsigned> segs, string startPoint, string endPoint);

#endif	/* DRAWSTREET_H */

