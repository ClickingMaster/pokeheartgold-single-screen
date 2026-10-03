#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {u8 pad[0x644];volatile u8 ids[32];} Work;
extern u8 *ov00_0221A784;
extern void ov00_021F2FA0(void),ov00_021EE800(int),ov00_021F3590(void);
extern u32 ov00_021EE880(void *,int);
void ov00_021EEB1C(int,int,int,int,int);
#define W32(x) (*(u32 *)(ov00_0221A784+(x)))
#define W8(x) (*(volatile u8 *)(ov00_0221A784+(x)))
void ov00_021EEB1C(int a,int b,int c,int d,int e){
 if(!a && b){if(!W32(0x514)){ov00_021F2FA0();ov00_021EE800(3);}}
 else if(!a){
  u8 *p;int i;u32 key;
  ov00_021EE800(6);p=ov00_0221A784;
  i=0;
  if(i<=*(volatile u8 *)(p+0x381)){
  key=*(u32 *)(p+0x64);
  do
   if(key==((u32 *)(p+0x468))[i]){W8(0x2c)=((Work *)ov00_0221A784)->ids[i];break;}
  while(++i<=*(volatile u8 *)(p+0x381));
  }
 }
 W32(0x664)=ov00_021EE880(ov00_0221A784+0x644,W8(0x381)+1);
 ov00_021F3590();
 if(W8(0x389)==2 || W8(0x389)==3)
  ((void (*)(int,int,int,int,int,u32))W32(0x88))(a,b,c,d,e,W32(0x8c));
 else ((void (*)(int,int,u32))W32(0x80))(a,b,W32(0x84));
 if(a && ov00_0221A784 && W32(0x24)==5)ov00_021EE800(3);
}
