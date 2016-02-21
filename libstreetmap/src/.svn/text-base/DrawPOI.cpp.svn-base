#include "DrawPOI.h"

t_point starPoints[10];

void initStarPoints()
{
    const float starRadius = 3;
    
    //this loop goes around 360 degrees of a circle starting from 90 degrees in 36 degree increments
    //this represents to 10 evenly spaced points of a 5 pointed star
    for (int j = 0; j < 10; j ++) {
        double radAngle = ((j * 36) + 90)*DEG_TO_RAD;
        double pointLength = ((j % 2 == 0) ? starRadius*2 : starRadius); //even points are longer than odd ones
        double pointX = (pointLength*cos(radAngle));
        double pointY = (pointLength*sin(radAngle));
        starPoints[j] = t_point(pointX,pointY);
    }
}

void drawPOI()
{
    if(trueScreen.get_width() < drawPOIThreshold)
    {
        initStarPoints();
        //draws all POIs
        setcolor(colorScheme[7][colorMode]);
        for (unsigned i = 0; i < allPoints.size(); i++) {
            if(!onScreen(allPoints[i].getLocation())) continue;
            drawPOINameID.push_back(i);
            double centerX = allPoints[i].getLocation().getx();
            double centerY = allPoints[i].getLocation().gety();

            t_point offsetPoints[10];
            for (int j = 0; j < 10; j ++){
                offsetPoints[j] = t_point(starPoints[j].x + centerX, starPoints[j].y + centerY);
            }

            fillpoly(offsetPoints, 10); 

        }
    }
}