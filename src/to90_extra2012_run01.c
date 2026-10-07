#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct State {int state;u8 pad4[8];void *menu;u8 pad10[0x204];int count;u8 pad218[0x17c];volatile int selected;u32 pad398;void(*callback)(void*,int);void *arg;} State;
typedef struct {u8 pad[0x48];u32 keys;} System;
extern System gSystem;
extern const void *ov27_0225D49C[];
extern int TouchscreenHitbox_FindRectAtTouchNew(const void*),ov27_0225CA68(volatile int*,int,int),ov01_021EF00C(void*);
extern void ov27_0225CD18(State*,int,int),ov27_0225CD74(State*,int),PlaySE(int);
int ov27_0225CA98(State*);
#define NOTIFY(s) do {if((s)->callback)(s)->callback((s)->arg,(s)->selected);}while(0)
#define CHANGE(s,d) do {if(ov27_0225CA68(&(s)->selected,(s)->count,d)){ov27_0225CD18(s,(s)->count,(s)->selected);PlaySE(1500);NOTIFY(s);}}while(0)
int ov27_0225CA98(State *s){int selected=TouchscreenHitbox_FindRectAtTouchNew(ov27_0225D49C[s->count-2]);if(selected!=-1){s->selected=selected;ov27_0225CD18(s,s->count,s->selected);PlaySE(1500);s->state=9;ov27_0225CD74(s,s->count);NOTIFY(s);}
 else if(gSystem.keys&0x40){CHANGE(s,0);}else if(gSystem.keys&0x80){CHANGE(s,1);}else if(gSystem.keys&0x20){CHANGE(s,2);}else if(gSystem.keys&0x10){CHANGE(s,3);}else if(gSystem.keys&1){ov27_0225CD74(s,s->count);s->state=9;PlaySE(1500);}else if(gSystem.keys&2){if(ov01_021EF00C(s->menu)){s->selected=s->count-1;ov27_0225CD18(s,s->count,s->selected);ov27_0225CD74(s,s->count);s->state=9;PlaySE(1500);}}return 0;}
