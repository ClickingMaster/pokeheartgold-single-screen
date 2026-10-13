#ifndef POKEHEARTGOLD_BATTLE_LINK_COMM_H
#define POKEHEARTGOLD_BATTLE_LINK_COMM_H
#include "global.h"

#include "battle/battle_setup.h"
#include "battle/battle_system.h"

#include "heap.h"
#include "string_util.h"
#include "sys_task.h"
#include "communication_command.h"
#include "communication_core.h"
#include "link_synchronization.h"
typedef struct BattleLinkWork {
    BattleSetup *setup;
    u8 pad4[12];
    SavePalPad *friends[4];
    union {
        u32 words[1024];
        Trainer trainer;
        SOUND_CHATOT chatot;
        SavePalPad palPad;
    } buffer;
    u8 received;
} BattleLinkWork;
typedef struct BattleLinkTask {
    BattleSystem *battle;
    u8 state;
} BattleLinkTask;
typedef struct BattleLinkHeader {
    u8 command, battler;
    u16 size;
} BattleLinkHeader;
extern const u32 gBattleLinkCommands[];
int Comm_GetSendBufferFreeSpace(void);
BOOL Comm_SendCommandNoCopy(int command, void *data, int size);
void BattleRecord_SetAiFlags(int sender, u32 flags);
BOOL BattleController_RecvLinkData(BattleSystem *battle, u8 *data);
void BattleLink_Start(BattleSystem *battle);
void BattleLink_RegisterSetupCommands(BattleLinkWork *work);
u32 BattleLink_GetAiFlagsSize(void);
u32 BattleLink_GetProfileSize(void);
u32 BattleLink_GetPartySize(void);
u32 BattleLink_GetChatotSize(void);
u32 BattleLink_GetTrainerSize(void);
void *BattleLink_GetProfileRecvBuffer(int sender, BattleLinkWork *work);
void *BattleLink_GetTrainerRecvBuffer(int sender, BattleLinkWork *work);
void *BattleLink_GetPartyRecvBuffer(int sender, BattleLinkWork *work);
void *BattleLink_GetChatotRecvBuffer(int sender, BattleLinkWork *work);
void *BattleLink_GetTrainer1RecvBuffer(int sender, BattleLinkWork *work);
void *BattleLink_GetTrainer3RecvBuffer(int sender, BattleLinkWork *work);
void *BattleLink_GetParty1RecvBuffer(int sender, BattleLinkWork *work);
void *BattleLink_GetParty3RecvBuffer(int sender, BattleLinkWork *work);
void *BattleLink_GetPalPadRecvBuffer(int sender, BattleLinkWork *work);
void BattleLink_QueueSendData(BattleSystem *battle, int command, int battler, u8 *data, u8 size);
void BattleLink_RecvData(int sender, int size, u8 *data, BattleSystem *battle);
BOOL BattleLink_SendAiFlags(BattleLinkWork *work, u32 value, ...);
void BattleLink_RecvAiFlags(int sender, int unused, u32 *data, BattleLinkWork *work);
BOOL BattleLink_PrepareProfile(BattleLinkWork *work);
BOOL BattleLink_SendProfile(BattleLinkWork *work);
void BattleLink_RecvProfile(int sender, int unused, void *data, BattleLinkWork *work);
BOOL BattleLink_PrepareTrainer(BattleLinkWork *work);
BOOL BattleLink_SendTrainer(BattleLinkWork *work);
void BattleLink_RecvTrainer(int sender, int unused, void *data, BattleLinkWork *work);
BOOL BattleLink_PrepareParty(BattleLinkWork *work);
BOOL BattleLink_SendParty(BattleLinkWork *work);
void BattleLink_RecvParty(int sender, int unused, void *data, BattleLinkWork *work);
BOOL BattleLink_PrepareChatot(BattleLinkWork *work);
BOOL BattleLink_SendChatot(BattleLinkWork *work);
BOOL BattleLink_PreparePalPad(BattleLinkWork *work);
BOOL BattleLink_SendPalPad(BattleLinkWork *work);
void BattleLink_RecvChatot(int sender, int unused, void *data, BattleLinkWork *work);
BOOL BattleLink_PreparePartnerTrainer(BattleLinkWork *work, int index);
BOOL BattleLink_SendPartnerTrainer(BattleLinkWork *work, int index, int command);
void BattleLink_RecvPartnerTrainer(int sender, int unused, void *data, BattleLinkWork *work);
BOOL BattleLink_PreparePartnerParty(BattleLinkWork *work, int index);
BOOL BattleLink_SendPartnerParty(BattleLinkWork *work, int index, int command);
void BattleLink_RecvPartnerParty(int sender, int unused, void *data, BattleLinkWork *work);
void BattleLink_SendTask(SysTask *task, void *ptr);
void BattleLink_RecvTask(SysTask *task, void *ptr);
void BattleLink_RecvEnd(int sender, int unused, void *data, BattleSystem *battle);
void BattleLink_BuildPalPad(PlayerProfile *profile, SavePalPad *friends, SavePalPad *out);
void BattleLink_RecvPalPad(int sender, int unused, SavePalPad *data, BattleLinkWork *work);
u32 BattleLink_GetPalPadSize(void);

#endif
