#include "Map.h"
#include "objectDefs/CoreObjects.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// TODO : When an island is created have the random island layout be created
Island::Island(string name_)
{
    name = name_;
    /* initialize random seed: */
    srand(time(NULL));
    
    /* generate random number between 1 and 10: */
    int chance;

    // for (int i = 0; i < 5; i++)
    //     for (int j = 0; j < 5; j++)
        
            
}

Island::Island(int x, int y)
{
    location = pair<int, int>(x, y);
}

pair<int, int> Island::getLocation()
{
    return location;    
}

void Island::setLocation(int x, int y)
{
    location.first = x;
    location.second = y;
}

Map::Map(int width_, int height_)
{
    // set the insace variables
    width = width_;
    height = height_;
}

Map::Map(int width_, int height_, pair<int, int> shipLocation_)
{
    width = width_;
    height = height_;
    shipLocation.first = shipLocation_.first;
    shipLocation.second = shipLocation_.second;
}

void Map::generateIsland(int distanceFromShip)
{
    islands.push_back(Island("The NutClam Aisles"));
    /* initialize random seed: */
    srand(time(NULL));

    /* generate random number between 1 and 10: */
    int numberOfIslands = rand() % 10 + 1;
    // islands[numberOfIslands];

    // for (int i = 0; i < numberOfIslands; i++)
    //     islands[i] = Island();
}

void Map::generateIsland(int x, int y)
{
    Island temp("The NutClam Aisles");
    temp.setLocation(x, y);
    islands.push_back(temp);
}

void Map::addShipLocation(int x, int y)
{
    shipLocation.first = x;
    shipLocation.second = y;
}

void Map::moveShip(int xAmount, int yAmount)
{
    shipLocation.first += xAmount;
    shipLocation.second += -yAmount; 
}

string Map::printMapView(int x, int y, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            for (auto island = islands.begin(); island != islands.end(); ++island)
            {
                if ((x + j) == island->getLocation().first && (y + i) == island->getLocation().second)
                {
                    cout << "O ";
                }
            }
            if ((x + j) == shipLocation.first && (y + i) == shipLocation.second)
            {
                cout << "S ";
            }
            else
            {
                cout << "* ";
            }
        }
        cout << "\n";
    }
}

