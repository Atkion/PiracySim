#pragma once
#include "CoreObjects.h"
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
* Ship::runAttacks()
*/
class Musketeer: public Crewmate { //No idea if this will make it into the final game, more of an example than anything
    public:
        Musketeer(int, int, int, int, string, string);
        Musketeer();
        void printInfo();
        void specialEffects();
};
