#ifndef POKEHEARTGOLD_FIELD_COMM_CMDS_H
#define POKEHEARTGOLD_FIELD_COMM_CMDS_H

#include "global.h"

#include "overlay_manager.h"

extern const OverlayManagerTemplate gApplication_OakSpeech;

typedef void (*FieldCommHandlerFunc)();
typedef u32 (*FieldCommSizeFunc)(void);

typedef struct FieldCommCommand {
    FieldCommHandlerFunc func1;
    FieldCommSizeFunc func2;
    FieldCommHandlerFunc func3;
} FieldCommCommand;

void FieldComm_RegisterCommands(void *context);
u32 CommCmd_GetTrainerCardSize();
u32 CommCmd_GetSize3008();
u32 CommCmd_GetSize1008();
u32 CommCmd_GetSize24();
u32 CommCmd_GetSize1424();
void CommCmd_RecvIgnored();
u32 CommCmd_GetSize10();
u32 CommCmd_GetSize50();
u32 CommCmd_GetSize4();
u32 CommCmd_GetSize4Copy();

#endif // POKEHEARTGOLD_FIELD_COMM_CMDS_H
