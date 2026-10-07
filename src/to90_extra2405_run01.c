#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{void*object;u8 p[0x14];int state;u8 q[0xc];s32 x,y;u8 r[0x12];u16 value;u8 tail[0x4c];}To90Entry;
void ov96_021EB06C(void*,s32,s32,int*,int*);int ov96_021F3180(u8,u16,u16,u8*);void ov96_021F2834(u8*);
void ov96_021F2834(u8*w){u8 indices[4][12],values[4],counts[4];u8 group;int a,b,i,j,result;if(!w[0x72b])return;for(i=0;i<4;i++)counts[i]=0;w[0x72b]=0;w[0x74e]=0;w[0x74f]=0;for(i=0;i<12;i++){To90Entry*e=(To90Entry*)(w+0x20+(i/3)*0x1b0+(i%3)*0x90);if(e->state==1){ov96_021EB06C(e->object,e->x/4096,e->y/4096,&a,&b);result=ov96_021F3180((u8)*(u16*)(w+0x730),(u16)a,(u16)b,&group);if(result){e->value+=result;indices[group][counts[group]++]=i;values[group]=result;}}}for(i=0;i<4;i++)for(j=0;j<counts[i];j++){w[0x750+w[0x74e]]=indices[i][j];w[0x75c+w[0x74e]]=values[i];w[0x74e]++;}}
