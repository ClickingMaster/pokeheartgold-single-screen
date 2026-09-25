#ifndef TO59_BANK_W16_PRIVATE_H
#define TO59_BANK_W16_PRIVATE_H
#include "global.h"

typedef struct Work {
    void *context;
    u8 pad04[4];
    s8 scale;
    u8 pad09[7];
    int mode10;
    int mode14;
    int mode18;
    u8 pad1C[8];
    int indexA;
    int indexB;
    VecFx32 value;
} Work;

typedef struct Param { int flag; VecFx32 value; } Param;
void ov07_02220978(void *, Work *);
void ov07_0221F9A8(void *, void *, int);
int ov07_02221664(Work *);
void ov07_022216A8(Work *, int, VecFx32 *);
void ov07_02221734(Work *, VecFx32 *);
void ov07_02220938(void *, Work *, int);
void ov07_022208F8(void *, Work *, int);
int ov07_02231924(void *, int);
int ov07_0221BFC0(void *);
#define DECL(n) void n(void *, int, VecFx32 *);
DECL(ov07_02231B90) DECL(ov07_02231BC0) DECL(ov07_02231BF0)
DECL(ov07_02231C20) DECL(ov07_02231C50) DECL(ov07_02231C80)
DECL(ov07_02231CB0) DECL(ov07_02231CE0) DECL(ov07_02231D10)
DECL(ov07_02231D40) DECL(ov07_02231DA0)
extern const VecFx32 ov07_022354E0[4];
extern const VecFx32 ov07_02235578[];
extern const VecFx32 ov07_02235540[];
#endif
