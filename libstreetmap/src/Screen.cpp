#include "Screen.h"

t_bound_box trueScreen;
t_bound_box visibleMap;
const int64_t border = 50;
int64_t zoom;

vector <int64_t> drawStreetNameID;
vector <int64_t> drawPOINameID;
 

void getScreenDimension()
{
   trueScreen = get_visible_world();
   visibleMap = t_bound_box(trueScreen.left() - border,trueScreen.bottom() - border,trueScreen.right() + border ,trueScreen.top() + border);
   zoom = max(log(Cord::height/(trueScreen.top() - trueScreen.bottom()))/(log(1.665)),log(Cord::width/(trueScreen.right() - trueScreen.left()))/(log(1.665)));
}

char capScreen()
{
    double xOffSet = 0;
    double yOffSet = 0;
    char changeWorld = 0;
    if(trueScreen.get_width() > Cord::xBound)
    {
        set_visible_world(1,trueScreen.get_ycenter()-1,Cord::xBound-1,trueScreen.get_ycenter()+1);
        return 1;
    } else if(trueScreen.get_width() < screenCapSmallest)
    {
        set_visible_world(trueScreen.get_xcenter()-1-screenCapSmallest/2,trueScreen.get_ycenter()-1-screenCapSmallest/2,
                          trueScreen.get_xcenter()+1+screenCapSmallest/2,trueScreen.get_ycenter()+1+screenCapSmallest/2);
        return 1;
    }
    if(trueScreen.get_height() > Cord::yBound)
    {
        set_visible_world(trueScreen.get_xcenter()-1,1,trueScreen.get_xcenter()+1,Cord::yBound-1);
        return 1;
    } else if(trueScreen.get_height() < screenCapSmallest)
    {
        set_visible_world(trueScreen.get_xcenter()-1-screenCapSmallest/2,trueScreen.get_ycenter()-1-screenCapSmallest/2,
                          trueScreen.get_xcenter()+1+screenCapSmallest/2,trueScreen.get_ycenter()+1+screenCapSmallest/2);
        return 1;
    }
    if(trueScreen.left() < -1)
    {
        xOffSet = -trueScreen.left()+1;
        changeWorld = 1;
    } else if(trueScreen.right() > Cord::xBound+1)
    {
        xOffSet = Cord::xBound - trueScreen.right() - 1;
        changeWorld = 1;
    }
    if(trueScreen.bottom() < -1)
    {
        yOffSet = -trueScreen.bottom()+1;
        changeWorld = 1;
    } else if(trueScreen.top() > Cord::yBound+1)
    {
        yOffSet = Cord::yBound - trueScreen.top() - 1;
        changeWorld = 1;
    }
    if(changeWorld == 1)
    {
        set_visible_world(trueScreen.left()+xOffSet,trueScreen.bottom()+yOffSet,
                trueScreen.right()+xOffSet,trueScreen.top()+yOffSet);
        return 1;
    }
    if(isnan(trueScreen.get_xcenter()) || isnan(trueScreen.get_ycenter()))
    {
        set_visible_world(1,1,Cord::xBound-1,Cord::yBound-1);
        return 1;
    }
    return 0;
}

char onScreen(Cord point1)
{
    if(point1.getx() < visibleMap.left()||point1.getx() > visibleMap.right()||point1.gety() < visibleMap.bottom()||point1.gety() > visibleMap.top())return 0;
    else return 1;
}

char onScreen(t_bound_box _boundary)
{
    if(_boundary.left()>visibleMap.right()||
        _boundary.right()<visibleMap.left()||
        _boundary.top()<visibleMap.bottom()||
        _boundary.bottom()>visibleMap.top())return 0;
    else return 1;
}
