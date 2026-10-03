#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 v[3];} Buffer;
typedef struct {u32 value[2];u16 number;u8 byte,pad;} Entry;
typedef union {u8 raw[0x6bc];struct {u8 pad[0x5c0];Entry entries[8],extra;u8 p2[0x644-0x62c];u16 half[8];u8 p3[0x668-0x654];u32 value,buffers[8];int length,num;u8 p4[2],flags[8],flags2[8],p5[12],flags3[2],flagA,flagB,p6,flagC;} fields;} Data;
extern struct {u8 flag;u8 pad[7];Data *data;} _021D4140;
extern int sub_0203993C(void),sub_02033FC4(u16);extern void sub_02033AE0(void*,void*,int),sub_02033F70(void*);
void sub_02035AE0(void);
#define D (_021D4140.data->raw)
#define P(off) (*(void**)(D+(off)))
#define I(off) (*(int*)(D+(off)))
#define H(off) (*(u16*)(D+(off)))
static inline void Reset(void){int i=0,offset,half=0;_021D4140.data->fields.flagA=i;_021D4140.data->fields.flagB=i;offset=0;for(;i<8;i++,offset+=12,half+=2){_021D4140.data->fields.flags[i]=0;_021D4140.data->fields.flags2[i]=1;*(u16*)(D+0x644+half)=0;((Entry*)(D+0x5c0+offset))->byte=0xee;((Entry*)(D+0x5c0+offset))->number=0xffff;((Entry*)(D+0x5c0+offset))->value[1]=0;((Entry*)(D+0x5c0+offset))->value[0]=0;_021D4140.data->fields.buffers[i]=0;}}
void sub_02035AE0(void){int count=sub_02033FC4((u16)sub_0203993C())+1;int length;D[0x660]=0;D[0x661]=0;MI_CpuFill8(P(0x48c),0,I(0x68c));MI_CpuFill8(D+0x51c,0,0x60);length=I(0x68c)/count;{int i;for(i=0;i<count;i++)sub_02033AE0(D+0x51c + i*12,(u8*)P(0x48c) + i*length,length);}MI_CpuFill8(P(0x488),0,I(0x68c));MI_CpuFill8(D+0x4b0,0,0x60);{int i;for(i=0;i<count;i++)sub_02033AE0(D+0x4b0 + i*12,(u8*)P(0x488) + i*length,length);}MI_CpuFill8(D+0x308,0,0x180);sub_02033AE0(D+0x510,D+0x308,0x180);MI_CpuFill8(D+0x80,0xee,0x180);MI_CpuFill8(D+0x140,0xee,0x180);MI_CpuFill8(D+0x200,0,0x108);sub_02033AE0(D+0x498,D+0x200,0x108);MI_CpuFill8(D,0xee,0x26);MI_CpuFill8(D+0x40,0xee,0x26);D[0]=0xff;D[0x40]=0xff;MI_CpuFill8(P(0x490),0,I(0x690)*2);sub_02033AE0(D+0x4a4,P(0x490),I(0x690)*2);Reset();I(0x668)=0;D[0x62a]=0xee;H(0x628)=0xffff;I(0x624)=0;I(0x620)=0;D[0x6b2]=1;D[0x6b3]=1;_021D4140.flag=0;sub_02033F70(D+0x580);sub_02033F70(D+0x5a0);D[0x6b7]=0;}
