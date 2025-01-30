#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
#pragma code-name (push, "LOWCODE")
#endif

#if defined (__NES__)
#pragma code-name (push, "STARTUP")
//#pragma rodata-name (push, "STARTUP")
#endif

void DebugGraphics()
{
  InitializeGraphics();
  ResizeMessageWindow(consolePosX, ROWS - 9, consoleWidth, 6);
  WriteLineMessageWindow("The Quick Brown Fox Jumps Over The Lazy Dog@", 0);
  WriteLineMessageWindow("ABCDEFGHIJKLMNOPQRSTUVWXYZ@", 0);
  WriteLineMessageWindow("abdefghijklmnopqrstuvwxyz@", 0);
  WriteLineMessageWindow("01234567890 !#$%^&@", 0);
  WriteLineMessageWindow("*()-=[];':<>,./?@", 0);
  
  DrawCharset();
  while(1);
}

struct
{
  int
  	GOLD;
  byte
  	CARRYING_WEIGHT,
  	CARRYING_CAPACITY; 
}Party;

uint16_t randseed;

byte MiniMapHighlightX;
byte MiniMapHighlightY;
void DrawMiniMap(bool highlightPlayer)
{
  byte x, y, tile = 0;
  DrawBorder("Minimap@", viewportPosX, viewportPosY, mapMatrixWidth + 2, mapMatrixHeight + 2, true);
  for (y = 0; y < mapMatrixHeight; ++y)
  {
    for (x = 0; x < mapMatrixWidth; ++x)
    {
      //tile = mapQuads[x + (y * mapMatrixWidth)];
      tile = (tile << 1) + ((tile >> 3) << 4);
      SetChar(tile, x + viewportPosX + 1, y + viewportPosY + 1);
    }
  }
  if(highlightPlayer)
    SetChar('X', viewportPosX + 1 + MiniMapHighlightX, viewportPosY + 1 + MiniMapHighlightY);
}

void DrawCharset()
{
  byte x, y;
  //DrawBorder("Charset@", viewportPosX - 1, viewportPosY - 1, 16+ 2, 16 + 2, true);
  for (y = 0; y < 16; ++y)
    for (x = 0; x < 16; ++x)
      SetChar(x + y*16, x + viewportPosX, y + viewportPosY);
}

byte RollDice(byte count, byte diceSize)
{
  byte result = 0;
  byte i;
  for (i = 0; i < count; i++)
    result += (rand() %diceSize + 1);
  return result;
}

struct playerChar *startRoster;
//struct playerChar *startRoster=NULL;

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
    return;

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
  temp = NULL;

  if(startRoster==NULL)
    return;
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
      temp->next = ptr->next ;
    }
    //sprintf(str, "Deleted element:%d",ptr->character.NAME);
    //WriteLineMessageWindow(str, 0);
    free(ptr);
  }
}

//Party
struct playerChar *startParty;
//struct playerChar *startParty=NULL;

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
    return;
  
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
  temp = NULL;

  if(startParty==NULL)
    return;
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
    return;
  
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


void DrawCharStatus(byte characterIndex)
{
  //byte statX = CharStatPosX;
  byte statY = contextMenuPosY + characterIndex * (3);
  struct playerChar *PlayerChar = getPartyMember(characterIndex);

  DrawBorder(PlayerChar->NAME, contextMenuPosX - 1, statY - 1, contextMenuWidth, 4, true);
  DrawTileDirectXY(PlayerChar->CLASS, contextMenuPosX, statY);
  ConsoleBufferReset();
  ConsoleBufferAdd(RaceDescription[PlayerChar->RACE].NAME);
  ConsoleBufferPrint(contextMenuPosX + 2, statY);
  ConsoleBufferAdd(ClassDescription[PlayerChar->CLASS].NAME);
  ConsoleBufferPrint(contextMenuPosX + 2, statY+1);
  sprintf(strTemp, "HP:%d/%d@", PlayerChar->HP, PlayerChar->HPMAX);  
  PrintString(strTemp, contextMenuPosX + 9, statY + 1, true, false);
  ConsoleBufferReset();
  
  //sprintf(strTemp, "%s@", ClassDescriptions[PlayerChar->CLASS].NAME);
  //PrintString(strTemp, CharStatPosX, statY + 1, true, false);
  //ReverseBufferArea(statX - 1, statY - 1, COLS - statX + 1, 5);
  //CopyDoubleBufferArea(statX - 1, statY - 1, COLS - statX + 1, 5);
}

byte moonA;
byte moonB;
const char phaseChar[] = " )*(";
byte moonTick;

void DrawMoonPhase()
{
  ConsoleBufferReset();
  sprintf(strTemp, "<%c><%c>@", phaseChar[moonA], phaseChar[moonB]);
  PrintString(strTemp, viewportPosX + (viewportWidth / 2) + 2, viewportPosY -1, true, false);
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
  //DrawBorder("Moon Phases@", contextMenuPosX - 1, contextMenuPosY - 1, contextMenuWidth + 2, 3, true);
  DrawMoonPhase();
  DrawBorder("Party@", contextMenuPosX - 1, contextMenuPosY - 1, contextMenuWidth + 2, viewportHeight*2 + 2, true);
  for (i = 0; i < CountParty(); ++i)
    DrawCharStatus(i);
}
