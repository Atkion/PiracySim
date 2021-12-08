#include "../Map.h"
#include "CoreObjects.h"
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <iostream>
#include <string>
#include <vector>
#include "../PDCurses/curses.h"
#include "CombatHandler.h"

CombatHandler::CombatHandler(Ship* player, Ship* enemy) {
    playerShip = player; 
    enemyShip = enemy;
    combatLog.push_back("Combat Handler Initialized.");
}

Ship* CombatHandler::getPlayer() { return playerShip; }
Ship* CombatHandler::getEnemy() { return enemyShip; }

void CombatHandler::enterCombat() {
    initscr();
    start_color();
    keypad(stdscr, TRUE);
    printCombatMenu(1);
}

void CombatHandler::printCombatMenu(int h) {
    clear();
    int row, col;
    getmaxyx(stdscr,row,col);
    float menuScale = 0.8;
    float logScale = 0.7;
    int menuRow = row-(row*menuScale);
    init_pair(1, COLOR_BLACK, COLOR_WHITE); //For highlighted menu options

    int* shipStats = playerShip->getStats(); //HP, maxHP, AR, WeaponSlots, WeaponsEquipped, CrewmateSlots, CrewmatesEquipped, CargoSize, CargoHeld, CR, balance
    mvprintw(2, 0, " Your Ship: "); printw(playerShip->name.c_str());
    mvprintw(4, 0, " Money: "); printw(to_string(shipStats[10]).c_str());
    mvprintw(6, 0, " Health: "); printw(to_string(shipStats[0]).c_str()); addch('/'); printw(to_string(shipStats[1]).c_str());
    mvprintw(8, 0, " Armor Rating: "); printw(to_string(shipStats[2]).c_str()); 
    mvprintw(10, 0, " Weapons: "); printw(to_string(shipStats[4]).c_str()); addch('/'); printw(to_string(shipStats[3]).c_str());
    mvprintw(12, 0, " Crewmates: "); printw(to_string(shipStats[6]).c_str()); addch('/'); printw(to_string(shipStats[5]).c_str());
    mvprintw(14, 0, " Loot: "); printw(to_string(shipStats[8]).c_str()); addch('/'); printw(to_string(shipStats[7]).c_str());
    mvprintw(16, 0, " Challenge Rating: "); printw(to_string(shipStats[9]).c_str());
    for (int i=0; i<19; i++) mvaddch(i, 35, '|');
    for (int i=0; i<35; i++) mvaddch(18, i, '_');

    int* enemyStats = enemyShip->getStats(); //HP, maxHP, AR, WeaponSlots, WeaponsEquipped, CrewmateSlots, CrewmatesEquipped, CargoSize, CargoHeld, CR, balance
    mvprintw(2, col*logScale-34, " Enemy Ship: "); printw(enemyShip->name.c_str());
    mvprintw(4, col*logScale-34, " Money: "); printw(to_string(enemyStats[10]).c_str());
    mvprintw(6, col*logScale-34, " Health: "); printw(to_string(enemyStats[0]).c_str()); addch('/'); printw(to_string(enemyStats[1]).c_str());
    mvprintw(8, col*logScale-34, " Armor Rating: "); printw(to_string(enemyStats[2]).c_str()); 
    mvprintw(10, col*logScale-34, " Weapons: "); printw(to_string(enemyStats[4]).c_str()); addch('/'); printw(to_string(enemyStats[3]).c_str());
    mvprintw(12, col*logScale-34, " Crewmates: "); printw(to_string(enemyStats[6]).c_str()); addch('/'); printw(to_string(enemyStats[5]).c_str());
    mvprintw(14, col*logScale-34, " Loot: "); printw(to_string(enemyStats[8]).c_str()); addch('/'); printw(to_string(enemyStats[7]).c_str());
    mvprintw(16, col*logScale-34, " Challenge Rating: "); printw(to_string(enemyStats[9]).c_str());
    for (int i=0; i<19; i++) mvaddch(i, col*logScale-35, '|');
    for (int i=0; i<35; i++) mvaddch(18, (col*logScale-34)+i, '_');    

    for (int i=0; i<col*logScale; i++) mvaddch((row*menuScale), i, '_'); //setup boundaries
    for (int i=0; i<col*logScale; i++) mvaddch((row*menuScale + menuRow/2), i, '_');
    for (int i=0; i<menuRow; i++) mvaddch(row*menuScale+i+1, (col*logScale)/2, '|');

    for (int i=0; i<row; i++) mvaddch(i, col*logScale, '|');//Combat log printing
    for (int r = combatLog.size()-1, i=row-1; i>0 && r>=0; i--, r--)
        mvprintw(i, col*logScale+1, combatLog[r].c_str());


    if (h == 1) attron(COLOR_PAIR(1));
    mvprintw((row*menuScale)+(menuRow*0.25)+1, (((col*logScale)/2)-6)/2, "Attack");
    if (h == 1) attroff(COLOR_PAIR(1));

    if (h == 2) attron(COLOR_PAIR(1));
    mvprintw((row*menuScale)+(menuRow*0.25)+1, (col*logScale/2)+(((col*logScale)/2)-9)/2, "Configure");
    if (h == 2) attroff(COLOR_PAIR(1));

    if (h == 3) attron(COLOR_PAIR(1));
    mvprintw((row*menuScale)+(menuRow*0.75), (((col*logScale)/2)-4)/2, "Info");
    if (h == 3) attroff(COLOR_PAIR(1));

    if (h == 4) attron(COLOR_PAIR(1));
    mvprintw((row*menuScale)+(menuRow*0.75), (col*logScale/2)+(((col*logScale)/2)-4)/2, "Flee");
    if (h == 4) attroff(COLOR_PAIR(1));

    refresh();

    int ch;
    ch = getch();
    switch(ch) {
        case KEY_UP: 
            if (h < 3) h += 2;
            else h -= 2;
            printCombatMenu(h);
            break;
        case KEY_DOWN: 
            if (h > 2) h -= 2;
            else h += 2;
            printCombatMenu(h);
            break;
        case KEY_LEFT:
            if (h == 1 || h == 3) h += 1;
            else h -= 1;
            printCombatMenu(h);
            break;
        case KEY_RIGHT: 
            if (h == 2 || h == 4) h -= 1;
            else h += 1;
            printCombatMenu(h);
            break;
        case 122: //Z pressed
            //Main menu options: Attack, Configure, Info, Flee
            //Submenus: Configure, Info
            switch(h) {
                case 1:
                    combatLog.push_back(" "); combatLog.push_back(" ");
                    playerShip->autoConfigure(enemyShip);
                    playerShip->runAttacks(&combatLog);
                    enemyShip->autoConfigure(playerShip);
                    enemyShip->runAttacks(&combatLog);
                    printCombatMenu(1);
                    break;
                case 2:
                    printConfigMenu(1, 0, 0, 0, 0);
                    break;
                case 3:
                    printInfoMenu();
                    break;
                case 4:
                    flee();
                    break;
            }
            break;
        case 120: //X pressed, does nothing here
            printCombatMenu(h);
            break;
        default:
            printCombatMenu(h);
            break;
    }
}

void CombatHandler::printInfoMenu() {
    clear();
    int row, col;
    getmaxyx(stdscr,row,col);
    float menuScale = 0.8;
    float logScale = 0.7;
    int menuRow = row-(row*menuScale);
    init_pair(1, COLOR_BLACK, COLOR_WHITE); //For highlighted menu options

    int* shipStats = playerShip->getStats(); //HP, maxHP, AR, WeaponSlots, WeaponsEquipped, CrewmateSlots, CrewmatesEquipped, CargoSize, CargoHeld, CR, balance
    mvprintw(2, 0, " Your Ship: "); printw(playerShip->name.c_str());
    mvprintw(4, 0, " Money: "); printw(to_string(shipStats[10]).c_str());
    mvprintw(6, 0, " Health: "); printw(to_string(shipStats[0]).c_str()); addch('/'); printw(to_string(shipStats[1]).c_str());
    mvprintw(8, 0, " Armor Rating: "); printw(to_string(shipStats[2]).c_str()); 
    mvprintw(10, 0, " Weapons: "); printw(to_string(shipStats[4]).c_str()); addch('/'); printw(to_string(shipStats[3]).c_str());
    mvprintw(12, 0, " Crewmates: "); printw(to_string(shipStats[6]).c_str()); addch('/'); printw(to_string(shipStats[5]).c_str());
    mvprintw(14, 0, " Loot: "); printw(to_string(shipStats[8]).c_str()); addch('/'); printw(to_string(shipStats[7]).c_str());
    mvprintw(16, 0, " Challenge Rating: "); printw(to_string(shipStats[9]).c_str());
    for (int i=0; i<19; i++) mvaddch(i, 35, '|');
    for (int i=0; i<35; i++) mvaddch(18, i, '_');

    int* enemyStats = enemyShip->getStats(); //HP, maxHP, AR, WeaponSlots, WeaponsEquipped, CrewmateSlots, CrewmatesEquipped, CargoSize, CargoHeld, CR, balance
    mvprintw(2, col*logScale-34, " Enemy Ship: "); printw(enemyShip->name.c_str());
    mvprintw(4, col*logScale-34, " Money: "); printw(to_string(enemyStats[10]).c_str());
    mvprintw(6, col*logScale-34, " Health: "); printw(to_string(enemyStats[0]).c_str()); addch('/'); printw(to_string(enemyStats[1]).c_str());
    mvprintw(8, col*logScale-34, " Armor Rating: "); printw(to_string(enemyStats[2]).c_str()); 
    mvprintw(10, col*logScale-34, " Weapons: "); printw(to_string(enemyStats[4]).c_str()); addch('/'); printw(to_string(enemyStats[3]).c_str());
    mvprintw(12, col*logScale-34, " Crewmates: "); printw(to_string(enemyStats[6]).c_str()); addch('/'); printw(to_string(enemyStats[5]).c_str());
    mvprintw(14, col*logScale-34, " Loot: "); printw(to_string(enemyStats[8]).c_str()); addch('/'); printw(to_string(enemyStats[7]).c_str());
    mvprintw(16, col*logScale-34, " Challenge Rating: "); printw(to_string(enemyStats[9]).c_str());
    for (int i=0; i<19; i++) mvaddch(i, col*logScale-35, '|');
    for (int i=0; i<35; i++) mvaddch(18, (col*logScale-34)+i, '_');
    for (int i=0; i<row; i++) mvaddch(i, col*logScale/2, '|');    

    for (int i=0; i<row; i++) mvaddch(i, col*logScale, '|');//Combat log printing
    for (int r = combatLog.size()-1, i=row-1; i>0 && r>=0; i--, r--)
        mvprintw(i, col*logScale+1, combatLog[r].c_str());

    int l = 20;
    for (int i=0; i<playerShip->weaponSlots; i++) {
        move(l, 1);
        if (playerShip->getWeapons()[i]->isValid()) {
            int *weaponStats = playerShip->getWeapons()[i]->getStats(); //HP, maxHP, AR, CrewmateSlots, CrewmatesEquipped, BaseDamage, BaseAccuracy, totalDamage, totalAccuracy, CR
                printw(playerShip->getWeapons()[i]->name.c_str());
                mvprintw(l+1, 1, "Health: "); printw(to_string(weaponStats[0]).c_str()); addch('/'); printw(to_string(weaponStats[1]).c_str());
                printw(" Armor: "); printw(to_string(weaponStats[2]).c_str()); printw(" Damage: "); printw(to_string(weaponStats[5]).c_str());
                printw(" Accuracy: "); printw(to_string(weaponStats[6]).c_str());

                mvprintw(l+2, 1, ("Operational: ")); printw(playerShip->getWeapons()[i]->isOperational(false) ? "true" : "false");
                printw((" Crewmates: " + to_string(weaponStats[4]) + "/" + to_string(weaponStats[3])).c_str());
                if (playerShip->getWeapons()[i]->specialType != Weapon::specialTypes::none) mvprintw(l++ +3, 1, playerShip->getWeapons()[i]->special.c_str());
                l+=3;
                for (int j=0; j<weaponStats[3]; j++) {
                    if (playerShip->getWeapons()[i]->getAssigned()[j]->isValid()) {
                        int* crewStats = playerShip->getWeapons()[i]->getAssigned()[j]->getStats();
                        move(l, 4);
                        printw(playerShip->getWeapons()[i]->getAssigned()[j]->name.c_str());
                        mvprintw(l+1, 4, "Health: "); printw(to_string(crewStats[0]).c_str()); addch('/'); printw(to_string(crewStats[1]).c_str());
                        printw(" Armor: "); printw(to_string(crewStats[2]).c_str()); printw(" Dmg Bonus: "); printw(to_string(crewStats[3]).c_str());
                        printw(" Acc Bonus: "); printw(to_string(crewStats[4]).c_str());
                        l+=2;
                        if (playerShip->getWeapons()[i]->getAssigned()[j]->specialType != Crewmate::specialTypes::none) mvprintw(l++, 4, playerShip->getWeapons()[i]->getAssigned()[j]->special.c_str());
                    }
                }
                l++;
        }
    }
    mvprintw(l++, 1, "Unassigned Crewmates: ");
    for (int i=0; i<playerShip->crewmateSlots; i++) {
        if (playerShip->getCrew()[i]->isValid() && !playerShip->getCrew()[i]->occupied) {
            int* crewStats = playerShip->getCrew()[i]->getStats();
            move(l, 4);
            printw(playerShip->getCrew()[i]->name.c_str());
            mvprintw(l+1, 4, "Health: "); printw(to_string(crewStats[0]).c_str()); addch('/'); printw(to_string(crewStats[1]).c_str());
            printw(" Armor: "); printw(to_string(crewStats[2]).c_str()); printw(" Dmg Bonus: "); printw(to_string(crewStats[3]).c_str());
            printw(" Acc Bonus: "); printw(to_string(crewStats[4]).c_str());
            l+=2;
            if (playerShip->getCrew()[i]->specialType != Crewmate::specialTypes::none) mvprintw(l++, 4, playerShip->getCrew()[i]->special.c_str());
        }
    }

    l=20;

    for (int i=0; i<enemyShip->weaponSlots; i++) {
        move(l, col*logScale/2+1);
        if (enemyShip->getWeapons()[i]->isValid()) {
            int *weaponStats = enemyShip->getWeapons()[i]->getStats(); //HP, maxHP, AR, CrewmateSlots, CrewmatesEquipped, BaseDamage, BaseAccuracy, totalDamage, totalAccuracy, CR
                printw(enemyShip->getWeapons()[i]->name.c_str());
                mvprintw(l+1, col*logScale/2+1, "Health: "); printw(to_string(weaponStats[0]).c_str()); addch('/'); printw(to_string(weaponStats[1]).c_str());
                printw(" Armor: "); printw(to_string(weaponStats[2]).c_str()); printw(" Damage: "); printw(to_string(weaponStats[5]).c_str());
                printw(" Accuracy: "); printw(to_string(weaponStats[6]).c_str());

                mvprintw(l+2, col*logScale/2+1, ("Operational: ")); printw(enemyShip->getWeapons()[i]->isOperational(false) ? "true" : "false");
                printw((" Crewmates: " + to_string(weaponStats[4]) + "/" + to_string(weaponStats[3])).c_str());
                if (enemyShip->getWeapons()[i]->specialType != Weapon::specialTypes::none) mvprintw(l++ +3, col*logScale/2+1, enemyShip->getWeapons()[i]->special.c_str());
                l+=3;
                for (int j=0; j<weaponStats[3]; j++) {
                    if (enemyShip->getWeapons()[i]->getAssigned()[j]->isValid()) {
                        int* crewStats = enemyShip->getWeapons()[i]->getAssigned()[j]->getStats();
                        move(l, col*logScale/2+5);
                        printw(enemyShip->getWeapons()[i]->getAssigned()[j]->name.c_str());
                        mvprintw(l+1, col*logScale/2+5, "Health: "); printw(to_string(crewStats[0]).c_str()); addch('/'); printw(to_string(crewStats[1]).c_str());
                        printw(" Armor: "); printw(to_string(crewStats[2]).c_str()); printw(" Dmg Bonus: "); printw(to_string(crewStats[3]).c_str());
                        printw(" Acc Bonus: "); printw(to_string(crewStats[4]).c_str());
                        l+=2;
                        if (enemyShip->getWeapons()[i]->getAssigned()[j]->specialType != Crewmate::specialTypes::none) mvprintw(l++, col*logScale/2+5, enemyShip->getWeapons()[i]->getAssigned()[j]->special.c_str());
                    }
                }
                l++;
        }
    }
    mvprintw(l++, col*logScale/2+1, "Unassigned Crewmates: ");
    for (int i=0; i<enemyShip->crewmateSlots; i++) {
        if (enemyShip->getCrew()[i]->isValid() && !enemyShip->getCrew()[i]->occupied) {
            int* crewStats = enemyShip->getCrew()[i]->getStats();
            move(l, col*logScale/2+5);
            printw(enemyShip->getCrew()[i]->name.c_str());
            mvprintw(l+1, col*logScale/2+5, "Health: "); printw(to_string(crewStats[0]).c_str()); addch('/'); printw(to_string(crewStats[1]).c_str());
            printw(" Armor: "); printw(to_string(crewStats[2]).c_str()); printw(" Dmg Bonus: "); printw(to_string(crewStats[3]).c_str());
            printw(" Acc Bonus: "); printw(to_string(crewStats[4]).c_str());
            l+=2;
            if (enemyShip->getCrew()[i]->specialType != Crewmate::specialTypes::none) mvprintw(l++, col*logScale/2+5, enemyShip->getCrew()[i]->special.c_str());
        }
    }
    refresh();

    int ch;
    ch = getch();
    switch(ch) {
        case 120: //X pressed, goes back to main combat menu
            printCombatMenu(3);
            break;
        default:
            printInfoMenu();
            break;
    }
}

void CombatHandler::printConfigMenu(int h, int h2=0, int h3=0, int h4=0, int mode=0) {//Modes: 0=top-level config, 1=selecting crewmate, 2=assigning crewmate to weapon, 3=selecting weapon slot if necessary, 4=targeting weapon

}

void CombatHandler::flee() {
    combatLog.push_back("Got away successfully!");
    //go back into map screen
}
