#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>
#include "CoreObjects.h"

using namespace std;

Crewmate::Crewmate(int hp, int ar, int aB, int dB, string n, string d) : //main constructor
  maxHealth(hp), accBonus(aB), dmgBonus(dB), name(n), desc(d) {
    health = hp; armorRating = ar;
    alive = true; occupied = false;
    specialType = none; ship = nullptr; assignedTo = nullptr;
}
Crewmate::Crewmate() : maxHealth(0), accBonus(0), dmgBonus(0), alive(true), name("Default") {} //Default constructors for placeholders in arrays

bool Crewmate::isValid() { //A simple check to see if the default constructor was used and if crewmate is alive
  return this != nullptr && maxHealth != 0 && alive;
}
void Crewmate::damage(int atkdmg) { //for use when crewmate TAKES damage. Probably only going to be called by Weapon::damage and by extension Ship::damage, since crew can't be targets.
  health -= ( (atkdmg - armorRating >= 0) ? atkdmg - armorRating : 0);
  cout << name << " has taken " << ( (atkdmg - armorRating >= 0) ? atkdmg - armorRating : 0) << " damage.\n";
  if (health <= 0) {
    alive = false;
    cout << name << " has died.\n";
  } 
}
int* Crewmate::getStats() {
  int *temp = new int[5]; //HP, maxHP, AR, dmgBonus, accBonus
  temp[0] = health; temp[1] = maxHealth; temp[2] = armorRating; 
  temp[3] = dmgBonus; temp[4] = accBonus;
  return temp;
}
void Crewmate::printInfo() { //mostly for testing purposes.
  cout << "Crewmate Name: " << name << ", onboard ship: " << (ship == nullptr ? "None" : ((Ship*)ship)->name) 
  << ", assigned to weapon: " << (assignedTo == nullptr ? "None" : ((Weapon*)assignedTo)->name) << "\n";
  cout << "Description: " << desc << "\n";
  printf("HP: %d/%d, AR: %d, Accuracy Bonus: %d, Damage Bonus: %d, Occupied: %s\n\n", health, maxHealth, armorRating, accBonus, dmgBonus, occupied ? "true":"false");
}

Weapon::Weapon (int hp, int dmg, int acc, int ar, int cS, string n, string d) : //main constructor
      maxHealth(hp), crewmateSlots(cS), name(n), desc(d) {
        health = hp;
        atkDamage = dmg; accuracy = acc; armorRating = ar;
        operational = true;
        assignedCrew = new Crewmate*[crewmateSlots];
        for (int i=0; i<crewmateSlots; i++) assignedCrew[i] = nullptr;
        specialType = none; target = nullptr; ship = nullptr;
      }
Weapon::Weapon() : maxHealth(0), crewmateSlots(0) {} //Default constructors for placeholders in arrays

bool Weapon::isValid() { //A simple check to see if the default constructor was used
  return this != nullptr && maxHealth != 0;
}
void Weapon::damage(int atkdmg) { //For use when a weapon TAKES damage. Should really only be called by Ship::damage(). Inflicts damage on all assigned crewmates.
  health -= ( (atkdmg - armorRating >= 0) ? atkdmg - armorRating : 0);
  cout << name << " has taken " << ( (atkdmg - armorRating >= 0) ? atkdmg - armorRating : 0) << " damage.\n";
  if (health <= 0) {
    operational = false;
    cout << name << " has been broken is is no longer operable.\n";
  }
  for (int i=0; i<crewmateSlots; i++)
    if (assignedCrew[i]->isValid())
      assignedCrew[i]->damage(atkdmg - armorRating); //So the final damage to crewmates is atkdmg - shipAR - weaponAR - crewAR. Should it be this way?
}
bool Weapon::rollHit() { //Adds up baseAccuracy and all crew modifiers and rolls to hit.
  if (isValid() && operational) {
    srand(time(NULL));
    int mod = 0;
    for (int i=0; i<crewmateSlots; i++)
      if (assignedCrew[i]->isValid()) 
        mod += assignedCrew[i]->accBonus;
    return (rand() % 100) + 1 < (accuracy + mod);
  }
  return false;
}
bool Weapon::attack() { //Adds up baseDamage and all crew modifiers and returns the total attack damage. Actually inflicting this damage should be handled along with targeting in the main combat loop.
  if (isValid() && operational) {
    int mod = 0;
    for (int i=0; i<crewmateSlots; i++)//calculate the modifier
      if (assignedCrew[i]->isValid()) {
        mod += assignedCrew[i]->dmgBonus;
        if (assignedCrew[i]->specialType == Crewmate::specialTypes::eachAttack) assignedCrew[i]->specialEffects(); // run crew special onAttack effects
      }
    if (specialType == eachAttack) specialEffects(); // run weapon special onAttack effects
    if (target->isValid()) {
      if (rollHit()) {
        cout << name << " dealt " << atkDamage+mod << " damage to " << target->name <<". \n";
        ((Ship*)(target->ship))->damage(atkDamage+mod, target);
        return true;
      }
      else cout << name << " missed.\n";
    }
  }
  return false;
}
bool Weapon::assignCrew(int slot, Crewmate* c) { //Assigns a crewmate to a weapon in the slot provided.
  if (slot<crewmateSlots && c->isValid() && !c->occupied) {
    if (assignedCrew[slot]->isValid()) {
      assignedCrew[slot] -> occupied = false;
      assignedCrew[slot] -> assignedTo = nullptr;
    }
    assignedCrew[slot] = c;
    assignedCrew[slot] -> occupied = true;
    assignedCrew[slot] -> assignedTo = (void*)this;
    return true;
  }
  return false;
}
bool Weapon::setTarget(Weapon *t) {
  if (t->isValid()) {
    target = t;
    return true;
  }
  return false;
}
bool Weapon::isOperational(bool v = false) { //send in true to toggle operability, used to query operability since it's private
  if (v) operational = !operational;
  return operational;
}
Crewmate** Weapon::getAssigned() {return assignedCrew;}
int* Weapon::getStats() {
  int equippedC = 0, dmgTotal = atkDamage, accTotal = accuracy;
  for (int i=0; i<crewmateSlots; i++) {
    if (assignedCrew[i]->isValid()) {
      equippedC++;
      dmgTotal += assignedCrew[i]->dmgBonus;
      accTotal += assignedCrew[i]->accBonus;
    }
  }
  int *temp = new int[9]; //HP, maxHP, AR, CrewmateSlots, CrewmatesEquipped, BaseDamage, BaseAccuracy, totalDamage, totalAccuracy
  temp[0] = health; temp[1] = maxHealth; temp[2] = armorRating; 
  temp[3] = crewmateSlots; temp[4] = equippedC;  
  temp[5] = atkDamage; temp[6] = accuracy; temp[7] = dmgTotal; temp[8] = accTotal;
  return temp;
}
void Weapon::printInfo() { //mostly for testing purposes.
  int* stats = getStats();
  cout << "Weapon Name: " << name << ", on ship: " << (ship == nullptr ? "None" : ((Ship*)ship)->name) << "\n";
  cout << "Description: " << desc << "\n";
  printf("HP: %d/%d, AR: %d, Base Accuracy: %d, Base Damage: %d\n", health, maxHealth, armorRating, accuracy, atkDamage);
  printf("Crewmate Assignment Slots Filled: %d/%d, Operational: %s\n", stats[4], stats[3], isOperational() ? "true":"false");
  printf("Effective Damage: %d, Effective Accuracy: %d\n\n", stats[7], stats[8]);
}

Ship::Ship (int hp, int baseArmor, int weaponS, int crewmateS, int cargoS, string n, string d) : //main constructor
  maxHealth(hp), weaponSlots(weaponS), crewmateSlots(crewmateS), cargoSize(cargoS), name(n), desc(d)
  {
    health = hp;
    armorRating = baseArmor; 
    cargoHeld = 0;
    crew = new Crewmate*[crewmateSlots];
    weapons = new Weapon*[weaponSlots];
    for (int i=0; i<crewmateSlots; i++) crew[i] = nullptr;
    for (int i=0; i<weaponSlots; i++) weapons[i] = nullptr;
    specialType = none;
  }
Ship::Ship () : maxHealth(0), weaponSlots(0), crewmateSlots(0), cargoSize(0), name(""), desc("") {} //default constructor for placeholders in arrays

void Ship::damage (int atkdmg, Weapon *target) { //for when a ship TAKES damage. ONLY CALL THIS ONE from the combat handler, passing in a pointer to the target weapon. Weapon::damage should be called from here only.
  health -= ( (atkdmg - armorRating >= 0) ? atkdmg - armorRating : 0);
  cout << name << " has been hit and has taken " << ( (atkdmg - armorRating >= 0) ? atkdmg - armorRating : 0) << " damage. ";
  cout << "Target hit: " << target->name << "\n";
  if (health <= 0) {
    /*Send Game Over state somehow */
    cout << name << " has capsized!";
  }
  if (target->isValid())
    target->damage(atkdmg - armorRating);
}
int Ship::emptyCargo() { //For cashing out at port. Returns the number of cargo held before emptying, to add to money(?)
  int temp = cargoHeld;
  cargoHeld = 0;
  return temp;
}
int Ship::repair() { //returns the value to deduct from money in game handler (it just costs the amount of health repaired, this can be changed to balance if needed)
  int temp = health;
  health = maxHealth;
  return maxHealth - temp;
}
void Ship::reinforce(int amt) { //todo: Probably balance this shit somehow, seems pretty broken if you can do it infinitely
  armorRating += amt; //price and money handling will be done in main game loop, this is just the stat change
}
int* Ship::getStats() {
  int equippedW = 0, equippedC = 0;
  for (int i=0; i<weaponSlots; i++)
    if (weapons[i]->isValid())
      equippedW++;
  for (int i=0; i<crewmateSlots; i++)
    if (crew[i]->isValid())
      equippedC++;
  int *temp = new int[9]; //HP, maxHP, AR, WeaponSlots, WeaponsEquipped, CrewmateSlots, CrewmatesEquipped, CargoSize, CargoHeld
  temp[0] = health; temp[1] = maxHealth; temp[2] = armorRating; 
  temp[3] = weaponSlots; temp[4] = equippedW;  
  temp[5] = crewmateSlots; temp[6] = equippedC;
  temp[7] = cargoSize; temp[8] = cargoHeld;
  return temp;
}
bool Ship::addCrew(Crewmate* c) { //Finds an empty crew slot and adds crew to it. Switching out crewmates is not yet implemented, they are permanently on the crew if added so far
  for (int i=0; i<crewmateSlots; i++) {
    if (!crew[i]->isValid()) {
      crew[i] = c;
      crew[i]->ship = (void*)this;
      return true;
    }
  }
  return false;
}
bool Ship::addWeapon(Weapon* w) {//Finds an empty weapon slot and adds weaopn to it. Switching out weapons is not yet implemented, they are permenantly fixed if added to the ship so far
  for (int i=0; i<weaponSlots; i++) {
    if (!weapons[i]->isValid()) {
      weapons[i] = w;
      weapons[i]->ship = (void*)this;
      return true;
    }
  }
  return false;
}
// Maybe merge these two with addCrew and addWeapon? Currently those ones just fill any empty slot available, but clearly we need a way
// to switch out weapons/crewmates when all the slots are full. With Weapon::assignCrew, there are few enough slots that you can just manually choose, but
// if a ship has 30 crew slots it doesn't really make sense to force players to figure out slots rather than just filling in crew until it's full, then switching
// them out by name if necessary. Thoughts? Maybe we run addCrew/addWeapon, and if it returns false (it couldn't find a slot) then enter a menu where they can switch
// using this method?
bool Ship::switchCrew(Crewmate *out, Crewmate *in) { //todo: implement these if it turns out they're necessary
  return true;
}
bool Ship::switchWeapon(Weapon *out, Weapon *in) { 
  return true;
}
void Ship::runAttacks() {
  for (int i=0; i<weaponSlots; i++)
    if (weapons[i]->isValid()) weapons[i]->attack();
  for (int i=0; i<crewmateSlots; i++)
    if (crew[i]->isValid() && crew[i]->specialType == Crewmate::specialTypes::eachTurn) crew[i]->specialEffects();
}
Crewmate** Ship::getCrew() {return crew;}
Weapon** Ship::getWeapons() {return weapons;}
void Ship::printInfo() { //mostly for testing purposes.
  int* stats = getStats();
  cout << "Ship Name: " << name << "\n";
  cout << "Description: " << desc << "\n";
  printf("HP: %d/%d, AR: %d\n", health, maxHealth, armorRating);
  printf("Weapon Slots Filled: %d/%d, Crewmate Slots Filled: %d/%d\n", stats[4], stats[3], stats[6], stats[5]);
  printf("Printing Weapons: \n\n");
  for (int i=0; i<weaponSlots; i++)
    if (weapons[i]->isValid())
      weapons[i]->printInfo();
  printf("Printing Crew: \n\n");
  for (int i=0; i<crewmateSlots; i++)
    if (crew[i]->isValid())
      crew[i]->printInfo();
  printf("\n\n");
}
