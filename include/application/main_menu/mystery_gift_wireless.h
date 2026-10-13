#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_WIRELESS_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_WIRELESS_H

#include "sys_task.h"

typedef void (*MysteryGiftCommStateFunc)(void);

void MysteryGiftComm_Start(void *owner);
void MysteryGiftComm_SetState(MysteryGiftCommStateFunc func, u16 arg);
void MysteryGiftComm_ParentWaitStartup(void);
void MysteryGiftComm_ParentCollectProfiles(void);
void MysteryGiftComm_ChildWaitStartup(void);
void MysteryGiftComm_ChildStateIdle(void);
void MysteryGiftComm_ChildConnect(void);
void MysteryGiftComm_ChildWaitLinked(void);
void MysteryGiftComm_ChildStateLinked(void);
void MysteryGiftComm_Task(SysTask *task, void *data);
void MysteryGiftComm_StartParent(void *owner, void *a1, void *a2);
int ov74_02229D0C(void *a0, void *a1);
void MysteryGiftComm_StartChild(void *owner, int a1);
void MysteryGiftComm_ConnectToChannel(int channel);
int MysteryGiftComm_GetFirstParentInfo(void *dst);
void MysteryGiftComm_HandleReceived(u8 channel);
u8 MysteryGiftComm_IsReceived(void);
int MysteryGiftComm_GetRecvBufferSize(void);
void *MysteryGiftComm_GetRecvBuffer(int netId, void *arg, int size);
void ov74_02229DE0(int i);
void MysteryGiftComm_Free(void);
void MysteryGiftComm_RegisterCommands(void *owner);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_WIRELESS_H
