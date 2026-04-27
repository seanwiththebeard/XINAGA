#ifndef _GAMEDATA_H
#define _GAMEDATA_H

#include "Xinaga.h"

#define screenName byte //Corresponds to NES banks at 0x8000
#define Title 3
#define EditParty 4
#define Map 0
#define Combat 1
//#define Menu 4
//#define SaveLoad 5
#define MapGen 5
#define Scenario 2
//extern screenName currentScreen;
//extern screenName nextScreen;

//Debug Functions
//void DebugMap();
//void DebugGraphics();

#define ATTRIB_NONE -1
#define ATTRIB_STR 0
#define ATTRIB_CON 1
#define ATTRIB_DEX 2
#define ATTRIB_INT 3
#define ATTRIB_WIS 4
#define ATTRIB_CHA 5

void DrawInterface();

#define mapMatrixWidth 16
#define mapMatrixHeight 16

//extern byte MiniMapPosX;
//extern byte MiniMapPosY;
extern byte SetPlayerPositionX;
extern byte SetPlayerPositionY;
extern const byte MiniMapGlyphs[64];
extern const byte OverworldGeoMorphSet[256];
extern const byte OverworldGeoMorphAttrib[32];
extern const byte DungeonGeoMorphSet[256];
extern const byte DungeonGeoMorphAttrib[32];
#define MiniMapOffset 0xE0
//Each mapQuad is a 16x16 area, corresponding to a 16x16 geomorph from the bitmap containing 64
extern byte mapQuads[mapMatrixHeight * mapMatrixWidth];

//extern byte followIndex;

void RunGame(screenName startingScreen);
void DrawMiniMap(bool highlightPlayer);
void WaitForInput(void);
void LoadMap(void);
void DrawMap(void);

extern byte MiniMapHighlightX;
extern byte MiniMapHighlightY;
void DrawLocalMiniMap(bool checkLast);

//Add Character
screenName DrawAddCharacterScreen(void);

//Map
screenName MapUpdate(void);
void LoadMapQuads();

//Combat
screenName Update_Combat(void);

//MapGen
screenName Update_MapGen(void);

//Scenario
//screenName Update_Scenario(void);

void TickMoonPhase(void);

byte RollDice(byte count, byte diceSize);
//extern struct Party;

void create(void);
void delete_pos(byte pos);
byte CountRoster(void);
void AddParty(byte index);
void RemoveParty(byte index);

typedef struct playerChar
{
  struct playerChar *next;
  int	EXP;
  char 	NAME[8];
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
        SUBCLASS,
  	ARMOR,
        WEAPON,
  	LEVEL_MAIN,
        LEVEL_SUB,
        ELEMENT,
  	HOMEWORLD;
};

struct playerChar *getPlayerChar(byte index);

typedef struct Session
{
        struct playerChar RosterChars[12];
        int GOLD, STEPS, CARRYING_WEIGHT, CARRYING_CAPACITY;
        byte PartyChars[4], SOLAR, LUNAR, MINAR;
};
//typedef struct SavedGame
//{
        //struct Session gameSessions[4];
//};

extern struct Session Sessions[1];

byte CountParty(void);

struct playerChar *getPartyMember(byte index);

//RPGData
typedef struct
{
  char *NAME;
  byte HITDICEMAX;
  byte ATTRIB_BONUS;
  byte ATTRIB_PENALTY;
  byte TRAITS;
  byte ELEMENT_WEAKNESS;
  byte ELEMENT_RESIST;
  byte CONDITION_RESIST;
}RaceDescriptionDef;
extern const RaceDescriptionDef const RaceDescription[8];

typedef struct
{
  char *NAME;
  byte HITDICE;
  byte ATTRIB_DEP;
  byte WEAPON_PROF;
  byte ARMOR_PROF;
  byte SPELLS_WIZ;
  byte SPELLS_CLER;
  byte SONGS_BARD;
}ClassDescriptionDef;
extern const ClassDescriptionDef const ClassDescription[8];

extern const sbyte const AbilityModifier[];
extern const char namePrefixA[8][8][8];
extern const char nameSuffixA[8][8][8];
#endif
