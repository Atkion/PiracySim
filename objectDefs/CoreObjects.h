#pragma once
#include <string>
using std::string;

class Crewmate {
  protected:
    int health, armorRating; //use getStats() to get/display
    bool alive; //use isValid() to check
  public:
    bool occupied;
    void* ship; //YES I KNOW HOW SCUFFED THESE ARE, I had to do this because the ship and weapon classes aren't defined yet so I can't have pointers to them
    void* assignedTo;
    const int accBonus, dmgBonus, maxHealth;
    const string name, desc;
    enum specialTypes {none, eachAttack, eachTurn} specialType; //add more options to this as need be

    Crewmate(int, int, int, int, string, string);
    Crewmate();

    bool isValid();
    void damage(int);
    virtual void printInfo();
    virtual void specialEffects() {}; //keep this empty, it's meant to be overloaded by subclasses
    int* getStats();
};

class Weapon {
  protected: 
    int atkDamage, accuracy, armorRating, health;
    Crewmate **assignedCrew;
    bool operational;
  public:
    void* ship; //see above where I do the same thing
    const int maxHealth, crewmateSlots;
    const string name, desc;
    enum specialTypes {none, eachAttack, eachTurn} specialType; //add more options to this as need be
    Weapon *target;

    Weapon (int, int, int, int, int, string, string);
    Weapon();

    bool isValid();
    void damage(int);
    bool rollHit();
    virtual bool attack();
    bool assignCrew(int, Crewmate*);
    bool unassignCrew(int);
    void unsetTarget();
    bool setTarget(Weapon*);
    int* getStats();
    Crewmate** getAssigned();
    virtual void printInfo();
    virtual void specialEffects() {}; //keep this empty, it's meant to be overloaded by subclasses
    bool isOperational(bool);
};

class Ship {
  protected:
    int armorRating, cargoHeld, health;
    Crewmate **crew;
    Weapon **weapons;
  public:
    void unsetWeapons();
    void autoConfigure(Ship*); //This automatically assigns crewmates to weapons without much strategy
    Ship* generateEncounter(); //This generates an appropriate-level enemy based on the Ship's CR
    const string name, desc;
    const int crewmateSlots, weaponSlots, cargoSize, maxHealth;
    enum specialTypes {none, eachAttack, eachTurn} specialType; //add more options to this as need be

    Ship (int, int, int, int, int, string, string);
    Ship();

    void damage (int, Weapon*);
    int emptyCargo();
    int repair();
    void reinforce(int);
    int* getStats();
    bool addCrew(Crewmate*);
    bool addWeapon(Weapon*);
    bool switchCrew(int, Crewmate*);
    bool switchWeapon(int, Weapon*);
    virtual void runAttacks();
    virtual void printInfo();
    Crewmate** getCrew();
    Weapon** getWeapons();
    virtual void specialEffects() {}; //keep this empty, it's meant to be overloaded by subclasses
};
