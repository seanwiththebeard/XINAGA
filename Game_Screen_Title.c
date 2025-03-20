#include "Xinaga.h"
#include "GameData.h"

#if defined(__APPLE2__)
#pragma code-name (push, "LOWCODE")
#endif

#if defined (__NES__)
#pragma code-name (push, "TITLE")
#pragma rodata-name (push, "TITLE")
#pragma data-name (push, "TITLE")
//#pragma bss-name (push, "TITLE")
#endif
int tetrismap();
screenName Update_Title()
{
  ClearScreen();

  ScreenFadeIn();

  tetrismap();

  DrawBorder("@",12, 5, 16, 5, true);
  PrintString("Title Screen@", 14, 7, true, false);
  DrawBorder("@", 12, 19, 16, 5, true);
  PrintString("Press Space@", 14, 21, true, false);

  while (1)
  {
    UpdateInput();
    if (InputChanged())
      if (InputFire())
        return EditParty;
  }  
}


#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAP_SIZE 16
#define TARGET_POINTS ((MAP_SIZE * MAP_SIZE) * (2 / 3))
#define empty ' '
#define point 'q'

typedef struct {
  int x, y;
} Point;

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
    int px, py;
    px = x + tetris_blocks[block_index][i].x;
    py = y + tetris_blocks[block_index][i].y;
    if (px >= 0 && px < MAP_SIZE && py >= 0 && py < MAP_SIZE) 
    {
      map[py][px] = point;
      //++total_points;
    }
  }
  for ( y = 0; y < MAP_SIZE; y++)
    for ( x = 0; x < MAP_SIZE; x++)
      SetChar(map[y][x], x, y);
}

void expand_continent() {
  byte i;
  {
    int x = rand() % MAP_SIZE;
    int y = rand() % MAP_SIZE;

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
      SetChar('X', x, y);
      wait_vblank(1);
      SetChar(map[y][x], x, y);
      wait_vblank(1);

    }
    //if (map[y][x] == point) 
    {
      byte block_index = rand() % num_blocks;
      for (i = 0; i < 4; i++) {
        int nx = x + tetris_blocks[block_index][i].x;
        int ny = y + tetris_blocks[block_index][i].y;
        //if (nx >= 0 && nx < MAP_SIZE && ny >= 0 && ny < MAP_SIZE && map[ny][nx] == 0) 
        {
          place_tetris_block(x, y, block_index);
          break;
        }
      }
    }
  }
}   

int tetrismap() {
  byte x, y;
  byte total_points = 0;
  srand(56); // Fixed seed for reproducibility

  for ( y = 0; y < MAP_SIZE; y++)
    for ( x = 0; x < MAP_SIZE; x++)
      map[y][x] = ' ';

  // Place Tetris blocks randomly
  while (total_points < 16) {
    byte x = rand() % (MAP_SIZE - 3);
    byte y = rand() % (MAP_SIZE - 3);
    byte block_index = rand() % num_blocks;
    place_tetris_block(x, y, block_index);
    total_points += 4;
  }

  // Expand continents with Tetris blocks
  while (total_points < 128)
  {
    expand_continent();
    total_points += 4;
  }

  return 0;
}
