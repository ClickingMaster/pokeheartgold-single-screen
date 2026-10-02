#pragma force_active on
#include "global.h"
#pragma force_active on
struct Slot{u8 active;u8 gap[13];u8 a,b;};struct S{u8 pad[0x7d4];struct Slot slots[1];};void ov96_02214258(struct S*,int,int,int);
void ov96_02214258(struct S*s,int index,int a,int b){s->slots[index].active=1;s->slots[index].a=a;s->slots[index].b=b;}
