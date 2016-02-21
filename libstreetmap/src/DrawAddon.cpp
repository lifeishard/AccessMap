#include "DrawAddon.h"

int64_t generalTextStart;
double maxX, maxY;
clock_t messageTime;
vector<string> messageText;
vector<string> generalText;
int64_t keyMode;
char isHelp;

// Draws a street segment line for the minimap, since we don't need polygons
// for minimap streets
void drawStreetSegLine(unsigned i)
{
    double x1, y1;
    double x2 = allLargeStreetSegs[i].getEndsCord().first.getx()*miniMultiplierX + miniX;
    double y2 = allLargeStreetSegs[i].getEndsCord().first.gety()*miniMultiplierY + miniY;

    vector<Cord> segBends = allLargeStreetSegs[i].getBendPoints();
    for (unsigned j = 0; j < segBends.size(); j++) {
        // Shifts old end point to new start point
        x1 = x2;
        y1 = y2;
        x2 = segBends[j].getx()*miniMultiplierX + miniX;
        y2 = segBends[j].gety()*miniMultiplierY + miniY;
        drawline(x1, y1, x2, y2);
    }
    x1 = x2;
    y1 = y2;
    x2 = allLargeStreetSegs[i].getEndsCord().second.getx()*miniMultiplierX + miniX;
    y2 = allLargeStreetSegs[i].getEndsCord().second.gety()*miniMultiplierY + miniY;
    drawline(x1, y1, x2, y2);
}

// Draws a compass on top left of the screen
void drawCompass()
{
    double compassOffset = trueScreen.get_width() * compassOffsetRatio;
    double screenLeft = trueScreen.get_xcenter() - trueScreen.get_width() / 2;
    double screenTop = trueScreen.get_ycenter() + trueScreen.get_height() / 2;
    double compassX = screenLeft + compassOffset;
    double compassY = screenTop - compassOffset;
    
    // "N" text border
    double textNX[10];
    double textNY[10];
    t_point textN[10];
    double compassIntermediate = pow(pow(compassNOutLarge,2)+pow(compassNInLarge,2),0.5);
    double compassLarge = pow(pow(compassNOutLarge,2)+pow(compassNOutLarge,2),0.5);
    double compassSmall = pow(pow(compassNInLarge,2)+pow(compassNInLarge,2),0.5);
    double compassAngleSmall = atan2(compassNInLarge,compassNOutLarge);
    double compassAngleLarge = atan2(compassNOutLarge,compassNInLarge);
    double NX = compassX;// - trueScreen.get_width()*(compassNDisplace) * sin(Cord::totalRotation);
    double NY = compassY;// + trueScreen.get_width()*(compassNDisplace) * cos(Cord::totalRotation);
    textNX[0] = NX - trueScreen.get_width()*compassLarge * cos(Cord::totalRotation+PI/4); // DONE
    textNX[1] = NX - trueScreen.get_width()*compassLarge * cos(Cord::totalRotation-PI/4); // DONE
    textNX[2] = NX - trueScreen.get_width()*compassIntermediate * cos(Cord::totalRotation-compassAngleLarge);
    textNX[3] = NX + trueScreen.get_width()*compassSmall * cos(Cord::totalRotation-PI/4); // DONE
    textNX[4] = NX + trueScreen.get_width()*compassIntermediate * cos(Cord::totalRotation+compassAngleLarge);
    textNX[5] = NX + trueScreen.get_width()*compassLarge * cos(Cord::totalRotation+PI/4); // DONE
    textNX[6] = NX + trueScreen.get_width()*compassLarge * cos(Cord::totalRotation-PI/4); // DONE
    textNX[7] = NX + trueScreen.get_width()*compassIntermediate * cos(Cord::totalRotation-compassAngleLarge);
    textNX[8] = NX - trueScreen.get_width()*compassSmall * cos(Cord::totalRotation-PI/4); // DONE
    textNX[9] = NX - trueScreen.get_width()*compassIntermediate * cos(Cord::totalRotation+compassAngleLarge);
    textNY[0] = NY - trueScreen.get_width()*compassLarge * cos(Cord::totalRotation-PI/4); // DONE
    textNY[1] = NY + trueScreen.get_width()*compassLarge * cos(Cord::totalRotation+PI/4); // DONE
    textNY[2] = NY + trueScreen.get_width()*compassIntermediate * cos(Cord::totalRotation+compassAngleSmall);
    textNY[3] = NY - trueScreen.get_width()*compassSmall * cos(Cord::totalRotation+PI/4); // DONE
    textNY[4] = NY + trueScreen.get_width()*compassIntermediate * cos(Cord::totalRotation-compassAngleSmall);
    textNY[5] = NY + trueScreen.get_width()*compassLarge * cos(Cord::totalRotation-PI/4); // DONE
    textNY[6] = NY - trueScreen.get_width()*compassLarge * cos(Cord::totalRotation+PI/4); // DONE
    textNY[7] = NY - trueScreen.get_width()*compassIntermediate * cos(Cord::totalRotation+compassAngleSmall);
    textNY[8] = NY + trueScreen.get_width()*compassSmall * cos(Cord::totalRotation+PI/4); // DONE
    textNY[9] = NY - trueScreen.get_width()*compassIntermediate * cos(Cord::totalRotation-compassAngleSmall);
    textN[0] = t_point(textNX[0], textNY[0]);
    textN[1] = t_point(textNX[1], textNY[1]);
    textN[2] = t_point(textNX[2], textNY[2]);
    textN[3] = t_point(textNX[3], textNY[3]);
    textN[4] = t_point(textNX[4], textNY[4]);
    textN[5] = t_point(textNX[5], textNY[5]);
    textN[6] = t_point(textNX[6], textNY[6]);
    textN[7] = t_point(textNX[7], textNY[7]);
    textN[8] = t_point(textNX[8], textNY[8]);
    textN[9] = t_point(textNX[9], textNY[9]);
    
    // Compass border and circle
    setcolor(WHITE);
    fillarc(compassX, compassY, compassCircle*trueScreen.get_width(), 0, 360);
    setlinestyle(SOLID);
    setlinewidth(3);
    setcolor(BLACK);
    drawarc(compassX, compassY, compassCircle*trueScreen.get_width(), 0, 360);
    setlinewidth(1);
    setcolor(colorScheme[THEMEC][colorMode]);
    drawarc(compassX, compassY, compassCircle*trueScreen.get_width(), 0, 360);
    
    // North arrow
    setcolor(BLACK);
    double northTriangleX[3];
    double northTriangleY[3];
    t_point compassTriangle[3];
    northTriangleX[0] = compassX - trueScreen.get_width()*compassArrowWidth * cos(Cord::totalRotation);
    northTriangleX[1] = compassX + trueScreen.get_width()*compassArrowWidth * cos(Cord::totalRotation);
    northTriangleX[2] = compassX - trueScreen.get_width()*(compassSizeRatio) * sin(Cord::totalRotation);
    northTriangleY[0] = compassY - trueScreen.get_width()*compassArrowWidth * sin(Cord::totalRotation);
    northTriangleY[1] = compassY + trueScreen.get_width()*compassArrowWidth * sin(Cord::totalRotation);
    northTriangleY[2] = compassY + trueScreen.get_width()*(compassSizeRatio) * cos(Cord::totalRotation);
    compassTriangle[0] = t_point(northTriangleX[0], northTriangleY[0]);
    compassTriangle[1] = t_point(northTriangleX[1], northTriangleY[1]);
    compassTriangle[2] = t_point(northTriangleX[2], northTriangleY[2]);
    // Compass arrow
    fillpoly(compassTriangle, 3);
    
    // Masking the arrow
    setcolor(WHITE);
    fillarc(compassX + trueScreen.get_width()*(compassMaskDisplace) * sin(Cord::totalRotation),
            compassY - trueScreen.get_width()*(compassMaskDisplace) * cos(Cord::totalRotation),
            (compassMask)*trueScreen.get_width(), 0, 360);
    
    // "N"
    setcolor(DARKGREY);
    fillpoly(textN, 10);
}

// Draws a scale on the top right of the screen
void drawScale()
{
    double scaleOffset = trueScreen.get_width() * scaleOffsetRatio;
    double scaleOffsetSmall = trueScreen.get_width() * scaleOffsetRatioSmall;
    double scaleLength = trueScreen.get_width() * scaleLengthRatio;
    double scaleX = trueScreen.right() - scaleLength - scaleOffsetSmall;
    double scaleY = trueScreen.top() - scaleOffset;
    double zoomUnit;
    double scaleMax;
    string scaleUnit;
    // This will make sure the scale is nice and rounded if it's above 100
    // while still keeping the distance measured accurate
    if(scaleLength > 100) scaleLength = 10*floor(scaleLength/10);
    // Draw the scale polygons here
    t_point scalePolygon[4];
    scalePolygon[0] = t_point(scaleX-trueScreen.get_width()*scaleBoxWidthRatio, scaleY-trueScreen.get_width()*scaleBoxLengthRatio);
    scalePolygon[1] = t_point(scaleX-trueScreen.get_width()*scaleBoxWidthRatio, scaleY+trueScreen.get_width()*scaleBoxLengthRatio);
    scalePolygon[2] = t_point(scaleX+trueScreen.get_width()*scaleBoxWidthRatio, scaleY+trueScreen.get_width()*scaleBoxLengthRatio);
    scalePolygon[3] = t_point(scaleX+trueScreen.get_width()*scaleBoxWidthRatio, scaleY-trueScreen.get_width()*scaleBoxLengthRatio);
    setcolor(BLACK);
    fillpoly(scalePolygon, 4);
    scalePolygon[0] = t_point(scaleX+scaleLength-trueScreen.get_width()*scaleBoxWidthRatio, scaleY-trueScreen.get_width()*scaleBoxLengthRatio);
    scalePolygon[1] = t_point(scaleX+scaleLength-trueScreen.get_width()*scaleBoxWidthRatio, scaleY+trueScreen.get_width()*scaleBoxLengthRatio);
    scalePolygon[2] = t_point(scaleX+scaleLength+trueScreen.get_width()*scaleBoxWidthRatio, scaleY+trueScreen.get_width()*scaleBoxLengthRatio);
    scalePolygon[3] = t_point(scaleX+scaleLength+trueScreen.get_width()*scaleBoxWidthRatio, scaleY-trueScreen.get_width()*scaleBoxLengthRatio);
    setcolor(BLACK);
    fillpoly(scalePolygon, 4);
    scalePolygon[0] = t_point(scaleX, scaleY-trueScreen.get_width()*scaleBoxWidthRatio);
    scalePolygon[1] = t_point(scaleX, scaleY+trueScreen.get_width()*scaleBoxWidthRatio);
    scalePolygon[2] = t_point(scaleX+scaleLength, scaleY+trueScreen.get_width()*scaleBoxWidthRatio);
    scalePolygon[3] = t_point(scaleX+scaleLength, scaleY-trueScreen.get_width()*scaleBoxWidthRatio);
    setcolor(BLACK);
    fillpoly(scalePolygon, 4);
    scalePolygon[0] = t_point(scaleX-trueScreen.get_width()*(scaleBoxWidthRatio-scaleBorderRatio), scaleY-trueScreen.get_width()*(scaleBoxLengthRatio-scaleBorderRatio));
    scalePolygon[1] = t_point(scaleX-trueScreen.get_width()*(scaleBoxWidthRatio-scaleBorderRatio), scaleY+trueScreen.get_width()*(scaleBoxLengthRatio-scaleBorderRatio));
    scalePolygon[2] = t_point(scaleX+trueScreen.get_width()*(scaleBoxWidthRatio-scaleBorderRatio), scaleY+trueScreen.get_width()*(scaleBoxLengthRatio-scaleBorderRatio));
    scalePolygon[3] = t_point(scaleX+trueScreen.get_width()*(scaleBoxWidthRatio-scaleBorderRatio), scaleY-trueScreen.get_width()*(scaleBoxLengthRatio-scaleBorderRatio));
    setcolor(WHITE);
    fillpoly(scalePolygon, 4);
    scalePolygon[0] = t_point(scaleX+scaleLength-trueScreen.get_width()*(scaleBoxWidthRatio-scaleBorderRatio), scaleY-trueScreen.get_width()*(scaleBoxLengthRatio-scaleBorderRatio));
    scalePolygon[1] = t_point(scaleX+scaleLength-trueScreen.get_width()*(scaleBoxWidthRatio-scaleBorderRatio), scaleY+trueScreen.get_width()*(scaleBoxLengthRatio-scaleBorderRatio));
    scalePolygon[2] = t_point(scaleX+scaleLength+trueScreen.get_width()*(scaleBoxWidthRatio-scaleBorderRatio), scaleY+trueScreen.get_width()*(scaleBoxLengthRatio-scaleBorderRatio));
    scalePolygon[3] = t_point(scaleX+scaleLength+trueScreen.get_width()*(scaleBoxWidthRatio-scaleBorderRatio), scaleY-trueScreen.get_width()*(scaleBoxLengthRatio-scaleBorderRatio));
    setcolor(WHITE);
    fillpoly(scalePolygon, 4);
    scalePolygon[0] = t_point(scaleX, scaleY-trueScreen.get_width()*(scaleBoxWidthRatio-scaleBorderRatio));
    scalePolygon[1] = t_point(scaleX, scaleY+trueScreen.get_width()*(scaleBoxWidthRatio-scaleBorderRatio));
    scalePolygon[2] = t_point(scaleX+scaleLength, scaleY+trueScreen.get_width()*(scaleBoxWidthRatio-scaleBorderRatio));
    scalePolygon[3] = t_point(scaleX+scaleLength, scaleY-trueScreen.get_width()*(scaleBoxWidthRatio-scaleBorderRatio));
    setcolor(WHITE);
    fillpoly(scalePolygon, 4);
    // Little conversion from km to m when user zooms in
    if(scaleLength >= scaleMetersThreshold)
    {
        zoomUnit = KM_TO_M;
        scaleUnit = " km";
    } else {
        zoomUnit = 1;
        scaleUnit = " m";
    }
    scaleMax = scaleLength*zoomUnit;
    settextrotation(0);
    // We're setting black on white text for easier reading, might change
    // this to something more solid later
    setfontsize(12);
    setcolor(WHITE);
    drawtext(scaleX, scaleY-trueScreen.get_width()*(scaleBoxLengthRatio+scaleTextOffsetRatio), "0", textMaxX, textMaxY);
    drawtext(scaleX+scaleLength, scaleY-trueScreen.get_width()*(scaleBoxLengthRatio+scaleTextOffsetRatio), to_string((int)floor(scaleMax))+scaleUnit, textMaxX, textMaxY);
    setfontsize(11);
    setcolor(BLACK);
    drawtext(scaleX, scaleY-trueScreen.get_width()*(scaleBoxLengthRatio+scaleTextOffsetRatio), "0", textMaxX, textMaxY);
    drawtext(scaleX+scaleLength, scaleY-trueScreen.get_width()*(scaleBoxLengthRatio+scaleTextOffsetRatio), to_string((int)floor(scaleMax))+scaleUnit, textMaxX, textMaxY);
}

// Draws a useful minimap on bottom right of the screen
// you can click on it as well
void drawMiniMap()
{
    setlinestyle(SOLID);
    // Minimap background
    t_point miniBackground[4];
    setcolor(t_color(0xDF, 0xDF, 0xDF));
    miniBackground[0] = t_point(trueScreen.get_xcenter()+trueScreen.get_width()/2-miniOffsetRatio*trueScreen.get_width()+miniSizeRatio*trueScreen.get_width()/2,
            trueScreen.get_ycenter()-trueScreen.get_height()/2+miniOffsetRatio*trueScreen.get_width()-miniSizeRatio*trueScreen.get_width()/2);
    miniBackground[1] = t_point(trueScreen.get_xcenter()+trueScreen.get_width()/2-miniOffsetRatio*trueScreen.get_width()-miniSizeRatio*trueScreen.get_width()/2,
            trueScreen.get_ycenter()-trueScreen.get_height()/2+miniOffsetRatio*trueScreen.get_width()-miniSizeRatio*trueScreen.get_width()/2);
    miniBackground[2] = t_point(trueScreen.get_xcenter()+trueScreen.get_width()/2-miniOffsetRatio*trueScreen.get_width()-miniSizeRatio*trueScreen.get_width()/2,
            trueScreen.get_ycenter()-trueScreen.get_height()/2+miniOffsetRatio*trueScreen.get_width()-miniSizeRatio*trueScreen.get_width()/2+miniSizeRatio*trueScreen.get_width()*mapRatio);
    miniBackground[3] = t_point(trueScreen.get_xcenter()+trueScreen.get_width()/2-miniOffsetRatio*trueScreen.get_width()+miniSizeRatio*trueScreen.get_width()/2,
            trueScreen.get_ycenter()-trueScreen.get_height()/2+miniOffsetRatio*trueScreen.get_width()-miniSizeRatio*trueScreen.get_width()/2+miniSizeRatio*trueScreen.get_width()*mapRatio);
    fillpoly(miniBackground,4);
    
    setlinewidth(1);
    // Minimap features
    for(unsigned i = 0; i < allLargeFeatures.size(); i++)
    {
        drawFeature(i,1);
    }
    
    // Minimap streets
    setcolor(colorScheme[HIGHWAYC][colorMode]);
    for (unsigned i = 0; i < allLargeStreetSegs.size(); i++)
    {
        // Segments
        drawStreetSegLine(i);
    }
    
    // Minimap current screen
    t_point topLeft = trueScreen.top_right();
    t_point topRight = trueScreen.top_right();
    t_point botLeft = trueScreen.bottom_left();
    t_point botRight = trueScreen.bottom_left();
    topLeft.x = topLeft.x - trueScreen.get_width();
    botRight.x = botRight.x + trueScreen.get_width();
    
    pair<double,double> tL = Cord::newToOld(topLeft.x,topLeft.y);
    pair<double,double> tR = Cord::newToOld(topRight.x,topRight.y);
    pair<double,double> bL = Cord::newToOld(botLeft.x,botLeft.y);
    pair<double,double> bR = Cord::newToOld(botRight.x,botRight.y);
    
    topLeft = t_point(tL.first,tL.second);
    topRight = t_point(tR.first,tR.second);
    botLeft = t_point(bL.first,bL.second);
    botRight = t_point(bR.first,bR.second);

    topLeft.x = topLeft.x*miniMultiplierX + miniX;
    topLeft.y = topLeft.y*miniMultiplierY + miniY;
    topRight.x = topRight.x*miniMultiplierX + miniX;
    topRight.y = topRight.y*miniMultiplierY + miniY;
    botLeft.x = botLeft.x*miniMultiplierX + miniX;
    botLeft.y = botLeft.y*miniMultiplierY + miniY;
    botRight.x = botRight.x*miniMultiplierX + miniX;
    botRight.y = botRight.y*miniMultiplierY + miniY;

    // Now actually draw it
    setcolor(WHITE);
    setlinewidth(2);
    drawline(topLeft,topRight);
    drawline(botLeft,botRight);
    drawline(botLeft,topLeft);
    drawline(botRight,topRight);
    
    // Minimap borders
    setcolor(BLACK);
    setlinewidth(3);
    drawline(miniBackground[0],miniBackground[1]);
    drawline(miniBackground[1],miniBackground[2]);
    drawline(miniBackground[2],miniBackground[3]);
    drawline(miniBackground[3],miniBackground[0]);
    setcolor(colorScheme[THEMEC][colorMode]);
    setlinewidth(1);
    drawline(miniBackground[0],miniBackground[1]);
    drawline(miniBackground[1],miniBackground[2]);
    drawline(miniBackground[2],miniBackground[3]);
    drawline(miniBackground[3],miniBackground[0]);
}

// Draws a timed message box on top of the screen when called
void drawMessageBox()
{
    // Checks if messageTime has recently been set to draw a message
    time_t currentTime = time(NULL);
    if(currentTime-messageTime < textStayTime)
    {
        t_point messageBox[4];
        messageBox[0] = t_point(trueScreen.get_xcenter()-messageBoxWidth*trueScreen.get_width()/2,
                                trueScreen.top()-messageBoxDisplace*trueScreen.get_width());
        messageBox[1] = t_point(trueScreen.get_xcenter()-messageBoxWidth*trueScreen.get_width()/2,
                                trueScreen.top()-messageBoxDisplace*trueScreen.get_width()
                                +messageBoxHeight*trueScreen.get_width());
        messageBox[2] = t_point(trueScreen.get_xcenter()+messageBoxWidth*trueScreen.get_width()/2,
                                trueScreen.top()-messageBoxDisplace*trueScreen.get_width()
                                +messageBoxHeight*trueScreen.get_width());
        messageBox[3] = t_point(trueScreen.get_xcenter()+messageBoxWidth*trueScreen.get_width()/2,
                                trueScreen.top()-messageBoxDisplace*trueScreen.get_width());
        setcolor(WHITE);
        fillpoly(messageBox,4);
        setlinestyle(SOLID);
        setlinewidth(3);
        setcolor(BLACK);
        drawline(messageBox[0],messageBox[1]);
        drawline(messageBox[2],messageBox[1]);
        drawline(messageBox[2],messageBox[3]);
        drawline(messageBox[0],messageBox[3]);
        setlinewidth(1);
        setcolor(colorScheme[THEMEC][colorMode]);
        drawline(messageBox[0],messageBox[1]);
        drawline(messageBox[2],messageBox[1]);
        drawline(messageBox[2],messageBox[3]);
        drawline(messageBox[0],messageBox[3]);
        t_point messageTextCenter = t_point(trueScreen.get_xcenter(),trueScreen.top()
                                            -messageBoxDisplace*trueScreen.get_width()
                                            +messageBoxHeight*trueScreen.get_width()/2);
        setcolor(BLACK);
        // Could draw one or two lines of text, but max is two
        if(messageText.size() <= 1)
        {
            drawtext(messageTextCenter, messageText[0], textMaxX, textMaxY);
        } else {
            drawtext(messageTextCenter.x,messageTextCenter.y+trueScreen.get_width()*messageTwoLineDisplace/2, messageText[0], textMaxX, textMaxY);
            drawtext(messageTextCenter.x,messageTextCenter.y-trueScreen.get_width()*messageTwoLineDisplace/2, messageText[1], textMaxX, textMaxY);
        }
        
    }
}

// Draws the previous searches box
void drawPreviousSearches(char open, char blue)
{
    t_point previousButton[4];
    t_point previousArrow[3];
    t_point previousBox[4];
    t_point previousBlue[4];
    if(!open)
    { // If box is closed
        previousButton[0] = t_point(trueScreen.get_xcenter()-trueScreen.get_width()*previousButtonWidth,
                                    trueScreen.bottom()-trueScreen.get_width()*previousButtonHeight);
        previousButton[1] = t_point(trueScreen.get_xcenter()-trueScreen.get_width()*previousButtonWidth,
                                    trueScreen.bottom()+trueScreen.get_width()*previousButtonHeight);
        previousButton[2] = t_point(trueScreen.get_xcenter()+trueScreen.get_width()*previousButtonWidth,
                                    trueScreen.bottom()+trueScreen.get_width()*previousButtonHeight);
        previousButton[3] = t_point(trueScreen.get_xcenter()+trueScreen.get_width()*previousButtonWidth,
                                    trueScreen.bottom()-trueScreen.get_width()*previousButtonHeight);
        previousArrow[0] = t_point(trueScreen.get_xcenter(),
                                   trueScreen.bottom()+trueScreen.get_width()*previousButtonHeight/2+trueScreen.get_width()*previousArrowHeight);
        previousArrow[1] = t_point(trueScreen.get_xcenter()+trueScreen.get_width()*previousArrowWidth,
                                   trueScreen.bottom()+trueScreen.get_width()*previousButtonHeight/2-trueScreen.get_width()*previousArrowHeight);
        previousArrow[2] = t_point(trueScreen.get_xcenter()-trueScreen.get_width()*previousArrowWidth,
                                   trueScreen.bottom()+trueScreen.get_width()*previousButtonHeight/2-trueScreen.get_width()*previousArrowHeight);
        if(currentHover == CLOSEDPREVH)
        {
            setcolor(WHITE);
        } else {
            setcolor(LIGHTGREY);
        }
        fillpoly(previousButton,4);
        setlinestyle(SOLID);
        setlinewidth(3);
        setcolor(BLACK);
        drawline(previousButton[0].x,previousButton[0].y,previousButton[1].x,previousButton[1].y);
        drawline(previousButton[2].x,previousButton[2].y,previousButton[1].x,previousButton[1].y);
        drawline(previousButton[2].x,previousButton[2].y,previousButton[3].x,previousButton[3].y);
        setlinewidth(1);
        setcolor(colorScheme[THEMEC][colorMode]);
        drawline(previousButton[0].x,previousButton[0].y,previousButton[1].x,previousButton[1].y);
        drawline(previousButton[2].x,previousButton[2].y,previousButton[1].x,previousButton[1].y);
        drawline(previousButton[2].x,previousButton[2].y,previousButton[3].x,previousButton[3].y);
        fillpoly(previousArrow,3);
    } else { // if box is open
        previousButton[0] = t_point(trueScreen.get_xcenter()-trueScreen.get_width()*previousButtonWidth,
                                    trueScreen.bottom()+previousElements*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder*2);
        previousButton[1] = t_point(trueScreen.get_xcenter()-trueScreen.get_width()*previousButtonWidth,
                                    trueScreen.bottom()+trueScreen.get_width()*previousButtonHeight+previousElements*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder*2);
        previousButton[2] = t_point(trueScreen.get_xcenter()+trueScreen.get_width()*previousButtonWidth,
                                    trueScreen.bottom()+trueScreen.get_width()*previousButtonHeight+previousElements*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder*2);
        previousButton[3] = t_point(trueScreen.get_xcenter()+trueScreen.get_width()*previousButtonWidth,
                                    trueScreen.bottom()+previousElements*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder*2);
        previousArrow[0] = t_point(trueScreen.get_xcenter(),
                                   trueScreen.bottom()+trueScreen.get_width()*previousButtonHeight/2-trueScreen.get_width()*previousArrowHeight+previousElements*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder*2);
        previousArrow[1] = t_point(trueScreen.get_xcenter()+trueScreen.get_width()*previousArrowWidth,
                                   trueScreen.bottom()+trueScreen.get_width()*previousButtonHeight/2+trueScreen.get_width()*previousArrowHeight+previousElements*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder*2);
        previousArrow[2] = t_point(trueScreen.get_xcenter()-trueScreen.get_width()*previousArrowWidth,
                                   trueScreen.bottom()+trueScreen.get_width()*previousButtonHeight/2+trueScreen.get_width()*previousArrowHeight+previousElements*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder*2);
        previousBox[0] = t_point(trueScreen.get_xcenter()-trueScreen.get_width()*previousBoxWidth,
                                    trueScreen.bottom());
        previousBox[1] = t_point(trueScreen.get_xcenter()-trueScreen.get_width()*previousBoxWidth,
                                    trueScreen.bottom()+previousElements*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder*2);
        previousBox[2] = t_point(trueScreen.get_xcenter()+trueScreen.get_width()*previousBoxWidth,
                                    trueScreen.bottom()+previousElements*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder*2);
        previousBox[3] = t_point(trueScreen.get_xcenter()+trueScreen.get_width()*previousBoxWidth,
                                    trueScreen.bottom());
        if(currentHover == OPENPREVH)
        { // This is stuff for hovering, it's white if mouse is over it
            setcolor(WHITE);
        } else {
            setcolor(LIGHTGREY);
        }
        fillpoly(previousButton,4);
        setcolor(WHITE);
        fillpoly(previousBox,4);
        if(blue != 0)
        { // If mouse is over a selection, draw the background of it blue
            previousBlue[0] = t_point(previousBox[0].x,
                                      previousBox[0].y+((double)blue-1)*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder);
            previousBlue[1] = t_point(previousBlue[0].x,
                                      previousBlue[0].y+trueScreen.get_width()*messageTwoLineDisplace);
            previousBlue[2] = t_point(previousBox[2].x,
                                      previousBlue[1].y);
            previousBlue[3] = t_point(previousBox[3].x,
                                      previousBlue[0].y);
            setcolor(colorScheme[LIGHTBLUEC][colorMode]);
            fillpoly(previousBlue,4);
        }
        setlinestyle(SOLID);
        setlinewidth(3);
        setcolor(BLACK);
        drawline(previousButton[0],previousButton[1]);
        drawline(previousButton[2],previousButton[1]);
        drawline(previousButton[2],previousButton[3]);
        drawline(previousBox[0],previousBox[1]);
        drawline(previousBox[2],previousBox[1]);
        drawline(previousBox[2],previousBox[3]);
        drawline(previousBox[0],previousBox[3]);
        setlinewidth(1);
        setcolor(colorScheme[THEMEC][colorMode]);
        drawline(previousButton[0],previousButton[1]);
        drawline(previousButton[2],previousButton[1]);
        drawline(previousButton[2],previousButton[3]);
        drawline(previousBox[0],previousBox[1]);
        drawline(previousBox[2],previousBox[1]);
        drawline(previousBox[2],previousBox[3]);
        drawline(previousBox[0],previousBox[3]);
        fillpoly(previousArrow,3);
        setcolor(BLACK);
        for(int64_t i = 0; i < previousElements; i++)
        {
            if((int64_t)previousSearches.size()-i > 0)
            { // Left align this text using my own function
                LEFTdrawtext(trueScreen.get_xcenter()-trueScreen.get_width()*(previousBoxWidth-generalTextOffset),
                         trueScreen.bottom()+((double)i+0.5)*trueScreen.get_width()*messageTwoLineDisplace+trueScreen.get_width()*messageTwoLineBorder,
                         previousSearches[previousSearches.size()-i-1],textMaxX,textMaxY);
            }
        }
    }
}

// Draws the general dialog box for help, directions, etc.
void drawGeneralDialog(char open)
{
    t_point generalButton[4];
    t_point generalArrow[3];
    t_point generalBox[4];
    t_point textCenter;
    t_point symbolCenter;
    if(!open)
    { // If closed
        generalButton[0] = t_point(trueScreen.left(),
                                   trueScreen.get_ycenter()-trueScreen.get_width()*generalButtonSize/2);
        generalButton[1] = t_point(trueScreen.left(),
                                   trueScreen.get_ycenter()+trueScreen.get_width()*generalButtonSize/2);
        generalButton[2] = t_point(trueScreen.left()+trueScreen.get_width()*generalButtonSize,
                                   trueScreen.get_ycenter()+trueScreen.get_width()*generalButtonSize/2);
        generalButton[3] = t_point(trueScreen.left()+trueScreen.get_width()*generalButtonSize,
                                   trueScreen.get_ycenter()-trueScreen.get_width()*generalButtonSize/2);
        generalArrow[0] = t_point(trueScreen.left()+trueScreen.get_width()*generalButtonSize/2-trueScreen.get_width()*generalArrowSize/2,
                                  trueScreen.get_ycenter()-trueScreen.get_width()*generalArrowSize/2);
        generalArrow[1] = t_point(trueScreen.left()+trueScreen.get_width()*generalButtonSize/2-trueScreen.get_width()*generalArrowSize/2,
                                  trueScreen.get_ycenter()+trueScreen.get_width()*generalArrowSize/2);
        generalArrow[2] = t_point(trueScreen.left()+trueScreen.get_width()*generalButtonSize/2+trueScreen.get_width()*generalArrowSize/2,
                                  trueScreen.get_ycenter());
        if(currentHover == CLOSEDGENH)
        {
            setcolor(WHITE);
        } else {
            setcolor(LIGHTGREY);
        }
        fillpoly(generalButton,4);
        setlinestyle(SOLID);
        setlinewidth(3);
        setcolor(BLACK);
        drawline(generalButton[0].x,generalButton[0].y,generalButton[1].x,generalButton[1].y);
        drawline(generalButton[2].x,generalButton[2].y,generalButton[1].x,generalButton[1].y);
        drawline(generalButton[2].x,generalButton[2].y,generalButton[3].x,generalButton[3].y);
        drawline(generalButton[0].x,generalButton[0].y,generalButton[3].x,generalButton[3].y);
        setlinewidth(1);
        setcolor(colorScheme[THEMEC][colorMode]);
        drawline(generalButton[0].x,generalButton[0].y,generalButton[1].x,generalButton[1].y);
        drawline(generalButton[2].x,generalButton[2].y,generalButton[1].x,generalButton[1].y);
        drawline(generalButton[2].x,generalButton[2].y,generalButton[3].x,generalButton[3].y);
        drawline(generalButton[0].x,generalButton[0].y,generalButton[3].x,generalButton[3].y);
        fillpoly(generalArrow,3);
    } else { // if open
        generalBox[0] = trueScreen.bottom_left();
        generalBox[1] = t_point(trueScreen.left(),
                                trueScreen.get_ycenter()+trueScreen.get_width()*generalButtonSize/2);
        generalBox[2] = t_point(trueScreen.left()+trueScreen.get_width()*generalBoxSize,
                                trueScreen.get_ycenter()+trueScreen.get_width()*generalButtonSize/2);
        generalBox[3] = t_point(trueScreen.left()+trueScreen.get_width()*generalBoxSize,
                                trueScreen.bottom());
        textCenter = t_point(trueScreen.left()+trueScreen.get_width()*(generalBoxSize-generalButtonSize)/2,
                             trueScreen.get_ycenter()+trueScreen.get_width()*generalButtonSize/2-trueScreen.get_width()*messageTwoLineBorder-trueScreen.get_width()*messageTwoLineDisplace/2);
        setcolor(WHITE);
        fillpoly(generalBox,4);
        setlinestyle(SOLID);
        setlinewidth(3);
        setcolor(BLACK);
        drawline(generalBox[0].x,generalBox[0].y,generalBox[1].x,generalBox[1].y);
        drawline(generalBox[2].x,generalBox[2].y,generalBox[1].x,generalBox[1].y);
        drawline(generalBox[2].x,generalBox[2].y,generalBox[3].x,generalBox[3].y);
        drawline(generalBox[0].x,generalBox[0].y,generalBox[3].x,generalBox[3].y);
        setlinewidth(1);
        setcolor(colorScheme[THEMEC][colorMode]);
        drawline(generalBox[0].x,generalBox[0].y,generalBox[1].x,generalBox[1].y);
        drawline(generalBox[2].x,generalBox[2].y,generalBox[1].x,generalBox[1].y);
        drawline(generalBox[2].x,generalBox[2].y,generalBox[3].x,generalBox[3].y);
        drawline(generalBox[0].x,generalBox[0].y,generalBox[3].x,generalBox[3].y);
        setcolor(BLACK);
        for(unsigned i = 0; (unsigned)(i+generalTextStart) < generalText.size(); i++)
        { // Assigning values to get ready to draw the actual text
            if(textCenter.y < trueScreen.bottom()) break;
            if(generalText[i+generalTextStart][0] == 91)
            { // If the [XX] style symbols are detected, draw a custom thing
                symbolCenter = t_point(trueScreen.left()+trueScreen.get_width()*generalSymbolDisplace,
                                       textCenter.y);
                setcolor(colorScheme[THEMEC][colorMode]);
                if(generalText[i+generalTextStart].substr(1,2) == "LF")
                { // LEFT
                    t_point leftSymbol[9];
                    leftSymbol[0] = t_point(symbolCenter.x+trueScreen.get_width()*(generalSymbolLength/2-generalSymbolWidth),
                                            symbolCenter.y-trueScreen.get_width()*generalSymbolLength/2);
                    leftSymbol[1] = t_point(leftSymbol[0].x+trueScreen.get_width()*generalSymbolWidth,
                                            leftSymbol[0].y);
                    leftSymbol[2] = t_point(leftSymbol[1].x,
                                            symbolCenter.y+trueScreen.get_width()*generalSymbolLength/2);
                    leftSymbol[3] = t_point(symbolCenter.x-trueScreen.get_width()*(generalSymbolLength/2-generalSymbolArrowSize/2),
                                            leftSymbol[2].y);
                    leftSymbol[4] = t_point(leftSymbol[3].x,
                                            leftSymbol[3].y+trueScreen.get_width()*(generalSymbolArrowSize/2-generalSymbolWidth/2));
                    leftSymbol[5] = t_point(leftSymbol[4].x-trueScreen.get_width()*generalSymbolArrowSize,
                                            leftSymbol[2].y-trueScreen.get_width()*generalSymbolWidth/2);
                    leftSymbol[6] = t_point(leftSymbol[3].x,
                                            leftSymbol[4].y-trueScreen.get_width()*generalSymbolArrowSize);
                    leftSymbol[7] = t_point(leftSymbol[3].x,
                                            leftSymbol[3].y-trueScreen.get_width()*generalSymbolWidth);
                    leftSymbol[8] = t_point(leftSymbol[0].x,
                                            leftSymbol[7].y);
                    fillpoly(leftSymbol,9);
                } else if(generalText[i+generalTextStart].substr(1,2) == "RT")
                { // RIGHT
                    t_point rightSymbol[9];
                    rightSymbol[0] = t_point(symbolCenter.x-trueScreen.get_width()*(generalSymbolLength/2-generalSymbolWidth),
                                            symbolCenter.y-trueScreen.get_width()*generalSymbolLength/2);
                    rightSymbol[1] = t_point(rightSymbol[0].x-trueScreen.get_width()*generalSymbolWidth,
                                            rightSymbol[0].y);
                    rightSymbol[2] = t_point(rightSymbol[1].x,
                                            symbolCenter.y+trueScreen.get_width()*generalSymbolLength/2);
                    rightSymbol[3] = t_point(symbolCenter.x+trueScreen.get_width()*(generalSymbolLength/2-generalSymbolArrowSize/2),
                                            rightSymbol[2].y);
                    rightSymbol[4] = t_point(rightSymbol[3].x,
                                            rightSymbol[3].y+trueScreen.get_width()*(generalSymbolArrowSize/2-generalSymbolWidth/2));
                    rightSymbol[5] = t_point(rightSymbol[4].x+trueScreen.get_width()*generalSymbolArrowSize,
                                            rightSymbol[2].y-trueScreen.get_width()*generalSymbolWidth/2);
                    rightSymbol[6] = t_point(rightSymbol[3].x,
                                            rightSymbol[4].y-trueScreen.get_width()*generalSymbolArrowSize);
                    rightSymbol[7] = t_point(rightSymbol[3].x,
                                            rightSymbol[3].y-trueScreen.get_width()*generalSymbolWidth);
                    rightSymbol[8] = t_point(rightSymbol[0].x,
                                            rightSymbol[7].y);
                    fillpoly(rightSymbol,9);
                } else if(generalText[i+generalTextStart].substr(1,2) == "ST")
                { // STRAIGHT
                    t_point straightSymbol[7];
                    straightSymbol[0] = t_point(symbolCenter.x+trueScreen.get_width()*generalSymbolWidth/2,
                                                symbolCenter.y-trueScreen.get_width()*generalSymbolLength/2);
                    straightSymbol[1] = t_point(straightSymbol[0].x-trueScreen.get_width()*generalSymbolWidth,
                                                straightSymbol[0].y);
                    straightSymbol[2] = t_point(straightSymbol[1].x,
                                                straightSymbol[0].y+trueScreen.get_width()*(generalSymbolLength-generalSymbolArrowSize/2));
                    straightSymbol[3] = t_point(symbolCenter.x-trueScreen.get_width()*generalSymbolArrowSize/2,
                                                straightSymbol[2].y);
                    straightSymbol[4] = t_point(symbolCenter.x,
                                                straightSymbol[2].y+trueScreen.get_width()*generalSymbolArrowSize);
                    straightSymbol[5] = t_point(symbolCenter.x+trueScreen.get_width()*generalSymbolArrowSize/2,
                                                straightSymbol[2].y);
                    straightSymbol[6] = t_point(straightSymbol[0].x,
                                                straightSymbol[2].y);
                    fillpoly(straightSymbol,7);
                } else if(generalText[i+generalTextStart].substr(1,2) == "MG")
                { // MERGE
                    t_point mergeSymbol[11];
                    mergeSymbol[0] = t_point(symbolCenter.x+trueScreen.get_width()*generalSymbolWidth/2,
                                             symbolCenter.y-trueScreen.get_width()*generalSymbolLength/2);
                    mergeSymbol[1] = t_point(mergeSymbol[0].x-trueScreen.get_width()*generalSymbolWidth,
                                             mergeSymbol[0].y);
                    mergeSymbol[2] = t_point(mergeSymbol[1].x,
                                             mergeSymbol[0].y+trueScreen.get_width()*(generalSymbolLength-generalSymbolArrowSize/2));
                    mergeSymbol[3] = t_point(symbolCenter.x-trueScreen.get_width()*generalSymbolArrowSize/2,
                                             mergeSymbol[2].y);
                    mergeSymbol[4] = t_point(symbolCenter.x,
                                             mergeSymbol[2].y+trueScreen.get_width()*generalSymbolArrowSize);
                    mergeSymbol[5] = t_point(symbolCenter.x+trueScreen.get_width()*generalSymbolArrowSize/2,
                                             mergeSymbol[2].y);
                    mergeSymbol[6] = t_point(mergeSymbol[0].x,
                                             mergeSymbol[2].y);
                    mergeSymbol[7] = t_point(mergeSymbol[0].x,
                                             symbolCenter.y+trueScreen.get_width()*generalSymbolWidth/2);
                    mergeSymbol[8] = t_point(mergeSymbol[1].x+trueScreen.get_width()*generalSymbolLength,
                                             mergeSymbol[0].y);
                    mergeSymbol[9] = t_point(mergeSymbol[8].x-trueScreen.get_width()*generalSymbolWidth,
                                             mergeSymbol[0].y);
                    mergeSymbol[10] = t_point(mergeSymbol[0].x,
                                              symbolCenter.y-trueScreen.get_width()*generalSymbolWidth/2);
                    fillpoly(mergeSymbol,11);
                } else if(generalText[i+generalTextStart].substr(1,2) == "RM")
                { // RAMP
                    t_point rampSymbol[11];
                    rampSymbol[0] = t_point(symbolCenter.x+trueScreen.get_width()*generalSymbolWidth/2,
                                            symbolCenter.y-trueScreen.get_width()*generalSymbolLength/2);
                    rampSymbol[1] = t_point(rampSymbol[0].x-trueScreen.get_width()*generalSymbolWidth,
                                            rampSymbol[0].y);
                    rampSymbol[2] = t_point(rampSymbol[1].x,
                                            rampSymbol[0].y+trueScreen.get_width()*generalSymbolLength);
                    rampSymbol[3] = t_point(rampSymbol[0].x,
                                            rampSymbol[2].y);
                    rampSymbol[4] = t_point(rampSymbol[0].x,
                                            symbolCenter.y);
                    rampSymbol[5] = t_point(rampSymbol[3].x+trueScreen.get_width()*generalSymbolWidth,
                                            rampSymbol[2].y-trueScreen.get_width()*generalSymbolArrowSize/2);
                    rampSymbol[6] = t_point(rampSymbol[5].x+trueScreen.get_width()*(generalSymbolWidth/2-generalSymbolArrowSize/2),
                                            rampSymbol[5].y);
                    rampSymbol[7] = t_point(rampSymbol[6].x+trueScreen.get_width()*generalSymbolArrowSize/2,
                                            rampSymbol[2].y+trueScreen.get_width()*generalSymbolArrowSize/2);
                    rampSymbol[8] = t_point(rampSymbol[6].x+trueScreen.get_width()*generalSymbolArrowSize,
                                            rampSymbol[5].y);
                    rampSymbol[9] = t_point(rampSymbol[7].x+trueScreen.get_width()*generalSymbolWidth/2,
                                             rampSymbol[5].y);
                    rampSymbol[10] = t_point(rampSymbol[4].x,
                                             rampSymbol[4].y-trueScreen.get_width()*generalSymbolWidth);
                    fillpoly(rampSymbol,11);
                } else if(generalText[i+generalTextStart].substr(1,2) == "HE")
                { // HELP
                    fillarc(symbolCenter.x,symbolCenter.y,trueScreen.get_width()*generalSymbolLength/2,0,360);
                    setcolor(WHITE);
                    setlinestyle(SOLID);
                    setlinewidth(2);
                    drawarc(symbolCenter.x,symbolCenter.y,trueScreen.get_width()*(generalSymbolLength/2-generalSymbolWidth/2),0,360);
                } // More to come!
                setcolor(BLACK);
                if(isHelp) 
                { // Variable helps us determine whether or not to left align
                    textCenter.x = trueScreen.left()+trueScreen.get_width()*generalTextOffset;
                    LEFTdrawtext(textCenter.x,textCenter.y,generalText[i+generalTextStart].substr(5,100),textMaxX,textMaxY);
                } else {
                    drawtext(textCenter.x,textCenter.y,generalText[i+generalTextStart].substr(5,100),textMaxX,textMaxY);
                }
            } else { // No special symbols
                if(isHelp)
                {
                    textCenter.x = trueScreen.left()+trueScreen.get_width()*generalTextOffset;
                    LEFTdrawtext(textCenter.x,textCenter.y,generalText[i+generalTextStart],textMaxX,textMaxY);
                } else {
                    drawtext(textCenter.x,textCenter.y,generalText[i+generalTextStart],textMaxX,textMaxY);
                }
            }
            // Drops the text down a line
            textCenter.y -= trueScreen.get_width()*messageTwoLineDisplace;
        }
    }
}

// We now separately draw the dialog buttons to render the hovering better without
// redrawing all the text. Easier on the eyes
void drawGeneralDialogOpenLayout(char open)
{
    if(!open) return; // Obviously doesn't matter if it's closed
    t_point generalButton[4];
    t_point generalArrow[3];
    t_point generalSuperUp[4];
    t_point generalUp[4];
    t_point generalDown[4];
    t_point generalSuperDown[4];
    t_point generalSuperUpArrow[7];
    t_point generalUpArrow[3];
    t_point generalDownArrow[3];
    t_point generalSuperDownArrow[7];
    generalButton[0] = t_point(trueScreen.left()+trueScreen.get_width()*generalBoxSize-trueScreen.get_width()*generalButtonSize,
                               trueScreen.get_ycenter()-trueScreen.get_width()*generalButtonSize/2);
    generalButton[1] = t_point(trueScreen.left()+trueScreen.get_width()*generalBoxSize-trueScreen.get_width()*generalButtonSize,
                               trueScreen.get_ycenter()+trueScreen.get_width()*generalButtonSize/2);
    generalButton[2] = t_point(trueScreen.left()+trueScreen.get_width()*generalBoxSize,
                               trueScreen.get_ycenter()+trueScreen.get_width()*generalButtonSize/2);
    generalButton[3] = t_point(trueScreen.left()+trueScreen.get_width()*generalBoxSize,
                               trueScreen.get_ycenter()-trueScreen.get_width()*generalButtonSize/2);
    generalSuperUp[0] = t_point(generalButton[2].x,
                                generalButton[0].y-trueScreen.get_width()*(generalButtonSize+generalButtonSeparation));
    generalSuperUp[1] = t_point(generalButton[1].x,generalSuperUp[0].y);
    generalSuperUp[2] = t_point(generalButton[1].x,generalSuperUp[0].y+trueScreen.get_width()*generalButtonSize);
    generalSuperUp[3] = t_point(generalButton[2].x,generalSuperUp[2].y);
    generalUp[0] = t_point(generalButton[2].x,
                           generalSuperUp[0].y-trueScreen.get_width()*(generalButtonSize+generalButtonSeparation));
    generalUp[1] = t_point(generalButton[1].x,generalUp[0].y);
    generalUp[2] = t_point(generalButton[1].x,generalUp[0].y+trueScreen.get_width()*generalButtonSize);
    generalUp[3] = t_point(generalButton[2].x,generalUp[2].y);
    generalSuperDown[0] = t_point(generalButton[2].x,trueScreen.bottom());
    generalSuperDown[1] = t_point(generalButton[1].x,trueScreen.bottom());
    generalSuperDown[2] = t_point(generalButton[1].x,trueScreen.bottom()+trueScreen.get_width()*generalButtonSize);
    generalSuperDown[3] = t_point(generalButton[2].x,trueScreen.bottom()+trueScreen.get_width()*generalButtonSize);
    generalDown[0] = t_point(generalButton[2].x,generalSuperDown[0].y+trueScreen.get_width()*(generalButtonSize+generalButtonSeparation));
    generalDown[1] = t_point(generalButton[1].x,generalDown[0].y);
    generalDown[2] = t_point(generalButton[1].x,generalDown[0].y+trueScreen.get_width()*generalButtonSize);
    generalDown[3] = t_point(generalButton[2].x,generalDown[2].y);
    generalArrow[0] = t_point(trueScreen.left()+trueScreen.get_width()*generalBoxSize-trueScreen.get_width()*generalButtonSize/2+trueScreen.get_width()*generalArrowSize/2,
                              trueScreen.get_ycenter()-trueScreen.get_width()*generalArrowSize/2);
    generalArrow[1] = t_point(trueScreen.left()+trueScreen.get_width()*generalBoxSize-trueScreen.get_width()*generalButtonSize/2+trueScreen.get_width()*generalArrowSize/2,
                              trueScreen.get_ycenter()+trueScreen.get_width()*generalArrowSize/2);
    generalArrow[2] = t_point(trueScreen.left()+trueScreen.get_width()*generalBoxSize-trueScreen.get_width()*generalButtonSize/2-trueScreen.get_width()*generalArrowSize/2,
                              trueScreen.get_ycenter());
    generalSuperUpArrow[0] = t_point(generalSuperUp[0].x-trueScreen.get_width()*(generalButtonSize/2-generalArrowSize/2),
                                     generalSuperUp[0].y+trueScreen.get_width()*(generalButtonSize/2-generalArrowSize/2));
    generalSuperUpArrow[1] = t_point(generalSuperUpArrow[0].x-trueScreen.get_width()*generalArrowSize,
                                     generalSuperUpArrow[0].y);
    generalSuperUpArrow[2] = t_point(generalSuperUpArrow[0].x-trueScreen.get_width()*generalArrowSize/2,
                                     generalSuperUpArrow[0].y+trueScreen.get_width()*generalArrowSize/2);
    generalSuperUpArrow[3] = t_point(generalSuperUpArrow[1].x,generalSuperUpArrow[2].y);
    generalSuperUpArrow[4] = t_point(generalSuperUpArrow[2].x,
                                     generalSuperUpArrow[0].y+trueScreen.get_width()*generalArrowSize);
    generalSuperUpArrow[5] = t_point(generalSuperUpArrow[0].x,generalSuperUpArrow[2].y);
    generalSuperUpArrow[6] = generalSuperUpArrow[2];
    generalSuperDownArrow[0] = t_point(generalSuperUpArrow[0].x,
                                       generalSuperDown[3].y-trueScreen.get_width()*(generalButtonSize/2-generalArrowSize/2));
    generalSuperDownArrow[1] = t_point(generalSuperUpArrow[1].x,generalSuperDownArrow[0].y);
    generalSuperDownArrow[2] = t_point(generalSuperUpArrow[2].x,
                                       generalSuperDown[3].y-trueScreen.get_width()*generalButtonSize/2);
    generalSuperDownArrow[3] = t_point(generalSuperUpArrow[3].x,generalSuperDownArrow[2].y);
    generalSuperDownArrow[4] = t_point(generalSuperUpArrow[4].x,
                                       generalSuperDownArrow[0].y-trueScreen.get_width()*generalArrowSize);
    generalSuperDownArrow[5] = t_point(generalSuperUpArrow[5].x,generalSuperDownArrow[2].y);
    generalSuperDownArrow[6] = t_point(generalSuperUpArrow[6].x,generalSuperDownArrow[2].y);
    generalUpArrow[0] = t_point(generalSuperUpArrow[0].x,
                                generalUp[0].y+trueScreen.get_width()*(generalButtonSize/2-generalArrowSize/2));
    generalUpArrow[1] = t_point(generalSuperUpArrow[1].x,generalUpArrow[0].y);
    generalUpArrow[2] = t_point(generalSuperUpArrow[4].x,
                                generalUpArrow[0].y+trueScreen.get_width()*generalArrowSize);
    generalDownArrow[0] = t_point(generalSuperUpArrow[0].x,
                                generalDown[3].y-trueScreen.get_width()*(generalButtonSize/2-generalArrowSize/2));
    generalDownArrow[1] = t_point(generalSuperUpArrow[1].x,generalDownArrow[0].y);
    generalDownArrow[2] = t_point(generalSuperUpArrow[4].x,
                                generalDownArrow[0].y-trueScreen.get_width()*generalArrowSize);
    // A bunch of hovering changing colors determining here
    if(currentHover == OPENGENH)
    {
        setcolor(WHITE);
    } else {
        setcolor(LIGHTGREY);
    }
    fillpoly(generalButton,4);
    if(currentHover == SCROLLSUPH)
    {
        setcolor(WHITE);
    } else {
        setcolor(LIGHTGREY);
    }
    fillpoly(generalSuperUp,4);
    if(currentHover == SCROLLUPH)
    {
        setcolor(WHITE);
    } else {
        setcolor(LIGHTGREY);
    }
    fillpoly(generalUp,4);
    if(currentHover == SCROLLDOWNH)
    {
        setcolor(WHITE);
    } else {
        setcolor(LIGHTGREY);
    }
    fillpoly(generalDown,4);
    if(currentHover == SCROLLSDOWNH)
    {
        setcolor(WHITE);
    } else {
        setcolor(LIGHTGREY);
    }
    fillpoly(generalSuperDown,4);
    setlinestyle(SOLID);
    setlinewidth(3);
    setcolor(BLACK);
    // Drawing the borders
    drawline(generalButton[0].x,generalButton[0].y,generalButton[1].x,generalButton[1].y);
    drawline(generalButton[2].x,generalButton[2].y,generalButton[1].x,generalButton[1].y);
    drawline(generalButton[2].x,generalButton[2].y,generalButton[3].x,generalButton[3].y);
    drawline(generalButton[0].x,generalButton[0].y,generalButton[3].x,generalButton[3].y);
    drawline(generalSuperUp[0].x,generalSuperUp[0].y,generalSuperUp[1].x,generalSuperUp[1].y);
    drawline(generalSuperUp[2].x,generalSuperUp[2].y,generalSuperUp[1].x,generalSuperUp[1].y);
    drawline(generalSuperUp[2].x,generalSuperUp[2].y,generalSuperUp[3].x,generalSuperUp[3].y);
    drawline(generalSuperUp[0].x,generalSuperUp[0].y,generalSuperUp[3].x,generalSuperUp[3].y);
    drawline(generalUp[0].x,generalUp[0].y,generalUp[1].x,generalUp[1].y);
    drawline(generalUp[2].x,generalUp[2].y,generalUp[1].x,generalUp[1].y);
    drawline(generalUp[2].x,generalUp[2].y,generalUp[3].x,generalUp[3].y);
    drawline(generalUp[0].x,generalUp[0].y,generalUp[3].x,generalUp[3].y);
    drawline(generalDown[0].x,generalDown[0].y,generalDown[1].x,generalDown[1].y);
    drawline(generalDown[2].x,generalDown[2].y,generalDown[1].x,generalDown[1].y);
    drawline(generalDown[2].x,generalDown[2].y,generalDown[3].x,generalDown[3].y);
    drawline(generalDown[0].x,generalDown[0].y,generalDown[3].x,generalDown[3].y);
    drawline(generalSuperDown[0].x,generalSuperDown[0].y,generalSuperDown[1].x,generalSuperDown[1].y);
    drawline(generalSuperDown[2].x,generalSuperDown[2].y,generalSuperDown[1].x,generalSuperDown[1].y);
    drawline(generalSuperDown[2].x,generalSuperDown[2].y,generalSuperDown[3].x,generalSuperDown[3].y);
    drawline(generalSuperDown[0].x,generalSuperDown[0].y,generalSuperDown[3].x,generalSuperDown[3].y);
    setlinewidth(1);
    setcolor(colorScheme[THEMEC][colorMode]);
    drawline(generalButton[0].x,generalButton[0].y,generalButton[1].x,generalButton[1].y);
    drawline(generalButton[2].x,generalButton[2].y,generalButton[1].x,generalButton[1].y);
    drawline(generalButton[2].x,generalButton[2].y,generalButton[3].x,generalButton[3].y);
    drawline(generalButton[0].x,generalButton[0].y,generalButton[3].x,generalButton[3].y);
    drawline(generalSuperUp[0].x,generalSuperUp[0].y,generalSuperUp[1].x,generalSuperUp[1].y);
    drawline(generalSuperUp[2].x,generalSuperUp[2].y,generalSuperUp[1].x,generalSuperUp[1].y);
    drawline(generalSuperUp[2].x,generalSuperUp[2].y,generalSuperUp[3].x,generalSuperUp[3].y);
    drawline(generalSuperUp[0].x,generalSuperUp[0].y,generalSuperUp[3].x,generalSuperUp[3].y);
    drawline(generalUp[0].x,generalUp[0].y,generalUp[1].x,generalUp[1].y);
    drawline(generalUp[2].x,generalUp[2].y,generalUp[1].x,generalUp[1].y);
    drawline(generalUp[2].x,generalUp[2].y,generalUp[3].x,generalUp[3].y);
    drawline(generalUp[0].x,generalUp[0].y,generalUp[3].x,generalUp[3].y);
    drawline(generalDown[0].x,generalDown[0].y,generalDown[1].x,generalDown[1].y);
    drawline(generalDown[2].x,generalDown[2].y,generalDown[1].x,generalDown[1].y);
    drawline(generalDown[2].x,generalDown[2].y,generalDown[3].x,generalDown[3].y);
    drawline(generalDown[0].x,generalDown[0].y,generalDown[3].x,generalDown[3].y);
    drawline(generalSuperDown[0].x,generalSuperDown[0].y,generalSuperDown[1].x,generalSuperDown[1].y);
    drawline(generalSuperDown[2].x,generalSuperDown[2].y,generalSuperDown[1].x,generalSuperDown[1].y);
    drawline(generalSuperDown[2].x,generalSuperDown[2].y,generalSuperDown[3].x,generalSuperDown[3].y);
    drawline(generalSuperDown[0].x,generalSuperDown[0].y,generalSuperDown[3].x,generalSuperDown[3].y);
    // Finally draw the arrows
    fillpoly(generalArrow,3);
    fillpoly(generalSuperUpArrow,7);
    fillpoly(generalSuperDownArrow,7);
    fillpoly(generalUpArrow,3);
    fillpoly(generalDownArrow,3);
}

// Since we are removing all the sidebar buttons, we draw all of them on the map
void drawZoomButtons()
{
    t_point zoomOut = t_point(trueScreen.right()-trueScreen.get_width()*(scaleLengthRatio+scaleOffsetRatioSmall),
                              trueScreen.top()-trueScreen.get_width()*scaleOffsetRatio/2);
    t_point zoomIn = t_point(trueScreen.right()-trueScreen.get_width()*scaleOffsetRatioSmall,
                             trueScreen.top()-trueScreen.get_width()*scaleOffsetRatio/2);
    t_point zoomMinus[4];
    t_point zoomPlus[12];
    zoomMinus[0] = t_point(zoomOut.x + trueScreen.get_width()*zoomSymbolLength/2,
                           zoomOut.y - trueScreen.get_width()*zoomSymbolWidth/2);
    zoomMinus[1] = t_point(zoomOut.x - trueScreen.get_width()*zoomSymbolLength/2,
                           zoomMinus[0].y);
    zoomMinus[2] = t_point(zoomMinus[1].x,
                           zoomOut.y + trueScreen.get_width()*zoomSymbolWidth/2);
    zoomMinus[3] = t_point(zoomMinus[0].x,
                           zoomMinus[2].y);
    zoomPlus[0] = t_point(zoomIn.x + trueScreen.get_width()*zoomSymbolWidth/2,
                          zoomIn.y - trueScreen.get_width()*zoomSymbolLength/2);
    zoomPlus[1] = t_point(zoomIn.x - trueScreen.get_width()*zoomSymbolWidth/2,
                          zoomPlus[0].y);
    zoomPlus[2] = t_point(zoomPlus[1].x,
                          zoomIn.y - trueScreen.get_width()*zoomSymbolWidth/2);
    zoomPlus[3] = t_point(zoomIn.x - trueScreen.get_width()*zoomSymbolLength/2,
                          zoomPlus[2].y);
    zoomPlus[4] = t_point(zoomPlus[3].x,
                          zoomIn.y + trueScreen.get_width()*zoomSymbolWidth/2);
    zoomPlus[5] = t_point(zoomPlus[2].x,
                          zoomPlus[4].y);
    zoomPlus[6] = t_point(zoomPlus[2].x,
                          zoomIn.y + trueScreen.get_width()*zoomSymbolLength/2);
    zoomPlus[7] = t_point(zoomPlus[0].x,
                          zoomPlus[6].y);
    zoomPlus[8] = t_point(zoomPlus[0].x,
                          zoomPlus[5].y);
    zoomPlus[9] = t_point(zoomIn.x + trueScreen.get_width()*zoomSymbolLength/2,
                          zoomPlus[8].y);
    zoomPlus[10] = t_point(zoomPlus[9].x,
                           zoomPlus[2].y);
    zoomPlus[11] = t_point(zoomPlus[0].x,
                           zoomPlus[2].y);
    if(currentHover == ZOOMOUTH)
    {
        setcolor(WHITE);
    } else {
        setcolor(LIGHTGREY);
    }
    fillarc(zoomOut, trueScreen.get_width()*zoomDiameter/2, 0, 360);
    if(currentHover == ZOOMINH)
    {
        setcolor(WHITE);
    } else {
        setcolor(LIGHTGREY);
    }
    fillarc(zoomIn, trueScreen.get_width()*zoomDiameter/2, 0, 360);
    setlinestyle(SOLID);
    setlinewidth(3);
    setcolor(BLACK);
    drawarc(zoomOut.x,zoomOut.y, trueScreen.get_width()*zoomDiameter/2, 0, 360);
    drawarc(zoomIn.x,zoomIn.y, trueScreen.get_width()*zoomDiameter/2, 0, 360);
    setlinewidth(1);
    setcolor(colorScheme[THEMEC][colorMode]);
    drawarc(zoomOut.x,zoomOut.y, trueScreen.get_width()*zoomDiameter/2, 0, 360);
    drawarc(zoomIn.x,zoomIn.y, trueScreen.get_width()*zoomDiameter/2, 0, 360);
    fillpoly(zoomMinus,4);
    fillpoly(zoomPlus,12);
    return;
}

void drawRotateButtons()
{
    t_point rotateLeftArrow[3];
    t_point rotateRightArrow[3];
    double rotateLeftX = trueScreen.left()+trueScreen.get_width()*(compassOffsetRatio-rotateButtonDisplace/2);
    double rotateRightX = trueScreen.left()+trueScreen.get_width()*(compassOffsetRatio+rotateButtonDisplace/2);
    double rotateY = trueScreen.top()-trueScreen.get_width()*(compassOffsetRatio+rotateButtonLowDisplace);
    rotateLeftArrow[0] = t_point(rotateLeftX-trueScreen.get_width()*(rotateButtonRadius/2-rotateArrowSize/2),
                                 rotateY);
    rotateLeftArrow[1] = t_point(rotateLeftX-trueScreen.get_width()*rotateButtonRadius/2,
                                 rotateY-trueScreen.get_width()*rotateArrowSize);
    rotateLeftArrow[2] = t_point(rotateLeftX-trueScreen.get_width()*(rotateButtonRadius/2+rotateArrowSize/2),
                                 rotateY);
    rotateRightArrow[0] = t_point(rotateRightX+trueScreen.get_width()*(rotateButtonRadius/2+rotateArrowSize/2),
                                  rotateY);
    rotateRightArrow[1] = t_point(rotateRightX+trueScreen.get_width()*rotateButtonRadius/2,
                                  rotateY-trueScreen.get_width()*rotateArrowSize);
    rotateRightArrow[2] = t_point(rotateRightX+trueScreen.get_width()*(rotateButtonRadius/2-rotateArrowSize/2),
                                  rotateY);
    if(currentHover == ROTATELH)
    {
        setcolor(WHITE);
    } else {
        setcolor(LIGHTGREY);
    }
    fillarc(rotateLeftX,rotateY,trueScreen.get_width()*rotateOuterRadius/2,0,360);
    if(currentHover == ROTATERH)
    {
        setcolor(WHITE);
    } else {
        setcolor(LIGHTGREY);
    }
    fillarc(rotateRightX,rotateY,trueScreen.get_width()*rotateOuterRadius/2,0,360);
    setlinestyle(SOLID);
    setlinewidth(3);
    setcolor(BLACK);
    drawarc(rotateLeftX,rotateY,trueScreen.get_width()*rotateOuterRadius/2,0,360);
    drawarc(rotateRightX,rotateY,trueScreen.get_width()*rotateOuterRadius/2,0,360);
    setlinewidth(1);
    setcolor(colorScheme[THEMEC][colorMode]);
    drawarc(rotateLeftX,rotateY,trueScreen.get_width()*rotateOuterRadius/2,0,360);
    drawarc(rotateRightX,rotateY,trueScreen.get_width()*rotateOuterRadius/2,0,360);
    fillpoly(rotateLeftArrow,3);
    fillpoly(rotateRightArrow,3);
    setlinewidth(5);
    drawarc(rotateLeftX,rotateY,trueScreen.get_width()*rotateButtonRadius/2,-45,285);
    drawarc(rotateRightX,rotateY,trueScreen.get_width()*rotateButtonRadius/2,-15,240);
    return;
}

void drawSideButtons()
{
    t_point exitButton[4];
    t_point helpButton[4];
    t_point searchButton[4];
    t_point directionsButton[4];
    t_point colorButton[4];
    t_point cancelButton[4];
    exitButton[0] = t_point(trueScreen.right(),
                            trueScreen.bottom()+trueScreen.get_width()*(miniOffsetRatio-miniSizeRatio/2+miniSizeRatio*mapRatio+sideButtonDisplace));
    exitButton[1] = t_point(exitButton[0].x-trueScreen.get_width()*sideButtonLength,
                            exitButton[0].y);
    exitButton[2] = t_point(exitButton[1].x,
                            exitButton[0].y+trueScreen.get_width()*sideButtonWidth);
    exitButton[3] = t_point(exitButton[0].x,
                            exitButton[2].y);
    helpButton[0] = t_point(exitButton[0].x, exitButton[2].y+trueScreen.get_width()*sideButtonDisplace);
    helpButton[1] = t_point(exitButton[1].x, helpButton[0].y);
    helpButton[2] = t_point(exitButton[1].x, helpButton[0].y+trueScreen.get_width()*sideButtonWidth);
    helpButton[3] = t_point(exitButton[0].x, helpButton[2].y);
    searchButton[0] = t_point(helpButton[0].x, helpButton[2].y+trueScreen.get_width()*sideButtonDisplace);
    searchButton[1] = t_point(helpButton[1].x, searchButton[0].y);
    searchButton[2] = t_point(helpButton[1].x, searchButton[0].y+trueScreen.get_width()*sideButtonWidth);
    searchButton[3] = t_point(helpButton[0].x, searchButton[2].y);
    directionsButton[0] = t_point(helpButton[0].x, searchButton[2].y+trueScreen.get_width()*sideButtonDisplace);
    directionsButton[1] = t_point(helpButton[1].x, directionsButton[0].y);
    directionsButton[2] = t_point(helpButton[1].x, directionsButton[0].y+trueScreen.get_width()*sideButtonWidth);
    directionsButton[3] = t_point(helpButton[0].x, directionsButton[2].y);
    colorButton[0] = t_point(helpButton[0].x, directionsButton[2].y+trueScreen.get_width()*sideButtonDisplace);
    colorButton[1] = t_point(helpButton[1].x, colorButton[0].y);
    colorButton[2] = t_point(helpButton[1].x, colorButton[0].y+trueScreen.get_width()*sideButtonWidth);
    colorButton[3] = t_point(helpButton[0].x, colorButton[2].y);
    if(keyMode == 0)
    {
        cancelButton[0] = t_point(trueScreen.left()-trueScreen.get_width(),trueScreen.bottom()-trueScreen.get_width());
        cancelButton[1] = t_point(trueScreen.left()-trueScreen.get_width(),trueScreen.bottom()-trueScreen.get_width());
        cancelButton[2] = t_point(trueScreen.left()-trueScreen.get_width(),trueScreen.bottom()-trueScreen.get_width());
        cancelButton[3] = t_point(trueScreen.left()-trueScreen.get_width(),trueScreen.bottom()-trueScreen.get_width());
    } else {
        cancelButton[0] = t_point(helpButton[0].x, colorButton[2].y+trueScreen.get_width()*sideButtonWidth);
        cancelButton[1] = t_point(helpButton[1].x, cancelButton[0].y);
        cancelButton[2] = t_point(helpButton[1].x, cancelButton[0].y+trueScreen.get_width()*sideButtonWidth);
        cancelButton[3] = t_point(helpButton[0].x, cancelButton[2].y);
    }
    
    if(currentHover == EXITH)
    {
        setcolor(WHITE);
    } else {
        setcolor(LIGHTGREY);
    }
    fillpoly(exitButton,4);
    if(currentHover == HELPH)
    {
        setcolor(WHITE);
    } else {
        setcolor(LIGHTGREY);
    }
    fillpoly(helpButton,4);
    if(keyMode == 1)
    {
        setcolor(WHITE);
        fillpoly(searchButton,4);
    } else {
        if(currentHover == SEARCHH)
        {
            setcolor(WHITE);
        } else {
            setcolor(LIGHTGREY);
        }
        fillpoly(searchButton,4);
    }
    if(keyMode == 2 || keyMode == 3)
    {
        setcolor(WHITE);
        fillpoly(directionsButton,4);
    } else {
        if(currentHover == DIRECTIONH)
        {
            setcolor(WHITE);
        } else {
            setcolor(LIGHTGREY);
        }
        fillpoly(directionsButton,4);
    }
    if(currentHover == COLORH)
    {
        setcolor(WHITE);
    } else {
        setcolor(LIGHTGREY);
    }
    fillpoly(colorButton,4);
    if(currentHover == CANCELH)
    {
        setcolor(WHITE);
    } else {
        setcolor(LIGHTGREY);
    }
    fillpoly(cancelButton,4);
    setlinestyle(SOLID);
    setlinewidth(3);
    setcolor(BLACK);
    drawline(exitButton[0],exitButton[1]);
    drawline(exitButton[2],exitButton[1]);
    drawline(exitButton[2],exitButton[3]);
    drawline(exitButton[0],exitButton[3]);
    drawline(helpButton[0],helpButton[1]);
    drawline(helpButton[2],helpButton[1]);
    drawline(helpButton[2],helpButton[3]);
    drawline(helpButton[0],helpButton[3]);
    drawline(searchButton[0],searchButton[1]);
    drawline(searchButton[2],searchButton[1]);
    drawline(searchButton[2],searchButton[3]);
    drawline(searchButton[0],searchButton[3]);
    drawline(directionsButton[0],directionsButton[1]);
    drawline(directionsButton[2],directionsButton[1]);
    drawline(directionsButton[2],directionsButton[3]);
    drawline(directionsButton[0],directionsButton[3]);
    drawline(colorButton[0],colorButton[1]);
    drawline(colorButton[2],colorButton[1]);
    drawline(colorButton[2],colorButton[3]);
    drawline(colorButton[0],colorButton[3]);
    drawline(cancelButton[0],cancelButton[1]);
    drawline(cancelButton[2],cancelButton[1]);
    drawline(cancelButton[2],cancelButton[3]);
    drawline(cancelButton[0],cancelButton[3]);
    setlinewidth(1);
    setcolor(colorScheme[THEMEC][colorMode]);
    drawline(exitButton[0],exitButton[1]);
    drawline(exitButton[2],exitButton[1]);
    drawline(exitButton[2],exitButton[3]);
    drawline(exitButton[0],exitButton[3]);
    drawline(helpButton[0],helpButton[1]);
    drawline(helpButton[2],helpButton[1]);
    drawline(helpButton[2],helpButton[3]);
    drawline(helpButton[0],helpButton[3]);
    drawline(searchButton[0],searchButton[1]);
    drawline(searchButton[2],searchButton[1]);
    drawline(searchButton[2],searchButton[3]);
    drawline(searchButton[0],searchButton[3]);
    drawline(directionsButton[0],directionsButton[1]);
    drawline(directionsButton[2],directionsButton[1]);
    drawline(directionsButton[2],directionsButton[3]);
    drawline(directionsButton[0],directionsButton[3]);
    drawline(colorButton[0],colorButton[1]);
    drawline(colorButton[2],colorButton[1]);
    drawline(colorButton[2],colorButton[3]);
    drawline(colorButton[0],colorButton[3]);
    drawline(cancelButton[0],cancelButton[1]);
    drawline(cancelButton[2],cancelButton[1]);
    drawline(cancelButton[2],cancelButton[3]);
    drawline(cancelButton[0],cancelButton[3]);
    setcolor(BLACK);
    drawtext((exitButton[0].x+exitButton[1].x)/2,(exitButton[0].y+exitButton[2].y)/2,
             "Exit",textMaxX,textMaxY);
    drawtext((helpButton[0].x+helpButton[1].x)/2,(helpButton[0].y+helpButton[2].y)/2,
             "Help",textMaxX,textMaxY);
    drawtext((searchButton[0].x+searchButton[1].x)/2,(searchButton[0].y+searchButton[2].y)/2,
             "Search",textMaxX,textMaxY);
    drawtext((directionsButton[0].x+directionsButton[1].x)/2,(directionsButton[0].y+directionsButton[2].y)/2,
             "Directions",textMaxX,textMaxY);
    drawtext((colorButton[0].x+colorButton[1].x)/2,(colorButton[0].y+colorButton[2].y)/2,
             "Color",textMaxX,textMaxY);
    drawtext((cancelButton[0].x+cancelButton[1].x)/2,(cancelButton[0].y+cancelButton[2].y)/2,
             "Cancel",textMaxX,textMaxY);
}