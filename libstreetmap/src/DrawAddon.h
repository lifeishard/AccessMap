/* 
 * File:   DrawAddon.h
 * Author: chenyu69
 *
 * Created on March 7, 2015, 3:26 PM
 */

#ifndef DRAWADDON_H
#define	DRAWADDON_H

#include <string>
#include <iostream>
#include <cfloat> // for FLT_MAX
#include <chrono>
#include <thread>
#include <cstdlib>
#include <vector>
#include "graphics.h"
#include "DrawBackground.h"
#include "Cord.h"

using namespace std;

enum // For current mouse hover
{
    CLOSEDPREVH = 0,
    OPENPREVH = 1,
    CLOSEDGENH = 2,
    OPENGENH = 3,
    SCROLLSUPH = 4,
    SCROLLUPH = 5,
    SCROLLSDOWNH = 6,
    SCROLLDOWNH = 7,
    ROTATELH = 8,
    ROTATERH = 9,
    ZOOMINH = 10,
    ZOOMOUTH = 11,
    EXITH = 12,
    SEARCHH = 13,
    DIRECTIONH = 14,
    COLORH = 15,
    HELPH = 16,
    CANCELH = 17,
    BLUEH = 18,
    NONEH = 19
};

void drawCompass();
void drawScale();
void drawMiniMap();
void drawMessageBox();
void drawPreviousSearches(char open, char blue);
void drawGeneralDialog(char open);
void drawGeneralDialogOpenLayout(char open);
void drawZoomButtons();
void drawRotateButtons();
void drawSideButtons();

void drawStreetSegLine(unsigned i);

#endif	/* DRAWADDON_H */

