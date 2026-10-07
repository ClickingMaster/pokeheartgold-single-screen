#pragma force_active on
#include "global.h"
#pragma force_active on
#define B(w,n) (*((u8*)(w)+(n)))
#define H(w,n) (*(u16*)((u8*)(w)+(n)))
#define U(w,n) (*(u32*)((u8*)(w)+(n)))
#define P(w,n) (*(void**)((u8*)(w)+(n)))
extern void *Save_SafariZone_Get(void*),*Save_PlayerData_GetOptionsAddr(void*),*Save_PlayerData_GetProfile(void*),*Save_SysInfo_RTC_Get(void*);extern void SafariZone_CopyAreaSet(void*,int,void*);extern int Options_GetTextSpeed(void*),Options_GetFrame(void*),MenuInputStateMgr_GetState(void*),GF_RTC_GetTimeOfDayByHour(int);extern u8 SafariZone_GetObjectUnlockLevel(void*);extern u32 PlayerProfile_GetTrainerID(void*),PlayerProfile_GetTrainerGender(void*);extern void*ov108_021EA63C(u32,int,u8,u8*,int);
void ov108_021E8968(u8*,u8*);
void ov108_021E8968(u8*w,u8*args){void*options,*profile;int unlocked;u32 trainer;u8 gender;P(w,0x20)=Save_SafariZone_Get(P(args,0));SafariZone_CopyAreaSet(P(w,0x20),0,w+0x24);options=Save_PlayerData_GetOptionsAddr(P(args,0));B(w,0x18)=Options_GetTextSpeed(options);B(w,0x19)=Options_GetFrame(options);U(w,0x10)=MenuInputStateMgr_GetState(P(args,4));P(w,0x1c)=args;B(w,0x431)=B(P(args,0x20),0)%6;B(w,0x430)=B(P(P(w,0x1c),0x20),0)/6;profile=Save_PlayerData_GetProfile(P(args,0));unlocked=SafariZone_GetObjectUnlockLevel(P(w,0x20));trainer=PlayerProfile_GetTrainerID(profile);gender=(u8)PlayerProfile_GetTrainerGender(profile);P(w,0x334)=ov108_021EA63C(trainer,unlocked,gender,w+0x42d,U(w,0));B(w,0x1a)=B(args,0x19);if(B(w,0x1a)>5)B(w,0x1a)=0;{u8*area=w+B(w,0x1a)*122;B(w,0x435)=30-B(area,0x25);}B(w,0x42e)=B(w,0x42d)/6;if(B(w,0x42d)%6>0)B(w,0x42e)++;if(B(w,0x430)>=B(w,0x42e))B(w,0x430)=0;H(P(w,0x1c),0x1c)=255;B(w,0x42c)=GF_RTC_GetTimeOfDayByHour(U(Save_SysInfo_RTC_Get(P(args,0)),0x14));if(B(w,0x42c)==4)B(w,0x42c)=3;}
