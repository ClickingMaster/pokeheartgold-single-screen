#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[8];void *party;u8 gap[0x34-12];u8 *data;}Work;
extern u32 Party_GetCount(void*);
extern void *Party_GetMonByIndex(void*,int),*Mon_GetBoxMon(void*);
extern void PCBox_LoadMonIcon(Work*,void*,int),PCBox_SetSpriteVisible(void*,int,int);

void PCBox_LoadPartyIcons(Work*w);

void PCBox_LoadPartyIcons(Work*w){u32 count=Party_GetCount(w->party),i;for(i=0;i<count;i++){void*mon=Mon_GetBoxMon(Party_GetMonByIndex(w->party,i));PCBox_LoadMonIcon(w,mon,w->data[0x40b2+i]);PCBox_SetSpriteVisible(w->data,w->data[0x40b2+i],0);}}
