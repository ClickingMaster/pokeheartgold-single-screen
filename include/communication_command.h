#ifndef POKEHEARTGOLD_COMMUNICATION_COMMAND_H
#define POKEHEARTGOLD_COMMUNICATION_COMMAND_H

#include "global.h"

typedef void (*CommCmdRecvFunc)(int netId, int size, void *data, void *context);
typedef u32 (*CommCmdSizeFunc)(void);
typedef void *(*CommCmdBufferFunc)(int netId, void *context, int size);

typedef struct CommCommand {
    CommCmdRecvFunc recv;
    CommCmdSizeFunc getSize;
    CommCmdBufferFunc getBuffer;
} CommCommand;

BOOL Comm_IsUnionRoomLinkMode(int);
void Comm_RegisterCommandTable(const void *, int, void *context);
void Comm_CallCommandRecv(int netId, int command, int size, void *data);
u32 Comm_GetCommandSize(int command);
void *Comm_GetCommandBuffer(int command, int netId, int size);
u32 CommCmd_GetSize1(void);
u32 CommCmd_GetSize0(void);
u32 CommCmd_GetSizeVariable(void);
u32 CommPlayerManager_GetHostPositionSize(void);

#endif // POKEHEARTGOLD_COMMUNICATION_COMMAND_H
