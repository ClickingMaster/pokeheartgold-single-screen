#ifndef POKEHEARTGOLD_DRAWING_SESSION_LINK_INTERNAL_H
#define POKEHEARTGOLD_DRAWING_SESSION_LINK_INTERNAL_H

#include "global.h"

#include "communication_command.h"
#include "drawing_session.h"
#include "field_comm_cmds.h"

typedef struct FourBytes {
    u8 data[4];
} FourBytes;

int Comm_GetLocalNetId(void);
void Comm_SendParentCommand(int, const void *, int);
int Comm_CountConnected(void);
int Wireless_GetConnectedBitmap(void);
int MATH_CountPopulation(u32);
void CommState_SetMaxEntries(int);
void GF_AssertFail(void);
void CommState_SetErrorWatch(int, int);
void Comm_SendParentCommandNoCopy(int, const void *, int);

void DrawingSessionLink_RegisterCommands(void *arg);
void DrawingSessionLink_SendPictureChunk(DrawingSessionWork *work, int index);

#endif
