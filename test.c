#include "stationADT.h"
#include <time.h>

int main(){
    // struct tm * date = malloc((struct(tm))
    // struct tm * date2;
    // date->tm_hour = 10;
    // date->tm_mday = 10;
    // date->tm_min = 10;
    // date->tm_mon = 10;
    // date->tm_sec = 10;
    // date->tm_year = 2010;
    // date2->tm_hour = 11;
    // date2->tm_mday = 11;
    // date2->tm_min = 11;
    // date2->tm_mon = 11;
    // date2->tm_sec = 11;
    // date2->tm_year = 2011;    
    stationADT newStations = newStation();
    stationsIdBST BST = NULL;
    addStation(newStations,"Caseros",BST,1945);
    addStation(newStations,"Congreso",BST,1969);
    // addRental(BST,date,1945,date2,1969,1);
    freeTree(BST);
    freeAssets(newStations);
}