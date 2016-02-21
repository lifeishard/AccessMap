#ifndef M2_H
#define	M2_H

#include <string>
#include "StreetsDatabaseAPI.h"
#include "m1.h"
#include "m3.h"
#include <iostream>
#include <cfloat> // for FLT_MAX
#include <chrono>
#include <thread>
#include <cstdlib>
#include <vector>
#include "graphics.h"
#include "DrawAddon.h"
#include "DrawStreet.h"
#include "DrawText.h"
#include "DrawPOI.h"
#include "Screen.h"
#include "pthread.h"

using namespace std;

// Draws the map whose at map_path; this should be a .bin file.
void draw_map(string map_path);
// Callbacks for event-driven window handling.
void drawscreen (void);

//button functions
void rotateLeft ();
void rotateRight();
void switchColor();
void search();
void directions();
void help();

void act_on_button_press (float x, float y, t_event_buttonPressed event);
void act_on_mouse_move (float x, float y);
void act_on_key_press (char c);

pair<double, double> cordOffset(Cord a, Cord b, double offset);
pair<double, double> cordOffset(pair<double,double> a, Cord b, double offset);
string cutChar(string input);
int64_t closestIntersection(float x, float y);

void doSearch();
vector<string> approximateFinder(string orig);
vector<string> approximateNoStreetsFinder(string orig);
char approximateString(string orig, string comp);

vector<string> printDirections(vector<unsigned> path, int64_t startID, int64_t endID, char arePOI, char same);
char isValidPath(vector<unsigned> outputSeg);
void redrawPreviousHover(char previousHover);

void *drawAdditional(void *data);

#endif

