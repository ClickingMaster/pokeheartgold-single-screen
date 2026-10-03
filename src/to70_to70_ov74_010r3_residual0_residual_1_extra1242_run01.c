#pragma force_active on
#include "global.h"
extern u8*ov74_0223D338;extern int ov74_02231764(int),TranslateAgbSpecies(int),GetMonIconNaixEx(int,int,int),GetMonIconPaletteEx(int,int,int);extern void ov74_02231E38(int,void**,int,int);
void ov74_02231F30(int species,int a,int b,int i,int active,int c,int d);
void ov74_02231F30(int species,int a,int b,int i,int active,int c,int d){u8*w=ov74_0223D338+i*524;void*p;if(active){if(ov74_02231764(species)==0){species=TranslateAgbSpecies(species);}else{species=0;}ov74_02231E38(GetMonIconNaixEx(species,a,b),&p,c,d);MIi_CpuCopyFast(*(void**)((u8*)p+20),(u32*)(w+12),512);*(u32*)w=(i*16+100)<<5;*(int*)(w+8)=active;*(int*)(w+4)=GetMonIconPaletteEx(species,b,a)+8;}else *(int*)(w+8)=0;}
