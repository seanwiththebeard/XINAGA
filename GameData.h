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
screenName Update_Title(void);

//Add Character
screenName DrawAddCharacterScreen(void);

//Map
screenName MapUpdate(void);

//Combat
screenName Update_Combat(void);

//Credits
screenName Update_Credits(void);

//MapGen
screenName Update_MapGen(void);

//Scenario
screenName Update_Scenario(void);

void TickMoonPhase(void);

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
  struct playerChar *next;
  int	EXP;
  bool 	inParty;
  char 	NAME[16];
  
  byte	HP,
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
};

struct playerChar *getPlayerChar(byte index);

	//Party
void AddParty(byte index);
void RemoveParty(void);
byte CountParty(void);

struct playerChar *getPartyMember(byte index);

#endif