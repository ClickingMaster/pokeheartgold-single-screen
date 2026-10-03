#pragma force_active on
#include "global.h"
typedef struct {int a,index;} Info;typedef struct {u32 a;Info*info;u8 pad[0x54];void*palette;} Work;
typedef struct {int x[5];} Entry;extern Entry _02264020[];extern void PaletteData_LoadFromNarc(void*,int,int,int,int,int,u16,u16);

void ov92_0225E130(Work*w,int idx);

void ov92_0225E130(Work*w,int idx){void*p=w->palette;int i;Entry*e=&_02264020[idx];for(i=0;i<4;i++)PaletteData_LoadFromNarc(p,193,73,113,1,32,(u16)((i+1)<<4),(u16)((*(int*)((u32)e+w->info->index*80+i*4-76))<<4));}
