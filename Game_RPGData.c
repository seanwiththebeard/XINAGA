#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
//#pragma code-name (push, "LOWCODE")
//#pragma rodata-name (push, "LOWCODE")
#endif

#if defined (__NES__)
//#pragma code-name (push, "GAME")
//#pragma rodata-name (push, "GAME")
//#pragma data-name (push, "XRAM")
#pragma bss-name (push, "XRAM")
#pragma rodata-name (push, "GAME_RPGDATA")
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
  byte CLASS_ALLOW;
  */

/*
  Traits
  Darkvision, Breathweapon, Toughskin, Subterranean, Topsider, Nimble, Affable, Lunacy
*/

/*
  Elements
  Fire, Water, Ice, Wind, Hope, Earth, Dark, Ethereal
*/

/*
  Classes
  "Fighter", "Ranger", "Wizard", "Barbarian", "Monk", "Bard", "Thief", "Cleric"
*/

/*
  Skills
  Thievery, Hide, Calm Animal, Calm Undead, Calm Humanoid, Cast Cleric, Cast Mage, Cast Bard
*/

const RaceDescriptionDef RaceDescription[8] =
{
  {"Human",
   10, //HITDICEMAX
   ATTRIB_NONE, //ATTRIB_BONUS
   ATTRIB_NONE, //ATTRIB_PENALTY
   0b00001000, //TRAITS Topsider
   0b00000000, //ELEMENT_WEAKNESS None
   0b00000000, //ELEMENT_RESIST None
   0b00000000, //CONDITION_RESIST None
   0b11111111}, //CLASS_ALLOW All

  {"Effel",
   8,
   ATTRIB_WIS, //ATTRIB_BONUS
   ATTRIB_CON, //ATTRIB_PENALTY
   0b00001001, //TRAITS Topsider, Lunacy
   0b00010010, //ELEMENT_WEAKNESS Wind, Dark
   0b01000100, //ELEMENT_RESIST Earth, Water
   0b00000000, //CONDITION_RESIST
   0b01101111}, //CLASS_ALLOW "Ranger", "Wizard", "Monk", "Bard", "Thief", "Cleric"

  {"Duerf",
   12,
   ATTRIB_CON, //ATTRIB_BONUS
   ATTRIB_WIS, //ATTRIB_PENALTY
   0b10010000, //TRAITS Darkvision, Subterranean
   0b01100000, //ELEMENT_WEAKNESS Water, Ice
   0b10000100, //ELEMENT_RESIST Earth, Fire
   0b00000000, //CONDITION_RESIST
   0b10111101}, //CLASS_ALLOW "Fighter", "Wizard", "Barbarian", "Monk", "Bard", "Cleric"

  {"Lefling",
   6,
   ATTRIB_CHA, //ATTRIB_BONUS
   ATTRIB_STR, //ATTRIB_PENALTY
   0b00000110, //TRAITS Nimble, Affable
   0b10000010, //ELEMENT_WEAKNESS Fire, Dark
   0b01100000, //ELEMENT_RESIST Water, Ice
   0b00000000, //CONDITION_RESIST
   0b11101110}, //CLASS_ALLOW "Fighter", "Ranger", "Barbarian", "Monk", "Bard", "Thief"

  {"Mork", //Not quite a puppet, not quite pork
   12,
   ATTRIB_STR, //ATTRIB_BONUS
   ATTRIB_WIS, //ATTRIB_PENALTY
   0b00101000, //TRAITS Toughskin, Topsider
   0b00100001, //ELEMENT_WEAKNESS Ice, Ethereal
   0b00001100, //ELEMENT_RESIST Hope, Earth
   0b00000000, //CONDITION_RESIST
   0b01111101}, //CLASS_ALLOW "Fighter", "Ranger", "Wizard", "Barbarian", "Monk", "Bard", "Thief", "Cleric"
  
  {"Fosz", //Sort of like a rakshasa, but less intense
   8,
   ATTRIB_DEX, //ATTRIB_BONUS
   ATTRIB_INT, //ATTRIB_PENALTY
   0b00001010, //TRAITS Topsider, Affable
   0b10000000, //ELEMENT_WEAKNESS Fire
   0b00000000, //ELEMENT_RESIST
   0b00000000, //CONDITION_RESIST
   0b01011110}, //CLASS_ALLOW  "Ranger", "Barbarian", "Monk", "Bard", "Thief"

  {"Drecon", //Poser, tryhard, stand-in for regular orcs
   12,
   ATTRIB_INT, //ATTRIB_BONUS
   ATTRIB_CHA, //ATTRIB_PENALTY
   0b01010001, //TRAITS BreathWeapon, Subterranean, Lunacy
   0b01111111, //ELEMENT_WEAKNESS All but fire
   0b10001000, //ELEMENT_RESIST Hope, Fire
   0b00000000, //CONDITION_RESIST
   0b11110110}, //CLASS_ALLOW "Fighter", "Ranger", "Wizard", "Barbarian", "Bard", "Thief"
   
  {"Construct", // Never levels up, strong early on but expensive to upgrade stats
   10,
   ATTRIB_STR, //ATTRIB_BONUS
   ATTRIB_DEX, //ATTRIB_PENALTY
   0b00000000, //TRAITS NONE
   0b11111111, //ELEMENT_WEAKNESS ALL
   0b00000000, //ELEMENT_RESIST NONE
   0b11111111, //CONDITION_RESIST All
   0b11111111}, //CLASS_ALLOW All
  };

/*
  char *NAME;
  byte HITDICE;
  byte ATTRIB_DEP;

  byte WEAPON_PROF;
  //Type = Slash, Bludgeon, Pierce, Casting
  //Length = Small, One-Handed, Two-Handed, Ranged
  //Slash Bludgeon Pierce Casting Small One-Handed Two-Handed Ranged

  byte ARMOR_PROF;
  //Weight = Magic, Clothes, Light, Heavy
  //Material = Cloth, Plant, Leather, Metal
  //Magic Clothes Light Heavy Cloth Plant Leather Metal

  byte SPELLS_WIZ;
  byte SPELLS_CLER;
  byte SONGS_BARD;
  byte ATTACKS_MULTIPLE;
  */
const ClassDescriptionDef ClassDescription[8]=
{
  {"Fighter", 8, ATTRIB_STR, 0b11101111, 255, 0, 0, 0, 1},
  {"Ranger", 8, ATTRIB_WIS, 0b10101101, 0b11101110, 0, 0, 0, 2},
  {"Wizard", 4, ATTRIB_INT, 0b01111000, 0b11001100, 255, 0, 0, 1},
  {"Barbarian", 6, ATTRIB_CON, 0b11100010, 0b00110110, 0, 0, 0, 2},
  {"Monk", 6, ATTRIB_CON, 0b01000010, 0b11001000, 0, 0, 0, 4},
  {"Bard", 4, ATTRIB_CHA, 0b11111100, 0b10001110, 0, 0, 255, 1},
  {"Thief", 4, ATTRIB_DEX, 0b11101101, 0b11001000, 0, 0, 0, 2},
  {"Cleric", 6, ATTRIB_WIS, 0b01011101, 0b11101010, 0, 255, 0, 1}
};

const sbyte AbilityModifier[20] = {-3, -3, -3, -3, -3, -2, -2, -1, -1, -1, 0, 0, 0, 0, +1, +1, +1, +2, +2, +3};
const char AttributeName[6][4] = {"STR", "CON", "DEX", "INT", "WIS", "CHA"};

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
  //Material = Cloth, Plant, Leather, Metal
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
#define SKILL_CALMHUMANOID 4
#define SKILL_CASTCLERIC 5
#define SKILL_CASTMAGE 6
#define SKILL_CASTBARD 7

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

//Elements
#define ELEMENT_FIRE 0
#define ELEMENT_WATER 1
#define ELEMENT_ICE 2
#define ELEMENT_WIND 3
#define ELEMENT_HOPE 4
#define ELEMENT_EARTH 5
#define ELEMENT_DARK 6
#define ELEMENT_ETHEREAL 7

//Item Effects 0-16
//Item Potency 0-16
#define ITEMEFFECT_HEAL 0
#define ITEMEFFECT_SPELLPOINTS 1
#define ITEMEFFECT_CUREPOISON 2
#define ITEMEFFECT_CURESTONE 3
#define ITEMEFFECT_CUREPARALYSIS 4
#define ITEMEFFECT_CUREBLIND 5
#define ITEMEFFECT_CUREBERSERK 6
#define ITEMEFFECT_CUREZOMBIE 7
#define ITEMEFFECT_CUREVAMPIRE 8
#define ITEMEFFECT_

const RaceDescriptionDef MonsterRace[] =
{
  //Plains
  {"Wolf", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Snake", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Troll", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Imp", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Bird", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Slime", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Lion", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Wyrmling", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  //Forrest
  {"Tree", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Gnome", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Witch", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Imp", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Fosz", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Lycanthrope", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Owl", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Bear", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  //Mountains
  {"Hillbilly", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Ogre", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Duerf", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Centaur", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Cloud", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Rat", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Bat", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Amphibian", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  //Dungeon
  {"Cube", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Abomination", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Goblin", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Centipede", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Eye", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Kobold", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Skeleton", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Shadow", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  //Desert
  {"Mantis", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Worm", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Snake", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Djinn", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Giant", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Drecon", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Tiger", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Turtle", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  //Ocean
  {"Shrimp", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Fish", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Dolphin", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Gull", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Shark", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Squid", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Pirate", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Siren", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  //City Humanoid Monsters (Normal Races)
  {"Commoner", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Guard", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Outlaw", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Assassin", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Pirate", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Madman", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Mage", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Construct", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  //Boss Monsters
  {"Dragon", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Lich", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"Kaiju", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
  {"", 6, ATTRIB_NONE, ATTRIB_NONE, 0b00000000, 0b00000000, 0b00000000, 0b00000000},
};

const ClassDescriptionDef MonsterClass[]=
{
  {"Normal", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  //Common
  {"Jelly", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Scaly", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Metal", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Undead", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Leafy", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Fuzzy", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Feathered", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Stone", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  //Uncommon
  {"Clockwork", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Spectral", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Berserker", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Inverted", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Double-armed", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Crystal", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Corrupting", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Two-Headed", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  //Boss
  {"Normal", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Thunder", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Fire", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Ice", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Acid", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Smasher", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Mechanoid", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
  {"Melting", 8, ATTRIB_STR, 255, 255, 0, 0, 0},
};

/*

*/
