#include "Map.h"
#include "objectDefs/CoreObjects.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <string>

using namespace std;

Island::Island()
{
    /* initialize random seed: */
    srand(time(NULL));
    
    /* generate random number between 1 and 10: */
    int chance;

    // for (int i = 0; i < 5; i++)
    //     for (int j = 0; j < 5; j++)
        
            
}

Map::Map(int width_, int height_)
{
    // set the insace variables
    width = width_;
    height = height_;
}

void Map::generateIslands()
{
    /* initialize random seed: */
    srand(time(NULL));

    /* generate random number between 1 and 10: */
    int numberOfIslands = rand() % 10 + 1;
    // islands[numberOfIslands];

    // for (int i = 0; i < numberOfIslands; i++)
    //     islands[i] = Island();
}

void Map::addShip(Ship* ship_, int x, int y)
{
    ship = ship_;
    shipLocation.first = x;
    shipLocation.second = y;
}

string Map::printMapView(int x, int y, int width, int height)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
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

