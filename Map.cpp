#include "Map.h"
#include "objectDefs/CoreObjects.h"
#include "objectDefs/Port.h"
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
    
    /* generate random number between 1 and 10: */
    int chance;

    // for (int i = 0; i < 5; i++)
    //     for (int j = 0; j < 5; j++)
}

Island::Island(int x, int y)
{
    location = pair<int, int>(x, y);
    islandSize = sizeof(islandMap) / sizeof(islandMap[0]);
    for (int a = 0; a < islandSize; a++)
    {
        for(int b = 0; b < islandSize; b++)
        {
            islandMap[a][b] = false;
        }
    }
    formIsland(islandSize / 2,islandSize / 2,100,islandSize,0);
    for (int y = islandSize / 2; y < islandSize; y++)
    {
        if (islandMap[y][islandSize / 2] == false)
        {
            port.first = islandSize / 2;
            port.second = y;
            break;
        }
    }
    for (int a = 0; a < islandSize; a++)
    {
        for(int b = 0; b < islandSize; b++)
        {
            if (islandMap[a][b] == true)
                cout << "O";
            else
                cout << "W";
        }
        cout << "\n";
    }
}

pair<int, int> Island::getLocation()
{
    return location;
}

pair<int, int> Island::getPort()
{
    return port;
}

void Island::setLocation(int x, int y)
{
    location.first = x;
    location.second = y;
}

void Island::formIsland(int x, int y, int chance, int size, int direction)
{
    double c = 1.1;

    // base case
    if ((x < 0 || x > size) || (y < 0 || y > size))
        return;
    
    /* generate random number between 1 and 100: */
    int number = rand() % 100 + 1;
    
    if (number <= chance)
    {
        islandMap[y][x] = true;
        if (direction == 0)
        {
            formIsland(x - 1, y, chance / c, size, 1);
            formIsland(x + 1, y, chance / c, size, 3);
            formIsland(x, y + 1, chance / c, size, 2);
            formIsland(x, y - 1, chance / c, size, 4);
        }
        else if (direction == 1) // left
        {
            formIsland(x + 1, y, chance / c, size, 3);
            formIsland(x, y + 1, chance / c, size, 2);
            formIsland(x, y - 1, chance / c, size, 4);
        } 
        else if (direction == 2) // up
        {
            formIsland(x + 1, y, chance / c, size, 3);
            formIsland(x - 1, y, chance / c, size, 1);
            formIsland(x, y - 1, chance / c, size, 4);
        }
        else if (direction == 3) // right
        {
            formIsland(x - 1, y, chance / c, size, 1);
            formIsland(x, y + 1, chance / c, size, 2);
            formIsland(x, y - 1, chance / c, size, 4);
        }
        else // down
        {
            formIsland(x + 1, y, chance / 2, size, 3);
            formIsland(x - 1, y, chance / 2, size, 1);
            formIsland(x, y + 1, chance / 2, size, 2);
        }
        
    }
}

Map::Map(int width_, int height_, Ship* ship_)
{
    // set the insace variables
    ship = ship_;
    width = width_;
    height = height_;

    /* initialize random seed: */
    srand(time(NULL));

    start_color();

    init_pair(1, COLOR_BLUE, COLOR_BLUE); // water
    init_pair(2, COLOR_BLACK, COLOR_BLACK); // ship
    init_pair(3, COLOR_GREEN, COLOR_GREEN); // island
    init_pair(4, COLOR_MAGENTA, COLOR_MAGENTA); // port
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
    srand(time(nullptr));

    /* generate random number between 1 and 10: */
    int numberOfIslands = rand() % 10 + 1;
    // islands[numberOfIslands];

    // for (int i = 0; i < numberOfIslands; i++)
    //     islands[i] = Island();
}

void Map::generateIsland(int x, int y)
{
    Island temp(x,y);
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
    int x = shipLocation.first + xAmount;
    int y = shipLocation.second + -yAmount;
    
    // loop through islands to check for collisions.
    for (auto island = islands.begin(); island != islands.end(); ++island)
    {
        int ix = island->getLocation().first;
        int iy = island->getLocation().second;
        for (int a = 0; a < island->islandSize; a++)
        {
            for (int b = 0; b < island->islandSize; b++)
            {
                if (x == ix + b && y == iy + a && island->islandMap[b][a] == true)
                    return;
                if (x == ix + island->getPort().second && y == iy + island->getPort().first)
                {
                    Port port;
                    port.enterPort(ship);
                }
            }
        }
    }
    shipLocation.first += xAmount;
    shipLocation.second += -yAmount;
}

void Map::printMapView(int x, int y, int width, int height)
{
    // stuff for the console
    start_color();

    init_pair(1, COLOR_BLUE, COLOR_BLUE); // water
    init_pair(2, COLOR_BLACK, COLOR_BLACK); // ship
    init_pair(3, COLOR_BLACK, COLOR_GREEN); // island
    init_pair(4, COLOR_MAGENTA, COLOR_MAGENTA); // island

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
            // loop through the vector of islands
            for (auto island = islands.begin(); island != islands.end(); ++island)
            {

                if (x + j == island->getLocation().first && y + i  == island->getLocation().second)
                {
                    attron(COLOR_PAIR(3));
                    for (int a = 0; a < island->islandSize; a++)
                    {
                        for (int b = 0; b < island->islandSize; b++)
                        {
                            if ((island->islandMap[a][b]) == true)
                            {
                                // mvaddch(j + a, i + b, ' ');
                                mvaddch(j + a, (i + b) * 2, ' ');
                                mvaddch(j + a, (i + b) * 2 + 1, ' ');
                            }
                            else if (island->getPort().first == b && island->getPort().second == a)
                            {
                                attroff(COLOR_PAIR(3));
                                attron(COLOR_PAIR(4));
                                mvaddch(j + a, (i + b) * 2, ' ');
                                mvaddch(j + a, (i + b) * 2 + 1, ' ');
                                attroff(COLOR_PAIR(4));
                                attron(COLOR_PAIR(3));
                            }
                        }
                    }
                    attroff(COLOR_PAIR(3));
                }
            }
            if ((x + j) == shipLocation.first && (y + i) == shipLocation.second)
            {
                attron(COLOR_PAIR(2));
                mvaddch(j, i * 2, ' ');
                mvaddch(j, i * 2 + 1, ' ');
                attroff(COLOR_PAIR(2));
            }
        }
    }
    refresh();
}

void Map::enterMap()
{
    initscr();
  keypad(stdscr, TRUE);
  int ch;
  ch = getch();
  do
  {
    ch = getch();
    switch (ch)
    {
    case KEY_RIGHT:
      moveShip(0,-1);
      printMapView(480, 480, 0, 0);
      // printw("RIGHT");
      break;
    case KEY_LEFT:
      moveShip(0,1);
      printMapView(480, 480, 0, 0);
      // printw("LEFT");
      break;
    case KEY_UP:
      moveShip(-1,0);
      printMapView(480, 480, 0, 0);
      // printw("UP");
      break;
    case KEY_DOWN:
      moveShip(1,0);
      printMapView(480, 480, 0, 0);
      // printw("DOWN");
      break;
    default:
      printw("USE ARROW KEYS!");
      break;
    }
  } while (ch != KEY_END);
  
  ch = getch();

  if (ch == KEY_LEFT)
  {
      printw("Left arrow is pressed\n");
  }
  refresh();

  ch = getch();
}

