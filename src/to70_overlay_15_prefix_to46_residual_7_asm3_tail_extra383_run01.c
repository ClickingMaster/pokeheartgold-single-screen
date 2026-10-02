#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[13];u8 value;} Item;
typedef struct {u8 pad[0x234];u8 *data;u8 pad2[0x619-0x238];u8 selected,pad3,state,timer;} State;
extern void ov15_021F9F08(State *),ov15_021FF364(State *,int,int,int),ov15_021FD574(State *,int,int,int),ov15_02200030(State *,int),ov15_021FD404(State *,int,int),ov15_021FA044(s16 *,s16 *,int),ov15_021FA070(s16 *,s16 *,int,int);
extern int ov15_021FA074(State *);
int ov15_021FAB34(State *);
int ov15_021FAB34(State *s){u8 *q=&s->selected;switch(q[2]){case 0:q[3]=0;q[2]++;break;case 1:if(q[3]<8){q[3]++;break;}s->data[0x64]=q[0];ov15_021F9F08(s);ov15_021FF364(s,*(s16 *)(s->data+s->data[0x64]*12+10),-1,0);ov15_021FD574(s,0,ov15_021FA074(s),0);ov15_02200030(s,s->data[0x64]);ov15_021FD404(s,1,s->data[0x64]);ov15_021FA044((s16 *)(s->data+10+s->data[0x64]*12),(s16 *)(s->data+8+s->data[0x64]*12),((Item *)(s->data+s->data[0x64]*12))->value);ov15_021FA070((s16 *)(s->data+10+s->data[0x64]*12),(s16 *)(s->data+8+s->data[0x64]*12),((Item *)(s->data+s->data[0x64]*12))->value,6);q[2]++;return 1;case 2:return 1;}return 0;}
