#pragma once
#include <string>
using std::string;

class Crewmate {
  private:
    int health, armorRating; //use getStats() to get/display
    bool alive; //use isValid() to check
  public:
    bool occupied;
    const int accBonus, dmgBonus, maxHealth;
    const string name, desc;

    Crewmate(int, int, int, int, string, string);
    Crewmate();

    bool isValid();
    void damage(int);
    void printInfo();
    void specialEffects(void (*)());
    int* getStats();
};

class Weapon {
  private: 
    int atkDamage, accuracy, armorRating, health;
    Crewmate **assignedCrew;
  public:
    bool operational;
    const int maxHealth, crewmateSlots;
    const string name, desc;

    Weapon (int, int, int, int, int, string, string);
    Weapon();

    bool isValid();
    void damage(int);
    bool rollHit();
    int attack();
    bool assignCrew(int, Crewmate*);
    int* getStats();
    void printInfo();
    void specialEffects(void (*)());
    bool isOperational(bool);
};

class Ship {
  private:
    int armorRating, cargoHeld, health;
    Crewmate **crew;
    Weapon **weapons;
  public:
    const string name, desc;
    const int crewmateSlots, weaponSlots, cargoSize, maxHealth;

    Ship (int, int, int, int, int, string, string);

    void damage (int, Weapon*);
    int emptyCargo();
    int repair();
    void reinforce(int);
    int* getStats();
    bool addCrew(Crewmate*);
    bool addWeapon(Weapon*);
    void printInfo();
};
