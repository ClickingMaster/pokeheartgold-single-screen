#pragma force_active on
#include "global.h"
typedef struct {u8 p[10];s16 value;} Item;typedef struct {Item item[8];u8 p[4];u8 selected;} Sub;
typedef struct {u8 p[0x234];Sub *sub;u8 q[0x43a];u8 value;} W;
int BagApp_GetMovingItemPageSlot(W*);
int BagApp_GetMovingItemPageSlot(W*w) {int n=w->value;Sub*s=w->sub;int quotient=n/6;if(s->item[s->selected].value==quotient*6)return n%6;return -1;}
