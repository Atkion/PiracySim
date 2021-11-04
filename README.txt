- You are a new pirate captain, full of spirit but short on rum and crew. You have a dinghy and a lifelong friend by your side.

- Roguelike gameplay loop (Roguelite? Savefiles?)

- Two main "areas", open ocean (random encounters) and ports
    - At port, you can repair your ship, buy ship upgrades (and new ships), recruit crewmates, sell loot
    - At sea, you are presented with a neverending progression of encounters that slowly increase in difficulty over time
    
- Each Ship has:
    - Size (determines crewmate slots and weapon slots (some weapons take multiple slots))
    - Armor Rating (can be upgraded at port)
    - Weapons (accuracy, damage)
    - Crewmates (up to 3 can be assigned to a weapon, increasing weapon abilities with each one)
    - Cargo Size (loot capacity)
    - Health
    
- Each Weapon has: 
    - Base Accuracy
    - Base Damage
    - Armor Rating
    - Health
    - Special Properties (Attacks per Action, etc)
    - Price
    
- Each Crewmate has:
    - Health
    - Armor Rating
    - Bonuses (Applied to whatever weapon they operate)
    - Name(and flavortext?)
- In an encounter, each you and your enemy will take turns firing upon each other. 
    - Every turn, each weapon can be aimed at an enemy weapon and gets an action (some weapons may take two actions per attack, or get more than one attack per action)
        - Weapons need at least one crewmate assigned to attack. Any additional ones just add stats (accuracy, damage, whatever the crewmate provides)
        - Each attack goes through an accuracy check. After all accuracy values are tallied, RNG determines if it hits.
            - If it hits, damage is calculated using all the relevant bonuses/modifiers from the ship/crew/weapon
            - If it doesn't hit, there is a chance it will go awry and hit a random other enemy weapon
        - When a target is hit, damage is done to the ship that was hit, the weapon that was hit, and the crewmates manning that weapon
            - This damage is calculated by (dmg - def), def being the AR of the target.
        - If a crewmate dies, they are permanently dead. They can only be replaced by visiting port and hiring another crewmate.
        - If a weapon 'dies', it enters a broken state and can no longer be used until it can be repaired back at port.
        - If your ship capsizes, it's game over, start again with another dinghy.
    - When you win an encounter, you raid the enemy ship and make off with the loot. Your cargo hold becomes fuller, and you can decide to head to port or enter another encounter.

- When you head to port, you have several options:
    - Empty your hold and sell your loot
    - Scour the common areas for potential crewmates (random assortment of people to hire, with various bonuses for various prices)
    - Repair your ship and its weapons
    - Reinforce/upgrade your ship
    - Buy larger/better ships
    - Buy and mount weapons on your ship