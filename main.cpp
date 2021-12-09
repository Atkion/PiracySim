#include "WindowSetup.h"
#include "objectDefs/CoreObjects.h"
#include "objectDefs/SpecialObjects.h"
#include "objectDefs/CombatHandler.h"
#include "objectDefs/Port.h"
#include "Map.h"
#include "PDCurses/curses.h"
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <time.h>

using namespace std;

int main (void)
{
  setupGameWindow();
  //220 characters wide
  //60 lines tall
  Ship BastardsGreed (1000, 20, 4, 10, 1200, "The Bastard's Greed", "Boat"); //Setup the demo objects
  BastardsGreed.balance = 1234; BastardsGreed.cargoHeld = 400;

  Crewmate Rhys (60, 25, 0, 60, "Rhys", "Listless Paladin");
  Crewmate Creak (35, 12, 25, 40, "Creak", "Hexblade Warlock");
  Crewmate Aiirbjorne (40, 38, 4, 35, "Aiirbjorne", "Devout Cleric");
  Crewmate ChaCha (120, 20, -10, 100, "Cha Cha", "Enraged Barbarian");
  Musketeer Azure(50, 10, 20, 50, "Azure", "Alcoholic Child"); //Testing subclass
  Crewmate *crew = new Crewmate[100];
  crew[0] = Rhys; crew[1] = Creak; crew[2] = Aiirbjorne; crew[3] = ChaCha; crew[4] = Azure;
  BastardsGreed.crewmateStorage = crew; //These storage variables are so we have handles to them in the port/battle menus.

  Weapon Ballista (100, 60, 50, 20, 2, "Ballista", "Massive bolt-firing weapon");
  Weapon Ballista2 (100, 60, 50, 20, 2, "Ballista", "Massive bolt-firing weapon");
  Weapon Cannon (50, 10, 15, 15, 1, "Cannon", "Basic Side Cannon");
  Weapon *weapons = new Weapon[100];
  weapons[0] = Ballista; weapons[1] = Ballista2; weapons[2] = Cannon;
  BastardsGreed.weaponStorage = weapons;

  for (int i=0; i<5; i++) BastardsGreed.addCrew(&crew[i]);
  for (int i=0; i<3; i++) BastardsGreed.addWeapon(&weapons[i]);

  int wCR, cCR = 0;
  Ship EnemyShip = BastardsGreed.generateEncounter(&wCR, &cCR); //This also sets the values for wCR and cCR
  Crewmate* enemyCrew = new Crewmate[EnemyShip.crewmateSlots];
  Weapon* enemyWeapons = new Weapon[EnemyShip.weaponSlots];
  srand(time(NULL));
  for (int i=0; i<EnemyShip.weaponSlots; i++) {
    int weaponCR = (wCR/(EnemyShip.weaponSlots-i)) + ((rand()%80) - 40); //The CR for this current weapon
    if (i==EnemyShip.weaponSlots-1) weaponCR = wCR; //If it's the last one, simply 'spend' all remaining wCR
    enemyWeapons[i] = Weapon(Weapon::generateEnemy(weaponCR));
    EnemyShip.addWeapon(&enemyWeapons[i]);
    wCR -= weaponCR;
  }
  for (int i=0; i<EnemyShip.crewmateSlots; i++) {
    int crewmateCR = (cCR/(EnemyShip.crewmateSlots-i)) + ((rand()%40) - 20); //The CR for this current crewmate
    if (i==EnemyShip.crewmateSlots-1) crewmateCR = cCR; //If it's the last one, simply 'spend' all remaining cCR
    enemyCrew[i] = Crewmate(Crewmate::generateEnemy(crewmateCR));
    EnemyShip.addCrew(&enemyCrew[i]);
    cCR -= crewmateCR;
  }
  
  CombatHandler encounter(&BastardsGreed, &EnemyShip);
  encounter.enterCombat();

  /*-------------------------------------------- 
    |             Map initialization            |
    --------------------------------------------*/
  Map map(1000, 1000, &BastardsGreed);
  map.addShipLocation(500, 500);
  map.generateIsland(480, 490);
  map.generateIsland(490, 540);
  map.generateIsland(500, 515);

  map.enterMap();
}