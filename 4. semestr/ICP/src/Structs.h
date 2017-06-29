#ifndef CARDID_H
#define CARDID_H

/**
* \brief struct defining a card
*/
typedef struct CardID {
	CardID(unsigned int suit, unsigned int value) :
		suit(suit), value(value){}
	CardID(){}
	unsigned int suit;
	unsigned int value;
} CardID;

/**
* \brief get struct which contains information about clicked card
*/
typedef struct ClickID{
  ClickID(unsigned int stackgroup,unsigned int stack, unsigned int stackpos) : stackgroup(stackgroup), stack(stack), stackpos(stackpos){}
  ClickID(unsigned int integer) : stackpos(integer % 128),stack(((integer)%1024)/128),stackgroup(integer/1024){}
  int ToInt(){return this->stackpos + this->stack*128 + this->stackgroup*1024;}

  unsigned int stackgroup;
  unsigned int stack;
  unsigned int stackpos;
} ClickID;
#endif // CARDID_H
