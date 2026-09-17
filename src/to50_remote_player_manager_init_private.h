#ifndef TO50_REMOTE_PLAYER_MANAGER_INIT_PRIVATE_H
#define TO50_REMOTE_PLAYER_MANAGER_INIT_PRIVATE_H
#include "global.h"

#include "sys_task_api.h"
typedef struct RemotePlayerSlot {
    u16 value0;
    u16 value2;
    s8 value4;
    u8 value5;
    u8 pad6[2];
} RemotePlayerSlot;
typedef struct RemotePlayerManager {
    u8 pad00[0x2c];
    SysTask *task;
    void *fieldSystem;
    RemotePlayerSlot slots1[8];
    RemotePlayerSlot slots2[8];
    u8 states1[8];
    u8 states2[8];
    u8 padC4[0x29];
    u8 valueED;
    u8 valueEE;
    u8 valueEF;
    u8 padF0[4];
} RemotePlayerManager;
extern RemotePlayerManager *_021D41C4;
extern const u16 _020FC7A0[4];
extern const u16 _020FC7A8[8];
extern u16 _0210FAB4[4];
extern u16 _0210FABC[4];
extern u16 _0210FAC4[4];
void sub_020572DC(SysTask *task, void *fieldSystem);
void sub_020374E4(void);
void sub_02056EF4(void);
BOOL sub_02056D7C(RemotePlayerManager *data, void *fieldSystem, int unused);
#endif
