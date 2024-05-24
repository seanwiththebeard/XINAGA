#include "xinaga.h"
#include <stdio.h>
#include <stdlib.h>
#include "game.h"

//Console
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

//Selection Menu
byte MenuHeight = 5;
byte MenuWidth = 5;
byte MenuPosX = 5;
byte MenuPosY = 5;
byte MenuCount = 5;
int MenuSelection = 0;
#define menuItemsCount 16
char *MenuItems[] = {};
void ResetMenu(char *title, byte posX, byte posY, byte w, byte h, byte c);
void SetMenuItem(byte index, char *value);
byte GetMenuSelection();

void ResetMenu(char *title, byte posX, byte posY, byte w, byte h, byte c)
{
  byte x, y;
  MenuPosX = posX;
  MenuPosY = posY;
  MenuWidth = w;
  MenuHeight = h;
  MenuCount = c;
  MenuSelection = 0;
  
  for (x = 0; x < menuItemsCount; ++x)
    MenuItems[x] = "No Menu";
  DrawBorder(title, MenuPosX - 1, MenuPosY - 1, MenuWidth + 2, MenuHeight + 2, true);
  for (y = 0; y < MenuHeight; ++y)
    for (x = 0; x < MenuWidth; ++x)
      SetChar(' ', MenuPosX + x, MenuPosY + y);
}
void SetMenuItem(byte index, char *value)
{
  MenuItems[index] = value;
}

void ClearLine()
{
  byte x;
  for (x = 0; x < MenuWidth; ++x)
  {
    SetChar(' ', MenuPosX + x, MenuPosY + MenuSelection);    
  }
}

void DrawMenu()
{
  byte x;
  for (x = 0; x < MenuCount; ++x)
  {
    char menuLine[38];
    char *selector;
    if (MenuSelection == x)
      selector = ">";
    else
      selector = "";
    
    sprintf(menuLine, "%s%s" , selector, MenuItems[x]);
    PrintString(menuLine, MenuPosX, MenuPosY + x, true, false);
  }
}


byte GetMenuSelection()
{
  bool exit = false;
  DrawMenu();
  
  while (!exit)
  {
    UpdateInput();
    if(InputChanged())
    {
      if(InputUp())
      {
        ClearLine();
        --MenuSelection;
        if (MenuSelection < 0)
          MenuSelection = MenuCount - 1;
      }
      
      if(InputDown())
      {
        ClearLine();
        ++MenuSelection;
        if (MenuSelection >= MenuCount)
          MenuSelection = 0;
      }
      
      if (InputFire())
        exit = true;
      
      DrawMenu();
    }
  }
  
  return MenuSelection;
}

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

//Move these to the game screens?
#define CharStatPosX 18
#define CharStatPosY 1
void DrawCharStatus(byte characterIndex)
{
  //byte statX = CharStatPosX;
  byte statY = CharStatPosY + characterIndex * 3;
  struct playerChar *PlayerChar = getPartyMember(characterIndex);

  DrawBorder("@", CharStatPosX - 1, statY - 1, COLS - CharStatPosX + 1, 4, true);
  sprintf(strTemp, "%s@", RaceDescription[PlayerChar->RACE].NAME);
  PrintString(strTemp, CharStatPosX, statY, true, false);
  sprintf(strTemp, "HP:%d/%d@", PlayerChar->HP, PlayerChar->HPMAX);  
  PrintString(strTemp, CharStatPosX + 9, statY, true, false);
  sprintf(strTemp, "%s@", ClassDescription[PlayerChar->CLASS].NAME);
  PrintString(strTemp, CharStatPosX, statY + 1, true, false);
  //ReverseBufferArea(statX - 1, statY - 1, COLS - statX + 1, 5);
  //CopyDoubleBufferArea(statX - 1, statY - 1, COLS - statX + 1, 5);
}

void DrawCharStats()
{
  byte i;
  for (i = 0; i < CountParty(); ++i)
    DrawCharStatus(i);
}