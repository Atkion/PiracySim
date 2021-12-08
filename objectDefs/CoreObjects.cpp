#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>
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
Crewmate::Crewmate(const Crewmate& c) {
  int* s = c.getStats();
  maxHealth = s[1]; accBonus = s[4]; dmgBonus = s[3]; name = c.name; desc = c.desc;
  health = s[0]; alive = c.alive; occupied = c.occupied; specialType = c.specialType; armorRating = c.armorRating;
}

void Crewmate::operator=(const Crewmate& c) {
  int* s = c.getStats();
  maxHealth = s[1]; accBonus = s[4]; dmgBonus = s[3]; name = c.name; desc = c.desc;
  health = s[0]; alive = c.alive; occupied = c.occupied; specialType = c.specialType;
  armorRating = c.armorRating;
}

bool Crewmate::isValid() { //A simple check to see if the default constructor was used and if crewmate is alive
  return this != nullptr && maxHealth != 0 && alive;
}

void Crewmate::damage(int atkdmg, vector<string> *combatLog) { //for use when crewmate TAKES damage. Probably only going to be called by Weapon::damage and by extension Ship::damage, since crew can't be targets.
  health -= ( (atkdmg - armorRating >= 0) ? atkdmg - armorRating : 0);
  combatLog->push_back(name + " has taken " + to_string((atkdmg - armorRating >= 0) ? atkdmg - armorRating : 0) + " damage.");
  //cout << name << " has taken " << ( (atkdmg - armorRating >= 0) ? atkdmg - armorRating : 0) << " damage.\n";
  if (health <= 0) {
    alive = false;
    combatLog->push_back(name + " has died.");
    //cout << name << " has died.\n";
  } 
}

Crewmate Crewmate::generateEnemy(int CR) {
  srand(time(NULL));
  int atkCCR = CR * (40 + (rand()%20)) / 100, statCCR = CR-atkCCR;
  int dmg = atkCCR * (40 + (rand()%20)) / 100, acc = (atkCCR-dmg)/2;
  int hp = statCCR * (40 + (rand()%20)) / 100, ar = (statCCR-hp)/2;

  Crewmate enemyCrewmate = Crewmate(hp, ar, acc, dmg, "EnemyCrewmate", "Placeholder");
  return enemyCrewmate;
}

int Crewmate::getCR() const{
  return (dmgBonus + (2*accBonus)) + (maxHealth+(2*armorRating));
}
int* Crewmate::getStats() const{
  int *temp = new int[6]; //HP, maxHP, AR, dmgBonus, accBonus, CR
  temp[0] = health; temp[1] = maxHealth; temp[2] = armorRating; 
  temp[3] = dmgBonus; temp[4] = accBonus; temp[5] = getCR();
  return temp;
}

void Crewmate::printInfo() { //mostly for testing purposes.
  cout << "Crewmate Name: " << name << ", Onboard Ship: " << (ship == nullptr ? "None" : ((Ship*)ship)->name) 
  << ", Assigned to Weapon: " << (assignedTo == nullptr ? "None" : ((Weapon*)assignedTo)->name) << "\n";
  cout << "Description: " << desc << "\n";
  printf("HP: %d/%d, AR: %d, Accuracy Bonus: %d, Damage Bonus: %d, Occupied: %s, CR: %d\n\n", health, maxHealth, armorRating, accBonus, dmgBonus, occupied ? "true":"false", getCR());
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
Weapon::Weapon(const Weapon& w) {
  int* s = w.getStats();
  maxHealth=s[1]; crewmateSlots = s[3]; name = w.name; desc = w.desc;
  atkDamage = s[5]; accuracy = s[6]; armorRating = s[2];
  health = s[0]; specialType = w.specialType; target = w.target;
  operational = health>0;
  assignedCrew = new Crewmate*[crewmateSlots];
  for (int i=0; i<w.crewmateSlots; i++) assignedCrew[i] = w.assignedCrew[i];  
}

void Weapon::operator=(const Weapon& w) {
  //HP, maxHP, AR, CrewmateSlots, CrewmatesEquipped, BaseDamage, BaseAccuracy, totalDamage, totalAccuracy, CR
  int* s = w.getStats();
  maxHealth=s[1]; crewmateSlots = s[3]; name = w.name; desc = w.desc;
  atkDamage = s[5]; accuracy = s[6]; armorRating = s[2];
  health = s[0]; specialType = w.specialType; target = w.target;
  operational = health>0;
  assignedCrew = new Crewmate*[crewmateSlots];
  for (int i=0; i<w.crewmateSlots; i++) assignedCrew[i] = w.assignedCrew[i];
}

bool Weapon::isValid() { //A simple check to see if the default constructor was used
  return this != nullptr && maxHealth != 0;
}

void Weapon::damage(int atkdmg, vector<string> *combatLog) { //For use when a weapon TAKES damage. Should really only be called by Ship::damage(). Inflicts damage on all assigned crewmates as well.
  health -= ( (atkdmg - armorRating >= 0) ? atkdmg - armorRating : 0);
  combatLog->push_back(name + " has taken " + to_string((atkdmg - armorRating >= 0) ? atkdmg - armorRating : 0) + " damage.");
  //cout << name << " has taken " << ( (atkdmg - armorRating >= 0) ? atkdmg - armorRating : 0) << " damage.\n";
  if (health <= 0) {
    operational = false;
    combatLog->push_back(name + " has been broken and is no longer operable.");
    //cout << name << " has been broken is is no longer operable.\n";
  }
  for (int i=0; i<crewmateSlots; i++)
    if (assignedCrew[i]->isValid())
      assignedCrew[i]->damage(atkdmg - armorRating, combatLog); //So the final damage to crewmates is atkdmg - shipAR - weaponAR - crewAR. Should it be this way?
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

//This next one was going to be implemented into the combat handler instead, but I realized I could just do it from here. This should only be called from Ship::runAttacks().
bool Weapon::attack(vector<string> *combatLog) { //Adds up baseDamage and all the modifiers, makes an attack roll with rollHit(), and deals damage to the enemy ship and target weapon. Also does onAttack special effects.
  if (isValid() && isOperational(false)) {
    int mod = 0;
    for (int i=0; i<crewmateSlots; i++)//calculate the modifier
      if (assignedCrew[i]->isValid()) {
        mod += assignedCrew[i]->dmgBonus;
        if (assignedCrew[i]->specialType == Crewmate::specialTypes::eachAttack) assignedCrew[i]->specialEffects(combatLog); // run crew special onAttack effects
      }
    if (specialType == eachAttack) specialEffects(combatLog); // run weapon special onAttack effects
    if (target->isValid()) {
      if (rollHit()) {
        combatLog->push_back(name + " dealt " + to_string(atkDamage+mod) + " damage to " + target->name + ".");
        //cout << name << " dealt " << atkDamage+mod << " damage to " << target->name <<". \n";
        ((Ship*)(target->ship))->damage(atkDamage+mod, target, combatLog);
        return true;
      }
      else cout << name << " missed.\n";
    }
  }
  return false;
}

bool Weapon::assignCrew(int slot, Crewmate* c) { //Assigns a crewmate to a weapon in the slot provided.
  if (slot<crewmateSlots && c->isValid() && !c->occupied) {
    unassignCrew(slot);
    assignedCrew[slot] = c;
    assignedCrew[slot] -> occupied = true;
    assignedCrew[slot] -> assignedTo = (void*)this;
    return true;
  }
  return false;
}

bool Weapon::unassignCrew(int slot) { //Used to unassign a crewmate without replacing it. Will just set the crewmate slot to nullptr.
  if (slot<crewmateSlots && assignedCrew[slot]->isValid()) {
    assignedCrew[slot] -> occupied = false;
    assignedCrew[slot] -> assignedTo = nullptr;
    assignedCrew[slot] = nullptr;
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

void Weapon::unsetTarget() {
  target = nullptr;
}

bool Weapon::isOperational(bool v = false) { //send in true to toggle operability, used to query operability and check that someone's assigned to it
  if (v) operational = !operational;
  bool hasAssigned = false;
  for (int i=0; i<crewmateSlots; i++) {
    if (assignedCrew[i]->isValid()) hasAssigned = true;
  }
  return operational && hasAssigned;
}

Crewmate** Weapon::getAssigned() {return assignedCrew;}

Weapon Weapon::generateEnemy(int CR) {
  srand(time(NULL));
  int cSlots = 1+(rand()%5); CR-=(cSlots*4);
  int atkCCR = CR * (40 + (rand()%20)) / 100, statCCR = CR-atkCCR;
  int dmg = atkCCR * (40 + (rand()%20)) / 100, acc = (atkCCR-dmg)/2;
  int hp = statCCR * (40 + (rand()%20)) / 100, ar = (statCCR-hp)/2;

  Weapon enemyWeapon = Weapon(hp, dmg, acc, ar, cSlots, "EnemyCannon", "Placeholder");
  return enemyWeapon;
}

int Weapon::getCR() const{
  return (atkDamage + (2*accuracy)) + (maxHealth + (2*armorRating)) + (crewmateSlots*4);
}

int* Weapon::getStats() const{
  int equippedC = 0, dmgTotal = atkDamage, accTotal = accuracy;
  for (int i=0; i<crewmateSlots; i++) {
    if (assignedCrew[i]->isValid()) {
      equippedC++;
      dmgTotal += assignedCrew[i]->dmgBonus;
      accTotal += assignedCrew[i]->accBonus;
    }
  }
  int *temp = new int[10]; //HP, maxHP, AR, CrewmateSlots, CrewmatesEquipped, BaseDamage, BaseAccuracy, totalDamage, totalAccuracy, CR
  temp[0] = health; temp[1] = maxHealth; temp[2] = armorRating; 
  temp[3] = crewmateSlots; temp[4] = equippedC;  
  temp[5] = atkDamage; temp[6] = accuracy; temp[7] = dmgTotal; temp[8] = accTotal; temp[9] = getCR();
  return temp;
}

int Weapon::repair() {
  int healed = maxHealth - health;
  health = maxHealth; 
  return healed;
}

int Weapon::getRepairCost() {
  return maxHealth - health;
}

void Weapon::printInfo() { //mostly for testing purposes.
  int* stats = getStats();
  cout << "Weapon Name: " << name << ", On Ship: " << (ship == nullptr ? "None" : ((Ship*)ship)->name) << "\n";
  cout << "Description: " << desc << "\n";
  printf("HP: %d/%d, AR: %d, Base Accuracy: %d, Base Damage: %d\n", health, maxHealth, armorRating, accuracy, atkDamage);
  printf("Crewmate Assignment Slots Filled: %d/%d, Operational: %s\n", stats[4], stats[3], isOperational() ? "true":"false");
  printf("Effective Damage: %d, Effective Accuracy: %d, CR: %d\n\n", stats[7], stats[8], stats[9]);
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
    specialType = none; balance = 0;
  }

Ship::Ship () : maxHealth(0), weaponSlots(0), crewmateSlots(0), cargoSize(0), name(""), desc("") {} //default constructor for placeholders in arrays
Ship::Ship(const Ship& s) {
  int* stats = s.getStats();
  maxHealth=stats[1]; crewmateSlots = stats[3]; name = s.name; desc = s.desc;
  armorRating = stats[2]; weaponSlots = s.weaponSlots;
  health = stats[0]; specialType = s.specialType; 
  crew = new Crewmate*[crewmateSlots];
  weapons = new Weapon*[weaponSlots];
  for (int i=0; i<crewmateSlots; i++) crew[i] = nullptr;
  for (int i=0; i<weaponSlots; i++) weapons[i] = nullptr; balance = 0;
}

void Ship::operator=(const Ship& s) {
  int* stats = s.getStats();
  maxHealth=stats[1]; crewmateSlots = stats[3]; name = s.name; desc = s.desc;
  armorRating = stats[2];
  health = stats[0]; specialType = s.specialType; 
  crew = new Crewmate*[crewmateSlots];
  weapons = new Weapon*[weaponSlots];
  for (int i=0; i<crewmateSlots; i++) crew[i] = nullptr;
  for (int i=0; i<weaponSlots; i++) weapons[i] = nullptr; balance = 0;
}

void Ship::damage (int atkdmg, Weapon *target, vector<string> *combatLog) { //for when a ship TAKES damage. Should probably only be called by Weapon::attack(), and by extension Ship::runAttacks().
  health -= ( (atkdmg - armorRating >= 0) ? atkdmg - armorRating : 0);
  combatLog->push_back(name + " has been hit and taken " + to_string((atkdmg - armorRating >= 0) ? atkdmg - armorRating : 0) + " damage.");
  //cout << name << " has been hit and has taken " << ( (atkdmg - armorRating >= 0) ? atkdmg - armorRating : 0) << " damage. ";
  combatLog->push_back("Target hit: " + target->name + ".");
  //cout << "Target hit: " << target->name << "\n";
  if (health <= 0) {
    /*Send Game Over state somehow */
    combatLog->push_back(name + " has capsized!");
    //cout << name << " has capsized!";
  }

  if (target->isValid())
    target->damage(atkdmg - armorRating, combatLog);
}

void Ship::sellLoot() { //For cashing out at port. Returns the number of cargo held before emptying, to add to money(?)
  balance += cargoHeld;
  cargoHeld = 0;
}

int Ship::repair() { //returns the value to deduct from money in game handler (it just costs the amount of health repaired, this can be changed to balance if needed)
  int temp = maxHealth - health;
  health = maxHealth;
  for (int i=0; i<weaponSlots; i++) {
    if (weapons[i]->isValid()) temp += weapons[i]->repair();
  }
  for (int i=0; i<crewmateSlots; i++) {//The crew healing is free, it would cost too much otherwise 
    if (crew[i]->isValid()) crew[i]->health = crew[i]->maxHealth;
  }
  return temp;
}

int Ship::getRepairCost() { //returns the value to deduct from money in game handler (it just costs the amount of health repaired, this can be changed to balance if needed)
  int temp = maxHealth - health;
  for (int i=0; i<weaponSlots; i++) {
    if (weapons[i]->isValid()) temp += weapons[i]->getRepairCost();
  }
  return temp;
}

bool Ship::isValid() const{
  return this != nullptr && maxHealth != 0;
}

void Ship::reinforce(int amt) { //todo: Probably balance this shit somehow, seems pretty broken if you can do it infinitely
  armorRating += amt; //price and money handling will be done in main game loop, this is just the stat change
}

int* Ship::getStats() const{
  int equippedW = 0, equippedC = 0;
  for (int i=0; i<weaponSlots; i++)
    if (weapons[i]->isValid())
      equippedW++;
  for (int i=0; i<crewmateSlots; i++)
    if (crew[i] != nullptr && crew[i]->isValid())
      equippedC++;
  int *temp = new int[11]; //HP, maxHP, AR, WeaponSlots, WeaponsEquipped, CrewmateSlots, CrewmatesEquipped, CargoSize, CargoHeld, CR, balance
  temp[0] = health; temp[1] = maxHealth; temp[2] = armorRating; 
  temp[3] = weaponSlots; temp[4] = equippedW;  
  temp[5] = crewmateSlots; temp[6] = equippedC;
  temp[7] = cargoSize; temp[8] = cargoHeld; temp[9] = getCR(); temp[10] = balance;
  return temp;
}

bool Ship::addCrew(Crewmate* c) { //Finds an empty crew slot and adds crew to it. Switching out crewmates is not yet implemented, they are permanently on the crew if added so far
  for (int i=0; i<crewmateSlots; i++) {
    if (!crew[i]->isValid()) {
      crew[i] = c;
      crew[i]->assignedTo = nullptr;
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
bool Ship::switchCrew(int slot, Crewmate *c) { //This should not be run to add crew to empty slots, only to switch a crewmate out with a new one.
  if (slot<crewmateSlots && c->isValid()) {
    if (crew[slot]->isValid()) { //Remove old crewmate from any weapons, and replace them with the new one in their slot.
      for (int i=0; i<weaponSlots; i++) {
        if (weapons[i]->isValid()) {
          for (int j=0; j<weapons[i]->crewmateSlots; j++) {
            if (weapons[i]->getAssigned()[j]->isValid()){
              if (crew[slot] == weapons[i]->getAssigned()[j]) {
                weapons[i]->assignCrew(j, c); //This automatically handles assignedTo and occupied for c and crew[slot]
              }
            }
          }
        }
      }
      crew[slot] -> ship = nullptr; //remove old crew from ship
    }
    crew[slot] = c;
    // c->occupied will already be set by assignCrew, or else it will already be false (and doesn't need to be set here).
    // c->assignedTo should already be nullptr, unless it was set above (if the old crewmate in this slot had an assignment), so setting it here is dumb
    crew[slot] -> ship = (void*)this; 
    return true;
  }
  return false;
}

bool Ship::switchWeapon(int slot, Weapon *w) { //This should not be run to add weapons to empty slots, only to switch out a weapon with a new one.
  if (slot<weaponSlots && w->isValid()) {
    if (weapons[slot]->isValid()) { //If there's already a weapon in this slot, set its target and ship to null
      weapons[slot] -> target = nullptr; 
      weapons[slot] -> ship = nullptr;
      for (int i=0; i<weapons[slot]->crewmateSlots; i++) { //If crewmates assigned to old weapon, assign them to new weapon. If not enough slots on new weapon, unassign leftover crew.
        if (weapons[slot]->getAssigned()[i]->isValid()) {
          Crewmate* c = weapons[slot]->getAssigned()[i];
          weapons[slot]->unassignCrew(i);
          if (!(w->assignCrew(i, c))) { 
            weapons[slot]->getAssigned()[i]-> occupied = false;
            weapons[slot]->getAssigned()[i]-> assignedTo = nullptr;
          }
        }
      }
    }
    weapons[slot] = w;
    weapons[slot] -> target = nullptr; //can't be aimed, because weapons can only be switched at port.
    weapons[slot] -> ship = (void*)this;
    return true;
  }
  return false;
}

void Ship::runAttacks(vector<string> *combatLog) {
  for (int i=0; i<weaponSlots; i++)
    if (weapons[i]->isValid()) weapons[i]->attack(combatLog);
  for (int i=0; i<crewmateSlots; i++)
    if (crew[i]->isValid() && crew[i]->specialType == Crewmate::specialTypes::eachTurn) crew[i]->specialEffects(combatLog);
}

Crewmate** Ship::getCrew() {return crew;}

Weapon** Ship::getWeapons() {return weapons;}

void Ship::printInfo() { //mostly for testing purposes.
  int* stats = getStats();
  cout << "Ship Name: " << name << "\n";
  cout << "Description: " << desc << "\n";
  printf("HP: %d/%d, AR: %d, CR: %d\n", health, maxHealth, armorRating, stats[9]);
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

Ship Ship::generateEncounter(int* outerWCR, int* outerCCR) {//TODO: Make this, probably implement a version for crewmates/weapons as well
  int CR = this->getCR();
  srand(time(NULL));
  int sCR = (CR * 0.3); //CR going to stats
  int wCR = (CR * 0.1) * ((rand()%2) + 3); //CR going to weapons
  int cCR = (CR * 0.7) - wCR; //CR going to crew

  int hp = sCR * (70+(rand()%30)) / 100; //60-90% of stats CR (which is 30% of overall CR) goes to health
  int ar = (sCR - hp)/2; //The remaining stats CR goes to armor, divided by 2 because I decided to weight it that way

  int wSlots = 1 + rand()%(int)(wCR/200); wCR -= wSlots*4;
  int cSlots = wSlots + rand()%(int)(cCR/100); cCR -= cSlots*2;

  int cargo = CR * (((rand()%100) + 50)/100);

  string n = "GeneratedEnemy"; string d = "Placeholder"; //TODO: get these to read from the flavortext files
  Ship enemyShip(hp, ar, wSlots, cSlots, cargo, n, d);

  *outerWCR = wCR; *outerCCR = cCR;
  return enemyShip;
}

int Ship::getCR() const{
  int CR = 0;
  for (int i=0; i<weaponSlots; i++)
    if (weapons[i]->isValid() && weapons[i]->isOperational())
      CR += weapons[i]->getCR();

  for (int i=0; i<crewmateSlots; i++)
    if (crew[i]->isValid())
      CR += crew[i]->getCR();

  return ((weaponSlots*4) + (crewmateSlots*2)) + (maxHealth + (2*armorRating)) + CR;
}

void Ship::autoConfigure(Ship* enemyShip) {//Meant to be a shortcut for configuration in battle. I imagine it'll be a hassle to test encounters if we have to set everything every time.
  for (int i=0; i<weaponSlots; i++) { //Try to make sure every weapon has at least one crewmate
    for (int j=0; j<crewmateSlots; j++) {
      if (weapons[i]->isValid() && weapons[i]->getStats()[0] > 0 && weapons[i]->getStats()[4] == 0 && !crew[j]->occupied) {
        weapons[i]->assignCrew(0, crew[j]);
      }
    }
  }
  for (int i=0; i<weaponSlots; i++) //Assign other crewmates on a FCFS basis
    for (int j=0; j<crewmateSlots; j++)
      if (weapons[i]->isValid() && weapons[i]->isOperational() && crew[j]->isValid() && !crew[j]->occupied)
        for (int k=0; k<weapons[i]->crewmateSlots; k++)
          if (weapons[i]->getAssigned()[k] == nullptr || !weapons[i]->getAssigned()[k]->isValid()) 
            weapons[i]->assignCrew(k, crew[j]);
  for (int i=0; i<weaponSlots; i++) //Point our weapons at random (valid and operational) enemy weapons
    if (weapons[i]->isValid() && weapons[i]->isOperational()) {
      int j;
      srand(time(NULL));
      do {
        j = rand()%enemyShip->weaponSlots;
      } while (!(enemyShip->getWeapons()[j]->isValid() && enemyShip->getWeapons()[j]->getStats()[0] > 0));
      weapons[i]->setTarget(enemyShip->getWeapons()[j]);
    }
}

void Ship::unsetWeapons() { //Just meant to be called after an encounter is won tbh
  for (int i=0; i<weaponSlots; i++)
    if (weapons[i]->isValid()) weapons[i]->unsetTarget();
}