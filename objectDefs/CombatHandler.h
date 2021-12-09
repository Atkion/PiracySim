#pragma once

#include "../Map.h"
#include "CoreObjects.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <string>
#include <vector>
#include "../PDCurses/curses.h"

class CombatHandler {// A combat handler represents an encounter, and the actions you can take within it.
    private:
        Ship* playerShip;
        Ship* enemyShip;

    public:
        CombatHandler(Ship* player, Ship* enemy);

        vector<string> combatLog;
        Ship* getPlayer();
        Ship* getEnemy();

        void enterCombat();
        void printCombatMenu(int);
        void printInfoMenu();
        void printConfigMenu(int, int, int, int, int);
        bool checkGameState();
        void printShip(int, int);
        void win(); 
        void lose();
        void flee();
};