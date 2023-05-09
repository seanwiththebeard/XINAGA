#include "game.h"
#include <stdlib.h>

byte strTemp[20];

PartyDef Party = { 0, 0, 0};
RaceDescriptionDef RaceDescription[] = {{"Human@", 255},{"Elf@", 6}, {"Dwarf@", 255}, {"Halfling@", 6}};
ClassDescriptionDef ClassDescription[4] = {{"Fighter@", 8},{"Magic-User@", 4}, {"Cleric@", 6}, {"Thief@", 4}};

int AbilityModifier[20] = {-3, -3, -3, -3, -3, -2, -2, -1, -1, -1, 0, 0, 0, 0, +1, +1, +1, +2, +2, +3};
int randseed = 0;

struct playerChar *startRoster=NULL;

byte CountRoster()
{
  struct playerChar *temp = startRoster;
  byte i = 0;
  while(temp != NULL)
  {
    ++i;
    temp = temp->next;
  }
  return i;
}

void create()
{
  struct playerChar *temp,*ptr;
  temp=(struct playerChar *)malloc(sizeof(struct playerChar));  

  if(temp==NULL)
    exit(0);

  temp->next=NULL;
  if(startRoster==NULL)
    startRoster=temp;
  else
  {
    ptr=startRoster;
    while(ptr->next!=NULL)
    {
      ptr=ptr->next;
    }
    ptr->next=temp;
  }
}

struct playerChar *getPlayerChar(byte index)
{
  byte i = 0;
  struct playerChar *tmp = startRoster;
  while (tmp != NULL)
  {
    if(i == index)
    {
      return tmp;
    }
    tmp = tmp->next;
    ++i;
  }
}

void delete_pos(byte pos)
{
  int i;
  struct playerChar *temp,*ptr;

  if(startRoster==NULL)
    exit(0);
  else
  {
    if(pos==0)
    {
      ptr=startRoster;
      startRoster=startRoster->next ;
    }
    else
    {
      ptr=startRoster;
      for(i=0;i<pos;i++)
      {
        temp=ptr; 
        ptr=ptr->next ;
        if(ptr==NULL)
        {
          WriteLineMessageWindow("Position not Found:@", 0);
          return;
        }
      }
      temp->next =ptr->next ;
    }
    //sprintf(str, "Deleted element:%d",ptr->character.NAME);
    //WriteLineMessageWindow(str, 0);
    free(ptr);
  }
}

//Party
struct playerChar *startParty=NULL;

byte CountParty()
{
  struct playerChar *temp = startParty;
  byte i = 0;
  while(temp != NULL)
  {
    ++i;
    temp = temp->next;
  }
  return i;
}

struct playerChar *getPartyMember(byte index)
{
  byte i = 0;
  struct playerChar *tmp = startParty;
  while (tmp != NULL)
  {
    if(i == index)
    {
      return tmp;
    }
    tmp = tmp->next;
    ++i;
  }
}

void AddParty(byte index)
{
  struct playerChar *temp,*ptr,*src;
  temp=(struct playerChar *)malloc(sizeof(struct playerChar));
  src = getPlayerChar(index);

  if(temp==NULL)
    exit(0);
  
  memcpy(temp, src, sizeof(struct playerChar));
  
  temp->next=NULL;
  if(startParty==NULL)
    startParty=temp;
  else
  {
    ptr=startParty;
    while(ptr->next!=NULL)
    {
      ptr=ptr->next;
    }
    ptr->next=temp;
  }
  delete_pos(index);
}

void DeleteParty(byte pos)
{
  int i;
  struct playerChar *temp,*ptr;

  if(startParty==NULL)
    exit(0);
  else
  {
    if(pos==0)
    {
      ptr=startParty;
      startParty=startParty->next ;
    }
    else
    {
      ptr=startParty;
      for(i=0;i<pos;i++)
      {
        temp=ptr; 
        ptr=ptr->next ;
        if(ptr==NULL)
        {
          WriteLineMessageWindow("Position not Found:@", 0);
          return;
        }
      }
      temp->next =ptr->next ;
    }
    //sprintf(str, "Deleted element:%d",ptr->character.NAME);
    //WriteLineMessageWindow(str, 0);
    free(ptr);
  }
}

void RemoveParty()
{
  byte index = CountParty()-1;
  struct playerChar *temp,*ptr,*src;
  temp=(struct playerChar *)malloc(sizeof(struct playerChar));
  src = getPartyMember(index);

  if(temp==NULL)
    exit(0);
  
  memcpy(temp, src, sizeof(struct playerChar));
  
  temp->next=NULL;
  if(startRoster==NULL)
    startRoster=temp;
  else
  {
    ptr=startRoster;
    while(ptr->next!=NULL)
    {
      ptr=ptr->next;
    }
    ptr->next=temp;
  }
  DeleteParty(index);
}