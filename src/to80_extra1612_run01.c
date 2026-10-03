#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 pad[12];s32 amplitude,angle;} State;
typedef struct {u8 pad[0x300];s16 left[192],right[192];} Output;
extern Output *sub_02010EE0(State *,int);extern s32 sub_02010A54(int,int),sub_02010A7C(int,int);extern void sub_02010A00(int,s32 *,int,int),GF_AssertFail(void);
static inline s32 Sin(s32 angle){return FX_SinCosTable_[(angle>>4)*2];}
void sub_02011F10(State *);
void sub_02011F10(State *s){s32 table[192];s32 radius;int count;Output *out;out=sub_02010EE0(s,0);{s32 wave=Sin(s->angle);s64 product=(s64)wave*s->amplitude;radius=((s32)((product+2048)>>12))>>12;}int angle=65535*(180-((radius*2/21)+1)*2)/360/2;count=sub_02010A54(angle,256)>>12;int i;if(count>=192)GF_AssertFail();sub_02010A00(angle,table,count,0);for(i=0;i<96;i++){int pos=count-(i+1),v=radius;int left,right;if(pos>0&&table[pos]>radius)v=table[pos];left=sub_02010A7C(128,-v);right=sub_02010A7C(128,v);out->left[i]=(s16)left;out->right[i]=(s16)right;out->left[191-i]=(s16)left;out->right[191-i]=(s16)right;}}
