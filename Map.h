#pragma once

#include "objectDefs/CoreObjects.h"
#include "objectDefs/Port.h"
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <string>

using namespace std;

/* TODO -> Add size int
        -> Add formation of islands
        -> possibly add print island??
*/
class Island
{
    private:
        pair<int, int> location;
        void formIsland(int, int, int, int, int);
        pair<int, int> port;
    public:
        int islandSize;
        bool islandMap[21][21];
        Island(string); // randomly generate stuff
        Island(int, int);
        pair<int, int> getPort();
        pair<int, int> getLocation(); // get location of the island;
        void setLocation(int x, int y);
        string name;
};

class Map
{
    private:
        pair<int, int> shipLocation;
        int height, width;
        vector<Island> islands;
        Ship* ship;
    public:
        Map(int, int, Ship*); // set the width and the height
        Map(int, int, pair<int, int>); // set the width and the height w/ ship location

        void generateIsland(int); // create an island a certain distance from the ship
        void generateIsland(int, int); // create an island with a specific location
        void addShipLocation(int, int);
        void moveShip(int, int); // move the ship x and y amount DOES NOT SET SHIP LOCATION ONLY MOVES
        void printMapView(int, int, int, int);
        void enterMap();
};