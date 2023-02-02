
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <conio.h>
#include <peekpoke.h>

char* flag = (char*)0xC010;
int lastKey;
int keyState;
int main (void)
{
  while(1)
  {
    int key = PEEK(0xC000);
    while(key - 128 > 0){}
    
    while(key - 128 < 0)
     key = PEEK(0xC000);
    
    printf("%c\n", key);
    
    flag[1]++;
  }
  return 0;
}

