#ifndef POKEHEARTGOLD_TO50_COMMUNICATION_PACKET_BUILDERS_PRIVATE_H
#define POKEHEARTGOLD_TO50_COMMUNICATION_PACKET_BUILDERS_PRIVATE_H

#include "global.h"

typedef struct CommManager {
    u8 initialized;
    u8 updateLock;
    u8 pad02[2];
    void *allocation;
    u8 *work;
} CommManager;

typedef struct ReceiveBuffer {
    u8 *dest;
    int remaining;
} ReceiveBuffer;

extern CommManager _021D4140;

int sub_0203993C(void);
int sub_0203772C(u16);
int sub_02033FC4(u16);
int sub_02035FF0(void);
int sub_02036DA0(u8 *);
int sub_02033C50(void *);
int sub_02033ECC(void *, ReceiveBuffer *, int);
u16 sub_02033250(void);
int sub_02036E60(u8 *);
void sub_02036F30(u8 *);

#endif
