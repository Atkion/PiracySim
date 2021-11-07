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
  Ship BastardsGreed (100, 20, 4, 10, 1200, "The Bastard's Greed");

  Crewmate Rhys (60, 25, 1, 8, "Rhys");
  Crewmate Creak (35, 12, 9, 5, "Creak");
  Crewmate Aiirbjorne (40, 18, 4, 7, "Aiirbjorne");
  Crewmate ChaCha (120, 20, -10, 20, "Cha Cha");
  Crewmate *crew[] = {&Rhys, &Creak, &Aiirbjorne, &ChaCha};

  Weapon Ballista (100, 60, 50, 20, 2, "Ballista");
  Weapon Ballista2 (100, 60, 50, 20, 2, "Ballista");
  Weapon Cannon (50, 10, 15, 15, 1, "Basic Cannon");
  Weapon *weapons[] = {&Ballista, &Ballista2, &Cannon};

  for (int i=0; i<4; i++) BastardsGreed.addCrew(crew[i]);
  for (int i=0; i<3; i++) BastardsGreed.addWeapon(weapons[i]);
  Ballista.assignCrew(&Rhys); Ballista.assignCrew(&Creak);
  Cannon.assignCrew(&ChaCha);
  Ballista2.assignCrew(&Aiirbjorne);

  BastardsGreed.printInfo();

  int i; cin >> i; //This is just to keep the console open because nothing else keeps the game running yet
}