//#define _GLIBCXX_USE_CXX11_ABI 0
#include "WindowSetup.h"
#include "CoreObjects.h"
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string>

using namespace std;

int main (void)
{
  setupGameWindow();
  Ship BastardsGreed (1000, 20, 4, 10, 1200, "The Bastard's Greed", "Boat");

  Crewmate Rhys (60, 25, 1, 8, "Rhys", "Listless Paladin");
  Crewmate Creak (35, 12, 9, 5, "Creak", "Hexblade Warlock");
  Crewmate Aiirbjorne (40, 18, 4, 7, "Aiirbjorne", "Devout Cleric");
  Crewmate ChaCha (120, 20, -10, 20, "Cha Cha", "Enraged Barbarian");
  Crewmate Azure(50, 10, 6, 6, "Azure", "Alcoholic Child"); //Testing occupied variable
  Crewmate *crew[] = {&Rhys, &Creak, &Aiirbjorne, &ChaCha, &Azure};

  Weapon Ballista (100, 60, 50, 20, 2, "Ballista", "Massive bolt-firing weapon");
  Weapon Ballista2 (100, 60, 50, 20, 2, "Ballista", "Massive bolt-firing weapon");
  Weapon Cannon (50, 10, 15, 15, 1, "Cannon", "Basic Side Cannon");
  Cannon.damage(65); Ballista.damage(100);
  Weapon *weapons[] = {&Ballista, &Ballista2, &Cannon};

  for (int i=0; i<5; i++) BastardsGreed.addCrew(crew[i]);
  for (int i=0; i<3; i++) BastardsGreed.addWeapon(weapons[i]);
  Ballista.assignCrew(0, &Rhys); Ballista.assignCrew(1, &Creak);
  Cannon.assignCrew(0, &ChaCha);
  Ballista2.assignCrew(0, &Aiirbjorne);

  Ship EnemyShip (1500, 20, 4, 10, 1200, "EnemyShip", "EnemyShip"); //Crewmate usually dies if RNG favors, testing weapons breaking and crewmates dying. Also tests accuracy RNG, seems to work okay so far (need to be more liberal on acc bonuses tho)
  Weapon EnemyCannon (1000, 10, 15, 15, 1, "Cannon", "Basic Side Cannon");
  Crewmate EnemyCrew (200, 30, 40, 10, "Enemy", "Enemy");
  EnemyShip.addCrew(&EnemyCrew); EnemyShip.addWeapon(&EnemyCannon); EnemyCannon.assignCrew(0, &EnemyCrew);
  for (int i=0; i<30; i++) 
    if (Ballista.rollHit()) //Attack roll, adds up Weapon Accuracy + All Crew Accbonuses. Special Effects aren't implemented yet because subclasses/characters don't exist yet lol
      EnemyShip.damage(Ballista.attack(), &EnemyCannon); //Damages EnemyShip, and also Damages EnemyCannon (the target) as well as every crewmate operating it
  
  EnemyShip.printInfo();

  BastardsGreed.printInfo();

  int i; cin >> i; //This is just to keep the console open because nothing else keeps the game running yet
}