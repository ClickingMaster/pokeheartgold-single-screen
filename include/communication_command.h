#ifndef POKEHEARTGOLD_COMMUNICATION_COMMAND_H
#define POKEHEARTGOLD_COMMUNICATION_COMMAND_H

#include "global.h"

BOOL sub_0203401C(int);
void Comm_RegisterCommandTable(const void *, int, void *arg2);
u32 CommCmd_GetSize1(void);
u32 CommCmd_GetSize0(void);
u32 CommCmd_GetSizeVariable(void);
u32 CommPlayerManager_GetHostPositionSize(void);

#endif // POKEHEARTGOLD_COMMUNICATION_COMMAND_H
