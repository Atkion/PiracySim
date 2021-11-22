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
        const Ship* playerShip;
        const Ship* enemyShip;
        enum Screen {Combat, Info, Config} prevScreen;

    public:
        CombatHandler(Ship* player, Ship* enemy);
        CombatHandler(Ship* player); //Auto-generates a random enemy based on challenge rating of the player

        vector<string> combatLog;
        Ship* getPlayer();
        Ship* getEnemy();

        void displayCombatScreen();
        void displayInfoScreen();
        void displayConfigScreen();
        void win();
        void flee();
        void attack();


};