#pragma once

#include "objectDefs/CoreObjects.h"
#include <iostream>
#include <string>
#include <utility>

using namespace std;

class Island
{
    private:
        generateIsland();
    public:
        Island(); // randomly generate stuff
};

class Map
{
    private:
        Ship* ship;
        pair<int, int> shipLocation;
        int height, width;
        // Island islands[];

    public:
        // int numberOfIslands;

        Map(int, int); // set the width and the height w/ the current ship on the map

        void generateIslands();
        void addShip(Ship*, int, int);
        string printMapView(int, int, int, int);
};