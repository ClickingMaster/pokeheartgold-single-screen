#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x2fc];void*sprites[4];} Sub;
typedef struct{u8 p[0x34];Sub*sub;} Work;
extern void ManagedSprite_SetDrawFlag(void*,int);
extern int PCBox_GetSlotMonData(Work*,int,int,int),PCBox_GetSlotBoxMon(Work*,int,int);
extern void PCBox_LoadMonIcon(Work*,int,int);
void PCBox_RefreshSlotIcon(Work*,int,int,int);
void PCBox_RefreshSlotIcon(Work*w,int a,int b,int i){void**sprites=w->sub->sprites;ManagedSprite_SetDrawFlag(sprites[i],0);if(PCBox_GetSlotMonData(w,b,172,0)){PCBox_LoadMonIcon(w,PCBox_GetSlotBoxMon(w,a,b),i);ManagedSprite_SetDrawFlag(sprites[i],1);}}
