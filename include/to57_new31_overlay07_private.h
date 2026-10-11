#ifndef NEW31_PRIVATE_H
#define NEW31_PRIVATE_H
#include "global.h"
typedef struct {
    s32 x, y, z;
} V3;
typedef struct {
    u8 pad00[0x18];
    s32 current;
    void *actor[16];
    void *out[16];
    u8 pad9C[0x10];
    void *spriteSystem;
} Ov07Inner;
typedef struct {
    u32 a;
    u8 b, c;
    u8 pad6[2];
    u32 d;
} Mid;
typedef struct {
    u8 pad00[0x10];
    u32 word10;
    u8 pad14[4];
    u32 *script;
    u32 words1C[3];
    Mid mids[3];
    u8 pad4C[0x20];
    u8 kind[16];
    u8 flag[16];
    u8 pad8C;
    u8 state8D;
    u16 half8E;
    u16 half90;
    u8 pad92[0x2E];
    Ov07Inner *inner;
    void *bg;
    u8 padC8[4];
    void *managers[4];
    u8 padDC[0xA1];
    u8 timer17D;
    u8 phase17E;
    u8 pad17F[0x2D];
    u8 prio0;
    u8 prio1;
    u8 prio2;
    u8 prio3;
} Ov07Ctx;
typedef struct {
    u32 v[6][6];
} Tbl36;
extern const Tbl36 ov07_02234CC8;
extern const Tbl36 ov07_02234D58;
int ParticleSystem_GetActiveEmitterCount(void *);
void ParticleSystem_GetCameraUp(void *, V3 *);
void ParticleSystem_SetCameraUp(void *, V3 *);
void ParticleSystem_SetPerspectiveType(void *, int);
void *ov07_0221FF18(void *, u32, u32, Ov07Ctx *);
int ov07_0221C468(Ov07Ctx *);
int ov07_0221C470(Ov07Ctx *);
int BattleAnimUtil_GetBattlerType(Ov07Ctx *, int);
void GF_AssertFail(void);
void ov07_0221C9C0(Ov07Ctx *);
void ov07_0221CBC0(Ov07Ctx *);
void ov07_0221CC54(Ov07Ctx *);
int ov07_0221CCF4(Ov07Ctx *);
int ov07_0221CD4C(Ov07Ctx *);
void ov07_0221CDA4(Ov07Ctx *);
void ov07_0221CE5C(Ov07Ctx *);
int GF_IsAnySEPlaying(void);
void SpriteSystem_FreeResourcesAndManager(void *, void *);
void ov07_0221D55C(Ov07Ctx *, int);
void ov07_0221FF2C(void *);
void ov07_0221DD14(void *, int);
void BattleSystem_SetDefaultBlend(void);
int ov07_0221FAEC(Ov07Ctx *, int);
int ov07_0221BFD0(Ov07Ctx *);
void BG_ClearCharDataRange(int, int, int, int);
void *ov07_0221C4A0(Ov07Ctx *);
void BgClearTilemapBufferAndCommit(void *, int);
void ToggleBgLayer(int, int);
void SetBgPriority(int, int);
void BgSetPosTextAndCommit(void *, int, int, int);
#endif
