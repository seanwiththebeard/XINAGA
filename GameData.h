#ifndef _GAMEDATA_H
#define _GAMEDATA_H

#include "Xinaga.h"

#define screenName byte
#define Title 0
#define EditParty 1
#define Map 2
#define Combat 3
#define Menu 4
#define SaveLoad 5
#define MapGen 6
#define Scenario 7

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

void TickMoonPhase();

//BFRPG
byte RollDice(byte count, byte diceSize);
typedef struct
{
  int
  	GOLD;
  byte
  	CARRYING_WEIGHT,
  	CARRYING_CAPACITY;  
}PartyDef;
extern PartyDef Party;

typedef struct 
{
  char
    	NAME[12];
  byte
    	HITDICEMAX;
}RaceDescriptionDef;
extern RaceDescriptionDef RaceDescription[4];

typedef struct
{
  char NAME[12]; 
  byte HITDICE;
}ClassDescriptionDef;
extern ClassDescriptionDef ClassDescription[4];

extern sbyte AbilityModifier[];

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
  	LEVEL,
  	CARRYING_WEIGHT,
  	CARRYING_CAPACITY;
  int
    	EXP;
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