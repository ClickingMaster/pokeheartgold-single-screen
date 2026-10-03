#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#define B(p,o) (*(u8*)((p)+(o)))
#define H(p,o) (*(u16*)((p)+(o)))
#define W(p,o) (*(u32*)((p)+(o)))
typedef struct IDs{u8 pad[0xf4];u32 values[1];}IDs;
typedef struct Addresses{u8 pad[0x24];u32 values[1];}Addresses;
typedef struct Ports{u8 pad[0xa4];u16 values[1];}Ports;
typedef struct Peers{u8 pad[0x2d0];u8 values[1];}Peers;
#define ID(p,i) (((IDs*)(p))->values[i])
#define ADDRESS(p,i) (((Addresses*)(p))->values[i])
#define PORT(p,i) (((Ports*)(p))->values[i])
#define PEER(p,i) (((Peers*)(p))->values[i])
#define P(p,o) (*(void**)((p)+(o)))
extern u8*ov00_021F955C(void);
extern void ov00_021F956C(int);
extern void ov00_02210CBC(void*);
extern void ov00_021F7294(int,int);
extern void ov00_021F5D0C(int,int,int);
extern u32 ov00_021F7D50(int);
extern void ov00_021F6FC4(void);
extern void ov00_021F4304(int,u32,u32,u32,void*,int);
extern int ov00_021F5B30(void);
extern void ov00_021F7DB4(void);
extern void ov00_021F04A0(int,void*,int);
extern int ov00_021F0404(u32);
extern void ov00_021F3C5C(void);
extern void ov00_022136E4(void*);
extern void ov00_021F125C(void);
extern void ov00_021F36BC(void);
extern int ov00_021F8278(void);
extern void ov00_0221396C(void*);
extern u8 ov00_02217A60[],ov00_0221A928[];
void ov00_021F64C0(unsigned);
void ov00_021F64C0(unsigned action){int bytes=3,notify=0;u32 message[5];int i;
 switch(action){
 case 0:
 if(B(ov00_021F955C(),0x1a4)<B(ov00_021F955C(),0xd)-1){
 ov00_021F956C(13);
 message[0]=ID(ov00_021F955C(),B(ov00_021F955C(),0x1a4)+1);
 message[1]=B(ov00_021F955C(),0x1a4)+1;
 message[2]=PEER(ov00_021F955C(),B(ov00_021F955C(),0x1a4)+1);
 message[3]=ADDRESS(ov00_021F955C(),B(ov00_021F955C(),0x1a4)+1);
 message[4]=PORT(ov00_021F955C(),B(ov00_021F955C(),0x1a4)+1);bytes=5;
 }else{
 B(ov00_021F955C(),0x17)=0;W(ov00_021F955C(),0x20)=0;ov00_02210CBC(P(ov00_021F955C(),0x10));
 if(B(ov00_021F955C(),0x15)==0)ov00_021F956C(bytes);else if(B(ov00_021F955C(),0x15)==1)ov00_021F956C(4);else ov00_021F956C(10);
 B(ov00_021F955C(),0x1a4)=0;
 if(B(ov00_021F955C(),0x15)==2||B(ov00_021F955C(),0xd)==B(ov00_021F955C(),0x16)){
 if(B(ov00_021F955C(),0x15)==2){u8*dst=ov00_021F955C();W(dst,0x20c)=ID(ov00_021F955C(),B(ov00_021F955C(),0xd));}
 else{W(ov00_021F955C(),0x20c)=0;W(ov00_021F955C(),0xf4)=W(ov00_021F955C(),0x200);}
 ov00_021F956C(16);W(ov00_021F955C(),0x1d4)=0;
 for(i=1;i<=B(ov00_021F955C(),0xd);i++)ov00_021F7294(PEER(ov00_021F955C(),i),2);
 }else{
 message[0]=0;message[1]=B(ov00_021F955C(),0xd);message[2]=PEER(ov00_021F955C(),B(ov00_021F955C(),0xd));
 if(B(ov00_021F955C(),0x15)==0){W(ov00_021F955C(),0xe8)=2;*(u64*)(ov00_021F955C()+0xec)=OS_GetTick();}
 else if(B(ov00_021F955C(),0x15)==1){ov00_021F5D0C(1,0,0);if(!B(ov00_021F955C(),0x1a6)&&B(ov00_021F955C(),0xd)>=2){{u8*saved=ov00_021F955C();u32 mask=ov00_021F7D50(0);u8*peers=ov00_021F955C();if(W(saved,0x1dc)!=(mask&~(1<<PEER(peers,B(ov00_021F955C(),0xd))))){ov00_021F6FC4();goto done;}}}}
 }
 if(B(ov00_021F955C(),0x15)!=2)notify=1;
 }
 if(W(ov00_021F955C(),0x1a0)==16)break;
 ov00_021F4304(8,ID(ov00_021F955C(),B(ov00_021F955C(),0xd)),ADDRESS(ov00_021F955C(),B(ov00_021F955C(),0xd)),PORT(ov00_021F955C(),B(ov00_021F955C(),0xd)),message,bytes);
 if(ov00_021F5B30())return;B(ov00_021F955C(),0x3cd)=0;break;
 case 1:ov00_021F956C(1);if(B(ov00_021F955C(),0x15)==3)W(ov00_021F955C(),0x20c)=ID(ov00_021F955C(),B(ov00_021F955C(),0xd));notify=1;break;
 case 2:ov00_021F956C(1);if(B(ov00_021F955C(),0x15)==0||B(ov00_021F955C(),0x15)==1){B(ov00_021F955C(),0x17)=1;W(ov00_021F955C(),0x20)=W(ov00_021F955C(),0x200);}ov00_021F7DB4();if(B(ov00_021F955C(),0xd)>1){ov00_021F4304(9,W(ov00_021F955C(),0xf4),W(ov00_021F955C(),0x24),H(ov00_021F955C(),0xa4),ov00_021F955C()+0xf4+(B(ov00_021F955C(),0xd)-1)*4,1);if(ov00_021F5B30())return;}break;
 case 3:ov00_021F956C(1);ov00_021F7DB4();W(ov00_021F955C(),0x20c)=0;notify=1;break;
 case 4:{int zero,result;u8*c;
 if(B(ov00_021F955C(),0x15)!=2)ov00_021F04A0(2,ov00_02217A60,notify);
 zero=W(ov00_021F955C(),0x20c)==0;c=ov00_021F955C();result=ov00_021F0404(W(ov00_021F955C(),0x20c));(*(void(**)(int,int,int,int,int,u32))(c+0x464))(0,0,zero,0,result,W(ov00_021F955C(),0x468));
 if(B(ov00_021F955C(),0x15)==0||B(ov00_021F955C(),0x15)==1)ov00_021F3C5C();else{if(P(ov00_021F955C(),0xe4)){ov00_022136E4(P(ov00_021F955C(),0xe4));P(ov00_021F955C(),0xe4)=0;}ov00_021F125C();if(B(ov00_021F955C(),0x15)==2){ov00_021F36BC();if(ov00_021F8278())return;if(ov00_0221A928[0x1c]==1)ov00_0221A928[0x1d]=1;ov00_021F956C(10);}else ov00_021F956C(1);W(ov00_021F955C(),0x20c)=0;}
 B(ov00_021F955C(),0x1a9)=0;break;}
 }
 done:if(notify&&B(ov00_021F955C(),0x15)!=3)ov00_0221396C(P(ov00_021F955C(),0xe4));
}
