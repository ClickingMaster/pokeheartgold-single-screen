#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u32 flags;u8 pad[20];s32 sx,sy;u8 p2[4];s32 tx,ty;u16 width,height;s32 rx,ry;} Material;
void NNS_G3dGeBufferOP_N(u32,const u32*,u32);
static inline u32 Mul(s32 a,s32 b){return (u32)((s64)a*b>>12);}
void NNSi_G3dSendTexSRTSi3d(const Material*);
void NNSi_G3dSendTexSRTSi3d(const Material *p){struct {u32 command,m[14];} packet;
#define command packet.command
#define m packet.m
if(p->flags&8)command=0x101710;else command=0x101910;m[0]=3;m[12]=0;m[9]=0;m[8]=0;m[7]=0;m[6]=0;m[4]=0;m[3]=0;m[2]=0;m[13]=2;if(p->flags&4){m[10]=0;m[11]=0;if(p->flags&1){m[1]=0x1000;m[5]=0x1000;}else{m[1]=p->sx;m[5]=p->sy;}}else if(p->flags&1){m[10]=(0u-((u32)p->tx<<4))*p->width;m[11]=(0u-((u32)p->ty<<4))*p->height;m[1]=0x1000;m[5]=0x1000;}else{m[10]=(0u-(u32)((s64)p->sx*p->tx>>8))*p->width;m[11]=(0u-(u32)((s64)p->sy*p->ty>>8))*p->height;m[1]=p->sx;m[5]=p->sy;}if(p->rx!=0x1000){m[1]=Mul(p->rx,(s32)m[1]);m[10]=Mul(p->rx,(s32)m[10]);}if(p->ry!=0x1000){m[5]=Mul(p->ry,(s32)m[5]);m[11]=Mul(p->ry,(s32)m[11]);}NNS_G3dGeBufferOP_N(command,(u32*)&packet+1,14);}
