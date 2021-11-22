#include "../Map.h"
#include "CoreObjects.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <string>
#include <vector>
#include "../PDCurses/curses.h"
#include "CombatHandler.h"

CombatHandler::CombatHandler(Ship* player, Ship* enemy) {
    playerShip = player; 
    enemyShip = enemy;
    prevScreen = CombatHandler::Screen::Combat;

}
CombatHandler::CombatHandler(Ship* player) {
    playerShip = player; 
    enemyShip = player->generateEncounter();
    prevScreen = CombatHandler::Screen::Combat;
}
