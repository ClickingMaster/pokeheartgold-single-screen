#ifndef POKEHEARTGOLD_UNK_02091564_H
#define POKEHEARTGOLD_UNK_02091564_H

#include "global.h"

#include "overlay_manager.h"

extern const OverlayManagerTemplate gApplication_OakSpeech;

typedef void (*UnkFuncPtr_02091564)();
typedef u32 (*UnkFuncPtr_02091564_U32)(void);

typedef struct FieldCommCommand {
    UnkFuncPtr_02091564 func1;
    UnkFuncPtr_02091564_U32 func2;
    UnkFuncPtr_02091564 func3;
} FieldCommCommand;

void FieldComm_RegisterCommands(void *arg0);
u32 CommCmd_GetTrainerCardSize();
u32 sub_02091590();
u32 sub_02091598();
u32 sub_02091564();
u32 sub_02091568();
void CommCmd_RecvIgnored();
u32 sub_020915A0();
u32 sub_020915A4();
u32 sub_020915A8();
u32 sub_020915AC();

#endif // POKEHEARTGOLD_UNK_02091564_H
