#include "xinaga.h"
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

byte Height = 10;
byte Width = 15;
byte PosX = 23;
byte PosY = 12;
//char *MessageLines;

#define MessageCount 4
#define MessageLength 20
char Messages[MessageCount][MessageLength] = {
  "Hello there!@",
  "This is a sign@",
  "Wizard's Forest@"};

void ResizeMessageWindow (byte xPos, byte yPos, byte w, byte h)
{
  byte x, y;
  PosX = xPos;
  PosY = yPos;
  Width = w;
  Height = h;
  //free(MessageLines);
  //size = w * h;
  //MessageLines = malloc(w*h);
  
  DrawBorder("Console@",PosX - 1, PosY - 1, Width + 2, Height + 2, true);
  for (y = 0; y < Height; ++y)
    for (x = 0; x < Width; ++x)
      SetChar(' ', PosX + x, PosY + y);
}

void ScrollMessageWindowUp()
{
  byte x, y;
  
  for (y = 0; y < Height - 1; ++y)  
    for (x = 0; x < (Width); ++x)
    {
      //MessageLines[x] = MessageLines[x + Width];
      SetChar(GetChar(PosX + x, PosY + y + 1), PosX + x, PosY + y);  
      
    }
  for (x = 0; x < (Width); ++x)
  {
      SetChar(' ', PosX + x, PosY + Height - 1);  
    
    //MessageLines[x] = ' ';
  }
  //DrawMessageWindow();
}

void SetLineMessageWindow(char message[38], byte delay)
{
  byte x;
  for(x = 0; x < Width; ++x)
  {
    if (message[x] == '@')
    {
      while (x < Width)
      {
        SetChar(' ', PosX + x, PosY + Height - 1);
        //MessageLines[x + (Width * (Height - 1))] = ' ';
        ++x;
      }
      break;
    }
    else
    {
      //MessageLines[x + (Width * (Height - 1))] = message[x];
      SetChar(message[x], PosX + x, PosY + Height - 1);  
      if (delay > 0)
      {
        byte d, z;
        for (z = 0; z < delay * 4; ++z)
          for(d = 0; d < 255; ++d)
          {}
      }
    }
  }
}

void WriteLineMessageWindow(char message[38], byte delay)
{
  ScrollMessageWindowUp();
  SetLineMessageWindow(message, delay);
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