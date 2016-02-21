#include "DrawBackground.h"

void drawAllFeatures()
{
    //features drawn here
    for (unsigned i = 0; i < allFeatures.size(); i++) {
       if(!onScreen(allFeatures[i].getboundary())) continue;
       if(!(allFeatures[i].getboundary().area()/trueScreen.area() > drawFeatureThreshold)) continue;
        drawFeature(i,0);
    }
}

void drawFeature(unsigned i, char mini)
{
    t_point* featurePoints;
    if(mini)
    {
        allLargeFeatures[i].updatepolyMiniPoints();
        featurePoints = allLargeFeatures[i].getpolyPoints();
        i = allLargeFeatures[i].getID();
    } else {
        featurePoints = allFeatures[i].getpolyPoints();
    }
    
    //closed features are polygons
    if (allFeatures[i].isClosed())
    {
        setcolor(colorScheme[allFeatures[i].getcolorID()][colorMode]);
        fillpoly(featurePoints, allFeatures[i].getnPoints());

    } else { //open features are lines
        setcolor(colorScheme[allFeatures[i].getcolorID()][colorMode]);
        if(mini)
        {
            for (unsigned j = 0; j < allFeatures[i].getnPoints() - 1; j ++) {
                drawline(featurePoints[j],featurePoints[j+1]);
            }
        } else {
            setlinewidth(floor(allFeatures[i].getlineWidth()/trueScreen.get_width()));
            setlinestyle(allFeatures[i].getlineStyle());
            for (unsigned j = 0; j < allFeatures[i].getnPoints() - 1; j ++) {
                drawline(featurePoints[j],featurePoints[j+1]);
                fillarc(featurePoints[j].x,featurePoints[j].y,allFeatures[i].getlineWidth()/2200,0,360);
            }
            fillarc(featurePoints[allFeatures[i].getnPoints() - 1].x,featurePoints[allFeatures[i].getnPoints() - 1].y,allFeatures[i].getlineWidth()/2200,0,360);
        }           
    }
}
