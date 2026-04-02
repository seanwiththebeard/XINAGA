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
/*
  char *NAME;
  byte HITDICEMAX;
  byte ATTRIB_BONUS;
  byte ATTRIB_PENALTY;
  byte TRAITS;
  byte ELEMENT_WEAK;
  byte ELEMENT_STRONG;
  byte CONDITION_RESIST;
  */
const RaceDescriptionDef RaceDescription[8] =
{
  {"Human", 10, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Effel", 8, ATTRIB_WIS, ATTRIB_CON, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Duerf", 12, ATTRIB_CON, ATTRIB_WIS, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Lefling", 6, ATTRIB_CHA, ATTRIB_STR, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Mork", 12, ATTRIB_STR, ATTRIB_WIS, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Fosz", 8, ATTRIB_DEX, ATTRIB_INT, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Drecon", 12, ATTRIB_INT, ATTRIB_CHA, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Construct", 10, ATTRIB_STR, ATTRIB_DEX, 0b00000000, 0b00000000, 0b00000000, 0b00000000} // Never levels up, strong early on but expensive to upgrade stats
};

/*
  char *NAME;
  byte HITDICE;
  byte ATTRIB_DEP;
  byte WEAPON_PROF;
  byte ARMOR_PROF;
  byte SPELLS_WIZ;
  byte SPELLS_CLER;
  byte SONGS_BARD;
  */
const ClassDescriptionDef ClassDescription[8]=
{
  {"Fighter", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Ranger", 8, ATTRIB_WIS, 0, 0, 0, 0, 0},
  {"Wizard", 4, ATTRIB_INT, 0, 0, 255, 0, 0},
  {"Barbarian", 6, ATTRIB_CON, 0, 0, 0, 0, 0},
  {"Monk", 6, ATTRIB_CON, 0, 0, 0, 0, 0},
  {"Bard", 4, ATTRIB_CHA, 0, 0, 0, 0, 255},
  {"Thief", 4, ATTRIB_DEX, 0, 0, 0, 0, 0},
  {"Cleric", 6, ATTRIB_WIS, 0, 0, 0, 0, 0}
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
#define SKILL_INVESTIGATE 0
#define SKILL_HIDE 1
#define SKILL_CALMANIMAL 2
#define SKILL_CALMUNDEAD 3
#define SKILL_CASTCLERIC 4
#define SKILL_CASTMAGE 5
#define SKILL_CASTBARD 6

//Traits
#define TRAIT_DARKVISION

//Conditions
#define CONDITION_POISON
#define CONDITION_STONE
#define CONDITION_PARALYSIS
#define CONDITION_BLIND
#define CONDITION_SILENCE
#define CONDITION_BERSERK
#define CONDITION_ZOMBIE
#define CONDITION_
