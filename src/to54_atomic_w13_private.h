#ifndef TO54_W13_PRIVATE_H
#define TO54_W13_PRIVATE_H

#include "global.h"

#include "camera.h"
#include "spl.h"

typedef struct Ov07Base {
    u32 unk00;
    VecFx32 position;
} Ov07Base;

typedef struct Ov07CallbackPosition {
    u8 pad00[0x20];
    Ov07Base **base;
    u32 unk24;
    VecFx32 position;
} Ov07CallbackPosition;

void *sub_02015504(void);
void sub_020154E4(void *, const VecFx32 *);
int sub_02015530(void *);
Camera *sub_02015524(SPLEmitter *);
void sub_02015708(SPLEmitter *, VecFx32 *);
void sub_02015720(SPLEmitter *, VecFx32 *);
void sub_02015628(SPLEmitter *, VecFx32 *);
void sub_02015640(SPLEmitter *, VecFx32 *);

int ov07_0221C468(void *);
int ov07_0221C470(void *);
SPLEmitter *ov07_0221C478(void *);
void *ov07_0221BFC0(void *);
int ov07_02231924(void *, int);
int ov07_0223192C(void *, int);
int ov07_022319E0(int);
void ov07_02231AD0(int, VecFx32 *, void *, int);
void ov07_02231B90(void *, int, VecFx32 *);
void ov07_02231DE8(int, VecFx32 *);
void ov07_0221F9A8(void *, void *, int);
int ov07_0221FF34(void *, int, int);

void ov07_0222036C(void *, SPLEmitter *, int, int, int, s8, VecFx32 *);
void ov07_0222043C(void *, SPLEmitter *, int, int, int, s8, VecFx32 *);
void ov07_0222050C(void *, void *, int, int, int, int);
void ov07_02220030(Ov07CallbackPosition *);
void ov07_0222008C(Ov07CallbackPosition *);
void ov07_022200D0(Ov07CallbackPosition *);
void ov07_02220114(Ov07CallbackPosition *);
void ov07_02220160(Ov07CallbackPosition *);
void ov07_022201C8(Ov07CallbackPosition *);
void ov07_022202D8(int, int, VecFx32 *);
void ov07_022207D0(void *, int, int);
void ov07_0221FF78(Ov07CallbackPosition *);

#endif
