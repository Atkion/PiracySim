#pragma once
#include <string>
#include <vector>
using namespace std;

class Crewmate {
  public:
    int health, armorRating; //use getStats() to get/display
    bool alive; //use isValid() to check
    string special;
    bool occupied;
    void* ship; //YES I KNOW HOW SCUFFED THESE ARE, I had to do this because the ship and weapon classes aren't defined yet so I can't have pointers to them
    void* assignedTo;
    int accBonus, dmgBonus, maxHealth;
    string name, desc;
    enum specialTypes {none, eachAttack, eachTurn} specialType; //add more options to this as need be

    Crewmate(int, int, int, int, string, string);
    Crewmate();
    Crewmate(const Crewmate&);
    void operator=(const Crewmate&);

    static Crewmate generateEnemy(int);
    virtual int getCR()const;
    bool isValid();
    void damage(int, vector<string>*);
    virtual void printInfo();
    virtual void specialEffects(vector<string>*) {}; //keep this empty, it's meant to be overloaded by subclasses
    int* getStats()const;
};

class Weapon {
  protected: 
    int atkDamage, accuracy, armorRating, health;
    Crewmate **assignedCrew;
    bool operational;
  public:
    void* ship; //see above where I do the same thing
    int maxHealth, crewmateSlots;
    string name, desc, special;
    enum specialTypes {none, eachAttack, eachTurn} specialType; //add more options to this as need be
    Weapon *target;

    Weapon (int, int, int, int, int, string, string);
    Weapon();
    Weapon(const Weapon&);

    void operator=(const Weapon&);

    static Weapon generateEnemy(int);
    virtual int getCR()const;
    bool isValid();
    void damage(int, vector<string>*);
    bool rollHit();
    int repair();
    int getRepairCost();
    virtual bool attack(vector<string>*);
    bool assignCrew(int, Crewmate*);
    bool unassignCrew(int);
    void unsetTarget();
    bool setTarget(Weapon*);
    int* getStats()const;
    Crewmate** getAssigned();
    virtual void printInfo();
    virtual void specialEffects(vector<string>*) {}; //keep this empty, it's meant to be overloaded by subclasses
    bool isOperational(bool);
};

class Ship {
  protected:
    Crewmate **crew;
    Weapon **weapons;
  public:
    int armorRating, cargoHeld, health;
    Weapon *weaponStorage; Crewmate *crewmateStorage; //Don't worry about these, or ask me about them. Just another scuffed solution to another scuffed problem
    void unsetWeapons();
    void autoConfigure(Ship*); //This automatically assigns crewmates to weapons without much strategy
    Ship generateEncounter(int*, int*); //This generates an appropriate-level enemy based on the Ship's CR
    virtual int getCR()const; //Gets the challenge rating of a ship and everything on it.

    int balance;
    string name, desc, special;
    int crewmateSlots, weaponSlots, cargoSize, maxHealth;
    enum specialTypes {none, eachAttack, eachTurn} specialType; //add more options to this as need be

    Ship (int, int, int, int, int, string, string);
    Ship();
    Ship(const Ship&);
    void operator=(const Ship&);

    bool isValid()const;
    void damage (int, Weapon*, vector<string>*);
    void sellLoot();
    int repair();
    int getRepairCost();
    void reinforce(int);
    int* getStats()const;
    bool addCrew(Crewmate*);
    bool addWeapon(Weapon*);
    bool switchCrew(int, Crewmate*);
    bool switchWeapon(int, Weapon*);
    virtual void runAttacks(vector<string>*);
    virtual void printInfo();
    Crewmate** getCrew();
    Weapon** getWeapons();
    virtual void specialEffects(vector<string>*) {}; //keep this empty, it's meant to be overloaded by subclasses
};
