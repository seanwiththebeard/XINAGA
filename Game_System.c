#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
//#pragma code-name (push, "CODE")
#endif

#if defined (__NES__)
#pragma code-name (push, "GAME")
#pragma rodata-name (push, "GAME")
#endif

#if defined (__C64__)
#pragma code-name (push, "GAME")
#pragma rodata-name (push, "GAME_RODATA")
#endif

byte MiniMapPosX = 2;
byte MiniMapPosY = 2;
byte MiniMapWidth = 16;
byte MiniMapHeight = 16;

const byte MiniMapGlyphs[64] = 
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

void DebugGraphics()
{
  DrawCharset();
  
  ResizeMessageWindow(consolePosX, consolePosY, consoleWidth, consoleHeight);
  WriteLineMessageWindow("The Quick Brown Fox Jumps Over The Lazy Dog@", 0);
  WriteLineMessageWindow("ABCDEFGHIJKLMNOPQRSTUVWXYZ@", 0);
  WriteLineMessageWindow("abcdefghijklmnopqrstuvwxyz@", 0);
  WriteLineMessageWindow("01234567890 !#$%^&@", 0);
  WriteLineMessageWindow("*()-=[];':<>,./?@", 0);
  
  while(1);
}

void DebugMap()
{
  
  LoadMap();
  memset(&mapQuads[0], 0x02, mapMatrixHeight * mapMatrixWidth);
  SetTileOrigin(viewportPosX, viewportPosY);
  LoadMapQuads();
  DrawMap();
  DrawCharStats();
  while(1);
}

uint16_t randseed;

byte MiniMapHighlightX;
byte MiniMapHighlightY;

void DrawMiniMap(bool highlightPlayer)
{
  byte x, y, tile = 0;
  //DrawBorder("Minimap@", MiniMapPosX - 1, MiniMapPosY- 1, mapMatrixWidth + 2, mapMatrixHeight + 2, false);
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

byte lastX;
byte lastY;
void DrawLocalMiniMap(bool checkLast)
{
        #define radius 4
        #define posX contextMenuPosX + 1
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
                                SetChar(target, sampleX + (posX + radius), sampleY + (posY + radius));
                        }
                }
        lastX = MiniMapHighlightX;
        lastY = MiniMapHighlightY;
}

void DrawCharset()
{
  byte x, y;
  for (y = 0; y < 16; ++y)
    for (x = 0; x < 16; ++x)
      SetChar(x + (y<<4), x + viewportPosX, y + viewportPosY);
}

byte RollDice(byte count, byte diceSize)
{
  byte result = 0;
  byte i;
  for (i = 0; i < count; i++)
    result += (rand() %diceSize + 1);
  return result;
}

void DrawCharStatus(byte characterIndex)
{
  byte statY = PartyStatsY + 1 + characterIndex * (3);
  struct playerChar *PlayerChar = getPartyMember(characterIndex);

  DrawBorder(PlayerChar->NAME, PartyStatsX, statY - 1, PartyStatsWidth, 4, true);
  DrawTileDirectXY(PlayerChar->CLASS, PartyStatsX + 1, statY);
  
  ConsoleBufferReset();
  ConsoleBufferAdd(RaceDescription[PlayerChar->RACE].NAME);
  ConsoleBufferPrint(PartyStatsX + 3, statY);
  ConsoleBufferAdd(ClassDescription[PlayerChar->CLASS].NAME);
  ConsoleBufferPrint(PartyStatsX + 3, statY+1);
  sprintf(strTemp, "HP:%d/%d@", PlayerChar->HP, PlayerChar->HPMAX);  
  PrintString(strTemp, PartyStatsX + 10, statY + 1, true);
  ConsoleBufferReset();
}

byte moonA;
byte moonB;
const char phaseChar[] = " )*(";
byte moonTick;

void DrawMoonPhase()
{
  ConsoleBufferReset();  
  sprintf(strTemp, "<%c||%c>@", phaseChar[moonA], phaseChar[moonB]);
  PrintString(strTemp, viewportPosX + (viewportWidth / 2) + 2, viewportPosY -1, true);
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

void DrawCharStats()
{
  byte i;
  DrawMoonPhase();
  DrawBorder("@", PartyStatsX, PartyStatsY, PartyStatsWidth, PartyStatsHeight, true);
  for (i = 0; i < CountParty(); ++i)
    DrawCharStatus(i);
}
