#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x1c];int state;u8 q[12];int next;u8 r[0x11ac];u16 value;} W;
extern void GfGfx_EngineATogglePlanes(int,int);extern void ov70_02238E50(W*,int,int);
int ov70_02239658(W*);
int ov70_02239658(W*w) {if(w->value!=0){GfGfx_EngineATogglePlanes(1,0);GfGfx_EngineATogglePlanes(2,0);GfGfx_EngineATogglePlanes(4,0);ov70_02238E50(w,7,11);w->state=1;w->next=9;}return 3;}
