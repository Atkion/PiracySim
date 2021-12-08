#include "Port.h"

#include "CoreObjects.h"
#include "SpecialObjects.h"
#include "../PDCurses/curses.h"

#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string>
#include <time.h>

using namespace std;

//Main menu options: Sell loot, Buy weapons, Hire crewmates, Buy ship, Repair ship/weapons, Reinforce ship
//Submenus: Weapon Shop, Crewmate Shop, Ship Shop

Port::Port() {
    Crewmate Bill(62, 26, 0, 60, "Billy Bob", "Hardened Veteran"); //Hardcoded for now, since I'm definitely not doing this shit before it's due
    Crewmate Margaret(35, 12, 25, 40, "Margaret Eventide", "Overly Brash");
    Crewmate Dirk(45, 34, 4, 35, "Dirk Overtaal", "Ambitious Leader");
    Crewmate Jim(90, 25, -5, 100, "Scurvy Jim", "Drunken Philospher");
    Musketeer Randy(51, 14, 26, 44, "Reckless Randy", "Couragous to Stupidity");
    Crewmate Steve(40, 34, 0, 0, "Stealin' Steve", "Completely Useless");
    hires[0]=Bill; hires[1]=Margaret; hires[2]=Dirk; hires[3]=Jim; hires[4]=Randy; hires[5]=Steve;

    Weapon Ballista (100, 60, 50, 20, 2, "Ballista", "Massive bolt-firing weapon");
    Weapon Ballista2 (100, 60, 50, 20, 2, "Ballista", "Massive bolt-firing weapon");
    Weapon Cannon (50, 10, 15, 15, 1, "Cannon", "Basic Side Cannon");
    Weapon LongCannon (20, 10, 100, 30, 2, "Long Cannon", "More Accuracy, Less Damage");
    Weapon BlackArrow (120, 70, 50, 20, 4, "Black Arrow", "why is this even here");
    weaponSale[0] = Ballista; weaponSale[1] = Ballista2; weaponSale[2] = Cannon; weaponSale[3] = LongCannon;

    //Ship::Ship (int hp, int baseArmor, int weaponS, int crewmateS, int cargoS, string n, string d) : //mai
    Ship Sloop(300, 30, 2, 6, 300, "Basic Sloop", "Irrelevant");
    shipSale[0] = Sloop;

    
}

void Port::enterPort(Ship *ship) { //The main method to call from the map screen. All other port methods will be called from here.
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    printMenu(ship, 1);
}

void Port::printMenu(Ship *ship, int highlighted) {
    clear();
    int row, col;
    getmaxyx(stdscr,row,col);
    float menuScale = 0.8;
    int menuRow = row-(row*menuScale);
    init_pair(1, COLOR_BLACK, COLOR_WHITE); //For highlighted menu options
    init_pair(2, COLOR_GREEN, COLOR_BLACK); //For affordable prices
    init_pair(3, COLOR_RED, COLOR_BLACK); //For unaffordable prices
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); //For gold increase

    int* shipStats = ship->getStats(); //HP, maxHP, AR, WeaponSlots, WeaponsEquipped, CrewmateSlots, CrewmatesEquipped, CargoSize, CargoHeld, CR, balance
    mvprintw(2, 0, " Your Ship: "); printw(ship->name.c_str());
    mvprintw(4, 0, " Balance: "); printw(to_string(shipStats[10]).c_str());
    mvprintw(6, 0, " Health: "); printw(to_string(shipStats[0]).c_str()); addch('/'); printw(to_string(shipStats[1]).c_str());
    mvprintw(8, 0, " Armor Rating: "); printw(to_string(shipStats[2]).c_str()); 
    mvprintw(10, 0, " Weapons: "); printw(to_string(shipStats[4]).c_str()); addch('/'); printw(to_string(shipStats[3]).c_str());
    mvprintw(12, 0, " Crewmates: "); printw(to_string(shipStats[6]).c_str()); addch('/'); printw(to_string(shipStats[5]).c_str());
    mvprintw(14, 0, " Loot: "); printw(to_string(shipStats[8]).c_str()); addch('/'); printw(to_string(shipStats[7]).c_str());
    mvprintw(16, 0, " Challenge Rating: "); printw(to_string(shipStats[9]).c_str());
    for (int i=0; i<19; i++) mvaddch(i, 35, '|');
    for (int i=0; i<35; i++) mvaddch(18, i, '_');
    printShip(2, 40);

    for (int i=0; i<col; i++) mvaddch((row*menuScale), i, '_'); //setup boundaries
    for (int i=0; i<col; i++) mvaddch((row*menuScale + menuRow/2), i, '_');
    for (int i=0; i<menuRow; i++) {
        mvaddch(row*menuScale+i+1, col/3, '|');
        mvaddch(row*menuScale+i+1, (col/3)*2, '|');
    }

    if (highlighted == 1) attron(COLOR_PAIR(1));
    mvprintw((row*menuScale)+(menuRow*0.25)+1, ((col/3)-9)/2, "Sell Loot");
    if (highlighted == 1) attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(4));
    addstr(" (+");
    addstr(to_string(ship->cargoHeld).c_str());
    addstr(")");
    attroff(COLOR_PAIR(4));

    if (highlighted == 2) attron(COLOR_PAIR(1));
    mvprintw((row*menuScale)+(menuRow*0.25)+1, (col/3+((col/3)-6)/2), "Repair");
    if (highlighted == 2) attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(ship->getRepairCost() < ship->balance ? 2 : 3));
    addstr(" (-");
    addstr(to_string(ship->getRepairCost()).c_str());
    addstr(")");
    attroff(COLOR_PAIR(ship->getRepairCost() < ship->balance ? 2 : 3));

    if (highlighted == 3) attron(COLOR_PAIR(1));
    mvprintw((row*menuScale)+(menuRow*0.25)+1, (col/3)*2+((col/3)-9)/2, "Reinforce");
    if (highlighted == 3) attroff(COLOR_PAIR(1));
    attron(COLOR_PAIR(500 < ship->balance ? 2 : 3));
    addstr(" (-500)");
    attroff(COLOR_PAIR(500 < ship->balance ? 2 : 3));

    if (highlighted == 4) attron(COLOR_PAIR(1));
    mvprintw((row*menuScale)+(menuRow*0.75), ((col/3)-8)/2, "Buy Ship");
    if (highlighted == 4) attroff(COLOR_PAIR(1));

    if (highlighted == 5) attron(COLOR_PAIR(1));
    mvprintw((row*menuScale)+(menuRow*0.75), (col/3+((col/3)-11)/2), "Buy Weapons");
    if (highlighted == 5) attroff(COLOR_PAIR(1));

    if (highlighted == 6) attron(COLOR_PAIR(1));
    mvprintw((row*menuScale)+(menuRow*0.75), (col/3)*2+((col/3)-14)/2, "Hire Crewmates");
    if (highlighted == 6) attroff(COLOR_PAIR(1));

    
    refresh();

    int ch; 
    ch = getch(); //Z (confirm) == 122, X (back) == 120
    switch(ch) {
        case KEY_UP: 
            if (highlighted < 4) highlighted += 3;
            else highlighted -= 3;
            printMenu(ship, highlighted);
            break;
        case KEY_DOWN: 
            if (highlighted > 3) highlighted -= 3;
            else highlighted += 3;
            printMenu(ship, highlighted);
            break;
        case KEY_LEFT:
            if (highlighted == 1 || highlighted == 4) highlighted += 2;
            else highlighted -= 1;
            printMenu(ship, highlighted);
            break;
        case KEY_RIGHT: 
            if (highlighted == 3 || highlighted == 6) highlighted -= 2;
            else highlighted += 1;
            printMenu(ship, highlighted);
            break;
        case 122: //Z pressed
            //Main menu options: Sell loot, Repair, Reinforce, Buy Ships, Buy weapons, Hire crewmates
            //Submenus: Weapon Shop, Crewmate Shop, Ship Shop
            switch(highlighted) {
                case 1: 
                    ship->sellLoot();
                    printMenu(ship, highlighted);
                    break;
                case 2:
                    if(ship->balance > ship->getRepairCost()) {
                        ship->balance -= ship->repair();
                    }
                    printMenu(ship, highlighted);
                    break;
                case 3:
                    if(ship->balance > 500) {
                        ship->balance -= 500;
                        ship->reinforce(25);
                    }
                    printMenu(ship, highlighted);
                    break;
                case 4:
                    printShipShop(ship, 0, 0, false);
                    break;
                case 5:
                    printWeaponShop(ship, 0, 0, false);
                    break;
                case 6:
                    printCrewmateShop(ship, 0, 0, false);
                    break;
            }
            break;
        case 120: //X pressed, goes back to map screen
            return;
        default:
            printMenu(ship, highlighted);
            break;
    }
}

void Port::printWeaponShop(Ship *ship, int highlighted, int highlighted2 = 0, bool slotSelect = false) {
    clear();
    int row, col;
    getmaxyx(stdscr,row,col);
    //bool slotSelect = false; //Keeping track of whether we are going through the list of available crew, or selecting a slot on our ship for a new crewmate
    float menuScale = 0.8;
    int menuRow = row-(row*menuScale);
    init_pair(1, COLOR_BLACK, COLOR_WHITE); //For highlighted menu options
    init_pair(2, COLOR_GREEN, COLOR_BLACK); //For affordable prices
    init_pair(3, COLOR_RED, COLOR_BLACK); //For unaffordable prices
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); //For gold increase

    int* shipStats = ship->getStats(); //HP, maxHP, AR, WeaponSlots, WeaponsEquipped, CrewmateSlots, CrewmatesEquipped, CargoSize, CargoHeld, CR, balance
    for (int i=0; i<row; i++) mvaddch(i, col/2-1, '|'); 
    mvprintw(2, (col/2-15)/2, "Your Weapons ");
    mvprintw(3, (col/2-5)/2, "Slots: "); printw(to_string(shipStats[4]).c_str()); addch('/'); printw(to_string(shipStats[3]).c_str()); 
    mvprintw(1, 1, "Balance: "); printw(to_string(ship->balance).c_str());
    for (int i=0; i<ship->weaponSlots; i++) {
        move(i*4+4, 1);
        if (ship->getWeapons()[i]->isValid()) {
            int *weaponStats = ship->getWeapons()[i]->getStats(); //HP, maxHP, AR, CrewmateSlots, CrewmatesEquipped, BaseDamage, BaseAccuracy, totalDamage, totalAccuracy, CR
            if (slotSelect && highlighted2 == i) attron(COLOR_PAIR(1));
                printw(ship->getWeapons()[i]->name.c_str());
            if (slotSelect && highlighted2 == i) attroff(COLOR_PAIR(1));
                mvprintw(i*4+5, 1, "Health: "); printw(to_string(weaponStats[0]).c_str()); addch('/'); printw(to_string(weaponStats[1]).c_str());
                printw(" Armor: "); printw(to_string(weaponStats[2]).c_str()); printw(" Damage: "); printw(to_string(weaponStats[5]).c_str());
                printw(" Accuracy: "); printw(to_string(weaponStats[6]).c_str());
                if (ship->getWeapons()[i]->specialType != Weapon::specialTypes::none) mvprintw(i*4+6, 1, ship->getWeapons()[i]->special.c_str());
        }
        else printw(" <EMPTY SLOT> ");
    }

    mvprintw(2, (col/2)+((col/2-19)/2), "Available Weapons ");
    for (int i=0; i<5; i++) {
        move(i*4+4, col/2+1);
        if (weaponSale[i].isValid()) {
            int *weaponStats = weaponSale[i].getStats(); //HP, maxHP, AR, dmgBonus, accBonus, CR
            if (highlighted == i) attron(COLOR_PAIR(1));
            printw(weaponSale[i].name.c_str());
            if (highlighted == i) attroff(COLOR_PAIR(1));
            attron(COLOR_PAIR(ship->balance > weaponStats[9] ? 2 : 3));
            printw(" Cost: ");
            printw(to_string(weaponStats[9]).c_str());
            attroff(COLOR_PAIR(ship->balance > weaponStats[9] ? 2 : 3));
            mvprintw(i*4+5, col/2+1, "Health: "); printw(to_string(weaponStats[0]).c_str()); addch('/'); printw(to_string(weaponStats[1]).c_str());
                printw(" Armor: "); printw(to_string(weaponStats[2]).c_str()); printw(" Damage: "); printw(to_string(weaponStats[5]).c_str());
                printw(" Accuracy: "); printw(to_string(weaponStats[6]).c_str());
                if (weaponSale[i].specialType != Weapon::specialTypes::none) mvprintw(i*4+6, 1, weaponSale[i].special.c_str());
        }
        else printw(" <EMPTY SLOT> ");
    }

    refresh();

    int ch; 
    ch = getch(); //Z (confirm) == 122, X (back) == 120
    switch(ch) {
        case KEY_UP:
            if (slotSelect) {
                highlighted2 = highlighted2 == 0 ? ship->weaponSlots : highlighted2-1;
                printWeaponShop(ship, highlighted, highlighted2, slotSelect);
            }
            else {
                highlighted = highlighted == 0 ? 5 : highlighted-1;
                printWeaponShop(ship, highlighted, slotSelect);
            } 
            break;
        case KEY_DOWN: 
            if (slotSelect) {
                highlighted2 = highlighted2 == ship->weaponSlots ? 0 : highlighted2+1;
                printWeaponShop(ship, highlighted, highlighted2, slotSelect);
            }
            else {
                highlighted = highlighted == 5 ? 0 : highlighted+1;
                printWeaponShop(ship, highlighted, slotSelect);
            }
            break;
        case 122: //Z pressed
            if (!slotSelect && ship->balance > weaponSale[highlighted].getCR()) {
                if (!ship->weaponStorage[ship->weaponSlots+highlighted].isValid()) 
                    ship->weaponStorage[ship->weaponSlots+highlighted] = weaponSale[highlighted];
                if (!ship->addWeapon(&(ship->weaponStorage[ship->weaponSlots+highlighted])) && weaponSale[highlighted].isValid()) {
                    printWeaponShop(ship, highlighted, 1, true);
                }
                else {
                    ship->balance -= weaponSale[highlighted].getCR();
                    weaponSale[highlighted] = Weapon();
                    printWeaponShop(ship, highlighted, slotSelect);
                }
            }
            else if (ship->balance > weaponSale[highlighted].getCR()) {
                weaponSale[highlighted] = Weapon();
                ship->balance -= hires[highlighted].getCR();
                ship->switchWeapon(highlighted2, &(ship->weaponStorage[ship->weaponSlots+highlighted]));
                printWeaponShop(ship, highlighted, false);
            }
            else {
                mvprintw(row, 0, "Not enough money.");
                printWeaponShop(ship, highlighted, highlighted2, slotSelect);
            }
            break;
        case 120: //X pressed, goes back to main menu screen or back out of slotSelect mode
            if (slotSelect) Port::printWeaponShop(ship, highlighted, highlighted2, false);
            else printMenu(ship, 5);
            break;
        default:
            printWeaponShop(ship, highlighted, highlighted2, slotSelect);
            break;
    }
}

void Port::printCrewmateShop(Ship *ship, int highlighted, int highlighted2 = 0, bool slotSelect = false) {
    clear();
    int row, col;
    getmaxyx(stdscr,row,col);
    //bool slotSelect = false; //Keeping track of whether we are going through the list of available crew, or selecting a slot on our ship for a new crewmate
    float menuScale = 0.8;
    int menuRow = row-(row*menuScale);
    init_pair(1, COLOR_BLACK, COLOR_WHITE); //For highlighted menu options
    init_pair(2, COLOR_GREEN, COLOR_BLACK); //For affordable prices
    init_pair(3, COLOR_RED, COLOR_BLACK); //For unaffordable prices
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); //For gold increase

    int* shipStats = ship->getStats(); //HP, maxHP, AR, WeaponSlots, WeaponsEquipped, CrewmateSlots, CrewmatesEquipped, CargoSize, CargoHeld, CR, balance
    for (int i=0; i<row; i++) mvaddch(i, col/2-1, '|'); 
    mvprintw(2, (col/2-15)/2, "Your Crewmates ");
    mvprintw(3, (col/2-5)/2, "Slots: "); printw(to_string(shipStats[6]).c_str()); addch('/'); printw(to_string(shipStats[5]).c_str()); 
    mvprintw(1, 1, "Balance: "); printw(to_string(ship->balance).c_str());
    for (int i=0; i<ship->crewmateSlots; i++) {
        move(i*4+4, 1);
        if (ship->getCrew()[i]->isValid()) {
            int *crewStats = ship->getCrew()[i]->getStats(); //HP, maxHP, AR, dmgBonus, accBonus, CR
            if (slotSelect && highlighted2 == i) attron(COLOR_PAIR(1));
                printw(ship->getCrew()[i]->name.c_str());
            if (slotSelect && highlighted2 == i) attroff(COLOR_PAIR(1));
                mvprintw(i*4+5, 1, "Health: "); printw(to_string(crewStats[0]).c_str()); addch('/'); printw(to_string(crewStats[1]).c_str());
                printw(" Armor: "); printw(to_string(crewStats[2]).c_str()); printw(" Dmg Bonus: "); printw(to_string(crewStats[3]).c_str());
                printw(" Acc Bonus: "); printw(to_string(crewStats[4]).c_str());
                if (ship->getCrew()[i]->specialType != Crewmate::specialTypes::none) mvprintw(i*4+6, 1, ship->getCrew()[i]->special.c_str());
        }
        else printw(" <EMPTY SLOT> ");
    }

    mvprintw(2, (col/2)+((col/2-19)/2), "Available Crewmates ");
    for (int i=0; i<5; i++) {
        move(i*4+4, col/2+1);
        if (hires[i].isValid()) {
            int *crewStats = hires[i].getStats(); //HP, maxHP, AR, dmgBonus, accBonus, CR
            if (highlighted == i) attron(COLOR_PAIR(1));
            printw(hires[i].name.c_str());
            if (highlighted == i) attroff(COLOR_PAIR(1));
            attron(COLOR_PAIR(ship->balance > crewStats[5] ? 2 : 3));
            printw(" Cost: ");
            printw(to_string(crewStats[5]).c_str());
            attroff(COLOR_PAIR(ship->balance > crewStats[5] ? 2 : 3));
            mvprintw(i*4+5, col/2+1, "Health: "); printw(to_string(crewStats[0]).c_str()); addch('/'); printw(to_string(crewStats[1]).c_str());
            printw(" Armor: "); printw(to_string(crewStats[2]).c_str()); printw(" Dmg Bonus: "); printw(to_string(crewStats[3]).c_str());
            printw(" Acc Bonus: "); printw(to_string(crewStats[4]).c_str());
            if (hires[i].specialType != Crewmate::specialTypes::none) mvprintw(i*4+6, 1, ship->getCrew()[i]->special.c_str());
        }
        else printw(" <EMPTY SLOT> ");
    }

    refresh();

    int ch; 
    ch = getch(); //Z (confirm) == 122, X (back) == 120
    switch(ch) {
        case KEY_UP:
            if (slotSelect) {
                highlighted2 = highlighted2 == 0 ? ship->crewmateSlots : highlighted2-1;
                printCrewmateShop(ship, highlighted, highlighted2, slotSelect);
            }
            else {
                highlighted = highlighted == 0 ? 5 : highlighted-1;
                printCrewmateShop(ship, highlighted, slotSelect);
            } 
            break;
        case KEY_DOWN: 
            if (slotSelect) {
                highlighted2 = highlighted2 == ship->crewmateSlots ? 0 : highlighted2+1;
                printCrewmateShop(ship, highlighted, highlighted2, slotSelect);
            }
            else {
                highlighted = highlighted == 5 ? 0 : highlighted+1;
                printCrewmateShop(ship, highlighted, slotSelect);
            }
            break;
        case 122: //Z pressed
            if (!slotSelect && ship->balance > hires[highlighted].getCR()) {
                if (!ship->crewmateStorage[ship->crewmateSlots+highlighted].isValid()) 
                    ship->crewmateStorage[ship->crewmateSlots+highlighted] = hires[highlighted];
                if (!ship->addCrew(&(ship->crewmateStorage[ship->crewmateSlots+highlighted])) && hires[highlighted].isValid()) {
                    printCrewmateShop(ship, highlighted, 1, true);
                }
                else {
                    ship->balance -= hires[highlighted].getCR();
                    hires[highlighted].alive = false;
                    printCrewmateShop(ship, highlighted, slotSelect);
                }
            }
            else if (ship->balance > hires[highlighted].getCR()) {
                hires[highlighted].alive = false;
                ship->balance -= hires[highlighted].getCR();
                ship->switchCrew(highlighted2, &(ship->crewmateStorage[ship->crewmateSlots+highlighted]));
                printCrewmateShop(ship, highlighted, false);
            }
            else {
                mvprintw(row, 0, "Not enough money.");
                printCrewmateShop(ship, highlighted, highlighted2, slotSelect);
            }
            break;
        case 120: //X pressed, goes back to main menu screen or back out of slotSelect mode
            if (slotSelect) Port::printCrewmateShop(ship, highlighted, highlighted2, false);
            else printMenu(ship, 6);
            break;
        default:
            printCrewmateShop(ship, highlighted, highlighted2, slotSelect);
            break;
    }
}

void Port::printShipShop(Ship *ship, int highlighted, int highlighted2 = 0, bool slotSelect = false) {
    clear();
    int row, col;
    getmaxyx(stdscr,row,col);
    //bool slotSelect = false; //Keeping track of whether we are going through the list of available crew, or selecting a slot on our ship for a new crewmate
    float menuScale = 0.8;
    int menuRow = row-(row*menuScale);
    init_pair(1, COLOR_BLACK, COLOR_WHITE); //For highlighted menu options
    init_pair(2, COLOR_GREEN, COLOR_BLACK); //For affordable prices
    init_pair(3, COLOR_RED, COLOR_BLACK); //For unaffordable prices
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); //For gold increase

    int* shipStats = ship->getStats(); //HP, maxHP, AR, WeaponSlots, WeaponsEquipped, CrewmateSlots, CrewmatesEquipped, CargoSize, CargoHeld, CR, balance
    mvprintw(2, 0, " Your Ship: "); printw(ship->name.c_str());
    mvprintw(4, 0, " Balance: "); printw(to_string(shipStats[10]).c_str());
    mvprintw(6, 0, " Health: "); printw(to_string(shipStats[0]).c_str()); addch('/'); printw(to_string(shipStats[1]).c_str());
    mvprintw(8, 0, " Armor Rating: "); printw(to_string(shipStats[2]).c_str()); 
    mvprintw(10, 0, " Weapons: "); printw(to_string(shipStats[4]).c_str()); addch('/'); printw(to_string(shipStats[3]).c_str());
    mvprintw(12, 0, " Crewmates: "); printw(to_string(shipStats[6]).c_str()); addch('/'); printw(to_string(shipStats[5]).c_str());
    mvprintw(14, 0, " Loot: "); printw(to_string(shipStats[8]).c_str()); addch('/'); printw(to_string(shipStats[7]).c_str());
    mvprintw(16, 0, " Challenge Rating: "); printw(to_string(shipStats[9]).c_str());

    for (int i=0; i<row; i++) mvaddch(i, col/2-1, '|'); 

    mvprintw(1, 1, "Balance: "); printw(to_string(ship->balance).c_str());
    mvprintw(2, (col/2)+((col/2-19)/2), "Available Ships ");
    for (int i=0; i<6; i++) {
        move(i*4+4, col/2+1);
        if (shipSale[i].isValid()) {
            int *sStats = shipSale[i].getStats();//HP, maxHP, AR, WeaponSlots, WeaponsEquipped, CrewmateSlots, CrewmatesEquipped, CargoSize, CargoHeld, CR, balance
            if (highlighted == i) attron(COLOR_PAIR(1));
            printw(shipSale[i].name.c_str());
            if (highlighted == i) attroff(COLOR_PAIR(1));
            attron(COLOR_PAIR(ship->balance > sStats[9] ? 2 : 3));
            printw(" Cost: ");
            printw(to_string(sStats[9]).c_str());
            attroff(COLOR_PAIR(ship->balance > sStats[9] ? 2 : 3));
            mvprintw(i*4+5, col/2+1, "Health: "); printw(to_string(sStats[0]).c_str()); addch('/'); printw(to_string(sStats[1]).c_str());
                printw(" Armor: "); printw(to_string(sStats[2]).c_str());
            mvprintw(i*4+6, col/2+1, " Weapon Slots: "); printw(to_string(shipStats[3]).c_str());
            mvprintw(i*4+7, col/2+1, " Crewmate Slots: "); printw(to_string(shipStats[5]).c_str());
            if (shipSale[i].specialType != Ship::specialTypes::none) mvprintw(i*4+6, 1, shipSale[i].special.c_str());
        }
        else printw(" <EMPTY SLOT> ");
    }

    refresh();

    int ch; 
    ch = getch(); //Z (confirm) == 122, X (back) == 120
    switch(ch) {
        case 122: //Z pressed, does nothing because it's not needed for the demo
            /*if (!slotSelect && ship->balance > weaponSale[highlighted].getCR()) {
                if (!ship->weaponStorage[ship->weaponSlots+highlighted].isValid()) 
                    ship->weaponStorage[ship->weaponSlots+highlighted] = weaponSale[highlighted];
                if (!ship->addWeapon(&(ship->weaponStorage[ship->weaponSlots+highlighted])) && weaponSale[highlighted].isValid()) {
                    printWeaponShop(ship, highlighted, 1, true);
                }
                else {
                    ship->balance -= weaponSale[highlighted].getCR();
                    weaponSale[highlighted] = Weapon();
                    printWeaponShop(ship, highlighted, slotSelect);
                }
            }
            else if (ship->balance > weaponSale[highlighted].getCR()) {
                weaponSale[highlighted] = Weapon();
                ship->balance -= hires[highlighted].getCR();
                ship->switchWeapon(highlighted2, &(ship->weaponStorage[ship->weaponSlots+highlighted]));
                printWeaponShop(ship, highlighted, false);
            }
            else {
                mvprintw(row, 0, "Not enough money.");
                printWeaponShop(ship, highlighted, highlighted2, slotSelect);
            }*/
            printShipShop(ship, highlighted, highlighted2, slotSelect);
            break;
        case 120: //X pressed, goes back to main menu screen or back out of slotSelect mode
            if (slotSelect) Port::printShipShop(ship, highlighted, highlighted2, false);
            else printMenu(ship, 4);
            break;
        default:
            printShipShop(ship, highlighted, highlighted2, slotSelect);
            break;
    }
}

void Port::printShip(int y, int x) {
    mvprintw(y, x, "           Ya...___|__..aab     .   .    ");
    mvprintw(y+1, x, "            Y88a  Y88o  Y88a   (     )   ");
    mvprintw(y+2, x, "             Y88b  Y88b  Y88b   `.oo'    ");
    mvprintw(y+3, x, "             :888  :888  :888  ( (`-'    ");
    mvprintw(y+4, x, "    .---.    d88P  d88P  d88P   `.`.     ");
    mvprintw(y+5, x, "   / .-._)  d8P'\"\"\"|\"\"\"'-Y8P      `.`.   ");
    mvprintw(y+6, x, "  ( (`._) .-.  .-. |.-.  .-.  .-.   ) )  ");
    mvprintw(y+7, x, "   \\ `---( O )( O )( O )( O )( O )-' /   ");
    mvprintw(y+8, x, "    `.    `-'  `-'  `-'  `-'  `-'  .' CJ ");
    mvprintw(y+9, x, "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
}
/*
           Ya...___|__..aab     .   .    
            Y88a  Y88o  Y88a   (     )   
             Y88b  Y88b  Y88b   `.oo'    
             :888  :888  :888  ( (`-'    
    .---.    d88P  d88P  d88P   `.`.     
   / .-._)  d8P'"""|"""'-Y8P      `.`.   
  ( (`._) .-.  .-. |.-.  .-.  .-.   ) )  
   \ `---( O )( O )( O )( O )( O )-' /   
    `.    `-'  `-'  `-'  `-'  `-'  .' CJ 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
*/