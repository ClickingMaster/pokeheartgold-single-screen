#include "global.h"

#pragma force_active on

typedef struct Ov08Ctx {
    u8 raw[0x2090];
} Ov08Ctx;
static inline void W8(Ov08Ctx *p, u32 o, u8 v) {
    p->raw[o] = v;
}
static inline u8 R8(Ov08Ctx *p, u32 o) {
    return p->raw[o];
}
static inline void *RP(Ov08Ctx *p, u32 o) {
    return *(void **)&p->raw[o];
}

extern int ov08_0221D4B0(Ov08Ctx *);
extern int ov08_0221D4F8(Ov08Ctx *);
extern int ov08_0221D540(Ov08Ctx *);
extern int ov08_0221D588(Ov08Ctx *);
extern int ov08_0221D614(Ov08Ctx *, int, int);
extern int ov08_0221D91C(void *);
extern int ov08_0221DAC4(Ov08Ctx *);
extern void ov08_022220AC(Ov08Ctx *, int);
extern void PlaySE(int);
int ov08_0221C318(Ov08Ctx *);
int ov08_0221C3C8(Ov08Ctx *);
int ov08_0221C488(Ov08Ctx *);
int ov08_0221C58C(Ov08Ctx *);

extern u32 PaletteData_GetSelectedBuffersBitmask(void *);
extern int ov08_0221D438(Ov08Ctx *);
extern int ov08_0221C1C8(Ov08Ctx *);
int ov08_0221C14C(Ov08Ctx *);

extern void *CreateSysTaskAndEnvironment(void *, u32, u32, u32);
extern void *SysTask_GetData(void *);
extern void *BattleSystem_GetBgConfig(void *);
extern void *BattleSystem_GetPaletteData(void *);
extern u8 ov12_0223AB0C(void *, int);
extern void *memset(void *, int, u32);
extern void ov08_0221BE98(void *, void *);
int ov08_0221BE20(void *);
int ov08_0221BE20(void *owner_) {
    typedef struct OwnerBE {
        u8 pad00[8];
        void *battleSystem;
        u32 heapId;
        u8 pad10;
        u8 battler;
        u8 pad12[0x16];
        int slot;
    } OwnerBE;
    typedef struct EnvBE {
        OwnerBE *owner;
        u8 pad04[0x1E0];
        void *bgConfig;
        void *palette;
        u8 pad1EC[0x1E8A];
        u8 battler;
        u8 low : 4;
        u8 high : 4;
        u8 state;
        u8 pad2079[0x16];
        u8 slot;
    } EnvBE;
    OwnerBE *owner = owner_;
    void *task;
    EnvBE *env;
    if (owner->battler > 5) {
        owner->battler = 0;
    }
    task = CreateSysTaskAndEnvironment(ov08_0221BE98, sizeof(EnvBE), 0, owner->heapId);
    env = SysTask_GetData(task);
    memset(env, 0, sizeof(EnvBE));
    env->owner = owner;
    env->bgConfig = BattleSystem_GetBgConfig(owner->battleSystem);
    env->palette = BattleSystem_GetPaletteData(owner->battleSystem);
    env->state = 0;
    env->battler = owner->battler;
    env->high = 0;
    env->slot = ov12_0223AB0C(owner->battleSystem, owner->slot);
}

extern void ov08_0221D184(Ov08Ctx *);
extern void ov08_0221CDF8(Ov08Ctx *);
extern void ov08_0221CF38(Ov08Ctx *);
extern void ov08_0221D0F4(Ov08Ctx *);
extern void FontID_Alloc(int, int);
extern void ov08_0221D8B0(Ov08Ctx *, int);
extern void ov08_022221CC(Ov08Ctx *, int);
extern void ov08_02222524(Ov08Ctx *, int);
extern void ov08_022205E0(Ov08Ctx *);
extern void ov08_02220C5C(Ov08Ctx *, int);
extern void ov08_0221DC00(Ov08Ctx *);
extern void ov08_0221DD70(Ov08Ctx *, int);
extern void BattleDpadCursor_SetActive(void *, int);
extern int ov08_0221DB24(void *, u32);
extern void ov08_0222171C(Ov08Ctx *, int);
extern void ov08_0221D6CC(Ov08Ctx *, int);
extern u8 PaletteData_BeginPaletteFade(void *, u16, u16, s8, u8, u8, u16);
extern void *BattleDpadCursor_New(int);
int ov08_0221C048(Ov08Ctx *);
