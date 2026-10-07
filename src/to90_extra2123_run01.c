#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct {s16 variables[16];u32 pad;} Player;
typedef struct {u8 pad[32];Player players[16];} Work;
extern Work*SNDi_SharedWork;s16 SND_GetPlayerLocalVariable(u32,u32);
s16 SND_GetPlayerLocalVariable(u32 player,u32 index){DC_InvalidateRange(&SNDi_SharedWork->players[player].variables[index],2);return SNDi_SharedWork->players[player].variables[index];}
