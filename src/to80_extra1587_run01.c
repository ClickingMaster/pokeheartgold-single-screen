#pragma force_active on
#include "global.h"
#pragma force_active on
void ov96_021FF1E0(int,const s32*,volatile s32*);
void ov96_021FF1E0(int inset,const s32*p,volatile s32*out){s32 word=*p;u32 type=(u8)word;int y=(u8)(word>>8);int x=(u16)(word>>16);u32 size;y+=32;out[0]=type;if(type==3){out[1]=x-32;out[2]=y-32;return;}size=type==1?48:32;x-=size/2;y-=size/2;out[1]=x;out[2]=y;out[3]=x+size;out[4]=y;out[5]=x+size;out[6]=y+size;out[7]=x;out[8]=y+size;{int a=out[1],b=out[2];out[9]=a;out[10]=b;}{int a=out[3],b=out[4];out[11]=a;out[12]=b;}{int a=out[3],b=out[4];out[13]=a;out[14]=b;}{int a=out[5],b=out[6];out[15]=a;out[16]=b;}{int a=out[5],b=out[6];out[17]=a;out[18]=b;}{int a=out[7],b=out[8];out[19]=a;out[20]=b;}{int a=out[7],b=out[8];out[21]=a;out[22]=b;}{int a=out[1],b=out[2];out[23]=a;out[24]=b;}out[10]-=inset;out[12]-=inset;out[13]+=inset;out[15]+=inset;out[18]+=inset;out[20]+=inset;out[21]-=inset;out[23]-=inset;}
