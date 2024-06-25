#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
#pragma code-name (push, "LOWCODE")
#endif

#if defined (__NES__)
#endif

byte strTemp[ConsoleBufferLength] = {};

struct
{
  int
  	GOLD;
  byte
  	CARRYING_WEIGHT,
  	CARRYING_CAPACITY; 
}Party = {};

uint16_t randseed = 0;

byte MiniMapHighlightX = 0;
byte MiniMapHighlightY = 0;

void DrawMiniMap(bool highlightPlayer)
{
  byte x, y, tile;
  DrawBorder("Minimap@", MiniMapX, MiniMapY, mapMatrixWidth + 2, mapMatrixHeight + 2, true);
  for (y = 0; y < mapMatrixHeight; ++y)
  {
    for (x = 0; x < mapMatrixWidth; ++x)
    {
      tile = mapQuads[y][x];
      tile = (tile << 1) + ((tile >> 3) << 4);
      SetChar(tile, x + MiniMapX + 1, y + MiniMapY + 1);
    }
  }
  if(highlightPlayer)
    SetChar('X', MiniMapX + MiniMapHighlightX + 1, MiniMapY + MiniMapHighlightY + 1);
}

byte RollDice(byte count, byte diceSize)
{
  byte result = 0;
  byte i;
  for (i = 0; i < count; i++)
    result += (rand() %diceSize + 1);
  return result;
}

struct playerChar *startRoster=NULL;

byte CountRoster()
{
  struct playerChar *temp = startRoster;
  byte i = 0;
  while(temp != NULL)
  {
    ++i;
    temp = temp->next;
  }
  return i;
}

void create()
{
  struct playerChar *temp,*ptr;
  temp=(struct playerChar *)malloc(sizeof(struct playerChar));  

  if(temp==NULL)
    exit(0);

  temp->next=NULL;
  if(startRoster==NULL)
    startRoster=temp;
  else
  {
    ptr=startRoster;
    while(ptr->next!=NULL)
    {
      ptr=ptr->next;
    }
    ptr->next=temp;
  }
}

struct playerChar *getPlayerChar(byte index)
{
  byte i = 0;
  struct playerChar *tmp = startRoster;
  while (tmp != NULL)
  {
    if(i == index)
    {
      return tmp;
    }
    tmp = tmp->next;
    ++i;
  }
}

void delete_pos(byte pos)
{
  byte i;
  struct playerChar *temp,*ptr;

  if(startRoster==NULL)
    exit(0);
  else
  {
    if(pos==0)
    {
      ptr=startRoster;
      startRoster=startRoster->next ;
    }
    else
    {
      ptr=startRoster;
      for(i=0;i<pos;i++)
      {
        temp=ptr; 
        ptr=ptr->next ;
        if(ptr==NULL)
        {
          WriteLineMessageWindow("Position not Found:@", 0);
          return;
        }
      }
      temp->next =ptr->next ;
    }
    //sprintf(str, "Deleted element:%d",ptr->character.NAME);
    //WriteLineMessageWindow(str, 0);
    free(ptr);
  }
}

//Party
struct playerChar *startParty=NULL;

byte CountParty()
{
  struct playerChar *temp = startParty;
  byte i = 0;
  while(temp != NULL)
  {
    ++i;
    temp = temp->next;
  }
  return i;
}

struct playerChar *getPartyMember(byte index)
{
  byte i = 0;
  struct playerChar *tmp = startParty;
  while (tmp != NULL)
  {
    if(i == index)
    {
      return tmp;
    }
    tmp = tmp->next;
    ++i;
  }
}

void AddParty(byte index)
{
  struct playerChar *temp,*ptr,*src;
  temp=(struct playerChar *)malloc(sizeof(struct playerChar));
  src = getPlayerChar(index);

  if(temp==NULL)
    exit(0);
  
  memcpy(temp, src, sizeof(struct playerChar));
  
  temp->next=NULL;
  if(startParty==NULL)
    startParty=temp;
  else
  {
    ptr=startParty;
    while(ptr->next!=NULL)
    {
      ptr=ptr->next;
    }
    ptr->next=temp;
  }
  delete_pos(index);
}

void DeleteParty(byte pos)
{
  byte i;
  struct playerChar *temp,*ptr;

  if(startParty==NULL)
    exit(0);
  else
  {
    if(pos==0)
    {
      ptr=startParty;
      startParty=startParty->next ;
    }
    else
    {
      ptr=startParty;
      for(i=0;i<pos;i++)
      {
        temp=ptr; 
        ptr=ptr->next ;
        if(ptr==NULL)
        {
          WriteLineMessageWindow("Position not Found:@", 0);
          return;
        }
      }
      temp->next =ptr->next ;
    }
    //sprintf(str, "Deleted element:%d",ptr->character.NAME);
    //WriteLineMessageWindow(str, 0);
    free(ptr);
  }
}

void RemoveParty() //Removes Last Party Member (?)
{
  byte index = CountParty()-1;
  struct playerChar *temp,*ptr,*src;
  temp=(struct playerChar *)malloc(sizeof(struct playerChar));
  src = getPartyMember(index);

  if(temp==NULL)
    exit(0);
  
  memcpy(temp, src, sizeof(struct playerChar));
  
  temp->next=NULL;
  if(startRoster==NULL)
    startRoster=temp;
  else
  {
    ptr=startRoster;
    while(ptr->next!=NULL)
    {
      ptr=ptr->next;
    }
    ptr->next=temp;
  }
  DeleteParty(index);
}

#define CharStatPosX 24
#define CharStatPosY 3
#define CharStatRows 2
void DrawCharStatus(byte characterIndex)
{
  //byte statX = CharStatPosX;
  byte statY = CharStatPosY + characterIndex * (CharStatRows + 1);
  struct playerChar *PlayerChar = getPartyMember(characterIndex);

  DrawBorder(PlayerChar->NAME, CharStatPosX - 1, statY - 1, COLS - CharStatPosX + 1, 2 + CharStatRows, true);
  DrawTileDirectXY(PlayerChar->CLASS, CharStatPosX, statY);
  ConsoleBufferReset();
  ConsoleBufferAdd(RaceDescription[PlayerChar->RACE].NAME);
  ConsoleBufferAdd(ClassDescription[PlayerChar->CLASS].NAME);
  ConsoleBufferPrint(CharStatPosX + 2, statY);
  sprintf(strTemp, "HP:%d/%d@", PlayerChar->HP, PlayerChar->HPMAX);  
  PrintString(strTemp, CharStatPosX + 9, statY + 1, true, false);
  ConsoleBufferReset();
  
  //sprintf(strTemp, "%s@", ClassDescriptions[PlayerChar->CLASS].NAME);
  //PrintString(strTemp, CharStatPosX, statY + 1, true, false);
  //ReverseBufferArea(statX - 1, statY - 1, COLS - statX + 1, 5);
  //CopyDoubleBufferArea(statX - 1, statY - 1, COLS - statX + 1, 5);
}

byte moonA = 0;
byte moonB = 0;
char phaseChar[4] = " )O(";
byte moonTick = 0;

void DrawMoonPhase()
{
  ConsoleBufferReset();
  sprintf(strTemp, "< %c > < %c >@", phaseChar[moonA], phaseChar[moonB]);
  PrintString(strTemp, CharStatPosX + 2, CharStatPosY - 2, true, false);
  ConsoleBufferReset();
}
void TickMoonPhase()
{
  bool draw = false;
  ++moonTick;
  if (moonTick % 4 == 0)
  {
    ++moonA;
    draw = true;
  }
  if (moonTick % 2 == 0)
  {
    ++moonB;
    draw = true;
  }

  if (draw)
  {
    if (moonA > 3)
      moonA = 0;
    if (moonB > 3)
      moonB = 0;
    DrawMoonPhase(); 
  }
}

void DrawCharStats()
{
  byte i;
  DrawBorder("Moon Phases@", CharStatPosX - 1, CharStatPosY - 3, COLS - CharStatPosX + 1, 3, true);
  DrawMoonPhase();
  
  DrawBorder("Party@", CharStatPosX - 1, CharStatPosY - 1, COLS - CharStatPosX + 1, 1 + 4 * (CharStatRows + 1), true);
  for (i = 0; i < CountParty(); ++i)
    DrawCharStatus(i);
}