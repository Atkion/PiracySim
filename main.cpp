//#define _GLIBCXX_USE_CXX11_ABI 0
#include "WindowSetup.h"
#include "objectDefs/CoreObjects.h"
#include "objectDefs/SpecialObjects.h"
#include "Map.h"
#include "PDCurses/curses.h"
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string>

using namespace std;

int main (void)
{
  setupGameWindow();
  //220 characters wide
  //60 lines tall

  Ship BastardsGreed (1000, 20, 4, 10, 1200, "The Bastard's Greed", "Boat");

  Crewmate Rhys (60, 25, 0, 60, "Rhys", "Listless Paladin");
  Crewmate Creak (35, 12, 25, 40, "Creak", "Hexblade Warlock");
  Crewmate Aiirbjorne (40, 38, 4, 35, "Aiirbjorne", "Devout Cleric");
  Crewmate ChaCha (120, 20, -10, 100, "Cha Cha", "Enraged Barbarian");
  Musketeer Azure(50, 10, 20, 50, "Azure", "Alcoholic Child"); //Testing occupied variable
  Crewmate *crew[] = {&Rhys, &Creak, &Aiirbjorne, &ChaCha, &Azure};

  Weapon Ballista (100, 60, 50, 20, 2, "Ballista", "Massive bolt-firing weapon");
  Weapon Ballista2 (100, 60, 50, 20, 2, "Ballista", "Massive bolt-firing weapon");
  Weapon Cannon (50, 10, 15, 15, 1, "Cannon", "Basic Side Cannon");
  Weapon *weapons[] = {&Ballista, &Ballista2, &Cannon};

  for (int i=0; i<5; i++) BastardsGreed.addCrew(crew[i]);
  for (int i=0; i<3; i++) BastardsGreed.addWeapon(weapons[i]);
  Ballista.assignCrew(0, &Rhys); Ballista.assignCrew(1, &Creak); Cannon.assignCrew(0, &Azure); Ballista2.assignCrew(0, &Aiirbjorne); 
  Cannon.assignCrew(0, &ChaCha); //Replace Azure with ChaCha, so Azure is no longer busy but is still on the ship

  Weapon CannonReplacement (50, 10, 15, 15, 1, "Cannon 2", "Basic Side Cannon 2");
  BastardsGreed.switchWeapon(2, &CannonReplacement); //Replace Cannon with Cannon 2, Cannon is no longer on the ship.
  //Note that this last line unassigns ChaCha from Cannon, and assigns him to Cannon 2. This should only be called from port.
  Crewmate Braids (20, 0, 95, 0, "Braids", "Feverish Artificer");
  BastardsGreed.switchCrew(3, &Braids); //Replace ChaCha with Braids, ChaCha is no longer on the ship.
  //Note that this last line unassigns ChaCha from Cannon2 and BastardsGreed, and assigns Braids in his place. This should only be called from port,
  //UNLESS we implement the ability to recruit survivors from defeated enemy ships? Is this worth doing? Maybe only add them to empty crew slots, because we'd have to throw an existing crewmate overboard to use this at sea lol

  Ship EnemyShip (1500, 20, 4, 10, 1200, "Enemy Ship", "Enemy Ship"); //Crewmate usually dies if RNG favors, testing weapons breaking and crewmates dying. Also tests accuracy RNG, seems to work okay so far (need to be more liberal on acc bonuses tho)
  Weapon EnemyCannon (1000, 140, 15, 15, 1, "Enemy Cannon", "Enemy Cannon");
  Crewmate EnemyCrew (200, 5, 40, 10, "Enemy Crewmate", "Enemy Crewmate");
  EnemyShip.addCrew(&EnemyCrew); EnemyShip.addWeapon(&EnemyCannon); EnemyCannon.assignCrew(0, &EnemyCrew); //Enemy ship setup

  Ballista.setTarget(&EnemyCannon); Ballista2.setTarget(&EnemyCannon); Cannon.setTarget(&EnemyCannon); //Setting sights on enemy cannon
  EnemyCannon.setTarget(&Ballista);

  BastardsGreed.runAttacks(); 
  EnemyShip.runAttacks();
  //Attack roll, adds up Weapon Accuracy + All Assigned Crew Accbonuses.
  //Damages EnemyShip, and also Damages EnemyCannon (the target) as well as every crewmate operating it

  //Hitting another weapon on miss is not implemented, and probably won't be at this point. Doesn't seem worth the effort, if you disagree let me know lol
  //Also Accuracy numbers are percentage chance to hit, so if total accuracy is over 100 you will always hit.
  
  EnemyShip.printInfo();

  BastardsGreed.printInfo();

  /*-------------------------------------------- 
    |             Map initialization            |
    --------------------------------------------*/
  Map map(1000, 1000);
  map.addShipLocation(500, 500);
  map.generateIsland(502, 502);
  map.printMapView(480, 480, 40, 40);
  // map.moveShip(1,1);
  // map.printMapView(480, 480, 40, 40);

   //This is just to keep the console open because nothing else keeps the game running yet
  initscr();
  // for (int i=0; i<10;i++) beep(); //https://github.com/wmcbrine/PDCurses/blob/master/docs/MANUAL.md for documentation on this shit
  // chtype test(1);
  // start_color();
  // init_pair(1, COLOR_RED, COLOR_BLUE);
  // attron(COLOR_PAIR(1));
  // for (int i = 0; i < 500; i++) addstr("This is a test!");
  // mvprintw(10, 10, "test");
  // refresh();
  int i; cin >> i;
}