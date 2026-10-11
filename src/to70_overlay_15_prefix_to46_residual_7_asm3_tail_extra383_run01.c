#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[13];u8 value;} Item;
typedef struct {u8 pad[0x234];u8 *data;u8 pad2[0x619-0x238];u8 selected,pad3,state,timer;} State;
extern void BagApp_LoadPocketItemNames(State *),BagApp_DrawItemList(State *,int,int,int),BagApp_LoadSubScreenLayout(State *,int,int,int),BagApp_LoadPocketObjPalette(State *,int),BagApp_LoadPocketSubBgPalette(State *,int,int),BagApp_ClampPocketScroll(s16 *,s16 *,int),ov15_021FA070(s16 *,s16 *,int,int);
extern int BagApp_GetPageItemCount(State *);
int BagApp_RunPocketSwitch(State *);
int BagApp_RunPocketSwitch(State *s){u8 *q=&s->selected;switch(q[2]){case 0:q[3]=0;q[2]++;break;case 1:if(q[3]<8){q[3]++;break;}s->data[0x64]=q[0];BagApp_LoadPocketItemNames(s);BagApp_DrawItemList(s,*(s16 *)(s->data+s->data[0x64]*12+10),-1,0);BagApp_LoadSubScreenLayout(s,0,BagApp_GetPageItemCount(s),0);BagApp_LoadPocketObjPalette(s,s->data[0x64]);BagApp_LoadPocketSubBgPalette(s,1,s->data[0x64]);BagApp_ClampPocketScroll((s16 *)(s->data+10+s->data[0x64]*12),(s16 *)(s->data+8+s->data[0x64]*12),((Item *)(s->data+s->data[0x64]*12))->value);ov15_021FA070((s16 *)(s->data+10+s->data[0x64]*12),(s16 *)(s->data+8+s->data[0x64]*12),((Item *)(s->data+s->data[0x64]*12))->value,6);q[2]++;return 1;case 2:return 1;}return 0;}
