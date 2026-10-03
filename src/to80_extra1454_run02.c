#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u16 dma;u8 p[6];u8*base,*other;}Global;extern Global ov13_0224CF40;
typedef struct{u16 a,b,c,d;u16*name;u32 mode;}Config;extern Config ov13_0224598C;
typedef struct{u8 a,count;u16 name[10];}User;
extern u16 WM_GetNextTgid(void);
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define W(p,o) (*(u32*)((u8*)(p)+(o)))
u32 MBi_GetBeaconPeriodDispersion(void);int MB_Init(void*,User*,u32,u32,int);
void MBi_ChildPortCallback(u16*);
u32 MBi_GetBeaconPeriodDispersion(void){u8 mac[6];u8*ptr;int i;u32 sum;OS_GetMacAddress(mac);i=0;ptr=mac;sum=0;do{sum+=*ptr++;i++;}while(i<6);return ((sum+*(volatile u32*)0x027ffc3c)*7)%20;}
