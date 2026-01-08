#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
#pragma code-name (push, "LOWCODE")
#endif

#if defined (__NES__)
#pragma code-name (push, "TITLE")
#pragma rodata-name (push, "TITLE")
//#pragma data-name (push, "TITLE")
//#pragma bss-name (push, "TITLE")
#endif

#if defined (__C64__)
#pragma code-name (push, "SCREEN_TITLE")
#pragma rodata-name (push, "SCREEN_TITLE")
#endif
//int tetrismap(byte seed);

void NoDiskAssemble();
//byte seed = 57;

screenName Update_Title()
{
  ClearScreen();

  ScreenFadeIn();

  //NoDiskAssemble();

  //while(1)
  {
    //tetrismap(seed);
    //++seed;
  }

  DrawBorder("@",12, 5, 16, 5, true);
  PrintString("Title Screen@", 14, 7, true);
  DrawBorder("@", 12, 19, 16, 5, true);
  PrintString("Press Space@", 14, 21, true);

  while (1)
  {
    UpdateInput();
    if (InputChanged())
      if (InputFire())
        return EditParty;
  }  
}

/*

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_SIZE 24
#define TARGET_POINTS ((MAP_SIZE * MAP_SIZE) * (2 / 3))
#define empty ' '
#define point 'q'

typedef struct {
  int x, y;
} Point;

int total_points = 0;
byte map[MAP_SIZE][MAP_SIZE];

// All Tetris blocks with their rotations
Point tetris_blocks[][4] = {
  // I-shape
  {{0, 0}, {1, 0}, {2, 0}, {3, 0}}, // Horizontal
  {{0, 0}, {0, 1}, {0, 2}, {0, 3}}, // Vertical
  // O-shape (only one rotation)
  {{0, 0}, {0, 1}, {1, 0}, {1, 1}},
  // T-shape
  {{0, 0}, {1, 0}, {2, 0}, {1, 1}}, // Up
  {{0, 0}, {0, 1}, {0, 2}, {1, 1}}, // Left
  {{0, 1}, {1, 0}, {1, 1}, {2, 1}}, // Down
  {{1, 0}, {1, 1}, {1, 2}, {0, 1}}, // Right
  // S-shape
  {{0, 1}, {1, 1}, {1, 0}, {2, 0}}, // Horizontal
  {{0, 0}, {0, 1}, {1, 1}, {1, 2}}, // Vertical
  // Z-shape
  {{1, 1}, {2, 1}, {0, 0}, {1, 0}}, // Horizontal
  {{0, 1}, {0, 2}, {1, 0}, {1, 1}}  // Vertical
};
byte num_blocks = sizeof(tetris_blocks) / sizeof(tetris_blocks[0]);

void place_tetris_block(int x, int y, int block_index) {
  int i;
  for (i = 0; i < 4; i++) {
    byte px, py;
    px = x + tetris_blocks[block_index][i].x;
    py = y + tetris_blocks[block_index][i].y;

    while(map[py][px] == point)
    {
      px++;
      if (px >= MAP_SIZE)
      {
        px = 0;
        ++py;
      }
      if (py >= MAP_SIZE)
        py = 0;
      //SetChar('X', x, y);
      //wait_vblank(1);
      //SetChar(map[y][x], x, y);
      //wait_vblank(1);
    }
    px = px % MAP_SIZE;
    py = py % MAP_SIZE;


    //if (px >= 0 && px < MAP_SIZE && py >= 0 && py < MAP_SIZE) 
    {
      if (map[py][px] != point)
        ++total_points;
      map[py][px] = point;
      SetChar(map[py][px], px, py);

    }
  }
}

void expand_continent() {
  byte i;
  {
    byte x = rand() % MAP_SIZE;
    byte y = rand() % MAP_SIZE;

    while(map[y][x] != point)
    {
      x++;
      if (x >= MAP_SIZE)
      {
        x = 0;
        ++y;
      }
      if (y >= MAP_SIZE)
        y = 0;
      //SetChar('X', x, y);
      //wait_vblank(1);
      //SetChar(map[y][x], x, y);
      //wait_vblank(1);

    }
    //if (map[y][x] == point) 
    {
      byte block_index = rand() % num_blocks;
      for (i = 0; i < 4; i++) {
        //byte nx = x + tetris_blocks[block_index][i].x;
        //byte ny = y + tetris_blocks[block_index][i].y;
        //if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && map[ny][nx] == 0) 
        {
          place_tetris_block(x, y, block_index);
          break;
        }
      }
    }
  }
}   

int tetrismap(byte seed) {
  byte x, y, block_index;
  total_points = 0;
  srand(seed); // Fixed seed for reproducibility

  for ( y = 0; y < MAP_SIZE; y++)
    for ( x = 0; x < MAP_SIZE; x++)
    {
      map[y][x] = ' ';
      SetChar(map[y][x], x, y);
    }

  // Place Tetris blocks randomly
  while (total_points < 16) {
    x = rand() % (MAP_SIZE - 3);
    y = rand() % (MAP_SIZE - 3);
    block_index = rand() % num_blocks;
    place_tetris_block(x, y, block_index);

    //for ( y = 0; y < MAP_SIZE; y++)
    //for ( x = 0; x < MAP_SIZE; x++)
    //SetChar(map[y][x], x, y);
  }

  // Expand continents with Tetris blocks
  while (total_points < 256)
  {
    expand_continent();

    //if (total_points % 32 == 0)
    //for ( y = 0; y < MAP_SIZE; y++)
    //for ( x = 0; x < MAP_SIZE; x++)
    //SetChar(map[y][x], x, y);
  }

  return 0;
}

byte OpCode[16];
int Address[16];
byte selectLine;
byte selectOp;
byte selectAddr;

void WriteLine()
{
  sprintf(strTemp, "%D $%02X 0x%0004X @", selectLine, OpCode[selectOp], Address[selectAddr]);
  WriteLineMessageWindow(strTemp, 0);
}
void RunCode()
{
}

void NoDiskAssemble()
{
  byte x;
  for (x = 0; x < 16; ++x)
  {
    OpCode[x] = x;
    Address[x] = x;
  }
  selectLine = 0;
  ResizeMessageWindow(consolePosX, consolePosY, consoleWidth-8, consoleHeight);
  ResetMenu("@", selectionPosX - 8, selectionPosY, selectionWidth + 8, selectionHeight, 5);
  SetMenuItem(0, "NextLine@");
  SetMenuItem(1, "LastLine@");
  SetMenuItem(2, "Edit Op");
  SetMenuItem(3, "Edit Addr@");
  SetMenuItem(4, "Run@");

  WriteLine();

  while(1)
  {
    switch (GetMenuSelection())
    {
      case 0:
        ++selectLine;
        break;
      case 1:
        --selectLine;
        break;
      case 2:
        ++selectOp;
        break;
      case 3:
        ++selectAddr;
        break;
      case 4:
        RunCode();
        break;
      default:
        break;
    }
    WriteLine();

  }
}
*/
