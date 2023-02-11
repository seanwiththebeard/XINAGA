#include "xinaga.h"

#ifndef _GAME_H
#define _GAME_H
typedef enum { Title, Credits, EditParty, Map, Combat, Menu, SaveLoad } screenName;

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
#endif