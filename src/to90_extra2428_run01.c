#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Entry{u8 pad0[12];int angle;u8 state,pad11[3],index;u8 pad15[0x37];}Entry;typedef struct Work{Entry entries[3];u8 pade4[4];int step;u8 padec[5];u8 ready;u8 padf2[2];u8 type;}Work;
extern u8 ov93_02262CC4[];void ov93_02261310(void*,Work*);
#pragma opt_strength_reduction off
void ov93_02261310(void*a,Work*w){w->step=0x1e000/ov93_02262CC4[w->type*4];int i=0;int angle=i;Entry *p=w->entries;do{p->angle=angle<<12;i++;p->index=i;angle+=90;p++;}while(i<3);w->entries[0].state=2;w->ready=1;}
