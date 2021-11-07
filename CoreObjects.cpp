#include <cstdio>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>
#include "CoreObjects.h"

using namespace std;

Crewmate::Crewmate(int hp, int ar, int aB, int dB, string n) :
  maxHealth(hp), accBonus(aB), dmgBonus(dB) {
    health = hp; armorRating = ar;
    alive = true;
    name = n; desc = ""; //todo: build desc into constructor
}
Crewmate::Crewmate() : maxHealth(0), accBonus(0), dmgBonus(0), alive(true) {name = "Default";} //Default constructors for placeholders in arrays

bool Crewmate::isValid() { //A simple check to see if the default constructor was used and if crewmate is alive
  return this != nullptr && maxHealth != 0 && alive;
}
void Crewmate::damage(int atkdmg) {
  health -= (atkdmg - armorRating);
  if (health <= 0) alive = false;
}
void Crewmate::printInfo() { //mostly for testing purposes.
  cout << "Crewmate Name: " << name << "\n";
  printf("HP: %d/%d, AR: %d, Accuracy Bonus: %d, Damage Bonus: %d\n\n", health, maxHealth, armorRating, accBonus, dmgBonus);
}

Weapon::Weapon (int hp, int dmg, int acc, int ar, int cS, string n) : 
      maxHealth(hp), crewmateSlots(cS) {
        health = hp;
        atkDamage = dmg; accuracy = acc; armorRating = ar;
        operational = true;
        assignedCrew = new Crewmate*[crewmateSlots];
        for (int i=0; i<crewmateSlots; i++) assignedCrew[i] = nullptr;
        name = n; desc = ""; //todo: build desc into constructor
      }
Weapon::Weapon() : maxHealth(0), crewmateSlots(0) {} //Default constructors for placeholders in arrays

bool Weapon::isValid() { //A simple check to see if the default constructor was used and weapon is operable
  return this != nullptr && maxHealth != 0;
}
void Weapon::damage(int atkdmg) {
  health -= (atkdmg - armorRating);
  if (health <= 0) operational = false;

  for (int i=0; i<crewmateSlots; i++)
    if (assignedCrew[i]->isValid())
      assignedCrew[i]->damage(atkdmg - armorRating); //So the final damage to crewmates is atkdmg - shipAR - weaponAR - crewAR. Should it be this way?
}
bool Weapon::rollHit() {
  if (isValid()) {
    srand(time(NULL));
    int mod = 0;
    for (int i=0; i<crewmateSlots; i++)
      if (assignedCrew[i]->isValid()) 
        mod += assignedCrew[i]->accBonus;
    return (rand() % 100) + 1 < (accuracy + mod);
  }
  return false;
}
int Weapon::attack() {
  if (isValid() && operational) {
    int mod = 0;
    for (int i=0; i<crewmateSlots; i++)
      if (assignedCrew[i]->isValid()) 
        mod += assignedCrew[i]->dmgBonus;
    return atkDamage + mod;
  }
  return 0;
}
bool Weapon::assignCrew(Crewmate* c) {
  for (int i=0; i<crewmateSlots; i++) {
    if (!assignedCrew[i]->isValid()) {
      assignedCrew[i] = c;
      return true;
    }
  }
  return false;
}
int* Weapon::getStats() {
  int equippedC = 0;
  for (int i=0; i<crewmateSlots; i++)
    if (assignedCrew[i]->isValid())
      equippedC++;
  int *temp = new int[9]; //HP, maxHP, AR, CrewmateSlots, CrewmatesEquipped, BaseDamage, BaseAccuracy
  temp[0] = health; temp[1] = maxHealth; temp[2] = armorRating; 
  temp[3] = crewmateSlots; temp[4] = equippedC;  
  temp[5] = atkDamage; temp[6] = accuracy;
  return temp;
}
void Weapon::printInfo() { //mostly for testing purposes.
  int* stats = getStats();
  cout << "Weapon Name: " << name << "\n";
  printf("HP: %d/%d, AR: %d, Base Accuracy: %d, Base Damage: %d\n", health, maxHealth, armorRating, accuracy, atkDamage);
  printf("Crewmate Assignment Slots Filled: %d/%d\n\n", stats[4], stats[3]);
}

Ship::Ship (int hp, int baseArmor, int weaponS, int crewmateS, int cargoS, string n) :
  maxHealth(hp), weaponSlots(weaponS), crewmateSlots(crewmateS), cargoSize(cargoS)
  {
    health = hp;
    armorRating = baseArmor; 
    cargoHeld = 0;
    crew = new Crewmate*[crewmateSlots];
    weapons = new Weapon*[weaponSlots];
    for (int i=0; i<crewmateSlots; i++) crew[i] = nullptr;
    for (int i=0; i<weaponSlots; i++) weapons[i] = nullptr;
    name = n;
    //for (int i=0; n[i] != '\0'; i++) name += n[i];
  }

void Ship::damage (int atkdmg, Weapon *target) { //todo: Integrate this method with the main game loop for state updates
  health -= atkdmg - armorRating;
  if (health <= 0) {/*Send Game Over state somehow */}
  if (target->isValid())
    target->damage(atkdmg - armorRating);
}
int Ship::emptyCargo() {
  int temp = cargoHeld;
  cargoHeld = 0;
  return temp;
}
int Ship::repair() {
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
bool Ship::addCrew(Crewmate* c) {
  for (int i=0; i<crewmateSlots; i++) {
    if (!crew[i]->isValid()) {
      crew[i] = c;
      return true;
    }
  }
  return false;
}
bool Ship::addWeapon(Weapon* w) {
  for (int i=0; i<weaponSlots; i++) {
    if (!weapons[i]->isValid()) {
      weapons[i] = w;
      return true;
    }
  }
  return false;
}
void Ship::printInfo() { //mostly for testing purposes.
  int* stats = getStats();
  cout << "Ship Name: " << name << "\n";
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
