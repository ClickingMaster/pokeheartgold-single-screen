#ifndef POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_WIRELESS_H
#define POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_WIRELESS_H

#include "sys_task.h"

typedef void (*MysteryGiftCommStateFunc)(void);

void MysteryGiftComm_Start(void *owner);
void MysteryGiftComm_SetState(MysteryGiftCommStateFunc func, u16 arg);
void ov74_02229BD0(void);
void ov74_02229C0C(void);
void ov74_02229C44(void);
void ov74_02229C5C(void);
void ov74_02229C60(void);
void ov74_02229C80(void);
void ov74_02229CB4(void);
void MysteryGiftComm_Task(SysTask *task, void *data);
void ov74_02229CE0(void *owner, void *a1, void *a2);
int ov74_02229D0C(void *a0, void *a1);
void ov74_02229D1C(void *owner, int a1);
void ov74_02229D50(int channel);
int ov74_02229D6C(void *dst);
void MysteryGiftComm_HandleReceived(u8 channel);
u8 MysteryGiftComm_IsReceived(void);
int MysteryGiftComm_GetRecvBufferSize(void);
void *MysteryGiftComm_GetRecvBuffer(int netId, void *arg, int size);
void ov74_02229DE0(int i);
void MysteryGiftComm_Free(void);
void MysteryGiftComm_RegisterCommands(void *owner);

#endif // POKEHEARTGOLD_APPLICATION_MAIN_MENU_MYSTERY_GIFT_WIRELESS_H
