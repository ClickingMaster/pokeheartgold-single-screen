#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x2fc];void*sprites[4];} Sub;
typedef struct{u8 p[0x34];Sub*sub;} Work;
extern void ManagedSprite_SetDrawFlag(void*,int);
extern int ov14_021E6070(Work*,int,int,int),ov14_021E60C0(Work*,int,int);
extern void ov14_021F2EA0(Work*,int,int);
void ov14_021F2ED0(Work*,int,int,int);
void ov14_021F2ED0(Work*w,int a,int b,int i){void**sprites=w->sub->sprites;ManagedSprite_SetDrawFlag(sprites[i],0);if(ov14_021E6070(w,b,172,0)){ov14_021F2EA0(w,ov14_021E60C0(w,a,b),i);ManagedSprite_SetDrawFlag(sprites[i],1);}}
