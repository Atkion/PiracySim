#pragma once

#include "objectDefs/CoreObjects.h"
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <string>

using namespace std;

class Island
{
    private:
        pair<int, int> location;
    public:
        Island(string); // randomly generate stuff
        Island(int, int);
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

    public:
        Map(int, int); // set the width and the height
        Map(int, int, pair<int, int>); // set the width and the height w/ ship location

        void generateIsland(int); // create an island a certain distance from the ship
        void generateIsland(int, int); // create an island with a specific location
        void addShipLocation(int, int);
        void moveShip(int, int); // move the ship x and y amount DOES NOT SET SHIP LOCATION ONLY MOVES
        string printMapView(int, int, int, int);
};