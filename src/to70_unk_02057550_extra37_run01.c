#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct { u16 x,y; s8 dir; u8 a,b,c; } Item;
extern u8 *_021D41C4;
extern int sub_020548C0(void *, u16,u16);
void sub_020576C0(int idx, int unused, u8 *data);
void sub_020576C0(int idx, int unused, u8 *data) {
 Item *item;
 int i;
 if (_021D41C4 == NULL) return;
 item = (Item *)(_021D41C4 + 0x34) + idx;
 if(data[4] & 0x80) { _021D41C4[0xec] = 1; return; }
 if(item->dir == -1) { for(i=0;i<8;i++) _021D41C4[0xcc+i]=1; }
 item->x=0; item->y=0;
 item->x+=data[0]; item->x+=(data[1]<<8)&0xff00;
 item->y+=data[2]; item->y+=(data[3]<<8)&0xff00;
 item->dir=data[4]&15;
 (_021D41C4+idx)[0x24]=1; (_021D41C4+idx)[0xcc]=1;
 GF_ASSERT(sub_020548C0(*(void **)(_021D41C4+0x30),item->x,item->y)==0);
}
