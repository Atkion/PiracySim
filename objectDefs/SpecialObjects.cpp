#include "SpecialObjects.h"
#include "CoreObjects.h"
#include <cstdio>
#include <stdlib.h>
#include <iostream>
#include <time.h>

using namespace std;
//Subclasses for Crewmates, Weapons, and Ships will go here.
//A subclass is NOT needed for every different crew/weapon/ship, only if they have some special feature that is not possible to implement using the base stats
//The virtual functions that can be overloaded in special classes are currently:
/*
* Crewmate::printInfo()
* Crewmate::specialEffects()
*
* Weapon::printInfo()
* Weapon::specialEffects()
* Weapon:: attack()
*
* Ship::printInfo()
* Ship::specialEffects()
*/

//No idea if this one will make it into the final game, more of an example than anything
Musketeer::Musketeer(int hp, int ar, int aB, int dB, string n, string d) : Crewmate(hp, ar, aB, dB, n, d) { specialType = eachTurn; special = "Special Effect (Musketeer): If this crewmate is not assigned to a weapon, they will make an independent rifle attack against an enemy weapon every turn.";} //Call parent constructor then set specialType
Musketeer::Musketeer() : Crewmate() { specialType = eachTurn; special = "Special Effect (Musketeer): If this crewmate is not assigned to a weapon, they will make an independent rifle attack against an enemy weapon every turn.";} //Default constructor

void Musketeer::printInfo() { 
    Crewmate::printInfo();
    cout << "Special Effect (Musketeer): If this crewmate is not occupied, they will make an extra rifle attack against a random enemy weapon every turn (Acc 50%, Dmg 25)\n";
}
//Get the enemy ship pointer from the combat loop somehow, will need to be built into the handler. The current method is fucked, unsafe, and frankly unacceptable.
void Musketeer::specialEffects() {//todo: fix this terrible fucking thing when possible
    Ship *enemyShip;
    srand(time(NULL));
    for (int i=0; i<((Ship*)ship)->weaponSlots; i++) { //This is pretty damn safe now, but not technically perfect. Idk how to do it better without an actual pointer to the enemy.
        if (((Ship*)ship)->getWeapons()[i] != nullptr && ((Ship*)ship)->getWeapons()[i]->target != nullptr) {
            enemyShip = (Ship*)(((Ship*)ship)->getWeapons()[i]->target->ship);
            if (enemyShip->isValid()) break;
        }
    }
    if (!occupied) {
        int i;
        enemyShip->printInfo();
        do { i = rand() % enemyShip->weaponSlots; } while (!enemyShip->getWeapons()[i]->isValid());

        if (rand() % 100 < 50) {
            //Somehow report success to combat log
            cout << name << " dealt 25 damage to " << enemyShip->getWeapons()[i]->name << ".\n";
            enemyShip->damage(25, enemyShip->getWeapons()[i]);
        }
        else {/*somehow report failure to combat log*/ cout << name << " missed their rifle attack.\n"; }
    }
}