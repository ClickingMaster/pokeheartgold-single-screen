#ifndef POKEHEARTGOLD_UNK_020355DC_PRIVATE_H
#define POKEHEARTGOLD_UNK_020355DC_PRIVATE_H

#include "global.h"

typedef struct NetworkState355DC {
    u16 localId;
    u16 padding02;
    int state;
    u8 *work;
} NetworkState355DC;

extern NetworkState355DC _021D4134;
BOOL sub_020355DC(u16 index);
extern u16 Wireless_GetConnectedBitmap(void);
extern u32 Wireless_GetState(void);

#endif
