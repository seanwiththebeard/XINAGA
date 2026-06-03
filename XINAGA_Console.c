#include "Xinaga.h"

#if defined(__APPLE2__)
#pragma code-name (push, "LOWCODE")
//#pragma code-name (push, "CODE")
#endif

#if defined (__NES__)
#pragma code-name (push, "XINAGA_CONSOLE")
#pragma rodata-name (push, "XINAGA_CONSOLE")
//#pragma data-name (push, "XRAM")
//#pragma bss-name (push, "XRAM")
#endif

#if defined (__C64__)
#pragma code-name (push, "XINAGA_CONSOLE")
//#pragma rodata-name (push, "GAME_RODATA")
#endif

//Console Buffer
int contentOffset; //Offset of Last Line
byte consoleContents[consoleHeight * consoleWidth];

//StringBuilder
byte StringLength;

byte menuSel;

//Selection Menu
byte MenuHeight;
byte MenuWidth;
byte MenuPosX;
byte MenuPosY;
byte MenuCount;
sbyte MenuSelection;
char *menutitle;
#define menuItemsCount contextMenuHeight
char *MenuItems[menuItemsCount];
bool MenuHighlight[menuItemsCount];
void SetMenuItem(byte index, char *value);
byte GetMenuSelection();

void ConsoleBufferReset()
{
  byte i;
  for (i = 0; i < ConsoleBufferLength; ++i)
    strTemp[i] = '\0';
  StringLength = 0;
}

void ConsoleBufferAdd(char *message)
{
  byte i;
  if(StringLength > 1)
    --StringLength;
  for (i = 0; message[i] != '\0' && message[i] != 0; ++i)
  {
    strTemp[StringLength] = message[i];
    ++StringLength;
  }
  strTemp[StringLength] = ' ';
  //strTemp[StringLength + 1] = '@';
  ++StringLength;
  strTemp[StringLength] = ' ';
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
  //strTemp[StringLength] = '\0';
  while (strTemp[StringLength - 1] == ' ')
    {
      strTemp[StringLength - 1] = '\0';
      --StringLength;
    }
}

void ConsoleBufferPrint(byte x, byte y) //Prints the contents of the buffer to a screen position
{
  ConsoleBufferEndline();
  ConsoleBufferBackspace();
  PrintString(strTemp, x, y, true);
  ConsoleBufferReset();
}

void ConsoleBufferPrintConsole(byte delay) //Prints the contents of the buffer to the console
{
  ConsoleBufferEndline();
  WriteLineMessageWindow(strTemp, delay);
  ConsoleBufferReset();
  WriteLineMessageWindow("", delay);
}

void ClearItem(byte index)
{
  byte x;
  for (x = 0; x < MenuWidth; ++x)
  {
    SetChar(' ', MenuPosX - 1 + x, MenuPosY + index);
  }
}

void DrawItem(byte index)
{
  ClearItem(index);
  ConsoleBufferReset();
  if(MenuSelection == index)
    ConsoleBufferAdd("+");
  else
    ConsoleBufferAdd(" ");
  ConsoleBufferBackspace();

  if (MenuHighlight[index] == true)
  {
    ConsoleBufferAdd("+");
    ConsoleBufferBackspace();
  }

  ConsoleBufferAdd(MenuItems[index]);
  ConsoleBufferAdd("");

  ConsoleBufferPrint(MenuPosX, MenuPosY + index);
  //wait_vblank(1);
}

void SetMenuSelect(byte sel)
{
        menuSel = sel;
}

void ClearMenuContents()
{
        byte x;
        //DrawBorder(menutitle, MenuPosX - 1, MenuPosY - 1, MenuWidth + 2, MenuHeight + 2, true);
        for (x = 0; x < menuItemsCount; ++x)
                {
                        MenuItems[x] = (char*)"";
                        MenuHighlight[x] = false;
                        //ClearItem(x);
                }
}

//void ResetMenu(char *title, byte posX, byte posY, byte w, byte h, byte c, byte clear)
void ResetMenu(char *title, byte c, byte clear)
{
  //byte x;
  menutitle = title;
  MenuPosX = contextMenuPosX;
  MenuPosY = contextMenuPosY;
  MenuWidth = contextMenuWidth;
  MenuHeight = contextMenuHeight;
  MenuCount = c;
  MenuSelection = menuSel;
  menuSel = 0; //Reset for next draw if we don't keep it
        if(clear)
        {
          ClearMenu();
          ClearMenuContents();
        }
}

void MovableMenu(char *title, byte posX, byte posY, byte w, byte h, byte c, byte clear)
{
  //byte x;
  menutitle = title;
  MenuPosX = posX;
  MenuPosY = posY;
  MenuWidth = w;
  MenuHeight = h;
  MenuCount = c;
  MenuSelection = menuSel;
  menuSel = 0; //Reset for next draw if we don't keep it
        if(clear);
  ClearMenuContents();
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
  //DrawBorder(menutitle, MenuPosX - 1, MenuPosY - 1, MenuWidth + 2, MenuHeight + 2, true);
  for (x = 0; x < MenuHeight; ++x)
  {
    ClearItem(x);
    if (x < MenuCount)
    DrawItem(x);
  }
}

void ClearMenu()
{
  byte x;
  byte xPos = MenuPosX;
  byte yPos = MenuPosY;

    for (x = 1; x < MenuHeight - 2; ++x)
      DrawLineH(' ', xPos, yPos + x - 1, contextMenuWidth);
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
  /*free(consoleContents);
  consoleContents = malloc(Width * Height);
  memset(&consoleContents[0], ' ', Width*Height);*/

  int size = consoleWidth * consoleHeight;
  //if (consoleContents == NULL)
    //consoleContents = (byte*)malloc(size);
  //else
    //consoleContents = (byte*)realloc(consoleContents, size);

  //if (consoleContents)
    memset(&consoleContents[0], ' ', size);
}

void DrawConsoleContent()
{
  byte x, y;
  for (y = 0; y < consoleHeight - 1; ++y)
    for (x = 0; x < consoleWidth; ++x)
      SetChar(consoleContents[x + y*consoleWidth], consolePosX + x, consolePosY + y);
}

void ResizeMessageWindow ()
{
  contentOffset = consoleWidth * (consoleHeight - 1);

  DrawBorder(" ", consolePosX - 1, consolePosY - 1, consoleWidth + 2, consoleHeight + 2, true);
  ClearConsoleContent();
  DrawConsoleContent();
}

void ScrollMessageWindowUp()
{
  //byte x;
  int y;

  for (y = 0; y < contentOffset; ++y)
    consoleContents[y] = consoleContents[y + consoleWidth];

  //for (x = 0; x < (consoleWidth); ++x)
    {
      //consoleContents[contentOffset + x] = '\0';
      //SetChar(' ', consolePosX + x, consolePosY + consoleHeight - 1);
    }

  DrawConsoleContent();
}

void SetLineMessageWindow(char *message, byte delay)
{
  byte x;
  byte length = 0;
  for (;message[length] != '\0' && length < ConsoleBufferLength; ++length);
  ++length;
  ConsoleBufferEndline();
  for (x = 0; x < consoleWidth; ++x)
    {
      consoleContents[contentOffset + x] = ' ';
      SetChar(consoleContents[contentOffset + x], consolePosX + x, consolePosY + consoleHeight - 1);
    }
  for(x = 0; x < length; ++x)
  {
    if (message[x] == '\0' || message[x] == 0)
    {
      while (x < consoleWidth)
      {
        consoleContents[contentOffset + x] = ' ';
        SetChar(consoleContents[contentOffset + x], consolePosX + x, consolePosY + consoleHeight - 1);
        ++x;
      }
      x = length;
      break;
    }
    else
    {
      //if (message[x] != ' ')
      if (x < consoleWidth)
      {
        SetChar(message[x], consolePosX + x, consolePosY + consoleHeight - 1);
        consoleContents[contentOffset + x] = message[x];
        if(delay)
          wait_vblank(delay);
      }
    }
    if (length >= consoleWidth)
      if (message[x] == ' ')
      {
        byte wordLength = 0;
        byte wordStart = x + 1;
        char temp[ConsoleBufferLength];
        byte i = 0;
        while (message[wordStart + wordLength] != ' ' && message[wordStart + wordLength] != '\0')
        {
          ++wordLength;
        }
        if(x + wordLength >= consoleWidth)
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
  ConsoleBufferReset();
}

bool AreYouSure()
{
  ResetMenu("Sure?", 2, true);
  SetMenuItem(0, "No ");
  SetMenuItem(1, "Yes");
  return (GetMenuSelection());
}