#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
//#pragma code-name (push, "LC")
//#pragma rodata-name (push, "LC")
#endif

#if defined (__NES__)
#pragma code-name (push, "GAME_RPGDATA")
#pragma rodata-name (push, "GAME_RPGDATA")
//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
#endif

#if defined (__C64__)
#pragma code-name (push, "GAME")
#pragma rodata-name (push, "GAME")
#endif

#define DefaultScreen Title

uint16_t randseed;
screenName nextScreen;

struct Session Sessions[1];
struct playerChar *startRoster;
struct playerChar *startParty;

//Minimap
byte MiniMapPosX;//= 2;
byte MiniMapPosY;// = 2;
byte MiniMapWidth;// = 16;
byte MiniMapHeight;// = 16;
byte MiniMapHighlightX;
byte MiniMapHighlightY;
byte lastX;
byte lastY;

//Moon
byte moonA;
byte moonB;
const char phaseChar[] = " )*(";
byte moonTick;

#if defined (__NES__)
void SelectBank()
{
    MMC3_PRG_8000(currentScreen);
}
#endif

//System
byte RollDice(byte count, byte diceSize)
{
  byte result = 0;
  byte i;
  for (i = 0; i < count; i++)
    result += (rand() %diceSize + 1);
  return result;
}
void SwitchScreen(screenName screen)
{
  //ResizeMessageWindow(1, 1, 10, 15);
  //WriteLineMessageWindow("Hello", 0);
  //ScreenDisable();
  //ClearScreen();
  //DrawInterface();
  //Load specified screen
  //UpdateInput();
  //currentScreen = screen;
  //ScreenEnable();
  #if defined (__NES__)
  SelectBank();
  #endif

  switch (screen)
  {
    case EditParty:
      #if !MSX
      nextScreen = DrawAddCharacterScreen();
      #endif
      break;
    case Map:
      //#if !MSX
      nextScreen = MapUpdate();
      //#endif
      break;
    case Combat:
      #if !MSX
      nextScreen = Update_Combat();
      #endif
      break;
    case MapGen:
      //currentScreen = MapUpdate();
      //#if !MSX
      nextScreen = Update_MapGen();
      //#endif
      break;
    default:
      nextScreen = DefaultScreen;
      break;
  }
  ClearInterface();
  SwitchScreen(nextScreen);
}
void RunGame(screenName startingScreen)
{
  srand(0);
  InitializeGraphics();
  ResizeMessageWindow();
  //ClearScreen();
  ScreenFadeOut();
  #if defined(__NES__)
  MMC3_PRG_8000(0);
  #endif
  LoadMap();
  //currentScreen = startingScreen;
  DrawInterface();

        //WriteLineMessageWindow("Greetings from the librarian Soodo Nim", 0);
        //WriteLineMessageWindow("..not to be confused with evil Anto Nim", 0);
        //WaitForInput();
  //DrawBorder(" ", 12, 19, 16, 5, true);
  //PrintString("Press Space", 14, 21, true);
  while(1)
  {
    SwitchScreen(startingScreen);
    //DrawBorder(" ", 12, 19, 16, 5, true);
  //PrintString("Press Space", 14, 21, true);

  while (1)
  {
    UpdateInput();
    if (InputChanged())
      if (InputFire())
        break;
  }
  }
}
void DebugGraphics()
{
  DrawCharset();
  ResizeMessageWindow();
  //WriteLineMessageWindow("The Quick Brown Fox Jumps Over The Lazy Dog", 0);
  //WriteLineMessageWindow("ABCDEFGHIJKLMNOPQRSTUVWXYZ", 0);
  //WriteLineMessageWindow("abcdefghijklmnopqrstuvwxyz", 0);
  //WriteLineMessageWindow("01234567890 !#$%^&", 0);
  //WriteLineMessageWindow("*()-=[];':<>,./?", 0);
  while(1);
}

//Characters
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
          WriteLineMessageWindow("Position not Found:", 0);
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
          WriteLineMessageWindow("Position not Found:", 0);
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
void RemoveParty(byte index) //Removes Last Party Member (?)
{
  //byte index = CountParty()-1;
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
  //byte statY = PartyStatsY + 1 + characterIndex * (3);
  byte statX = PartyStatsX + characterIndex * (7);
  
  struct playerChar *PlayerChar = getPartyMember(characterIndex);

  if (PlayerChar != NULL)
  {
  //DrawBorder(PlayerChar->NAME, statX, PartyStatsY - 1, 7, 4, true);
  DrawTileDirectXY(PlayerChar->CLASS, statX , PartyStatsY + 1);
  PrintString("       ", statX, PartyStatsY, true);
  ConsoleBufferReset();
  ConsoleBufferAdd(PlayerChar->NAME);
  ConsoleBufferPrint(statX, PartyStatsY);
  //ConsoleBufferAdd(RaceDescription[PlayerChar->RACE].NAME);
  //ConsoleBufferPrint(statX + 3, PartyStatsY);
  //ConsoleBufferAdd(ClassDescription[PlayerChar->CLASS].NAME);
  //ConsoleBufferPrint(statX + 3, PartyStatsY+1);
  sprintf(strTemp, "%2d/%2d", PlayerChar->HP, PlayerChar->HPMAX);
  PrintString(strTemp, statX + 2, PartyStatsY + 1, true);
  ConsoleBufferReset();
  }
  else
  ClearBorder(statX-1, PartyStatsY-1, 9, 5);
}
void DrawCharStats()
{
  byte i;
  DrawMoonPhase();
  //DrawBorder(" ", PartyStatsX, PartyStatsY, PartyStatsWidth, PartyStatsHeight, true);
  for (i = 0; i < 4; ++i)
    DrawCharStatus(i);
}

//Interface
void DrawInterface()
{
  //Viewport
  DrawBorder("", viewportPosX - 1, viewportPosY - 1, viewportWidth* 2 + 2, viewportHeight * 2 + 2, false);
  //Console
  DrawBorder("", consolePosX - 1, consolePosY - 1, consoleWidth + 2, consoleHeight + 2, false);
  //Menu
  DrawBorder("", contextMenuPosX - 1, contextMenuPosY - 1, contextMenuWidth + 2, contextMenuHeight - 1, false);
  //Map
  DrawBorder("", MiniMapPosXInit - 1, MiniMapPosYInit - 1, MiniMapWidthInit + 2, MiniMapHeightInit + 2, false);
  //DrawCharStats();
  //ResizeMessageWindow();
  SetTileOrigin(viewportPosX, viewportPosY);
}
void ClearInterface()
{
  //Viewport
  ClearBorder(viewportPosX - 1, viewportPosY - 1, viewportWidth* 2 + 2, viewportHeight * 2 + 2);
  //Console
  //ClearBorder(consolePosX - 1, consolePosY - 1, consoleWidth + 2, consoleHeight + 2);
  //Menu
  ClearBorder(contextMenuPosX - 1, contextMenuPosY - 1, contextMenuWidth + 2, contextMenuHeight - 1);
  //Map
  ClearBorder(MiniMapPosXInit - 1, MiniMapPosYInit - 1, MiniMapWidthInit + 2, MiniMapHeightInit + 2);
  DrawCharStats();
  //ResizeMessageWindow();
  SetTileOrigin(viewportPosX, viewportPosY);
}

//Minimap
const byte OverworldGlyphs[64] =
{
  0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
  0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0xe6, 0xe7, 0xe8, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0,
  0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe2, 0xe7
};
const byte DungeonGlyphs[64] =
{
  0xe0, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef,
  0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff
};
void DrawMiniMap(bool highlightPlayer)
{
  byte x, y, tile = 0;
  //DrawBorder("Minimap", MiniMapPosX - 1, MiniMapPosY- 1, mapMatrixWidth + 2, mapMatrixHeight + 2, false);
  UpdateAttributes();
  for (y = 0; y < MiniMapHeight; ++y)
  {
    byte tY = (y * mapMatrixWidth);
    for (x = 0; x < MiniMapWidth; ++x)
    {
      //tile = mapQuads[x + tY] + MiniMapOffset;
      //tile = mapQuads[x + tY];
      tile = MiniMapGlyphs[mapQuads[x + tY]];
      SetChar(tile, x + MiniMapPosX, y + MiniMapPosY);
    }
  }
  if(highlightPlayer)
    SetChar('X', MiniMapPosX + MiniMapHighlightX, MiniMapPosY + MiniMapHighlightY);
  ScreenFadeIn();
}
void DrawLocalMiniMap(bool checkLast, bool clear)
{
  #define radius 3
  #define posX 19 //contextMenuPosX + 1
  #define posY contextMenuPosY
  sbyte sampleX, sampleY, sampleXX, sampleYY;
  byte offset;
  char target;

  if(checkLast)
  if ((lastX == MiniMapHighlightX) && (lastY == MiniMapHighlightY))
          return;
  UpdateAttributes();
  for (sampleY = -radius; sampleY <= radius; ++sampleY)
    {
      sampleYY = sampleY + MiniMapHighlightY;
      if (sampleYY < 0)
        sampleYY += mapMatrixHeight;
      if (sampleYY >= mapMatrixHeight)
        sampleYY -= mapMatrixHeight;
      for (sampleX = -radius; sampleX <= radius; ++sampleX)
        {
          sampleXX = sampleX + MiniMapHighlightX;
          if (sampleXX < 0)
                  sampleXX += mapMatrixWidth;
          if (sampleXX >= mapMatrixWidth)
                  sampleXX -= mapMatrixWidth;
          offset = sampleXX + mapMatrixWidth* sampleYY;
          if ((sampleX == 0) && (sampleY == 0))
                  target = 'X';
          else
                  target = MiniMapGlyphs[mapQuads[offset]];
          if (clear)
            target = ' ';
          SetChar(target, sampleX + (posX + radius), sampleY + (posY + radius));
        }
      }
  lastX = MiniMapHighlightX;
  lastY = MiniMapHighlightY;
}

//Moon
void DrawMoonPhase()
{
  ConsoleBufferReset();
  sprintf(strTemp, "<%c  %c>", phaseChar[moonA], phaseChar[moonB]);
  PrintString(strTemp, 6, 18, true);
  ConsoleBufferReset();
}
void TickMoonPhase() //The SOLUS and the LUNUS and the MOONUS
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
    {
      moonA = 0;
            ++Sessions[0].MINAR;
            if (Sessions[0].MINAR == 24)
            {
                    Sessions[0].MINAR = 0;
                    ++Sessions[0].LUNAR;
                    if (Sessions[0].LUNAR == 10)
                    {
                            Sessions[0].LUNAR = 0;
                            ++Sessions[0].SOLAR;
                    }
            }
    }
    if (moonB > 3)
    {
      moonB = 0;
    }
    DrawMoonPhase();
  }
}
