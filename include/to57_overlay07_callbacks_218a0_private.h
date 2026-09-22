#ifndef TO57_R3_12_PRIVATE_H
#define TO57_R3_12_PRIVATE_H
#include "global.h"

#include "spl.h"

typedef struct Ov07ParticleParam {
    int mode;
    int forcePositive;
    VecFx32 value;
} Ov07ParticleParam;

typedef struct Ov07ParticleCallbackWork {
    void *context;
    u8 pad04[4];
    s8 scale;
    u8 pad09[0x1b];
    int indexA;
    int indexB;
    fx32 baseX;
    fx32 baseY;
} Ov07ParticleCallbackWork;

void ov07_022217A4(Ov07ParticleCallbackWork *, Ov07ParticleParam *);
int ov07_02221664(Ov07ParticleCallbackWork *);
void ov07_02231B90(void *, int, VecFx32 *);
void sub_02015628(SPLEmitter *, VecFx32 *);
void sub_02015640(SPLEmitter *, VecFx32 *);
void sub_02015674(SPLEmitter *, s16 *);
void sub_0201568C(SPLEmitter *, s16 *);
void sub_020156A8(SPLEmitter *, u16 *);
void sub_020156BC(SPLEmitter *, u16 *);
void sub_020156D8(SPLEmitter *, u16 *);
void sub_020156EC(SPLEmitter *, u16 *);
BOOL ov07_022218A0(SPLEmitter *, Ov07ParticleCallbackWork *);
BOOL ov07_02221968(SPLEmitter *, Ov07ParticleCallbackWork *);
BOOL ov07_022219EC(SPLEmitter *, Ov07ParticleCallbackWork *);
BOOL ov07_02221A6C(SPLEmitter *, Ov07ParticleCallbackWork *);
#endif
