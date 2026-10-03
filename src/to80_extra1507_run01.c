#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad0[2];u8 x,y;u8 pad4[6];} Entry;
extern const Entry ov102_021EC820[];
int ov102_021EC5A0(u32,u32);
int ov102_021EC5A0(u32 x,u32 y) {u32 i;const Entry *entry;for(i=0,entry=ov102_021EC820;i<27;i++,entry++){if(entry->x<=x && x<entry->x+16 && entry->y<=y && y<entry->y+16)return i;}return -1;}
