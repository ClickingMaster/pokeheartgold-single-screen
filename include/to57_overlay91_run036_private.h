#ifndef TO57_NEW54_PRIVATE_H
#define TO57_NEW54_PRIVATE_H
#include "global.h"
void ov91_0225E8BC(void *, void *);
void ov91_0225E9F4(void *, void *);
void ov91_0225E67C(u8 *, u8 *);
void ov91_0225E40C(u8 *);
void ov91_0225E62C(u8 *);
void ov91_0225E648(u8 *);

typedef struct Ov91R36Work {
    u8 state;
    u8 pad01;
    u16 counter;
    u8 pad04[0x1c];
    u32 active;
    u8 pad24[8];
    VecFx32 position;
    s16 timer;
    u16 kind;
    u8 pad3C[4];
    fx32 magnitude;
    u8 pad44[4];
    u32 frames;
} Ov91R36Work;
void ov91_0225E728(Ov91R36Work *, VecFx32 *);
int ov91_0225DE8C(void *, VecFx32 *, const VecFx32 *, VecFx32 *);
void ov91_0225EA54(Ov91R36Work *, const VecFx32 *);
int ov91_0225D600(void *);
void ov91_0225E9E4(Ov91R36Work *, void *);
void ov91_0225D694(void *, int, int, void *);
void ov91_02261808(void *, void *, int);
void ov91_022618B0(void *, void *, int);
int ov91_0225DFB0(void *, VecFx32 *);
BOOL ov91_0225E990(Ov91R36Work *);
BOOL ov91_0225E9C0(Ov91R36Work *);
void ov91_0225E3F4(void *, Ov91R36Work *, int, int);
void ov91_0225E7C4(Ov91R36Work *, void *, int);
void ov91_0225E7E8(Ov91R36Work *, void *, int, fx32, int);
void ov91_02261890(void *);
void PlaySE(u16);
void ov91_0225E45C(void *, Ov91R36Work *);
#endif
