#include "Xinaga.h"

#if defined(__APPLE2__)
#pragma code-name (push, "LOWCODE")
#endif

//StringBuilder
byte StringLength = 0;

void ConsoleBufferReset()
{
  byte i;
  for (i = 0; i < ConsoleBufferLength; ++i)
    strTemp[i] = ' ';
  StringLength = 0;
}

void ConsoleBufferAdd(char *message)
{
  byte i;
  if (StringLength > 0)
  {
    //++StringLength;
  }
  for (i = 0; message[i] != '@' && message[i] != 0; ++i)
  {
    strTemp[StringLength] = message[i];
    ++StringLength;
  }
  strTemp[StringLength] = ' ';
  strTemp[StringLength + 1] = '@';
  ++StringLength;
}

void ConsoleBufferAddNumber(int value)
{
  char string[8];
  sprintf(string, "%d", value);
  ConsoleBufferAdd(string);
}
void ConsoleBufferBackspace()
{
  --StringLength;
}

void ConsoleBufferEndline()
{
  while (strTemp[StringLength] == ' ')
    --StringLength;
  strTemp[StringLength - 1] = '@';
  //strTemp[StringLength + 1] = '@';
}
  
void ConsoleBufferPrint(byte x, byte y)
{
  ConsoleBufferEndline();
  PrintString(strTemp, x, y, true, false);
  ConsoleBufferReset();
}

void ConsoleBufferPrintConsole(byte delay)
{
  ConsoleBufferEndline();
  WriteLineMessageWindow(strTemp, delay);
  ConsoleBufferReset();
  WriteLineMessageWindow("@", delay);
}

//Console
byte Height = 10;
byte Width = 15;
byte PosX = 23;
byte PosY = 12;
//char *MessageLines;

#define MessageCount 4
char *Messages[MessageCount]= {
  "Hello there!@",
  "This is a sign@",
  "Wizard's Forest@"};

//Selection Menu
byte MenuHeight = 5;
byte MenuWidth = 5;
byte MenuPosX = 5;
byte MenuPosY = 5;
byte MenuCount = 5;
sbyte MenuSelection = 0;
#define menuItemsCount 16
char *MenuItems[menuItemsCount] = {("")};
bool MenuHighlight[menuItemsCount] = { 0 };
void ResetMenu(char *title, byte posX, byte posY, byte w, byte h, byte c);
void SetMenuItem(byte index, char *value);
byte GetMenuSelection();

void ClearLine()
{
  byte x;
  for (x = 0; x < MenuWidth; ++x)
  {
    SetChar(' ', MenuPosX + x, MenuPosY + MenuSelection);    
  }
}
void ClearItem(byte index)
{
  byte x;
  for (x = 0; x < MenuWidth; ++x)
  {
    SetChar(' ', MenuPosX + x, MenuPosY + index);    
  }
}

void DrawItem(byte index)
{
  //char menuLine[38];
  //char *selector, *highlight;
  //if (MenuSelection == index)
    //selector = ">";
  //else
   // selector = "";

  //if(MenuHighlight[index])
   // highlight = "+";
  //else
    //highlight = "";
  //ClearItem(index);
  ConsoleBufferReset();
  if(MenuSelection == index)
    ConsoleBufferAdd(">");
  else
    ConsoleBufferAdd(" ");
  ConsoleBufferBackspace();
  
  if (MenuHighlight[index] == true)
  {
    ConsoleBufferAdd("+@");
    ConsoleBufferBackspace();
  }
  
  ConsoleBufferAdd(MenuItems[index]);
  ConsoleBufferPrint(MenuPosX, MenuPosY + index);
  
  //sprintf(menuLine, "%s%s%s", selector, highlight, MenuItems[index]);
  //PrintString(menuLine, MenuPosX, MenuPosY + index, true, false);
}

void ResetMenu(char *title, byte posX, byte posY, byte w, byte h, byte c)
{
  byte x;
  MenuPosX = posX;
  MenuPosY = posY;
  MenuWidth = w;
  MenuHeight = h;
  MenuCount = c;
  MenuSelection = 0;
  
  DrawBorder(title, MenuPosX - 1, MenuPosY - 1, MenuWidth + 2, MenuHeight + 2, true);
  for (x = 0; x < menuItemsCount; ++x)
  {
    MenuItems[x] = "";
    MenuHighlight[x] = false;    
  }
}
void SetMenuItem(byte index, char *value)
{
  MenuItems[index] = value;
  MenuHighlight[index] = false;
  ClearItem(index);
  DrawItem(index);
}

void HighlightMenuItem(byte index)
{
  MenuHighlight[index] = true;
  ClearItem(index);
  DrawItem(index);
}

byte IsMenuItemHighlighted(byte index)
{
  return MenuHighlight[index];
}

void DrawMenu()
{
  byte x;
  for (x = 0; x < MenuCount; ++x)
  {
    ClearItem(x);
    DrawItem(x);
  }
}

byte GetMenuSelection()
{
  bool exit = false;
  //DrawMenu();
  while (!exit)
  {
    UpdateInput();
    if(InputChanged())
    {
      byte LastSelection = MenuSelection;
      if(InputUp())
      {
        //ClearLine();
        --MenuSelection;
        if (MenuSelection < 0)
          MenuSelection = MenuCount - 1;
      }
      
      if(InputDown())
      {
        //ClearLine();
        ++MenuSelection;
        if (MenuSelection >= MenuCount)
          MenuSelection = 0;
      }
      //ClearItem(LastSelection);
      DrawItem(LastSelection); 
      //ClearItem(MenuSelection);
      DrawItem(MenuSelection);
      
      if (InputFire())
        exit = true;
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
}

void SetLineMessageWindow(char *message, byte delay)
{
  byte x;
  byte length = 0;
  for (x = 0; x < ConsoleBufferLength; ++x)
  {
    if (message[x] != '@')
      ++length;
  }
  
  for (x = 0; x < Width; ++x)
  {
    if (GetChar(PosX + x, PosY + Height - 1) != message[x])
      SetChar(' ', PosX + x, PosY + Height - 1);
  }
  
  for(x = 0; x < length; ++x)
  {
    if (message[x] == '@')
    {
      while (x < Width)
      {
        SetChar(' ', PosX + x, PosY + Height - 1);
        ++x;
      }
      x = length;
      break;
    }
    else
    {
      if (message[x] != ' ')
      {
        SetChar(message[x], PosX + x, PosY + Height - 1);  
        wait_vblank(delay);
      }
    }
    
    if (length > Width)
      if (message[x] == ' ')
      {
        byte wordLength = 0;
        byte wordStart = x + 1;
        char temp[128];
        byte i = 0;
        
        while (message[wordStart + wordLength] != ' ' && message[wordStart + wordLength] != '@')
        {
          ++wordLength;
        }
        
        if(x + wordLength > Width - 1)
        {
          ++x; //Skips the space
          while (x < length)
          {
            temp[i] = message[x];
            ++i;
            ++x;
          }
          WriteLineMessageWindow(temp, delay);
          //break;
        }
      }
  }
}

void WriteLineMessageWindow(char *message, byte delay)
{
  ScrollMessageWindowUp();
  SetLineMessageWindow(message, delay);
}

