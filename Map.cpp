#include "Map.h"
#include "objectDefs/CoreObjects.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <string>
#include <vector>
#include "PDCurses/curses.h"

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
    // islandMap =
    // {{' ', 'L', 'L', ' ',' '},
    //  {' ', 'L', 'L', 'L',' '},
    //  {'L', 'L', 'L', 'L','L'},
    //  {' ', 'L', 'L', 'L',' '},
    //  {' ', ' ', 'P', ' ',' '}
    // };
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
    start_color();

    init_pair(1, COLOR_BLUE, COLOR_BLUE); // water
    init_pair(2, COLOR_BLACK, COLOR_BLACK); // ship
    init_pair(3, COLOR_GREEN, COLOR_GREEN); // island
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

void Map::printMapView(int x, int y, int width, int height)
{
    // stuff for the console
    start_color();

    init_pair(1, COLOR_BLUE, COLOR_BLUE); // water
    init_pair(2, COLOR_BLACK, COLOR_BLACK); // ship
    init_pair(3, COLOR_GREEN, COLOR_GREEN); // island

    clear();
    int cols, rows;
    getmaxyx(stdscr, rows, cols);

    attron(COLOR_PAIR(1)); // setting the water attribute

    // for making the water
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            addch(' '); // add the blank space
        }
    }
    attroff(COLOR_PAIR(1));
    for (int i = 0; i < cols; i++)
    {
        for (int j = 0; j < rows; j++)
        {
            for (auto island = islands.begin(); island != islands.end(); ++island)
            {
                if ((x + j) == island->getLocation().first && (y + i) == island->getLocation().second)
                {
                    attron(COLOR_PAIR(3));
                    mvaddch(j, i, ' ');
                    attroff(COLOR_PAIR(3));
                }
            }
            if ((x + j) == shipLocation.first && (y + i) == shipLocation.second)
            {
                attron(COLOR_PAIR(2));
                mvaddch(j, i, ' ');
                attroff(COLOR_PAIR(2));
            }
        }
    }
    refresh();
}

