/* 
 * File:   screen.h
 * Author: chenyu69
 *
 * Created on March 6, 2015, 11:26 PM
 */

#ifndef SCREEN_H
#define	SCREEN_H

#include "Cord.h"
#include "graphics.h"
#include "math.h"
#include <algorithm>
#include <queue>
using namespace std;

const double screenCapSmallest = 10;

extern int64_t zoom;
extern t_bound_box visibleMap;
extern t_bound_box trueScreen;  
extern vector <int64_t> drawStreetNameID;
extern vector <int64_t> drawPOINameID;
void getScreenDimension();
char capScreen();
char onScreen(Cord point1);
char onScreen(t_bound_box _boundary);

#endif	/* SCREEN_H */

