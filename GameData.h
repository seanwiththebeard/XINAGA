#ifndef _GAMEDATA_H
#define _GAMEDATA_H
#include "Xinaga.h"
#include <stdint.h>
#include <stdlib.h>
typedef enum { Title, Credits, EditParty, Map, Combat, Menu, SaveLoad, MapGen, Scenario } screenName;
extern int randseed;

void Demo(void);

//Title
screenName Update_Title();

//Add Character
screenName DrawAddCharacterScreen();

//Map
screenName MapUpdate();

//Combat
screenName Update_Combat();

//Credits
screenName Update_Credits();

//MapGen
screenName Update_MapGen();

//Scenario
screenName Update_Scenario();

//BFRPG
byte RollDice(byte count, byte diceSize);
typedef struct
{
  int
  	CARRYING_WEIGHT,
  	CARRYING_CAPACITY,
  	GOLD;
}PartyDef;
extern PartyDef Party;

typedef struct 
{
  char
    	NAME[16];
  byte
    	HITDICEMAX;
}RaceDescriptionDef;
extern RaceDescriptionDef RaceDescription[4];

typedef struct
{
  char NAME[16]; 
  byte HITDICE;
}ClassDescriptionDef;
extern ClassDescriptionDef ClassDescription[4];

extern int AbilityModifier[];

//Lists
	//Roster
void create(void);
void delete_pos(byte pos);
byte CountRoster(void);
typedef struct playerChar
{
  bool inParty;
  byte
    	HP,
  	HPMAX,
  	STR,
  	DEX,
        CON,
        INT,
  	WIS,
        CHR,
  
  	RACE,
  	CLASS,
  	HITDICE,
  	LEVEL;
  int
    	EXP,
  	CARRYING_WEIGHT,
  	CARRYING_CAPACITY;
  char
    	NAME[16];
        struct playerChar *next;
};

struct playerChar *getPlayerChar(byte index);


	//Party
void AddParty(byte index);
void RemoveParty(void);
byte CountParty(void);

struct playerChar *getPartyMember(byte index);
#endif