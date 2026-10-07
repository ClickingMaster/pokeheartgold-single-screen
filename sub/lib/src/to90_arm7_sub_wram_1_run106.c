#pragma force_active on
#include <nitro/types.h>
#include <nitro/os/message.h>
#include <nitro/os/alarm.h>
#include <nitro/os/tick.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
extern int sMasterPan,sSurroundDecay;
extern u8 sOrgPan[],sOrgVolume[];
extern int CalcSurroundDecay(int,int);
#define R8(c,o) (*(volatile u8 *)(0x04000000u+((u32)(c)<<4)+(o)))
#define R16(c,o) (*(volatile u16 *)(0x04000000u+((u32)(c)<<4)+(o)))
#define R32(c,o) (*(volatile u32 *)(0x04000000u+((u32)(c)<<4)+(o)))
extern OSMessageQueue sndMesgQueue;extern OSMessage sndMesgBuffer[];extern OSAlarm sndAlarm;

void SND_Enable(void);void SND_SetOutputSelector(u32,u32,u32,u32);void SND_SetMasterVolume(s32);void SndAlarmCallback(void*);
void SND_SeqMain(int);
u16 SND_CalcRandom(void);
void SndThread(void*unused);

extern void SND_AlarmInit(void);
extern void SND_CommandProc(void);
extern void SND_ExChannelInit(void);
extern void SND_ExChannelMain(int update);
extern void SND_SeqInit(void);
extern void SND_UpdateExChannel(void);
extern void SND_UpdateSharedWork(void);
// @fn SndThread
void SndThread(void*unused){OSMessage message;OS_InitMessageQueue(&sndMesgQueue,sndMesgBuffer,8);OS_CreateAlarm(&sndAlarm);SND_ExChannelInit();SND_SeqInit();SND_AlarmInit();SND_Enable();SND_SetOutputSelector(0,0,0,0);SND_SetMasterVolume(127);OS_SetPeriodicAlarm(&sndAlarm,OS_GetTick()+65536,2728,SndAlarmCallback,0);for(;;){int tick=0;OS_ReceiveMessage(&sndMesgQueue,&message,1);switch((u32)message){case 1:tick=1;break;case 2:break;}SND_UpdateExChannel();SND_CommandProc();SND_SeqMain(tick);SND_ExChannelMain(tick);SND_UpdateSharedWork();SND_CalcRandom();}}
