#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef void(*Callback)(int,int,int);
typedef struct {Callback cb;u32 pad;char user[32],pass[32];int active;u32 info;u8 space[0x970];OSThread thread;} Work;
extern Work*ov00_0221A768;extern char ov00_022179D8[];extern int ov00_021EC230(void);extern void *ov00_021ECECC(int,int);extern void ov00_021ECEE4(void*);extern int ov00_021ECC6C(char*,u32*);extern void ov00_021ED038(void*);

int ov00_021ED1F0(Callback cb,char*user,char*pass);

int ov00_021ED1F0(Callback cb,char*user,char*pass){if(ov00_021EC230()!=0)return 0;ov00_0221A768=ov00_021ECECC(2688,32);ov00_0221A768->active=0;MI_CpuFill8(ov00_0221A768,0,2688);ov00_0221A768->cb=cb;strncpy(ov00_0221A768->user,user,31);strncpy(ov00_0221A768->pass,pass,31);if(ov00_021ECC6C(ov00_022179D8,&ov00_0221A768->info)==0){ov00_021ECEE4(ov00_0221A768);ov00_0221A768=0;return 0;}OS_CreateThread(&ov00_0221A768->thread,ov00_021ED038,0,&ov00_0221A768->thread,2048,16);OS_WakeupThreadDirect(&ov00_0221A768->thread);return 1;}
