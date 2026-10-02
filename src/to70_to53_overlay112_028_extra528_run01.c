#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x20];void *save;u8 bytes[0x1e440-0x24+4];} Work;
extern void *Save_PlayerData_GetProfile(void *);extern void *PlayerProfile_GetPlayerName_NewString(void *,int);extern u32 PlayerProfile_GetTrainerID(void *);extern void CopyStringToU16Array(void *,u16 *,u32);extern void *sub_02032728(void *);extern void ov112_021E73C8(void *,int,int);extern void ov112_021E5EB8(void *,void *);extern void ov112_021E5D68(void *);extern void String_Delete(void *);
void ov112_021E7CC8(Work *);
void ov112_021E7CC8(Work *w){u8 *p=(u8 *)w;void *name=PlayerProfile_GetPlayerName_NewString(Save_PlayerData_GetProfile(w->save),154);*(int *)(p+0x1024)=1;*(u16 *)(p+0x102c)=GAME_VERSION;p[0x107f]&=~1;p[0x107e]=0;*(u32 *)(p+0x1030)=PlayerProfile_GetTrainerID(*(void **)(p+0x1e438));CopyStringToU16Array(name,(u16 *)(p+0x106c),8);*(int *)(p+0x1028)=1;*(u16 *)(p+0x102e)=GAME_VERSION;MI_CpuCopyFast(sub_02032728(*(void **)(p+0x1e440)),p+0x1034,40);ov112_021E73C8(p+0x24,0x1000,15);ov112_021E5EB8(p+0x1024,p+0x108c);ov112_021E5D68(p+0x10834);String_Delete(name);}
