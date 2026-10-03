#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x36c];void *sprites[16];u8 p2[0x184df-0x3ac];u8 first,second,count;u8 unused:2,bank:1,rest:5;} Work;
extern s8 _021EA720[][2];extern void ManagedSprite_OffsetPositionXY(void *,s16,s16);
int ov108_021E82E0(Work *,int);
int ov108_021E82E0(Work *w,int mode){u8 slots[2],groups[2];int bank=6*w->bank;int i;slots[0]=w->first+bank;slots[1]=w->second+bank;groups[0]=w->first/3;groups[1]=w->second/3;for(i=0;i<2;i++)ManagedSprite_OffsetPositionXY(w->sprites[slots[i]],0,12*_021EA720[mode][groups[i]]);w->count++;if(w->count>=8){w->count=0;return 1;}return 0;}
