#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
#pragma warn_uninitializedvar off
#define B(p,o) (*(u8*)((p)+(o)))
#define H(p,o) (*(u16*)((p)+(o)))
#define W(p,o) (*(u32*)((p)+(o)))
typedef void(*CB)(int,int,int,int,int,u32);
extern u8*ov00_021F955C(void);
extern u32*ov00_021EE73C(u32,int);
extern u8 ov00_02217A60[],ov00_02217AAC[];
extern int ov00_021EE31C();
extern int ov00_021F0404();
extern int ov00_021F04A0();
extern int ov00_021F2EE4();
extern int ov00_021F3334();
extern int ov00_021F381C();
extern int ov00_021F3C5C();
extern int ov00_021F3CF8();
extern int ov00_021F4304();
extern int ov00_021F5D0C();
extern int ov00_021F7BA0();
extern int ov00_021F7CEC();
extern int ov00_021F8278();
extern int ov00_021F82EC();
extern int ov00_021F956C();
extern int ov00_021F9630();
extern int ov00_0220D218();
extern int ov00_0220D238();
extern int ov00_0221019C();
extern int ov00_02212DB4();
extern int ov00_02212E78();
extern int ov00_02212E80();
extern int ov00_022139B8();
void ov00_021F5AE0(u32 x,u32*p);
int ov00_021F5B30(u32 x);
int ov00_021F5FD4(int x);
int ov00_021F60AC(int x);
int ov00_021F631C(void);
int ov00_021F63DC(void);
int ov00_021F6B20(u32*p,u32 n);
void ov00_021F6D30(void);
int ov00_021F6DF0(void);
void ov00_021F6FC4(void);
int ov00_021F6FF8(void);
int ov00_021F70B8(u32 x);
void ov00_021F7128(void);
int ov00_021F75F4(int x,int y);
void ov00_021F6BA8(void);
int ov00_021F6EC4(int x);
int ov00_021F7158(u32 mask);
void ov00_021F7294(int x,int y);
int ov00_021F5B5C(u32 x,int y);
int ov00_021F631C(void) {int r=1;u8*p;if(B(ov00_021F955C(),0x15)==3){if(B(ov00_021F955C(),13))ov00_021F7128();ov00_021F2EE4(6,-80430);return 0;}p=ov00_021F955C();B(p,20)=B(ov00_021F955C(),13);W(ov00_021F955C(),0x208)=0;if(W(ov00_021F955C(),0x19c)){ov00_0221019C(W(ov00_021F955C(),0x19c));W(ov00_021F955C(),0x19c)=0;}if(B(ov00_021F955C(),13))ov00_021F6FC4();else{ov00_021F956C(4);r=ov00_021F5FD4(0);}return r;}
