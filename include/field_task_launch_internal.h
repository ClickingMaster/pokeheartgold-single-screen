#ifndef TO40_UNK_02059B64_PRIVATE_H
#define TO40_UNK_02059B64_PRIVATE_H
#include "global.h"

#include "field_system.h"
#include "task.h"
typedef struct TrainerCardTaskEnv {
    u8 pad00[0x24];
    u32 player;
    u32 state;
    u8 pad2C[0x67C];
} TrainerCardTaskEnv;
u32 Comm_GetLocalNetId(void);
int CommPlayerManager_GetPlayerFrontX(void);
int CommPlayerManager_GetPlayerFrontZ(int player);
int Comm_CountConnected(void);
int CommPlayerManager_GetPlayerX(int player);
int CommPlayerManager_GetPlayerZ(int player);
void sub_0203E2F4(void);
void sub_02059D44(FieldSystem *fieldSystem);
BOOL sub_02059B64(TaskManager *taskManager);
#endif
