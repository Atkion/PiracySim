#pragma once
#include <string>
using std::string;

class Crewmate {
  public:
    int health, armorRating;
    bool alive;
    const int accBonus, dmgBonus, maxHealth;
    string name, desc;

    Crewmate(int, int, int, int, string);
    Crewmate();

    bool isValid();
    void damage(int);
    void printInfo();
};

class Weapon {
  public:
    int atkDamage, accuracy, armorRating, health;
    bool operational;
    const int maxHealth, crewmateSlots;
    string name, desc;
    Crewmate **assignedCrew;

    Weapon (int, int, int, int, int, string);
    Weapon();

    bool isValid();
    void damage(int);
    bool rollHit();
    int attack();
    bool assignCrew(Crewmate*);
    int* getStats();
    void printInfo();
};

class Ship {
  public:
    int armorRating, cargoHeld, health;
    const int crewmateSlots, weaponSlots, cargoSize, maxHealth;
    string name, desc;
    Crewmate **crew;
    Weapon **weapons;

    Ship (int, int, int, int, int, string);

    void damage (int, Weapon*);
    int emptyCargo();
    int repair();
    void reinforce(int);
    int* getStats();
    bool addCrew(Crewmate*);
    bool addWeapon(Weapon*);
    void printInfo();
};
