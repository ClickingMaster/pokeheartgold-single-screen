#ifndef POKEHEARTGOLD_OV115_TRANSITION_HELPERS_H
#define POKEHEARTGOLD_OV115_TRANSITION_HELPERS_H

#include "global.h"

typedef struct Ov115Motion { u8 raw[0x14]; } Ov115Motion;
typedef struct Ov115Work {
    s16 delay;
    s16 count;
    void *sprites[4];
    Ov115Motion motion[4];
} Ov115Work;
struct FieldSystem;
struct MessageFormat;

void ov115_0225F020(struct Ov115Work *work, void *manager, void *resource, fx32 x, fx32 y, int heapID);
void *ov115_0225F158(u32 trainerId, int heapID);
int ov115_0225F968(struct FieldSystem *fieldSystem);
void BufferTrainerName(struct MessageFormat *messageFormat, u32 slot, u32 trainerId);

#endif
