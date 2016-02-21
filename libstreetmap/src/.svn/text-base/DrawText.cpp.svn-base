#include "DrawText.h"

priority_queue <textToDraw,vector<textToDraw>,textToDrawcomparison> drawTextQueue;   
vector<pair<double,double> > drawnText;

void drawAllText()
{
    drawStreetNames();
    drawPOINames();
    drawnText.clear();
    textArrange();
}

void textArrange()
{
    textToDraw currentText;
    char textType;
    vector<string> textName;
    setfontsize(11);
    setcolor(colorScheme[9][colorMode]);
    char textTooClose;
    double lineSeparation = trueScreen.get_width()*0.012;
    while (!drawTextQueue.empty())
    {
        currentText = drawTextQueue.top();
        drawTextQueue.pop();
        
        // Don't draw POI names under a threshold
        textType = currentText.type;
        if(textType == POINAMEDISPLAY && trueScreen.get_width() > drawPOINameThreshold) continue;
        
        textTooClose = 0;
        for(unsigned i = 0; i < drawnText.size(); i++)
        {
            if(tooClose(drawnText[i].first,drawnText[i].second,currentText.x,currentText.y))
            {
                textTooClose = 1;
                break;
            }
        }
        if(textTooClose) continue;
        
        drawnText.push_back(make_pair(currentText.x,currentText.y));
        
        if(textType == POINAMEDISPLAY)
        {
            textName = allPoints[currentText.ID].getdisplayString();
        } else {
            textName.push_back(allStreets[currentText.ID].getName());

        }
        
        settextrotation(int(currentText.angle)%360);
        
        if(textName.size() <= 1)
        {
            drawtext(currentText.x, currentText.y, textName[0], textMaxX, textMaxY);
        } else {
            currentText.x = currentText.x - lineSeparation*sin(currentText.angle*DEG_TO_RAD)/2;
            currentText.y = currentText.y + lineSeparation*cos(currentText.angle*DEG_TO_RAD)/2;
            for(unsigned i = 0; i < textName.size(); i++)
            {
                drawtext(currentText.x, currentText.y, textName[i], textMaxX, textMaxY);
                currentText.x = currentText.x + lineSeparation*sin(currentText.angle*DEG_TO_RAD);
                currentText.y = currentText.y - lineSeparation*cos(currentText.angle*DEG_TO_RAD);
            }
        }
        
        
        textName.clear();
    }
}

char tooClose(double x1, double y1, double x2, double y2)
{//change this variable around or expression depending on how you close you want
   // the text
    if(hypot(x2-x1,y2-y1) < trueScreen.get_width()*nameTooCloseThreshold) return 1;
    else return 0;
}

void drawStreetNames() {
    int64_t t1, t2;
    int64_t motherStreet; // street ID of a street segment
    int64_t repetition;
    double segAngle;
    string motherStreetName;
    double x1, x2, y1, y2;
    double xc, yc;
    vector<int64_t> textSeg;
    //vector<pair<double,double> > alreadyDrawn;
    //char willDraw = 1;
    //double tooCloseThreshold = streetNameTooCloseThreshold * tBoundCurrent.get_width();
    //unsigned alreadyDrawnSize;
    textToDraw textInfo;

    setfontsize(11);
    setcolor(colorScheme[9][colorMode]);
    for(unsigned i = 0; i < allStreets.size(); i++)
    {
        if(!onScreen(allStreets[i].getboundary())) continue;
        textSeg = allStreets[i].getSegs();
        repetition = 1;
        for (unsigned j = 0; j < textSeg.size(); j++)
        {
            if (!onScreen(allStreetSegs[textSeg[j]].getboundary())) continue;
            motherStreet = allStreetSegs[textSeg[j]].getStreet();
            motherStreetName = allStreets[motherStreet].getName();
            if ((motherStreetName != "(unknown)") &&
                ((allStreetSegs[textSeg[j]].getLength()) / (tBoundCurrent.get_width()) > segLengthDisplayRatio))
            {
                repetition++;
                t1 = allStreetSegs[textSeg[j]].getEnds().first;
                t2 = allStreetSegs[textSeg[j]].getEnds().second;
                x1 = allIntersects[t1].getLocation().getx();
                y1 = allIntersects[t1].getLocation().gety();
                x2 = allIntersects[t2].getLocation().getx();
                y2 = allIntersects[t2].getLocation().gety();
                xc = (x1 + x2) / 2;
                yc = (y1 + y2) / 2;
                segAngle = atan((y2 - y1) / (x2 - x1));
                if (segAngle > PI/2)
                {
                    segAngle -= PI;
                }
                else if (segAngle < -PI/2)
                {
                    segAngle += PI;
                }
                segAngle = segAngle / DEG_TO_RAD;
                
                textInfo.priority = (streetInherentPriority +
                        floor(allStreetSegs[textSeg[j]].getSpeed()/10) +
                        floor(allStreets[motherStreet].getLength()/1000)) / (repetition*10);
                textInfo.ID = motherStreet;
                textInfo.x = xc;
                textInfo.y = yc;
                textInfo.angle = segAngle;
                textInfo.type = STREETNAMEDISPLAY;
                drawTextQueue.push(textInfo);

                /*settextrotation(segAngle);

                alreadyDrawnSize = alreadyDrawn.size();
                for(unsigned j = 0; j < alreadyDrawnSize; j++)
                {
                    if(abs(alreadyDrawn[j].first - xc) < tooCloseThreshold
                            && abs(alreadyDrawn[j].second - yc) < tooCloseThreshold)
                    {
                        willDraw = 0;
                    }
                }
                if(willDraw)
                {
                    alreadyDrawn.push_back(make_pair(xc,yc));
                    drawtext(xc, yc, motherStreetName, textMaxX, textMaxY);
                }
                willDraw = 1;*/
            }
        }
    }
    //alreadyDrawn.clear();    
}

void drawPOINames()
{
    //draws POI test labels
    textToDraw textInfo;
    setfontsize(11);
    setcolor(colorScheme[9][colorMode]);
    settextrotation(0);
    for (unsigned i = 0; i < allPoints.size(); i++) {
        if(!onScreen(allPoints[i].getLocation())) continue;
        if(trueScreen.get_width() > drawPOINameThreshold) continue;
            
        textInfo.priority = 10;
        textInfo.ID = i;
        textInfo.x = allPoints[i].getLocation().getx();
        textInfo.y = allPoints[i].getLocation().gety();
        textInfo.angle = 0;
        textInfo.type = POINAMEDISPLAY;
        drawTextQueue.push(textInfo);
                
        /*double centerX = allPoints[i].getLocation().getx();
        double centerY = allPoints[i].getLocation().gety();
        //text labels only drawn if zoom level exceeds 1km and is onScreen
        if(tBoundCurrent.get_width() < 1000){
            string pointName = allPoints[i].getName();
            drawtext(centerX, centerY, pointName, textMaxX, textMaxY); 
        }*/
    }
}

/*
        // Draws street names
        if(tBoundCurrent.get_width() < streetNameThreshold)
        {
            const double streetDisplay = 0.35 * tBoundCurrent.get_width();
            const double textSeparation = 0.3 * tBoundCurrent.get_width();
            const double charSeparation = 0.01 * tBoundCurrent.get_width();
            cout << "char separation: " << charSeparation << endl;
            double currentDistanceProgress;
            double currentCharProgress;
            double segAngle;
            unsigned charRepetition;
            string motherStreetName;
            string charStreetName;
            int64_t drawNameMultiple;
            pair<double, double> currentOffset, charOffset;
            vector<int64_t> currentSegs;
            vector<Cord> currentBends;
            setcolor (BLACK);
            Cord currentCord;
            Cord textCord1, textCord2;
            for(unsigned i = 0; i < allStreets.size(); i++)
            { // For each street
                // If name is (unknown) skip this loop
                if((allStreets[i].getName() == "(unknown)")&&(allStreets[i].getLength()>streetDisplay)) continue;
                // Resets distance counter
                cout << "IN STREET: ";
                currentDistanceProgress = 0;
                currentCharProgress = 0;
                currentSegs = allStreets[i].getSegs();
                motherStreetName = allStreets[i].getName();
                for(unsigned j = 0; j < currentSegs.size(); j++) {
                    if(!onScreenStreetSeg(currentSegs[j])) continue;
                    if(((allStreetSegs[currentSegs[j]].getLength() + currentDistanceProgress) < textSeparation)) {
                        currentDistanceProgress += allStreetSegs[currentSegs[j]].getLength();
                        cout << "|too small| ";
                        continue;
                    }
                    cout << "In Seg: ";
                    currentBends = allStreetSegs[currentSegs[j]].getBendPoints();
                    // Sets up loop so loop puts the second into first, then queries for the second
                    textCord2 = allIntersects[allStreetSegs[currentSegs[j]].getEnds().first].getLocation();
                    for(unsigned k = 0; k < currentBends.size(); k++)
                    {
                        cout << "has curves: ";
                        textCord1 = textCord2;
                        textCord2 = currentBends[k];
                        currentDistanceProgress += cordDistance(textCord1, textCord2);
                        if(currentDistanceProgress >= textSeparation) {
                            drawNameMultiple = floor(currentDistanceProgress/textSeparation);
                            for(unsigned l = 0; l < drawNameMultiple; l++)
                            {
                                cout << "drawing! " << currentDistanceProgress-textSeparation << endl;
                                currentOffset = cordOffset(textCord1, textCord2, (cordDistance(textCord1, textCord2) - l*textSeparation - fmod(currentDistanceProgress, textSeparation)));
                                segAngle = atan2((textCord2.gety() - textCord1.gety()),(textCord2.getx() - textCord1.getx()));
                                if (segAngle > PI/2)
                                {
                                    segAngle -= PI;
                                }
                                else if (segAngle < -PI/2)
                                {
                                    segAngle += PI;
                                }
                                settextrotation(segAngle / DEG_TO_RAD);
                                
                                charOffset = currentOffset;
                                charRepetition = motherStreetName.size();
                                drawtext(currentOffset.first, currentOffset.second, "#", textMaxX, textMaxY);
                                for(unsigned n = 0; n < charRepetition; n++)
                                {
                                    charStreetName.push_back(motherStreetName[0]);
                                    drawtext(charOffset.first, charOffset.second, charStreetName, textMaxX, textMaxY);
                                    charOffset = cordOffset(charOffset, textCord2, charSeparation+currentCharProgress);
                                    currentCharProgress = 0;
                                    motherStreetName = cutChar(motherStreetName);
                                    if(motherStreetName.size() == 0)
                                    {
                                        motherStreetName = allStreets[i].getName();
                                        n = charRepetition;
                                    }
                                    if(cordDistance(charOffset, textCord2) < charSeparation)
                                    {
                                        n = charRepetition;
                                        currentCharProgress = charSeparation - cordDistance(charOffset, textCord2);
                                    }
                                }
                                drawtext(currentOffset.first, currentOffset.second, motherStreetName, textMaxX, textMaxY);
                                currentDistanceProgress -= textSeparation;
                            }
                        }
                    }
                    textCord1 = textCord2;
                    textCord2 = allIntersects[allStreetSegs[currentSegs[j]].getEnds().second].getLocation();
                    currentDistanceProgress += cordDistance(textCord1, textCord2);
                    if(currentDistanceProgress >= textSeparation) {
                        drawNameMultiple = floor(currentDistanceProgress/textSeparation);
                        for(unsigned l = 0; l < drawNameMultiple; l++)
                        {
                            currentOffset = cordOffset(textCord1, textCord2, (cordDistance(textCord1, textCord2) - l*textSeparation - fmod(currentDistanceProgress, textSeparation)));
                            segAngle = atan2((textCord2.gety() - textCord1.gety()),(textCord2.getx() - textCord1.getx()));
                            if (segAngle > PI/2)
                            {
                                segAngle -= PI;
                            }
                            else if (segAngle < -PI/2)
                            {
                                segAngle += PI;
                            }
                            settextrotation(segAngle / DEG_TO_RAD);
                            
                            charOffset = currentOffset;
                            charRepetition = motherStreetName.size();
                            for(unsigned n = 0; n < charRepetition; n++)
                            {
                                charStreetName = "";
                                charStreetName.push_back(motherStreetName[0]);
                                drawtext(charOffset.first, charOffset.second, charStreetName, textMaxX, textMaxY);
                                charOffset = cordOffset(charOffset, textCord2, charSeparation+currentCharProgress);
                                currentCharProgress = 0;
                                motherStreetName = cutChar(motherStreetName);
                                if(motherStreetName.size() == 0)
                                {
                                    motherStreetName = allStreets[i].getName();
                                    n = charRepetition;
                                }
                                if(cordDistance(charOffset, textCord2) < charSeparation)
                                {
                                    n = charRepetition;
                                    currentCharProgress = charSeparation - cordDistance(charOffset, textCord2);
                                }
                            }
                            drawtext(currentOffset.first, currentOffset.second, motherStreetName, textMaxX, textMaxY);
                            currentDistanceProgress -= textSeparation;
                        }
                    }
                }
            }
        }
        */