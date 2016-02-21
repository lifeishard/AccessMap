



#include "m2.h"
#include <unistd.h>
#include <cstring>
using namespace std;






int main(int argc,char *argv[]) {
    string baseDirectory = "/cad2/ece297s/public/maps/";
    string mapFile;
    string defaultMap = "toronto.bin";
    
    // If there is a command line argument, run user-inputted map
    if(argc > 1)
    {
        char mapName[100];
        strcpy(mapName,baseDirectory.c_str());
        strcat(mapName,argv[1]);

        // If map exists
        if(access(mapName,R_OK) != -1)
        {
            draw_map(string(mapName));
        } else {
            // Always draws toronto as alternative
            cout << "Map " << string(argv[1]) << " does not exist." << endl;
            cout << "Drawing default map " << defaultMap << " instead." << endl; 
            draw_map(baseDirectory+=defaultMap);
        }
    
    }   
    
    else draw_map(baseDirectory+=defaultMap);
}
 
