#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
//#pragma code-name (push, "LOWCODE")
//#pragma code-name (push, "CODE")
#endif

#if defined (__NES__)
//#pragma code-name (push, "XINAGA_CONSOLE")
#pragma rodata-name (push, "XINAGA")
//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
#endif

#if defined (__C64__)
//#pragma code-name (push, "XINAGA")
#pragma rodata-name (push, "GAME_RPGDATA")
#endif

const RaceDescriptionDef RaceDescription[8] =
{
  {"Human", 255},
  {"Elf", 6},
  {"Dwarf", 255},
  {"Frobit", 6},
  {"Orc", 6},
  {"Fuzzer", 6},
  {"Dracon", 6},
  {"Construct", 6} // Never levels up, strong early on but expensive to upgrade stats
};


const ClassDescriptionDef ClassDescription[8]=
{
  {"Fighter", 8},
  {"Ranger", 8},
  {"Wizard", 4},
  {"Paladin", 6},
  {"Monk", 6},
  {"Bard", 4},
  {"Thief", 4},
  {"Cleric", 6}
};

const sbyte AbilityModifier[20] = {-3, -3, -3, -3, -3, -2, -2, -1, -1, -1, 0, 0, 0, 0, +1, +1, +1, +2, +2, +3};

//Weapons
//Type = Slash, Bludgeon, Pierce, Casting
//Length = Small, One-Handed, Two-Handed, Ranged
//Quality = Normal, +1, +2, +3
//Condition = Normal, -1, -2, -3

const char *WeaponName[] =
{
  //Small
  "Knife", "Club", "Dagger", "Spellbook",
  //One-Handed
  "Shortsword", "Stick", "Spear", "Wand",
  //Two-Handed
  "Greatsword", "Hammer", "Halberd", "Device",
  //Ranged
  "Star Glove", "Sling", "Bow", "Staff"
};

const byte WeaponHitDice[] =
{
  4, 4, 4, 0,
  6, 4, 6, 0,
  8, 6, 8, 0,
  6, 4, 8, 4
};

const byte WeaponPrice[] = 
{
  10, 5, 10, 250,
  20, 15, 15, 250,
  100, 20, 30, 250,
  1, 3, 30, 250
};

//Armor
const char *ArmorName[] =
{
  //Weight = Magic, Clothes, Light, Heavy
  //Cloth
  "Regal Tunic", "Clothes", "Robe", "Overcoat"
  //Plant
  "Goldflower Tunic", "Flax Robe", "Coconut Plate", "Shale Plate",
  //Leather
  "Kitsune Pelt", "Fur Pelt", "Studded Hide", "Leathermail",
  //Metal
  "Mithril Mail", "Construct Frame", "Chainmail", "Platemail", 
};
//Enchantment 0-16
const byte ArmorAC[] =
{
  2, 0, 1, 1,
  4, 1, 2, 4,
  5, 2, 3, 4,
  8, 2, 4, 5
};

const byte ArmorPrice[] = 
{
  250, 1, 4, 20,
  250, 8, 12, 24,
  250, 16, 32, 96,
  250, 90, 160, 225
};

//Skills
#define SKILL_PICKLOCK 0
#define SKILL_IDENTIFY 1
#define SKILL_TURNANIMAL 2
#define SKILL_TURNUNDEAD 3
#define SKILL_CASTCLERIC 4
#define SKILL_CASTMAGE 5
#define SKILL_CASTBARD 6
