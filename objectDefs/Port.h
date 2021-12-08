#pragma once

#include "CoreObjects.h"
#include "SpecialObjects.h"

class Port {
    public:
        Crewmate hires[6];
        Weapon weaponSale[6];
        Ship shipSale[6];

        Port();

        void enterPort(Ship *);
        void printMenu(Ship *, int);
        void printWeaponShop(Ship *, int, int, bool);
        void printCrewmateShop(Ship *, int, int, bool);
        void printShipShop(Ship *, int, int, bool);

};