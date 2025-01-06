#include "Xinaga.h"

#if defined(__APPLE2__)
//#pragma code-name (push, "LOWCODE")
#endif

#if defined (__NES__)
#pragma code-name (push, "STARTUP")
//#pragma rodata-name (push, "STARTUP")
//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
#endif

//Console Buffer
byte* consoleContents;
int contentOffset; //Offset of Last Line

//StringBuilder
byte StringLength;

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
byte Height;
byte Width;
byte PosX;
byte PosY;

#define MessageCount 4
const char* const Messages[MessageCount]= {
  "Hello there!@",
  "This is a sign@",
  "Wizard's Forest@"};

//Selection Menu
byte MenuHeight;
byte MenuWidth;
byte MenuPosX;
byte MenuPosY;
byte MenuCount;
sbyte MenuSelection;
char *menutitle;
#define menuItemsCount 16
char *MenuItems[menuItemsCount];
bool MenuHighlight[menuItemsCount];
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
  //wait_vblank(1);
}

void ResetMenu(char *title, byte posX, byte posY, byte w, byte h, byte c)
{
  byte x;
  menutitle = title;
  MenuPosX = posX;
  MenuPosY = posY;
  MenuWidth = w;
  MenuHeight = h;
  MenuCount = c;
  MenuSelection = 0;
  
  DrawBorder(menutitle, MenuPosX - 1, MenuPosY - 1, MenuWidth + 2, MenuHeight + 2, true);
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
  DrawBorder(menutitle, MenuPosX - 1, MenuPosY - 1, MenuWidth + 2, MenuHeight + 2, true);
  for (x = 0; x < MenuCount; ++x)
  {
    ClearItem(x);
    DrawItem(x);
  }
}
void ClearMenu()
{
  byte x;
  byte xPos = MenuPosX;
  byte yPos = MenuPosY;
  
    for (x = 1; x < MenuHeight; ++x)
      DrawLineH(' ', xPos, yPos + x - 1, MenuWidth);
}


byte GetMenuSelection()
{
  bool exit = false;
  bool refresh = false;
  
  while (1)
  {
    UpdateInput();
    if(InputChanged())
    {
      byte LastSelection = MenuSelection;
      if(InputUp())
      {
        --MenuSelection;
        if (MenuSelection < 0)
          MenuSelection = MenuCount - 1;
        
        refresh = true;
      }
      
      if(InputDown())
      {
        ++MenuSelection;
        if (MenuSelection >= MenuCount)
          MenuSelection = 0;
        
        refresh = true;
      }
      
      if (refresh)
      {
        DrawItem(LastSelection); 
        DrawItem(MenuSelection);
      }
      
      if (InputFire())
        return MenuSelection;
    }
  }  
}

void ClearConsoleContent()
{
  free(consoleContents);
  consoleContents = malloc(consoleWidth * consoleHeight);
  memset(&consoleContents[0], ' ', consoleHeight*consoleWidth);
}

void DrawConsoleContent()
{
  byte x, y;
  for (y = 0; y < Height; ++y)
  {
    for (x = 0; x < Width; ++x)
    {
      SetChar(consoleContents[x + y*consoleWidth], PosX + x, PosY + y);
    }
  }
}

void ResizeMessageWindow (byte xPos, byte yPos, byte w, byte h)
{
  PosX = xPos;
  PosY = yPos;
  Width = w;
  Height = h;
  
  contentOffset = Width * (Height - 1);

  DrawBorder("@",PosX - 1, PosY - 1, Width + 2, Height + 2, true);
  ClearConsoleContent();
  DrawConsoleContent();
}



void ScrollMessageWindowUp()
{
  byte x;
  int y;
  
  for (y = 0; y < contentOffset; ++y)
  {
    consoleContents[y] = consoleContents[y + consoleWidth];
  }
  
  for (x = 0; x < (Width); ++x)
  {
    //SetChar(' ', PosX + x, PosY + Height - 1);
    consoleContents[contentOffset + x] = ' ';
  }
  DrawConsoleContent();
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
    {
      SetChar(' ', PosX + x, PosY + Height - 1);
      consoleContents[contentOffset + x] = ' ';
    }
  }
  
  for(x = 0; x < length; ++x)
  {
    if (message[x] == '@')
    {
      while (x < Width)
      {
        SetChar(' ', PosX + x, PosY + Height - 1);
        consoleContents[contentOffset + x] = ' ';
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
        consoleContents[contentOffset + x] = message[x];
        if(delay)
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
  wait_vblank(1);
}

void WriteLineMessageWindow(char *message, byte delay)
{
  ScrollMessageWindowUp();
  SetLineMessageWindow(message, delay);
  //wait_vblank(1);
}

