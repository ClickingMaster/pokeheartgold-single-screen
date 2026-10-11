#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x10];void *p;u8 pad2[0x98];s8 kind;} W;
extern void PokeathlonUtil_BufferNumber(void *,u32,int,int),PokeathlonUtil_PrintMessageAt(void *,int,int,int,u8);
void ov99_021E88EC(W *,int,int,int,int);
void ov99_021E88EC(W *w,int value,int text,int ignored,int unused){if(value==65535)GF_AssertFail();if(w->kind==0){PokeathlonUtil_BufferNumber(w->p,(u8)(value/30),3,0);PokeathlonUtil_BufferNumber(w->p,(u8)((value%30)*10/30),1,1);}else if(w->kind==6){PokeathlonUtil_BufferNumber(w->p,(u32)value>>10,3,0);PokeathlonUtil_BufferNumber(w->p,((value%1024)*10)/1024,1,1);}else PokeathlonUtil_BufferNumber(w->p,value,3,0);PokeathlonUtil_PrintMessageAt(w->p,text,w->kind+0x56,0,(u8)unused);}
