#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
//#pragma rodata-name (push, "LC")
//#pragma code-name (push, "CODE")
#endif

#if defined (__NES__)
#pragma code-name (push, "GAME")
#pragma rodata-name (push, "GAME")
//#pragma data-name (push, "XRAM")
#pragma bss-name (push, "XRAM")
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
  byte ELEMENT_WEAKNESS;
  byte ELEMENT_RESIST;
  byte CONDITION_RESIST;

  //Traits
#define TRAIT_DARKVISION 0 //See without fire in combat in low-light environments
#define TRAIT_BREATHWEAPON 1 //Create fire projectile adjacent to character for current turn
#define TRAIT_TOUGHSKIN 2 //-4 on evasion rolls but halves physical damage
#define TRAIT_SUBTERRANEAN 3 //+1 for all rolls below ground
#define TRAIT_TOPSIDER 4 //+1 for all rolls above ground
#define TRAIT_NIMBLE 5 //+2 for all DEX rolls
#define TRAIT_AFFABLE 6 //+2 for all CHR rolls
#define TRAIT_LUNACY 7 //+2 for all rolls for each full moon
  */

#define ELEMENT_FIRE 0
#define ELEMENT_WATER 1
#define ELEMENT_ICE 2
#define ELEMENT_WIND 3
#define ELEMENT_HOPE 4
#define ELEMENT_EARTH 5
#define ELEMENT_DARK 6
#define ELEMENT_ETHEREAL 7

const RaceDescriptionDef RaceDescription[8] =
{
  {"Human",
   10, //HITDICEMAX
   ATTRIB_NONE, //ATTRIB_BONUS
   ATTRIB_NONE, //ATTRIB_PENALTY
   0b00001000, //TRAITS Topsider
   0b00000000, //ELEMENT_WEAKNESS None
   0b00000000, //ELEMENT_RESIST None
   0b00000000}, //CONDITION_RESIST None

  {"Effel",
   8,
   ATTRIB_WIS, //ATTRIB_BONUS
   ATTRIB_CON, //ATTRIB_PENALTY
   0b00001001, //TRAITS Topsider, Lunacy
   0b00010010, //ELEMENT_WEAKNESS Wind, Dark
   0b01000100, //ELEMENT_RESIST Earth, Water
   0b00000000}, //CONDITION_RESIST

  {"Duerf",
   12,
   ATTRIB_CON, //ATTRIB_BONUS
   ATTRIB_WIS, //ATTRIB_PENALTY
   0b10010000, //TRAITS Darkvision, Subterranean
   0b01100000, //ELEMENT_WEAKNESS Water, Ice
   0b10000100, //ELEMENT_RESIST Earth, Fire
   0b00000000}, //CONDITION_RESIST

  {"Lefling",
   6,
   ATTRIB_CHA, //ATTRIB_BONUS
   ATTRIB_STR, //ATTRIB_PENALTY
   0b00000110, //TRAITS Nimble, Affable
   0b10000010, //ELEMENT_WEAKNESS Fire, Dark
   0b01100000, //ELEMENT_RESIST Water, Ice
   0b00000000}, //CONDITION_RESIST

  {"Mork", //Not quite a puppet, not quite pork
   12,
   ATTRIB_STR, //ATTRIB_BONUS
   ATTRIB_WIS, //ATTRIB_PENALTY
   0b00101000, //TRAITS Toughskin, Topsider
   0b00100001, //ELEMENT_WEAKNESS Ice, Ethereal
   0b00001100, //ELEMENT_RESIST Hope, Earth
   0b00000000}, //CONDITION_RESIST

  {"Fosz", //Sort of like a rakshasa, but more chill
   8,
   ATTRIB_DEX, //ATTRIB_BONUS
   ATTRIB_INT, //ATTRIB_PENALTY
   0b00001010, //TRAITS Topsider, Affable
   0b00000000, //ELEMENT_WEAKNESS
   0b00000000, //ELEMENT_RESIST
   0b00000000}, //CONDITION_RESIST

  {"Drecon", //Poser, tryhard, stand-in for regular orcs
   12,
   ATTRIB_INT, //ATTRIB_BONUS
   ATTRIB_CHA, //ATTRIB_PENALTY
   0b10010001, //TRAITS BreathWeapon, Subterranean, Lunacy
   0b11111111, //ELEMENT_WEAKNESS All but fire
   0b00000000, //ELEMENT_RESIST Hope, Fire
   0b00000000}, //CONDITION_RESIST

  {"Construct", // Never levels up, strong early on but expensive to upgrade stats
   10,
   ATTRIB_STR, //ATTRIB_BONUS
   ATTRIB_DEX, //ATTRIB_PENALTY
   0b00000000, //TRAITS
   0b00000000, //ELEMENT_WEAKNESS None
   0b11111111, //ELEMENT_RESIST All
   0b11111111} //CONDITION_RESIST All
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

const char WeaponName[16][11] =
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
const char ArmorName[16][16] =
{
  //Weight = Magic, Clothes, Light, Heavy
  //Cloth
  "RegalTunic", "Clothes", "Robe", "Overcoat",
  //Plant
  "GoldflowerTunic", "Flax Robe", "CoconutPlate", "ShalePlate",
  //Leather
  "KitsunePelt", "Fur Pelt", "Studded Hide", "Leathermail",
  //Metal
  "MithrilMail", "Constr Frame", "Chainmail", "Platemail"
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
#define SKILL_THIEVERY 0
#define SKILL_HIDE 1
#define SKILL_CALMANIMAL 2
#define SKILL_CALMUNDEAD 3
#define SKILL_CASTCLERIC 4
#define SKILL_CASTMAGE 5
#define SKILL_CASTBARD 6

//Traits
#define TRAIT_DARKVISION 0 //See without fire in combat in low-light environments
#define TRAIT_BREATHWEAPON 1 //Create fire projectile adjacent to character for current turn
#define TRAIT_TOUGHSKIN 2 //-4 on evasion rolls but halves physical damage
#define TRAIT_SUBTERRANEAN 3 //+1 for all rolls below ground
#define TRAIT_TOPSIDER 4 //+1 for all rolls above ground
#define TRAIT_NIMBLE 5 //+2 for all DEX rolls
#define TRAIT_AFFABLE 6 //+2 for all CHR rolls
#define TRAIT_LUNACY 7 //+2 for all rolls for each full moon

//Conditions
#define CONDITION_POISON 0
#define CONDITION_STONE 1
#define CONDITION_PARALYSIS 2
#define CONDITION_BLIND 3
#define CONDITION_SILENCE 4
#define CONDITION_BERSERK 5 //Barbarians enter combat with status of berserk for first 4 turns
#define CONDITION_ZOMBIE 6
#define CONDITION_VAMPIRE 7

//Item Effects 0-16
//Item Potency 0-16
#define ITEMEFFECT_HEAL
#define ITEMEFFECT_SPELLPOINTS
#define ITEMEFFECT_CUREPOISON
#define ITEMEFFECT_CURESTONE
#define ITEMEFFECT_CUREPARALYSIS
#define ITEMEFFECT_CUREBLIND
#define ITEMEFFECT_CUREBERSERK
#define ITEMEFFECT_CUREZOMBIE
#define ITEMEFFECT_CUREVAMPIRE
#define ITEMEFFECT_

const RaceDescriptionDef MonsterRace[8] =
{
  {"Slime", 10, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Wolf", 8, ATTRIB_WIS, ATTRIB_CON, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Bird", 12, ATTRIB_CON, ATTRIB_WIS, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Rodent", 6, ATTRIB_CHA, ATTRIB_STR, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Goblin", 12, ATTRIB_STR, ATTRIB_WIS, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Fishman", 8, ATTRIB_DEX, ATTRIB_INT, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Wyrm", 12, ATTRIB_INT, ATTRIB_CHA, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Humanoid", 10, ATTRIB_STR, ATTRIB_DEX, 0b00000000, 0b00000000, 0b00000000, 0b00000000}
};

const ClassDescriptionDef MonsterClass[8]=
{
  {"Normal", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Magic", 8, ATTRIB_WIS, 0, 0, 0, 0, 0},
  {"Furry", 4, ATTRIB_INT, 0, 0, 255, 0, 0},
  {"Scaly", 6, ATTRIB_CON, 0, 0, 0, 0, 0},
  {"", 6, ATTRIB_CON, 0, 0, 0, 0, 0},
  {"", 4, ATTRIB_CHA, 0, 0, 0, 0, 255},
  {"", 4, ATTRIB_DEX, 0, 0, 0, 0, 0},
  {"", 6, ATTRIB_WIS, 0, 0, 0, 0, 0}
};
