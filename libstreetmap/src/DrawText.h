/* 
 * File:   DrawText.h
 * Author: chenyu69
 *
 * Created on March 7, 2015, 3:23 PM
 */

#ifndef DRAWTEXT_H
#define	DRAWTEXT_H

#include <string>
#include <iostream>
#include <cfloat> // for FLT_MAX
#include <chrono>
#include <thread>
#include <cstdlib>
#include <queue>
#include "graphics.h"
#include "Custapi.h"
#include "Screen.h"
#include <stdio.h>
using namespace std;

const int64_t streetInherentPriority = 100;

void drawStreetNames();
void drawPOINames();
void drawAllText();

typedef struct{
    char priority;
    char type;
    int64_t ID;
    double x,y;
    double angle;
} textToDraw;

typedef struct 
{
  
        bool operator() (const textToDraw& lhs, const textToDraw& rhs)
        {
            return lhs.priority < rhs.priority;
        }
        
} textToDrawcomparison;


enum 
{
    STREETNAMEDISPLAY = 0,
    POINAMEDISPLAY = 1
};

void drawAllText();


void textArrange();


char tooClose(double x1, double y1, double x2, double y2);

#endif	/* DRAWTEXT_H */

