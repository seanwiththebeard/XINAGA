#include "xinaga.h"
#include <stdio.h>
#include "game.h"

#define Height 10
#define Width 15
byte PosX = COLS - (Width + 2);
byte PosY = ROWS - (Height + 2);
char MessageLines[Width][Height];

#define MessageCount 4
#define MessageLength 20
char Messages[MessageCount][MessageLength] = {
  "Hello there!@",
  "This is a sign@",
  "Wizard's Forest@"};

void DrawMessageWindow()
{
  byte x, y;
  DrawBorder("Console@",PosX - 1, PosY - 1, Width + 2, Height + 2, false);
  for (y = 0; y < Height; ++y)
    for (x = 0; x < Width; ++x)
      SetChar(MessageLines[x][y], PosX + x, PosY + y);  
}
void BlankMessageWindow()
{
  DrawBorder("Console@",PosX - 1, PosY - 1, Width + 2, Height + 2, true);
  memset(&MessageLines[0][0], ' ', Width * Height);
  DrawMessageWindow();
}

void ScrollMessageWindowUp()
{
  byte x, y;

  for (y = 0; y < Height - 1; ++y)  
    for (x = 0; x < Width; ++x)
      MessageLines[x][y] = MessageLines[x][y + 1];
  for (x =0; x < Width; ++x)
    MessageLines[x][Height - 1] = ' ';
  DrawMessageWindow();
}

void WriteLineMessageWindow(char message[16], byte delay)
{
  byte x;
  
  ScrollMessageWindowUp();
  for(x = 0; x < Width; ++x)
  {
    if (message[x] == '@')
    {
      while (x < Width)
      {
        MessageLines[x][Height - 1] = ' ';
        ++x;
      }
      break;
    }
    else
    {
      MessageLines[x][Height - 1] = message[x];
      SetChar(MessageLines[x][Height - 1], PosX + x, PosY + Height - 1);  
    }
    if (delay > 0)
    {
      byte d, z;
      for (z = 0; z < delay * 8; ++z)
        for(d = 0; d < 255; ++d)
        {}
    }
  }
  DrawMessageWindow();
}


void DrawCharStatus(byte characterIndex)
{
  byte statX = PosX;
  byte statY = 2 + characterIndex * 3;
  struct playerChar *PlayerChar = getPartyMember(characterIndex);

  DrawBorder("@", statX - 1, statY - 1, COLS - statX + 1, 4, true);
  sprintf(strTemp, "%s@", RaceDescription[PlayerChar->RACE].NAME);
  PrintString(strTemp, statX, statY, true, false);
  sprintf(strTemp, "HP:%d/%d@", PlayerChar->HP, PlayerChar->HPMAX);  
  PrintString(strTemp, statX + 9, statY, true, false);
  sprintf(strTemp, "%s@", ClassDescription[PlayerChar->CLASS].NAME);
  PrintString(strTemp, statX, statY + 1, true, false);
  //ReverseBufferArea(statX - 1, statY - 1, COLS - statX + 1, 5);
  //CopyDoubleBufferArea(statX - 1, statY - 1, COLS - statX + 1, 5);
}

void DrawCharStats()
{
  byte i;
  for (i = 0; i < CountParty(); ++i)
    DrawCharStatus(i);
}