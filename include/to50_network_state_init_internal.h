#ifndef POKEHEARTGOLD_TO50_NETWORK_STATE_INIT_INTERNAL_H
#define POKEHEARTGOLD_TO50_NETWORK_STATE_INIT_INTERNAL_H
#include "global.h"

#include "heap.h"
#include "mail_message.h"
#include "save_link_ruleset.h"
typedef struct NetworkState {
    u16 localId;
    u16 padding02;
    int state;
    u8 *work;
} NetworkState;
extern NetworkState _021D4134;
u32 sub_020335B4(void);
void sub_02034DF0(int value);
u16 WM_GetNextTgid(void);
void sub_02034B0C(int value, int mode);
#endif
