#pragma force_active on
#pragma dollar_identifiers on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#define BASE (*(u8 **)0x0380fff4)
#define H(p,o) (*(u16 *)((u8*)(p)+(o)))
#define P(p,o) (*(u8 **)((u8*)(p)+(o)))
#define HW(a) (*(volatile u16 *)(a))
void TimeoutDummy(void*);void WaitLoop_Waitus(u32,void(*)(void*));void WaitLoop_BbpAccess(void);void BBP_Write(u32,u32);void InitRF(void);void WWakeUp(void);
// @fn WWakeUp
void WWakeUp(void){HW(0x04808036)=0;WaitLoop_Waitus(8000,TimeoutDummy);HW(0x04808168)=0;switch(H(BASE,0x5f8)){case 2:{u32 value;HW(0x04808158)=0x6001;WaitLoop_BbpAccess();value=HW(0x0480815c);BBP_Write(1,value&0x7f);BBP_Write(1,value);WaitLoop_Waitus(40000,TimeoutDummy);InitRF();break;}case 3:InitRF();break;}}
