#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 f0:1,f1:1,f2:1,f3:1,f4:1,f5:1,f6:1,f7:1;} A35198_Flags;
typedef struct {A35198_Flags flags;u8 id;u8 pad[22];u16 tempo,mult,acc,pad2;void*bank;} A35198_Player;
typedef struct A35198_Channel {u8 pad[34];u8 marker;u8 pad2[45];struct A35198_Channel*next;} A35198_Channel;
typedef struct {A35198_Flags flags;u8 priority;u16 wait;u8 pad[32];const u8*base,*pos;u8 tail[12];u32 time;A35198_Channel*channel;} A35198_Track;
int ReadByte(A35198_Track*);
s16 *GetVariablePtr(void*,int);
u16 SND_CalcRandom(void);
int ReadArg(A35198_Track*,void*,int);
int ReadArg(A35198_Track*t,void*p,int type){int result;switch(type){case 0:result=ReadByte(t);break;case 1:{int low=ReadByte(t);result=(u16)(low|(ReadByte(t)<<8));break;}case 2:{u32 v=0;int byte;do{byte=ReadByte(t);v=((u32)byte&127)|(v<<7);}while(byte&128);result=(s32)v;break;}case 4:{int idx=ReadByte(t);s16*v=GetVariablePtr(p,idx);if(v)result=*v;break;}case 3:{int lo=ReadByte(t);s16 min=(u16)(lo|(ReadByte(t)<<8));int hi=ReadByte(t);u16 max=(u16)(hi|(ReadByte(t)<<8));u32 random=SND_CalcRandom();result=((s32)(random*(u32)((s32)(s16)max-min+1))>>16)+min;break;}}return result;}
