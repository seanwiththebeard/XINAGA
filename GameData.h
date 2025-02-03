#ifndef _GAMEDATA_H
#define _GAMEDATA_H

#include "Xinaga.h"


#if defined (__NES__)
//#pragma code-name (push, "STARTUP")
//#pragma rodata-name (push, "STARTUP")
//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
#endif

#define screenName byte //Corresponds to NES banks at 0x8000
#define Title 3
#define EditParty 4
#define Map 0
#define Combat 1
//#define Menu 4
//#define SaveLoad 5
#define MapGen 5
#define Scenario 2
extern screenName currentScreen;
void DebugGraphics();

#define mapMatrixWidth 16
#define mapMatrixHeight 16
extern byte mapQuads[mapMatrixHeight * mapMatrixWidth];

extern byte followIndex;

void Demo(void);
void DrawMiniMap(bool highlightPlayer);
void WaitForInput(void);
void LoadMap(void);
void DrawMap(void);

extern byte MiniMapHighlightX;
extern byte MiniMapHighlightY;

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

//RPG Data
byte RollDice(byte count, byte diceSize);
extern struct Party;

typedef struct 
{
  char
    	NAME[12];
  byte
    	HITDICEMAX;
}RaceDescriptionDef;
extern const RaceDescriptionDef const RaceDescription[4];

typedef struct
{
  char NAME[12]; 
  byte HITDICE;
}ClassDescriptionDef;
extern const ClassDescriptionDef const ClassDescription[8];

extern const sbyte const AbilityModifier[];

//Lists
	//Roster
void create(void);
void delete_pos(byte pos);
byte CountRoster(void);
typedef struct playerChar
{
  struct playerChar *next;
  int	EXP;
  char 	NAME[16];
  bool 	inParty;
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
  	CARRYING_CAPACITY,
  	HOMEWORLD;
};

struct playerChar *getPlayerChar(byte index);

	//Party
void AddParty(byte index);
void RemoveParty(void);
byte CountParty(void);

struct playerChar *getPartyMember(byte index);
#endif
