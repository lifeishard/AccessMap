#include "m2.h"

using namespace std;

pthread_t threadME;

t_bound_box tBoundCurrent;

vector<unsigned> pathSegs;
vector<string> previousSearches;
vector<string> similarEntries;
vector<vector<t_color> >colorScheme;

double borderWidth;
double mapAngle;
double mapRatio;
char drawBorders;
char openThread;
char previousSearchesOpen;
char generalOpen;
char currentHover;
string keyboardInput;
string lastInput;
string dirSource, dirDestination;
string inputMessage;
int64_t dirSourceID, dirDestinationID;
int64_t inputShown;
extern int64_t keyMode;
// 0: Normal
// 1: Search
// 2: Directions 1
// 3: Directions 2

// You can use any coordinate system you want.
// The coordinate system below matches what you have probably seen in math 
// (ie. with origin in bottom left).
// Note, that text orientation is not affected. Zero degrees will always be the normal, readable, orientation.
const t_bound_box initial_coords = t_bound_box(0,0,Cord::xBound,Cord::yBound); 
const t_bound_box current_coords;

void draw_map(string map_path)
{
    load_map(map_path);




    
    
    maxX = Cord::xBound;
    maxY = Cord::yBound;
    mapRatio = maxY/maxX;
    messageTime = time(NULL) - textStayTime;
    previousSearchesOpen = 0;
    generalOpen = 0;
    inputShown = 0;
    lastInput = "notthis";
    keyMode = 0;
    
    cout << "INFO: Loaded graphics" << endl;
    
	init_graphics("AccessMap", t_color(0xDF, 0xDF, 0xDF));

	set_visible_world(t_bound_box(0,0,Cord::xBound,Cord::yBound));
 
	t_bound_box old_coords = get_visible_world(); // save the current view for later;

	clearscreen();
        inputMessage = "AccessMap";
	update_message(inputMessage);

        create_button ("Window", "Rotate ->", NULL);
        create_button ("Rotate ->", "Rotate <-", NULL);
        create_button ("Rotate <-", "High Contrast", NULL);
        create_button ("High Contrast", "Search", NULL);
        create_button ("Search", "Directions", NULL);
        create_button ("Directions", "Help", NULL);
        
	// Enable mouse movement (not just button presses) and key board input.
	// The appropriate callbacks will be called by event_loop.
	set_keypress_input (true);
	set_mouse_move_input (true);

	set_visible_world(old_coords);
        
        // Threads here
        openThread = 0;
        int threadOneOK = pthread_create(&threadME, NULL, drawAdditional, (void*)NULL);
        if(!threadOneOK) cout << "INFO: Loaded 1 thread" << endl;

        event_loop(act_on_button_press,act_on_mouse_move,act_on_key_press, drawscreen);

	close_graphics ();
	cout << "INFO: Exited graphics" << endl;

    close_map();
}

void drawscreen (void)
{
    // Disables execution in other threads while panning/zooming
    // unless we want to, of course
    openThread = 0;

    set_draw_mode (DRAW_NORMAL);  // Should set this if your program does any XOR drawing in callbacks.

    clearscreen();  /* Should precede drawing for all drawscreens */

    // Captures current visible world to be used later
    tBoundCurrent = get_visible_world();

    getScreenDimension();

    if(capScreen()) drawscreen();

    // Some calculations needed to draw the minimap, done here so
    // functions don't have to do it over and over again
    miniMultiplierX = miniSizeRatio*tBoundCurrent.get_width()/maxX;
    miniMultiplierY = miniSizeRatio*tBoundCurrent.get_width()/maxY*mapRatio;
    miniX = (tBoundCurrent.get_xcenter()+tBoundCurrent.get_width()/2)-miniOffsetRatio*tBoundCurrent.get_width()
            -miniSizeRatio*tBoundCurrent.get_width()/2;
    miniY = (tBoundCurrent.get_ycenter()-tBoundCurrent.get_height()/2)+miniOffsetRatio*tBoundCurrent.get_width()
            -miniSizeRatio*tBoundCurrent.get_width()/2;

    // Variable to determine whether or not to draw borders
    // set here so we don't have to get_width every time
    if(tBoundCurrent.get_width() <= segBorderThreshold)
    {
        drawBorders = 1;
    } else {
        drawBorders = 0;
    }

    // Default values, used for debugging only
    setfontsize (10);
    setlinestyle (SOLID);
    setlinewidth (10);
    setcolor (PURPLE);

    drawAllFeatures();

    borderWidth = floor(segBorderMultiplier / tBoundCurrent.get_width());
    setlinewidth(borderWidth);

    // Draws street borders, then streets
    drawFunctionals();

    highlightPath(pathSegs);

    drawOneWay();

    drawPOI();

    drawAllText();

    // draw compass
    drawCompass();

    // draw rotate buttons
    drawRotateButtons();

    // draw scale
    drawScale();

    // draw zoom buttons
    drawZoomButtons();

    // draw minimap
    drawMiniMap();

    // draw sidebar buttons
    drawSideButtons();

    // draws message dialog
    // usage: clear messageText
    // push back a string into messageText (max two strings)
    // and do messageTime = time(NULL);
    drawMessageBox();

    // draws the previous searches box
    drawPreviousSearches(previousSearchesOpen,0);

    // draws general dialog box (direction printing etc)
    drawGeneralDialog(generalOpen);
    drawGeneralDialogOpenLayout(generalOpen);
    
    openThread = 1;
}

void switchColor()
{
    colorMode ++;
    colorMode %= 2;
    drawscreen();
}

void search()
{
    if(keyMode == 2 || keyMode == 3)
    {
        dirSource = dirDestination = "";
    }
    // Just sets current keyboard input mode to "search" mode
    keyMode = 1; // Search
    keyboardInput = "";
    inputMessage = "Enter POI, street, or intersection name: ";
    update_message(inputMessage + keyboardInput);
    drawSideButtons();
    flushinput();
    return;
}

void help()
{
    isHelp = 1;
    generalText.clear();
    generalText.push_back("");
    generalText.push_back("[HE]           Welcome to AccessMap!");
    generalText.push_back("");
    generalText.push_back("    AccessMap is a map interface");
    generalText.push_back("designed specifically to be access-");
    generalText.push_back("-ible to a vast amount of people,");
    generalText.push_back("hosting many features such as");
    generalText.push_back("rotation, compass, scale, mini-");
    generalText.push_back("map, on-screen search, search");
    generalText.push_back("suggestions/corrections, search");
    generalText.push_back("history, and much more!");
    generalText.push_back("");
    generalText.push_back("Using the buttons on the right of");
    generalText.push_back("this box, you can scroll through");
    generalText.push_back("texts such as this.");
    generalText.push_back("");
    generalText.push_back("This is the help dialog. If you ever");
    generalText.push_back("need help again, press the \"Help\"");
    generalText.push_back("button on the menu to your right.");
    generalText.push_back("");
    generalText.push_back("Scroll down to see the entire");
    generalText.push_back("help documentation.");
    generalText.push_back("");
    generalText.push_back("[HE]           Information Box");
    generalText.push_back("");
    generalText.push_back("This is the general informations");
    generalText.push_back("box. Here you can see the help");
    generalText.push_back("file as well as directions. You");
    generalText.push_back("can close or open this box at");
    generalText.push_back("any time with the button on");
    generalText.push_back("top right. Scrolling is also");
    generalText.push_back("enabled with the buttons on the");
    generalText.push_back("right. You can scroll through one");
    generalText.push_back("line at a time or ten lines at once.");
    generalText.push_back("");
    generalText.push_back("[HE]           Rotation");
    generalText.push_back("");
    generalText.push_back("The map is completely");
    generalText.push_back("rotatable. To rotate, simply");
    generalText.push_back("press the rotate buttons on");
    generalText.push_back("the right side. To see the");
    generalText.push_back("current orientation of the");
    generalText.push_back("map, there is a compass on");
    generalText.push_back("the top left of the screen.");
    generalText.push_back("");
    generalText.push_back("[HE]           Mini-Map");
    generalText.push_back("");
    generalText.push_back("Traverse between areas on the");
    generalText.push_back("map even faster with the");
    generalText.push_back("mini-map. This is designed to");
    generalText.push_back("help you know where you are");
    generalText.push_back("currently, and also get to");
    generalText.push_back("another area on the map with");
    generalText.push_back("speed and efficiency. Click");
    generalText.push_back("anywhere on the mini-map to");
    generalText.push_back("go there immediately. This");
    generalText.push_back("also tracks your current");
    generalText.push_back("rotation.");
    generalText.push_back("");
    generalText.push_back("[HE]           Search");
    generalText.push_back("");
    generalText.push_back("You can search for specific");
    generalText.push_back("points of interest, street or");
    generalText.push_back("intersection using the Search");
    generalText.push_back("button on the right side. Enter");
    generalText.push_back("the name on the bottom and");
    generalText.push_back("AccessMap will find the place");
    generalText.push_back("for you! By hitting TAB while");
    generalText.push_back("typing we will try to find");
    generalText.push_back("similar names and offer you");
    generalText.push_back("suggestions. Hitting TAB again");
    generalText.push_back("cycles through suggestions.");
    generalText.push_back("");
    generalText.push_back("[HE]           Directions");
    generalText.push_back("");
    generalText.push_back("Find directions between points");
    generalText.push_back("of interest and/or intersections");
    generalText.push_back("can also be done with AccessMap.");
    generalText.push_back("Click on Directions on the right");
    generalText.push_back("to start. You can either type");
    generalText.push_back("your start/end points or simply");
    generalText.push_back("click on them in the map interface.");
    generalText.push_back("Search suggestions are also");
    generalText.push_back("enabled by default by pressing");
    generalText.push_back("TAB or simply hitting enter. Once");
    generalText.push_back("the optimal path has been found");
    generalText.push_back("the directions will show up here,");
    generalText.push_back("and the path will be highlighted");
    generalText.push_back("on the map as well.");
    generalText.push_back("");
    generalText.push_back("[HE]           Alternative Colors");
    generalText.push_back("");
    generalText.push_back("AccessMap also caters to");
    generalText.push_back("individuals with color-blindness");
    generalText.push_back("or impaired vision. The Color");
    generalText.push_back("Change button on the right");
    generalText.push_back("alternates between different");
    generalText.push_back("color schemes that may be");
    generalText.push_back("easier to see for some people");
    generalText.push_back("with these impairments.");
    generalText.push_back("");
    generalText.push_back("[HE]           Other Features");
    generalText.push_back("");
    generalText.push_back("On the top right of the screen");
    generalText.push_back("there is also a scale that");
    generalText.push_back("indicates the current zoom");
    generalText.push_back("level. You can also see your");
    generalText.push_back("previously entered searches");
    generalText.push_back("by pressing the small button");
    generalText.push_back("on the bottom of your screen.");
    generalText.push_back("When you are searching or");
    generalText.push_back("finding directions, you can");
    generalText.push_back("click on them to save you the");
    generalText.push_back("hassle of entering them again.");
    generalTextStart = 0;
    generalOpen = 1;
    drawGeneralDialog(generalOpen);
    drawGeneralDialogOpenLayout(generalOpen);
    flushinput();
    return;
}

void directions()
{
    // Just sets current keyboard input mode to "directions" mode
    keyMode = 2; // Direction source
    keyboardInput = "";
    dirSource = dirDestination = "";
    //destroy_button ("Directions");
    //create_button ("Search", "Cancel Directions", cancelDirections);
    inputMessage = "Click or enter POI or intersection name: ";
    update_message(inputMessage + keyboardInput);
    drawSideButtons();
    flushinput();
    return;
}

void rotateRight()
{
    //this function is very similar to the rotate code in custapi.cpp
    double xMaxCalc = -1 * XYBOUNDS;
    double xMinCalc = XYBOUNDS;
    double yMaxCalc = -1 * XYBOUNDS;
    double yMinCalc = XYBOUNDS;

    Cord::rotationCenter(tBoundCurrent.get_xcenter(),tBoundCurrent.get_ycenter());
    
    Cord::setupRotate(-15);

    //rotates all data
    vector<vector<Cord> >tempFeatures;
    for (unsigned i = 0; i < getFeatureCount(); i++)
    { 
        tempFeatures.push_back(allFeatures[i].getdrawPoints());
        for(unsigned j = 0; j < tempFeatures[i].size();j++)
        {
            tempFeatures[i][j].rotateMapCord();
        }
    }
    
    vector<Cord> tempPoints;
    for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++)
    {
        tempPoints.push_back(allPoints[i].getLocation());
        tempPoints[i].rotateMapCord();
    }
    
    vector<Cord> tempInter;
    for (unsigned i = 0; i < getNumberOfIntersections(); i++)
    { 
         tempInter.push_back(allIntersects[i].getLocation());
         tempInter[i].rotateMapCord();
    }
      
    vector<vector<Cord> >tempSegs;
    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++)
    { 
        tempSegs.push_back(allStreetSegs[i].getBendPoints());
        for(unsigned j = 0; j < tempSegs[i].size();j++)
        {
            tempSegs[i][j].rotateMapCord();
        }
    }
        
   Cord::calcOffset(0,0);
   
   
   //offsets all data and calculates new boundary rectangle for features and 
   //street segments
   
    for (unsigned i = 0; i < getFeatureCount(); i++)
    { 
        for(unsigned j = 0; j < tempFeatures[i].size();j++)
        {
            tempFeatures[i][j].drawOffset();
        }
           
        allFeatures[i].setdrawPoints(tempFeatures[i]);
        allFeatures[i].setboundary();
        allFeatures[i].updatepolyPoints();
     //   allFeatures[i].setCords( yMinCalc,  yMaxCalc, xMinCalc,xMaxCalc);  
    }
   
    for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++)
    {
        tempPoints[i].drawOffset();
        allPoints[i].setLocation(tempPoints[i]);
    }
     
    for (unsigned i = 0; i < getNumberOfIntersections(); i++)
    { 
         tempInter[i].drawOffset();
         allIntersects[i].setLocation(tempInter[i]);
    }
       
    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++)
    { 
        for(unsigned j = 0; j < tempSegs[i].size();j++)
        {
            tempSegs[i][j].drawOffset();
        }

        allStreetSegs[i].setBendPoints(tempSegs[i]);
        allStreetSegs[i].setEndsCord(allIntersects[allStreetSegs[i].getEnds().first].getLocation(),
                allIntersects[allStreetSegs[i].getEnds().second].getLocation());
        allStreetSegs[i].setboundary();
    } 
   
    vector<int64_t> tempStreetSeg;
    for(unsigned i = 0; i < allStreets.size(); i++)
    {
        xMaxCalc = -1 * XYBOUNDS;
        xMinCalc = XYBOUNDS;
        yMaxCalc = -1 * XYBOUNDS;
        yMinCalc = XYBOUNDS;
        tempStreetSeg.clear();
        for(unsigned j = 0; j < allStreets[i].getSegs().size(); j++)
        {
            tempStreetSeg = allStreets[i].getSegs();
            if (allStreetSegs[tempStreetSeg[j]].getboundary().left() < xMinCalc)
                xMinCalc = allStreetSegs[tempStreetSeg[j]].getboundary().left ();
            if (allStreetSegs[tempStreetSeg[j]].getboundary().right() > xMaxCalc)
                xMaxCalc = allStreetSegs[tempStreetSeg[j]].getboundary().right();
            if (allStreetSegs[tempStreetSeg[j]].getboundary().bottom() < yMinCalc)
                yMinCalc = allStreetSegs[tempStreetSeg[j]].getboundary().bottom();
            if (allStreetSegs[tempStreetSeg[j]].getboundary().top() > yMaxCalc)
                yMaxCalc = allStreetSegs[tempStreetSeg[j]].getboundary().top();
        }
        if(xMaxCalc - xMinCalc < 2)
        {
            xMaxCalc ++;
            xMinCalc --;
        }      
        if(yMaxCalc - yMinCalc < 2)
        {
            yMaxCalc ++;
            yMinCalc --;
        }      
        allStreets[i].setboundary(  t_bound_box(xMinCalc, yMinCalc, xMaxCalc, yMaxCalc));
    }
     
    Cord::recordRotation();
 
    tBoundCurrent.offset(Cord::xOffset ,Cord::yOffset );

    set_visible_world(tBoundCurrent);
    return;  
}


void rotateLeft()
{
    double xMaxCalc = -1 * XYBOUNDS;
    double xMinCalc = XYBOUNDS;
    double yMaxCalc = -1 * XYBOUNDS;
    double yMinCalc = XYBOUNDS;

    t_bound_box tBoundCurrent = get_visible_world();
    
    Cord::rotationCenter(tBoundCurrent.get_xcenter(),tBoundCurrent.get_ycenter());

    Cord::setupRotate(15);

    vector<vector<Cord> >tempFeatures;
    for (unsigned i = 0; i < getFeatureCount(); i++)
    { 
        tempFeatures.push_back(allFeatures[i].getdrawPoints());
        for(unsigned j = 0; j < tempFeatures[i].size();j++)
        {
            tempFeatures[i][j].rotateMapCord();
        }
    }
    
    vector<Cord> tempPoints;
    for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++)
    {
        tempPoints.push_back(allPoints[i].getLocation());
        tempPoints[i].rotateMapCord();
    }
    
    vector<Cord> tempInter;
    for (unsigned i = 0; i < getNumberOfIntersections(); i++)
    { 
        tempInter.push_back(allIntersects[i].getLocation());
        tempInter[i].rotateMapCord();
    }
    
    vector<vector<Cord> >tempSegs; 
    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++)
    { 
        tempSegs.push_back(allStreetSegs[i].getBendPoints());
        for(unsigned j = 0; j < tempSegs[i].size();j++)
        {
            tempSegs[i][j].rotateMapCord();
        }
    }
        
    Cord::calcOffset(0,0);
    for (unsigned i = 0; i < getFeatureCount(); i++)
    { 
        for(unsigned j = 0; j < tempFeatures[i].size();j++)
        {
            tempFeatures[i][j].drawOffset();
        }
        allFeatures[i].setdrawPoints(tempFeatures[i]);
        allFeatures[i].setboundary();
        allFeatures[i].updatepolyPoints();
    //    allFeatures[i].setCords( yMinCalc,  yMaxCalc, xMinCalc,xMaxCalc);
    }
   
    for (unsigned i = 0; i < getNumberOfPointsOfInterest(); i++)
    {
        tempPoints[i].drawOffset();
        allPoints[i].setLocation(tempPoints[i]);
    }
     
    for (unsigned i = 0; i < getNumberOfIntersections(); i++)
    { 
        tempInter[i].drawOffset();
        allIntersects[i].setLocation(tempInter[i]);
    }
       
    for (unsigned i = 0; i < getNumberOfStreetSegments(); i++)
    { 
        for(unsigned j = 0; j < tempSegs[i].size();j++)
        {
            tempSegs[i][j].drawOffset();
        }
        allStreetSegs[i].setBendPoints(tempSegs[i]);
        allStreetSegs[i].setEndsCord(allIntersects[allStreetSegs[i].getEnds().first].getLocation(),
                allIntersects[allStreetSegs[i].getEnds().second].getLocation());
        allStreetSegs[i].setboundary();
    }
 
    vector<int64_t> tempStreetSeg;
    for(unsigned i = 0; i < allStreets.size(); i++)
    {
        xMaxCalc = -1 * XYBOUNDS;
        xMinCalc = XYBOUNDS;
        yMaxCalc = -1 * XYBOUNDS;
        yMinCalc = XYBOUNDS;
        tempStreetSeg.clear();
        for(unsigned j = 0; j < allStreets[i].getSegs().size(); j++)
        {
            tempStreetSeg = allStreets[i].getSegs();
            if (allStreetSegs[tempStreetSeg[j]].getboundary().left() < xMinCalc)
                xMinCalc = allStreetSegs[tempStreetSeg[j]].getboundary().left ();
            if (allStreetSegs[tempStreetSeg[j]].getboundary().right() > xMaxCalc)
                xMaxCalc = allStreetSegs[tempStreetSeg[j]].getboundary().right();
            if (allStreetSegs[tempStreetSeg[j]].getboundary().bottom() < yMinCalc)
                yMinCalc = allStreetSegs[tempStreetSeg[j]].getboundary().bottom();
            if (allStreetSegs[tempStreetSeg[j]].getboundary().top() > yMaxCalc)
                yMaxCalc = allStreetSegs[tempStreetSeg[j]].getboundary().top();
        }
        if(xMaxCalc - xMinCalc < 2)
        {
            xMaxCalc ++;
            xMinCalc --;
        }      
        if(yMaxCalc - yMinCalc < 2)
        {
            yMaxCalc ++;
            yMinCalc --;
        }      
        allStreets[i].setboundary(  t_bound_box(xMinCalc, yMinCalc, xMaxCalc, yMaxCalc));
    }
    
    Cord::recordRotation();

    tBoundCurrent.offset(Cord::xOffset ,Cord::yOffset );
    
    set_visible_world(tBoundCurrent);
    return;
}

void act_on_button_press (float x, float y, t_event_buttonPressed event)
{
    //if (event.shift_pressed || event.ctrl_pressed)
    if(x > miniX && x < (miniX+trueScreen.get_width()*miniSizeRatio) &&
       y > miniY && y < (miniY+trueScreen.get_width()*miniSizeRatio*mapRatio))
    { // CLICK on minimap
        double miniMouseX = (x - miniX)/miniMultiplierX;
        double miniMouseY = (y - miniY)/miniMultiplierY;
        pair<double,double> miniMouse = Cord::oldToNew(miniMouseX,miniMouseY);
        miniMouseX = miniMouse.first;
        miniMouseY = miniMouse.second;
        set_visible_world(t_bound_box(miniMouseX-trueScreen.get_width()/2,miniMouseY-trueScreen.get_height()/2,
                miniMouseX+trueScreen.get_width()/2,miniMouseY+trueScreen.get_height()/2));
        drawscreen();
    } else if(x > trueScreen.get_xcenter()-trueScreen.get_width()*previousButtonWidth &&
              x < trueScreen.get_xcenter()+trueScreen.get_width()*previousButtonWidth &&
              y < trueScreen.bottom()+trueScreen.get_width()*previousButtonHeight &&
              previousSearchesOpen == 0)
    { // OPENING previous searches
        previousSearchesOpen = 1;
        drawPreviousSearches(1,0);
        flushinput();
        return;
    } else if(previousSearchesOpen == 1 && x > trueScreen.get_xcenter()-trueScreen.get_width()*previousButtonWidth &&
              x < trueScreen.get_xcenter()+trueScreen.get_width()*previousButtonWidth &&
              y > trueScreen.bottom()+5*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder &&
              y < trueScreen.bottom()+trueScreen.get_width()*previousButtonHeight+5*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder*2)
    { // CLOSING previous searches
        previousSearchesOpen = 0;
        drawscreen();
        return;
    } else if(previousSearchesOpen == 1 && x > trueScreen.get_xcenter()-trueScreen.get_width()*previousBoxWidth &&
              x < trueScreen.get_xcenter()+trueScreen.get_width()*previousBoxWidth &&
              y > trueScreen.bottom()+trueScreen.get_width()*messageTwoLineBorder &&
              y < trueScreen.bottom()+5*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder &&
              keyMode != 0)
    { // GET previous search
        if(previousSearches.size()-floor((y-(trueScreen.bottom()+trueScreen.get_width()*messageTwoLineBorder))/(trueScreen.get_width()*messageTwoLineDisplace)) > 0)
        {
            keyboardInput = previousSearches[previousSearches.size()-1-floor((y-(trueScreen.bottom()+trueScreen.get_width()*messageTwoLineBorder))/(trueScreen.get_width()*messageTwoLineDisplace))];
            previousSearchesOpen = 1;
            if(keyMode == 1)
            {
                inputMessage = "Enter POI, street, or intersection name: ";
            } else if(keyMode == 2)
            {
                inputMessage = "Click or enter POI or intersection name: " ;
            } else if(keyMode == 3)
            {
                inputMessage = "Click or enter POI or intersection destination name: ";
            }
            update_message(inputMessage + keyboardInput);
            flushinput();
            return;
        }
    } else if(x < trueScreen.left()+trueScreen.get_width()*generalButtonSize &&
              y > trueScreen.get_ycenter()-trueScreen.get_width()*generalButtonSize/2 &&
              y < trueScreen.get_ycenter()+trueScreen.get_width()*generalButtonSize/2 &&
              !generalOpen)
    { // OPENING general dialog box
        generalOpen = 1;
        drawGeneralDialog(generalOpen);
        drawGeneralDialogOpenLayout(generalOpen);
        flushinput();
        return;
    } else if(x > trueScreen.left()+trueScreen.get_width()*generalBoxSize-trueScreen.get_width()*generalButtonSize &&
              x < trueScreen.left()+trueScreen.get_width()*generalBoxSize &&
              y > trueScreen.get_ycenter()-trueScreen.get_width()*generalButtonSize/2 &&
              y < trueScreen.get_ycenter()+trueScreen.get_width()*generalButtonSize/2 &&
              generalOpen)
    { // CLOSING general dialog box
        generalOpen = 0;
        drawscreen();
        return;
    } else if(x > trueScreen.left()+trueScreen.get_width()*generalBoxSize-trueScreen.get_width()*generalButtonSize &&
              x < trueScreen.left()+trueScreen.get_width()*generalBoxSize &&
              y > trueScreen.get_ycenter()-trueScreen.get_width()*(generalButtonSize/2+generalButtonSize*2+generalButtonSeparation*2) &&
              y < trueScreen.get_ycenter()-trueScreen.get_width()*(-generalButtonSize/2+generalButtonSize*2+generalButtonSeparation*2) &&
              generalOpen)
    { // Scroll UP
        generalTextStart--;
        if(generalTextStart < 0) 
        {
            generalTextStart = 0;
            return;
        }
        drawGeneralDialog(generalOpen);
        drawGeneralDialogOpenLayout(generalOpen);
        flushinput();
        return;
    } else if(x > trueScreen.left()+trueScreen.get_width()*generalBoxSize-trueScreen.get_width()*generalButtonSize &&
              x < trueScreen.left()+trueScreen.get_width()*generalBoxSize &&
              y > trueScreen.bottom()+trueScreen.get_width()*(generalButtonSize+generalButtonSeparation) &&
              y < trueScreen.bottom()+trueScreen.get_width()*(2*generalButtonSize+generalButtonSeparation) &&
              generalOpen)
    { // Scroll DOWN
        generalTextStart++;
        if(generalTextStart > (int64_t)generalText.size()-1) 
        {
            generalTextStart = generalText.size()-1;
            return;
        }
        drawGeneralDialog(generalOpen);
        drawGeneralDialogOpenLayout(generalOpen);
        flushinput();
        return;
    } else if(x > trueScreen.left()+trueScreen.get_width()*generalBoxSize-trueScreen.get_width()*generalButtonSize &&
              x < trueScreen.left()+trueScreen.get_width()*generalBoxSize &&
              y > trueScreen.get_ycenter()-trueScreen.get_width()*(generalButtonSize/2+generalButtonSize+generalButtonSeparation) &&
              y < trueScreen.get_ycenter()-trueScreen.get_width()*(-generalButtonSize/2+generalButtonSize+generalButtonSeparation) &&
              generalOpen)
    { // Scroll SUPERUP
        if(generalTextStart == 0) return;
        generalTextStart -= 10;
        if(generalTextStart < 0) generalTextStart = 0;
        drawGeneralDialog(generalOpen);
        drawGeneralDialogOpenLayout(generalOpen);
        flushinput();
        return;
    } else if(x > trueScreen.left()+trueScreen.get_width()*generalBoxSize-trueScreen.get_width()*generalButtonSize &&
              x < trueScreen.left()+trueScreen.get_width()*generalBoxSize &&
              y < trueScreen.bottom()+trueScreen.get_width()*generalButtonSize &&
              generalOpen)
    { // Scroll SUPERDOWN
        if(generalTextStart == (int64_t)generalText.size()-1) return;
        generalTextStart += 10;
        if(generalTextStart > (int64_t)generalText.size()-1) generalTextStart = (int64_t)generalText.size()-1;
        drawGeneralDialog(generalOpen);
        drawGeneralDialogOpenLayout(generalOpen);
        flushinput();
        return;
    } else if(hypot(x-(trueScreen.right()-trueScreen.get_width()*(scaleLengthRatio+scaleOffsetRatioSmall)),
                    y-(trueScreen.top()-trueScreen.get_width()*scaleOffsetRatio/2)) < trueScreen.get_width()*zoomDiameter/2)
    { // Zoom OUT
        MYzoom_out();
        drawscreen();
        return;
    } else if(hypot(x-(trueScreen.right()-trueScreen.get_width()*scaleOffsetRatioSmall),
                    y-(trueScreen.top()-trueScreen.get_width()*scaleOffsetRatio/2)) < trueScreen.get_width()*zoomDiameter/2)
    { // Zoom IN
        MYzoom_in();
        drawscreen();
        return;
    } else if(hypot(x-(trueScreen.left()+trueScreen.get_width()*(compassOffsetRatio-rotateButtonDisplace/2)),
                    y-(trueScreen.top()-trueScreen.get_width()*(compassOffsetRatio+rotateButtonLowDisplace))) < trueScreen.get_width()*rotateOuterRadius/2)
    { // Rotate LEFT
        rotateRight(); // the rotate functions do opposite of their name
        drawscreen();
        return;
    } else if(hypot(x-(trueScreen.left()+trueScreen.get_width()*(compassOffsetRatio+rotateButtonDisplace/2)),
                    y-(trueScreen.top()-trueScreen.get_width()*(compassOffsetRatio+rotateButtonLowDisplace))) < trueScreen.get_width()*rotateOuterRadius/2)
    { // Rotate RIGHT
        rotateLeft();
        drawscreen();
        return;
    } else if(x > trueScreen.right()-trueScreen.get_width()*sideButtonLength &&
              y > trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+sideButtonDisplace) &&
              y < trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+sideButtonDisplace+sideButtonWidth))
    { // EXIT button
        MYquit();
        return;
    } else if(x > trueScreen.right()-trueScreen.get_width()*sideButtonLength &&
              y > trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+2*sideButtonDisplace+sideButtonWidth) &&
              y < trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+2*sideButtonDisplace+2*sideButtonWidth))
    { // HELP button
        help();
        return;
    } else if(x > trueScreen.right()-trueScreen.get_width()*sideButtonLength &&
              y > trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+3*sideButtonDisplace+2*sideButtonWidth) &&
              y < trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+3*sideButtonDisplace+3*sideButtonWidth) &&
              keyMode != 1)
    { // SEARCH button
        search();
        return;
    } else if(x > trueScreen.right()-trueScreen.get_width()*sideButtonLength &&
              y > trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+4*sideButtonDisplace+3*sideButtonWidth) &&
              y < trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+4*sideButtonDisplace+4*sideButtonWidth) &&
              keyMode != 2 && keyMode != 3)
    { // DIRECTIONS button
        directions();
        return;
    } else if(x > trueScreen.right()-trueScreen.get_width()*sideButtonLength &&
              y > trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+5*sideButtonDisplace+4*sideButtonWidth) &&
              y < trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+5*sideButtonDisplace+5*sideButtonWidth))
    { // COLOR button
        switchColor();
        return;
    } else if(x > trueScreen.right()-trueScreen.get_width()*sideButtonLength &&
              y > trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+5*sideButtonDisplace+6*sideButtonWidth) &&
              y < trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+5*sideButtonDisplace+7*sideButtonWidth) &&
              keyMode != 0)
    { // CANCEL button
        keyMode = 0; // Normal
        keyboardInput = "";
        dirSource = dirDestination = "";
        inputMessage = "AccessMap";
        update_message(inputMessage);
        drawscreen();
        return;
    }
    
    else if(keyMode == 2)
    { // Click to select intersection
        int64_t closeInt = closestIntersection(x,y);
        messageText.clear();
        messageText.push_back("Start point accepted:");
        messageText.push_back(allIntersects[closeInt].getName());
        messageTime = time(NULL);
        keyMode = 3;
        dirSource = allIntersects[closeInt].getName();
        inputMessage = "Click or enter POI or intersection destination name: ";
        update_message(inputMessage + keyboardInput);
        drawMessageBox();
        flushinput();
        return;
    } else if(keyMode == 3)
    {
        int64_t closeInt = closestIntersection(x,y);
        messageText.clear();
        messageText.push_back("End point accepted:");
        keyboardInput = allIntersects[closeInt].getName();
        messageText.push_back(keyboardInput);
        messageTime = time(NULL);
        keyMode = 0;
        inputMessage = "AccessMap";
        update_message(inputMessage);
        char arePOI;
        char same = 0;
        int64_t startID;
        if(PointNametoID.find(dirSource) == PointNametoID.end())
        {
            dirSourceID = IntersectNametoID.find(dirSource)->second;
            startID = dirSourceID;
            arePOI = 0;
        } else {
            dirSourceID = closestIntersection(allPoints[PointNametoID.find(dirSource)->second].getLocation().getx(),
                                              allPoints[PointNametoID.find(dirSource)->second].getLocation().gety());
            startID = PointNametoID.find(dirSource)->second;
            arePOI = 1;
        }
        dirDestination = allIntersects[closeInt].getName();
        dirDestinationID = closeInt;
        // This is temporary
        pathSegs.clear();
        pathSegs = find_path_between_intersections(dirSourceID, dirDestinationID);
        if(dirSourceID == dirDestinationID) same = 1;
        generalText = printDirections(pathSegs, startID, dirDestinationID, arePOI, same);//getDirectionsString(pathSegs, dirSource, dirDestination);
        if(!isValidPath(pathSegs)) cout << "INVALID PATH!!!" << endl;
        generalTextStart = 0;
        generalOpen = 1;
        isHelp = 0;
        /*
         * Actually carries out algorithms here
         * using dirSourceID and dirDestinationID
         */
        drawscreen();
        return;
    }
    return;
}


void act_on_mouse_move (float x, float y)
{
    if(x > trueScreen.get_xcenter()-trueScreen.get_width()*previousButtonWidth &&
              x < trueScreen.get_xcenter()+trueScreen.get_width()*previousButtonWidth &&
              y < trueScreen.bottom()+trueScreen.get_width()*previousButtonHeight &&
              previousSearchesOpen == 0)
    { // OPENING previous searches
        redrawPreviousHover(currentHover);
        currentHover = CLOSEDPREVH;
        drawPreviousSearches(previousSearchesOpen,0);
        flushinput();
        return;
    } else if(previousSearchesOpen == 1 && x > trueScreen.get_xcenter()-trueScreen.get_width()*previousButtonWidth &&
              x < trueScreen.get_xcenter()+trueScreen.get_width()*previousButtonWidth &&
              y > trueScreen.bottom()+5*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder &&
              y < trueScreen.bottom()+trueScreen.get_width()*previousButtonHeight+5*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder*2)
    { // CLOSING previous searches
        redrawPreviousHover(currentHover);
        currentHover = OPENPREVH;
        drawPreviousSearches(previousSearchesOpen,0);
        flushinput();
        return;
    } else if(previousSearchesOpen == 1 && x > trueScreen.get_xcenter()-trueScreen.get_width()*previousBoxWidth &&
              x < trueScreen.get_xcenter()+trueScreen.get_width()*previousBoxWidth &&
              y > trueScreen.bottom()+trueScreen.get_width()*messageTwoLineBorder &&
              y < trueScreen.bottom()+5*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder)
    { // GET previous search
        if(previousSearches.size()-floor((y-(trueScreen.bottom()+trueScreen.get_width()*messageTwoLineBorder))/(trueScreen.get_width()*messageTwoLineDisplace)) > 0)
        {
            redrawPreviousHover(currentHover);
            currentHover = BLUEH;
            drawPreviousSearches(previousSearchesOpen,1+floor((y-(trueScreen.bottom()+trueScreen.get_width()*messageTwoLineBorder))/(trueScreen.get_width()*messageTwoLineDisplace)));
            flushinput();
            return;
        } else {
            redrawPreviousHover(currentHover);
            currentHover = NONEH;
            drawPreviousSearches(previousSearchesOpen,0);
            flushinput();
            return;
        }
    } else if(x < trueScreen.left()+trueScreen.get_width()*generalButtonSize &&
              y > trueScreen.get_ycenter()-trueScreen.get_width()*generalButtonSize/2 &&
              y < trueScreen.get_ycenter()+trueScreen.get_width()*generalButtonSize/2 &&
              !generalOpen)
    { // OPENING general dialog box
        redrawPreviousHover(currentHover);
        currentHover = CLOSEDGENH;
        drawGeneralDialog(generalOpen);
        drawGeneralDialogOpenLayout(generalOpen);
        flushinput();
        return;
    } else if(x > trueScreen.left()+trueScreen.get_width()*generalBoxSize-trueScreen.get_width()*generalButtonSize &&
              x < trueScreen.left()+trueScreen.get_width()*generalBoxSize &&
              y > trueScreen.get_ycenter()-trueScreen.get_width()*generalButtonSize/2 &&
              y < trueScreen.get_ycenter()+trueScreen.get_width()*generalButtonSize/2 &&
              generalOpen)
    { // CLOSING general dialog box
        redrawPreviousHover(currentHover);
        currentHover = OPENGENH;
        drawGeneralDialogOpenLayout(generalOpen);
        flushinput();
        return;
    } else if(x > trueScreen.left()+trueScreen.get_width()*generalBoxSize-trueScreen.get_width()*generalButtonSize &&
              x < trueScreen.left()+trueScreen.get_width()*generalBoxSize &&
              y > trueScreen.get_ycenter()-trueScreen.get_width()*(generalButtonSize/2+generalButtonSize*2+generalButtonSeparation*2) &&
              y < trueScreen.get_ycenter()-trueScreen.get_width()*(-generalButtonSize/2+generalButtonSize*2+generalButtonSeparation*2) &&
              generalOpen)
    { // Scroll UP
        redrawPreviousHover(currentHover);
        currentHover = SCROLLUPH;
        drawGeneralDialogOpenLayout(generalOpen);
        flushinput();
        return;
    } else if(x > trueScreen.left()+trueScreen.get_width()*generalBoxSize-trueScreen.get_width()*generalButtonSize &&
              x < trueScreen.left()+trueScreen.get_width()*generalBoxSize &&
              y > trueScreen.bottom()+trueScreen.get_width()*(generalButtonSize+generalButtonSeparation) &&
              y < trueScreen.bottom()+trueScreen.get_width()*(2*generalButtonSize+generalButtonSeparation) &&
              generalOpen)
    { // Scroll DOWN
        redrawPreviousHover(currentHover);
        currentHover = SCROLLDOWNH;
        drawGeneralDialogOpenLayout(generalOpen);
        flushinput();
        return;
    } else if(x > trueScreen.left()+trueScreen.get_width()*generalBoxSize-trueScreen.get_width()*generalButtonSize &&
              x < trueScreen.left()+trueScreen.get_width()*generalBoxSize &&
              y > trueScreen.get_ycenter()-trueScreen.get_width()*(generalButtonSize/2+generalButtonSize+generalButtonSeparation) &&
              y < trueScreen.get_ycenter()-trueScreen.get_width()*(-generalButtonSize/2+generalButtonSize+generalButtonSeparation) &&
              generalOpen)
    { // Scroll SUPERUP
        redrawPreviousHover(currentHover);
        currentHover = SCROLLSUPH;
        drawGeneralDialogOpenLayout(generalOpen);
        flushinput();
        return;
    } else if(x > trueScreen.left()+trueScreen.get_width()*generalBoxSize-trueScreen.get_width()*generalButtonSize &&
              x < trueScreen.left()+trueScreen.get_width()*generalBoxSize &&
              y < trueScreen.bottom()+trueScreen.get_width()*generalButtonSize &&
              generalOpen)
    { // Scroll SUPERDOWN
        redrawPreviousHover(currentHover);
        currentHover = SCROLLSDOWNH;
        drawGeneralDialogOpenLayout(generalOpen);
        flushinput();
        return;
    } else if(hypot(x-(trueScreen.right()-trueScreen.get_width()*(scaleLengthRatio+scaleOffsetRatioSmall)),
                    y-(trueScreen.top()-trueScreen.get_width()*scaleOffsetRatio/2)) < trueScreen.get_width()*zoomDiameter/2)
    { // Zoom OUT
        redrawPreviousHover(currentHover);
        currentHover = ZOOMOUTH;
        drawZoomButtons();
        flushinput();
        return;
    } else if(hypot(x-(trueScreen.right()-trueScreen.get_width()*scaleOffsetRatioSmall),
                    y-(trueScreen.top()-trueScreen.get_width()*scaleOffsetRatio/2)) < trueScreen.get_width()*zoomDiameter/2)
    { // Zoom IN
        redrawPreviousHover(currentHover);
        currentHover = ZOOMINH;
        drawZoomButtons();
        flushinput();
        return;
    } else if(hypot(x-(trueScreen.left()+trueScreen.get_width()*(compassOffsetRatio-rotateButtonDisplace/2)),
                    y-(trueScreen.top()-trueScreen.get_width()*(compassOffsetRatio+rotateButtonLowDisplace))) < trueScreen.get_width()*rotateOuterRadius/2)
    { // Rotate LEFT
        redrawPreviousHover(currentHover);
        currentHover = ROTATELH;
        drawRotateButtons();
        flushinput();
        return;
    } else if(hypot(x-(trueScreen.left()+trueScreen.get_width()*(compassOffsetRatio+rotateButtonDisplace/2)),
                    y-(trueScreen.top()-trueScreen.get_width()*(compassOffsetRatio+rotateButtonLowDisplace))) < trueScreen.get_width()*rotateOuterRadius/2)
    { // Rotate RIGHT
        redrawPreviousHover(currentHover);
        currentHover = ROTATERH;
        drawRotateButtons();
        flushinput();
        return;
    } else if(x > trueScreen.right()-trueScreen.get_width()*sideButtonLength &&
              y > trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+sideButtonDisplace) &&
              y < trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+sideButtonDisplace+sideButtonWidth))
    { // EXIT button
        redrawPreviousHover(currentHover);
        currentHover = EXITH;
        drawSideButtons();
        flushinput();
        return;
    } else if(x > trueScreen.right()-trueScreen.get_width()*sideButtonLength &&
              y > trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+2*sideButtonDisplace+sideButtonWidth) &&
              y < trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+2*sideButtonDisplace+2*sideButtonWidth))
    { // HELP button
        redrawPreviousHover(currentHover);
        currentHover = HELPH;
        drawSideButtons();
        flushinput();
        return;
    } else if(x > trueScreen.right()-trueScreen.get_width()*sideButtonLength &&
              y > trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+3*sideButtonDisplace+2*sideButtonWidth) &&
              y < trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+3*sideButtonDisplace+3*sideButtonWidth) &&
              keyMode != 1)
    { // SEARCH button
        redrawPreviousHover(currentHover);
        currentHover = SEARCHH;
        drawSideButtons();
        flushinput();
        return;
    } else if(x > trueScreen.right()-trueScreen.get_width()*sideButtonLength &&
              y > trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+4*sideButtonDisplace+3*sideButtonWidth) &&
              y < trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+4*sideButtonDisplace+4*sideButtonWidth) &&
              keyMode != 2 && keyMode != 3)
    { // DIRECTIONS button
        redrawPreviousHover(currentHover);
        currentHover = DIRECTIONH;
        drawSideButtons();
        flushinput();
        return;
    } else if(x > trueScreen.right()-trueScreen.get_width()*sideButtonLength &&
              y > trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+5*sideButtonDisplace+4*sideButtonWidth) &&
              y < trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+5*sideButtonDisplace+5*sideButtonWidth))
    { // COLOR button
        redrawPreviousHover(currentHover);
        currentHover = COLORH;
        drawSideButtons();
        flushinput();
        return;
    } else if(x > trueScreen.right()-trueScreen.get_width()*sideButtonLength &&
              y > trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+5*sideButtonDisplace+6*sideButtonWidth) &&
              y < trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+5*sideButtonDisplace+7*sideButtonWidth) &&
              keyMode != 0)
    { // CANCEL button
        redrawPreviousHover(currentHover);
        currentHover = CANCELH;
        drawSideButtons();
        flushinput();
        return;
    } else {
        redrawPreviousHover(currentHover);
        currentHover = NONEH;
        return;
    }
}


void act_on_key_press (char c) {
    // function to handle keyboard press event, the ASCII character is returned
    if (keyMode == 1)
    {
        inputMessage = "Enter POI, street, or intersection name: ";
        if(c >= 32 && c <= 126) // These are regular characters
        {
            keyboardInput.push_back(c);
            update_message(inputMessage + keyboardInput);
            flushinput();
            return;
        } else if(c == 8) // BACKSPACE
        {
            if(keyboardInput.size() != 0)
            {
                keyboardInput.resize(keyboardInput.size()-1); 
                update_message(inputMessage + keyboardInput);
                flushinput();
                return;
            }
            return;
        } else if(c == 13) // CARRIAGE RETURN
        {
            if(lastInput == keyboardInput)
            {
                if((unsigned)(inputShown+1) < similarEntries.size())
                {
                    inputShown++;
                    messageText.clear();
                    messageText.push_back(to_string(similarEntries.size())+" matches for "+keyboardInput+".");
                    messageText.push_back("Do you mean: "+similarEntries[inputShown]+"?");
                    messageTime = time(NULL);
                    previousSearches.push_back(keyboardInput);
                    keyboardInput = "";
                    update_message(inputMessage + keyboardInput);
                    drawMessageBox();
                    drawPreviousSearches(previousSearchesOpen,0);
                    flushinput();
                    return;
                }
            }
            lastInput = keyboardInput;
            inputShown = 0;
            similarEntries.clear();
            similarEntries = approximateFinder(keyboardInput);
            if(similarEntries.size() == 0)
            {
                messageText.clear();
                messageText.push_back("No matches for "+keyboardInput+",");
                messageText.push_back("please check your input and try again.");
                messageTime = time(NULL);
                previousSearches.push_back(keyboardInput);
                keyboardInput = "";
                update_message(inputMessage + keyboardInput);
                drawMessageBox();
                drawPreviousSearches(previousSearchesOpen,0);
                flushinput();
                return;
            } else if(similarEntries.size() > 1)
            {
                messageText.clear();
                messageText.push_back(to_string(similarEntries.size())+" matches for "+keyboardInput+".");
                messageText.push_back("Do you mean: "+similarEntries[0]+"?");
                messageTime = time(NULL);
                previousSearches.push_back(keyboardInput);
                keyboardInput = "";
                update_message(inputMessage + keyboardInput);
                drawMessageBox();
                drawPreviousSearches(previousSearchesOpen,0);
                flushinput();
                return;
            } else {
                keyboardInput = similarEntries[0];
                keyMode = 0;
                inputMessage = "AccessMap";
                previousSearches.push_back(keyboardInput);
                messageText.clear();
                messageText.push_back(keyboardInput+" found!");
                messageTime = time(NULL);
                doSearch();
                return;
            }
            return;
        } else if(c == 9) // HORIZONTAL TAB (for auto-completion)
        {
            inputMessage = "Enter POI, street, or intersection name: ";
            if(lastInput == keyboardInput)
            {
                if((unsigned)(inputShown+1) < similarEntries.size())
                {
                    inputShown++;
                    messageText.clear();
                    messageText.push_back(to_string(similarEntries.size())+" matches for "+keyboardInput+".");
                    messageText.push_back("Do you mean: "+similarEntries[inputShown]+"?");
                    messageTime = time(NULL);
                    update_message(inputMessage + keyboardInput);
                    drawMessageBox();
                    flushinput();
                    return;
                }
            }
            lastInput = keyboardInput;
            inputShown = 0;
            similarEntries.clear();
            similarEntries = approximateFinder(keyboardInput);
            if(similarEntries.size() == 0)
            {
                messageText.clear();
                messageText.push_back("No matches for "+keyboardInput+",");
                messageText.push_back("please check your input and try again.");
                messageTime = time(NULL);
                update_message(inputMessage + keyboardInput);
                drawMessageBox();
                flushinput();
                return;
            } else if(similarEntries.size() > 1)
            {
                messageText.clear();
                messageText.push_back(to_string(similarEntries.size())+" matches for "+keyboardInput+".");
                messageText.push_back("Do you mean: "+similarEntries[0]+"?");
                messageTime = time(NULL);
                update_message(inputMessage + keyboardInput);
                drawMessageBox();
                flushinput();
                return;
            } else {
                keyboardInput = similarEntries[0];
                messageText.clear();
                messageText.push_back("Auto-completion has found:");
                messageText.push_back(keyboardInput);
                messageTime = time(NULL);
                update_message(inputMessage + keyboardInput);
                drawMessageBox();
                flushinput();
                return;
            }
            return;
        }
    } else if(keyMode == 2)
    {
        inputMessage = "Click or enter POI or intersection name: ";
        if(c >= 32 && c <= 126) // These are regular characters
        {
            keyboardInput.push_back(c);
            update_message(inputMessage + keyboardInput);
            flushinput();
            return;
        } else if(c == 8) // BACKSPACE
        {
            if(keyboardInput.size() != 0)
            {
                keyboardInput.resize(keyboardInput.size()-1);
                update_message(inputMessage + keyboardInput);
                flushinput();
                return;
            }
            return;
        } else if(c == 13) // CARRIAGE RETURN
        {
            if(lastInput == keyboardInput)
            {
                if((unsigned)(inputShown+1) < similarEntries.size())
                {
                    inputShown++;
                    messageText.clear();
                    messageText.push_back(to_string(similarEntries.size())+" matches for "+keyboardInput+".");
                    messageText.push_back("Do you mean: "+similarEntries[inputShown]+"?");
                    messageTime = time(NULL);
                    previousSearches.push_back(keyboardInput);
                    keyboardInput = "";
                    update_message(inputMessage + keyboardInput);
                    drawMessageBox();
                    drawPreviousSearches(previousSearchesOpen,0);
                    flushinput();
                    return;
                }
            }
            lastInput = keyboardInput;
            inputShown = 0;
            similarEntries.clear();
            similarEntries = approximateNoStreetsFinder(keyboardInput);
            if(similarEntries.size() == 0)
            {
                messageText.clear();
                messageText.push_back("No matches for "+keyboardInput+",");
                messageText.push_back("please check your input and try again.");
                messageTime = time(NULL);
                previousSearches.push_back(keyboardInput);
                keyboardInput = "";
                update_message(inputMessage + keyboardInput);
                drawMessageBox();
                drawPreviousSearches(previousSearchesOpen,0);
                flushinput();
                return;
            } else if(similarEntries.size() > 1)
            {
                messageText.clear();
                messageText.push_back(to_string(similarEntries.size())+" matches for "+keyboardInput+".");
                messageText.push_back("Do you mean: "+similarEntries[0]+"?");
                messageTime = time(NULL);
                previousSearches.push_back(keyboardInput);
                keyboardInput = "";
                update_message(inputMessage + keyboardInput);
                drawMessageBox();
                drawPreviousSearches(previousSearchesOpen,0);
                flushinput();
                return;
            } else {
                keyboardInput = similarEntries[0];
                messageText.clear();
                messageText.push_back("Start point accepted:");
                messageText.push_back(keyboardInput);
                messageTime = time(NULL);
                keyMode = 3;
                previousSearches.push_back(keyboardInput);
                dirSource = keyboardInput;
                inputMessage = "Click or enter POI or intersection destination name: ";
                update_message(inputMessage + keyboardInput);
                doSearch();
                return;
            }
            return;
        } else if(c == 9) // HORIZONTAL TAB (for auto-completion)
        {
            inputMessage = "Click or enter POI or intersection name: ";
            if(lastInput == keyboardInput)
            {
                if((unsigned)(inputShown+1) < similarEntries.size())
                {
                    inputShown++;
                    messageText.clear();
                    messageText.push_back(to_string(similarEntries.size())+" matches for "+keyboardInput+".");
                    messageText.push_back("Do you mean: "+similarEntries[inputShown]+"?");
                    messageTime = time(NULL);
                    update_message(inputMessage + keyboardInput);
                    drawMessageBox();
                    flushinput();
                    return;
                }
            }
            lastInput = keyboardInput;
            inputShown = 0;
            similarEntries.clear();
            similarEntries = approximateNoStreetsFinder(keyboardInput);
            if(similarEntries.size() == 0)
            {
                messageText.clear();
                messageText.push_back("No matches for "+keyboardInput+",");
                messageText.push_back("please check your input and try again.");
                messageTime = time(NULL);
                update_message(inputMessage + keyboardInput);
                drawMessageBox();
                flushinput();
                return;
            } else if(similarEntries.size() > 1)
            {
                messageText.clear();
                messageText.push_back(to_string(similarEntries.size())+" matches for "+keyboardInput+".");
                messageText.push_back("Do you mean: "+similarEntries[0]+"?");
                messageTime = time(NULL);
                update_message(inputMessage + keyboardInput);
                drawMessageBox();
                flushinput();
                return;
            } else {
                keyboardInput = similarEntries[0];
                messageText.clear();
                messageText.push_back("Auto-completion has found:");
                messageText.push_back(keyboardInput);
                messageTime = time(NULL);
                update_message(inputMessage + keyboardInput);
                drawMessageBox();
                flushinput();
                return;
            }
            return;
        }
    } else if(keyMode == 3)
    {
        inputMessage = "Click or enter POI or intersection destination name: ";
        if(c >= 32 && c <= 126) // These are regular characters
        {
            keyboardInput.push_back(c);
            update_message(inputMessage + keyboardInput);
            flushinput();
            return;
        } else if(c == 8) // BACKSPACE
        {
            if(keyboardInput.size() != 0)
            {
                keyboardInput.resize(keyboardInput.size()-1);
                update_message(inputMessage + keyboardInput);
                flushinput();
                return;
            }
            return;
        } else if(c == 13) // CARRIAGE RETURN
        {
            if(lastInput == keyboardInput)
            {
                if((unsigned)(inputShown+1) < similarEntries.size())
                {
                    inputShown++;
                    messageText.clear();
                    messageText.push_back(to_string(similarEntries.size())+" matches for "+keyboardInput+".");
                    messageText.push_back("Do you mean: "+similarEntries[inputShown]+"?");
                    messageTime = time(NULL);
                    previousSearches.push_back(keyboardInput);
                    keyboardInput = "";
                    update_message(inputMessage + keyboardInput);
                    drawMessageBox();
                    drawPreviousSearches(previousSearchesOpen,0);
                    flushinput();
                    return;
                }
            }
            lastInput = keyboardInput;
            inputShown = 0;
            similarEntries.clear();
            similarEntries = approximateNoStreetsFinder(keyboardInput);
            if(similarEntries.size() == 0)
            {
                messageText.clear();
                messageText.push_back("No matches for "+keyboardInput+",");
                messageText.push_back("please check your input and try again.");
                messageTime = time(NULL);
                previousSearches.push_back(keyboardInput);
                keyboardInput = "";
                update_message(inputMessage + keyboardInput);
                drawMessageBox();
                drawPreviousSearches(previousSearchesOpen,0);
                flushinput();
                return;
            } else if(similarEntries.size() > 1)
            {
                messageText.clear();
                messageText.push_back(to_string(similarEntries.size())+" matches for "+keyboardInput+".");
                messageText.push_back("Do you mean: "+similarEntries[0]+"?");
                messageTime = time(NULL);
                previousSearches.push_back(keyboardInput);
                keyboardInput = "";
                update_message(inputMessage + keyboardInput);
                drawMessageBox();
                drawPreviousSearches(previousSearchesOpen,0);
                flushinput();
                return;
            } else {
                keyboardInput = similarEntries[0];
                messageText.clear();
                messageText.push_back("End point accepted:");
                messageText.push_back(keyboardInput);
                messageTime = time(NULL);
                keyMode = 0;
                previousSearches.push_back(keyboardInput);
                dirDestination = keyboardInput;
                inputMessage = "AccessMap";
                char arePOI;
                char same = 0;
                int64_t startID, endID;
                if(PointNametoID.find(dirSource) == PointNametoID.end())
                {
                    dirSourceID = IntersectNametoID.find(dirSource)->second;
                    startID = dirSourceID;
                    arePOI = 0;
                } else {
                    dirSourceID = closestIntersection(allPoints[PointNametoID.find(dirSource)->second].getLocation().getx(),
                                                      allPoints[PointNametoID.find(dirSource)->second].getLocation().gety());
                    startID = PointNametoID.find(dirSource)->second;
                    arePOI = 1;
                }
                if(PointNametoID.find(dirDestination) == PointNametoID.end())
                {
                    dirDestinationID = IntersectNametoID.find(dirDestination)->second;
                    endID = dirDestinationID;
                } else {
                    dirDestinationID = closestIntersection(allPoints[PointNametoID.find(dirDestination)->second].getLocation().getx(),
                                                      allPoints[PointNametoID.find(dirDestination)->second].getLocation().gety());
                    endID = PointNametoID.find(dirDestination)->second;
                    if(arePOI) arePOI = 3;
                    else arePOI = 2;
                }
                pathSegs.clear();
                pathSegs = find_path_between_intersections(dirSourceID, dirDestinationID);
                if(dirSourceID == dirDestinationID) same = 1;
                generalText = printDirections(pathSegs, startID, endID, arePOI, same);//getDirectionsString(pathSegs, dirSource, dirDestination);
                if(!isValidPath(pathSegs)) cout << "INVALID PATH!!!" << endl;
                generalTextStart = 0;
                generalOpen = 1;
                isHelp = 0;
                /*
                 * Actually carries out algorithms here
                 * using dirSource and dirDestination
                 */
                doSearch();
                return;
            }
            return;
        } else if(c == 9) // HORIZONTAL TAB (for auto-completion)
        {
            inputMessage = "Click or enter POI or intersection destination name: ";
            if(lastInput == keyboardInput)
            {
                if((unsigned)(inputShown+1) < similarEntries.size())
                {
                    inputShown++;
                    messageText.clear();
                    messageText.push_back(to_string(similarEntries.size())+" matches for "+keyboardInput+".");
                    messageText.push_back("Do you mean: "+similarEntries[inputShown]+"?");
                    messageTime = time(NULL);
                    update_message(inputMessage + keyboardInput);
                    drawMessageBox();
                    flushinput();
                    return;
                }
            }
            lastInput = keyboardInput;
            inputShown = 0;
            similarEntries.clear();
            similarEntries = approximateNoStreetsFinder(keyboardInput);
            if(similarEntries.size() == 0)
            {
                messageText.clear();
                messageText.push_back("No matches for "+keyboardInput+",");
                messageText.push_back("please check your input and try again.");
                messageTime = time(NULL);
                update_message(inputMessage + keyboardInput);
                drawMessageBox();
                flushinput();
                return;
            } else if(similarEntries.size() > 1)
            {
                messageText.clear();
                messageText.push_back(to_string(similarEntries.size())+" matches for "+keyboardInput+".");
                messageText.push_back("Do you mean: "+similarEntries[0]+"?");
                messageTime = time(NULL);
                update_message(inputMessage + keyboardInput);
                drawMessageBox();
                flushinput();
                return;
            } else {
                keyboardInput = similarEntries[0];
                messageText.clear();
                messageText.push_back("Auto-completion has found:");
                messageText.push_back(keyboardInput);
                messageTime = time(NULL);
                update_message(inputMessage + keyboardInput);
                drawMessageBox();
                flushinput();
                return;
            }
            return;
        }
    }
}

// Gets the absolute scalar distance of two coordinates, in x-y system
pair<double, double> cordOffset(Cord a, Cord b, double offset)
{
    pair<double, double> newCord;
    double tdist = cordDistance(a,b);
    newCord.first = b.getx()+(b.getx()-a.getx())*(offset/tdist);
    newCord.second = b.gety()+(b.gety()-a.gety())*(offset/tdist);
    return newCord;
}

// Same function but overloaded to have pair<double,double> as first argument
pair<double, double> cordOffset(pair<double,double> a, Cord b, double offset)
{
    pair<double, double> newCord;
    double tdist = cordDistance(a,b);
    newCord.first = b.getx()+(b.getx()-a.first)*(offset/tdist);
    newCord.second = b.gety()+(b.gety()-a.second)*(offset/tdist);
    return newCord;
}

string cutChar(string input)
{
    string output;
    for(string::iterator i = input.begin()+1; i != input.end(); ++i)
    {
        output.push_back(*i);
    }
    return output;
}

// Actual search function implemented here (the button is only an initiator)
void doSearch()
{
    //moves screen to a location based on user text input
    //first searches in points of interest
    //then searches in intersection
    //last searches for street names
    double qx=-1 *XYBOUNDS;
    double qy=-1 *XYBOUNDS;
 
   unordered_map<string,int64_t>::iterator  query = PointNametoID.find(keyboardInput);
   if(query != PointNametoID.end())
   {
       qx = allPoints[query->second].getLocation().getx();
       qy = allPoints[query->second].getLocation().gety();
   }
   query = IntersectNametoID.find(keyboardInput);
   
   if(query != IntersectNametoID.end())
   {
       qx = allIntersects[query->second].getLocation().getx();
       qy = allIntersects[query->second].getLocation().gety();
   }
   query = StreetNametoID.find(keyboardInput);   
   
   if(query != StreetNametoID.end())
   {
       //if matched a street it moves screen to middle of the street
     vector<int64_t> queryIntersects = allStreets[query->second].getIntersects();
     qx = allIntersects[queryIntersects[queryIntersects.size()/2]].getLocation().getx();
     qy = allIntersects[queryIntersects[queryIntersects.size()/2]].getLocation().gety();
   }
   if(qx == -1 *XYBOUNDS){
       set_visible_world(tBoundCurrent);
   }
   else
   {
       set_visible_world(t_bound_box(qx-500,qy-500,qx+500,qy+500));
   }
   keyboardInput = "";
   update_message(inputMessage);
   drawscreen();
   return;
}

vector<string> approximateFinder(string orig)
{
    vector<string> approxStrings;
    for(unsigned i = 0; i < allPoints.size(); i++)
    {
        if(approximateString(orig, allPoints[i].getName()))
        {
            if(orig.size() == allPoints[i].getName().size())
            { // If sizes are the same it is an exact match (most of the time)
                approxStrings.clear();
                approxStrings.push_back(allPoints[i].getName());
                return approxStrings;
            }
            approxStrings.push_back(allPoints[i].getName());
        }
    }
    for(unsigned i = 0; i < allStreets.size(); i++)
    {
        if(approximateString(orig, allStreets[i].getName()))
        {
            if(orig.size() == allStreets[i].getName().size())
            { // If sizes are the same it is an exact match (most of the time)
                approxStrings.clear();
                approxStrings.push_back(allStreets[i].getName());
                return approxStrings;
            }
            approxStrings.push_back(allStreets[i].getName());
        }
    }
    for(unsigned i = 0; i < allIntersects.size(); i++)
    {
        if(approximateString(orig, allIntersects[i].getName()))
        {
            if(orig.size() == allIntersects[i].getName().size())
            { // If sizes are the same it is an exact match (most of the time)
                approxStrings.clear();
                approxStrings.push_back(allIntersects[i].getName());
                return approxStrings;
            }
            approxStrings.push_back(allIntersects[i].getName());
        }
    }
    return approxStrings;
}

vector<string> approximateNoStreetsFinder(string orig)
{
    vector<string> approxStrings;
    for(unsigned i = 0; i < allPoints.size(); i++)
    {
        if(approximateString(orig, allPoints[i].getName()))
        {
            if(orig.size() == allPoints[i].getName().size())
            { // If sizes are the same it is an exact match (most of the time)
                approxStrings.clear();
                approxStrings.push_back(allPoints[i].getName());
                return approxStrings;
            }
            approxStrings.push_back(allPoints[i].getName());
        }
    }
    for(unsigned i = 0; i < allIntersects.size(); i++)
    {
        if(approximateString(orig, allIntersects[i].getName()))
        {
            if(orig.size() == allIntersects[i].getName().size())
            { // If sizes are the same it is an exact match (most of the time)
                approxStrings.clear();
                approxStrings.push_back(allIntersects[i].getName());
                return approxStrings;
            }
            approxStrings.push_back(allIntersects[i].getName());
        }
    }
    return approxStrings;
}

char approximateString(string orig, string comp)
{
    string origCopy = orig;
    string compCopy = comp;
    for(unsigned i = 0; i < orig.size(); i++)
    {
        origCopy[i] = tolower(origCopy[i]);
    }
    for(unsigned i = 0; i < comp.size(); i++)
    {
        compCopy[i] = tolower(compCopy[i]);
    }
    char *tempOrig = strtok((char*)origCopy.c_str()," ,./:;'\"");
    while(tempOrig != NULL)
    {
        string tempString = tempOrig;
        if(tempString != "and")
        {
            if(strstr(compCopy.c_str(),tempOrig) == NULL) return 0;
        }
        tempOrig = strtok(NULL," ,./:;'\"");
    }
    return 1;
}

int64_t closestIntersection(float x, float y)
{
    double shortestDistance = INFINITY;
    double currentDistance;
    int64_t shortestID = 0;
    for(unsigned i = 0; i < allIntersects.size(); i++)
    {
        currentDistance = pow(pow(allIntersects[i].getLocation().gety()-y,2)+pow(allIntersects[i].getLocation().getx()-x,2),0.5);
        if(currentDistance < shortestDistance)
        {
            shortestDistance = currentDistance;
            shortestID = i;
        }
    }
    return shortestID;
}

void redrawPreviousHover(char previousHover)
{
    currentHover = NONEH;
    if(previousHover == NONEH)
    {
        return;
    } else if(previousHover == CLOSEDPREVH || previousHover == OPENPREVH)
    {
        drawPreviousSearches(previousSearchesOpen,0);
        flushinput();
        return;
    } else if(previousHover == CLOSEDGENH)
    {
        drawGeneralDialog(generalOpen);
        drawGeneralDialogOpenLayout(generalOpen);
        flushinput();
        return;
    } else if(previousHover == OPENGENH ||
              previousHover == SCROLLSUPH || previousHover == SCROLLUPH || 
              previousHover == SCROLLSDOWNH || previousHover == SCROLLDOWNH)
    {
        drawGeneralDialogOpenLayout(generalOpen);
        flushinput();
        return;
    } else if(previousHover == ROTATELH || previousHover == ROTATERH)
    {
        drawRotateButtons();
        flushinput();
        return;
    } else if(previousHover == ZOOMINH || previousHover == ZOOMOUTH)
    {
        drawZoomButtons();
        flushinput();
        return;
    } else if(previousHover == EXITH || previousHover == SEARCHH ||
              previousHover == DIRECTIONH || previousHover == COLORH ||
              previousHover == HELPH)
    {
        drawSideButtons();
        flushinput();
        return;
    } else if(previousHover == BLUEH)
    {
        drawPreviousSearches(previousSearchesOpen,0);
        flushinput();
        return;
    } else {
        return;
    }
}

void *drawAdditional(void *data)
{
    while(1)
    {
        if(!openThread) continue;
        int newChar = getchar();
        cout << newChar << endl;
    }
    return 0;
}
