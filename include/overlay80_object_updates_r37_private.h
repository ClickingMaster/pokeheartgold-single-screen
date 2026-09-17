#ifndef TO45_OV80_0222BDF4_R37_PRIVATE_H
#define TO45_OV80_0222BDF4_R37_PRIVATE_H

#include "global.h"

typedef struct Ov80Position {
    s16 x;
    s16 y;
} Ov80Position;
typedef struct Ov80PositionCommand {
    u16 x;
    u16 y;
    u16 sequence;
    u8 direction;
    u8 index;
} Ov80PositionCommand;
typedef struct Ov80ManagedSprite {
    void *sprite;
} Ov80ManagedSprite;
typedef struct Ov80ManagedSpriteTemplate {
    s16 x;
    s16 y;
    s16 z;
    u16 animation;
    int drawPriority;
    int pal;
    int vram;
    int resIdList[6];
    int bgPriority;
    int vramTransfer;
} Ov80ManagedSpriteTemplate;
typedef struct Ov80FrontierMap {
    void *bgConfig;
    void *plttData;
    u8 padding08[0x34 - 0x08];
    void *spriteSystem;
    void *spriteManager;
    u8 padding3C[0x80 - 0x3C];
    Ov80ManagedSprite *sprites[4];
} Ov80FrontierMap;

extern const u16 _0223DC20[];
extern const u16 ov80_0223DC22[];
extern const Ov80ManagedSpriteTemplate ov80_0223BD80;
u32 ov42_022282DC(void *object);
u32 ov42_022282E8(void *object);
void GF_AssertFail(void);
void Sprite_DeleteAndFreeResources(Ov80ManagedSprite *sprite);
BOOL SpriteManager_UnloadCharObjById(void *manager, u32 resourceId);
BOOL SpriteManager_UnloadPlttObjById(void *manager, u32 resourceId);
BOOL SpriteManager_UnloadCellObjById(void *manager, u32 resourceId);
BOOL SpriteManager_UnloadAnimObjById(void *manager, u32 resourceId);
u32 sub_02074490(void);
u32 sub_02074498(void);
u32 sub_020744A4(void);
u8 SpriteSystem_LoadPaletteBuffer(void *, int, void *, void *, int, int, BOOL, int, int, int);
BOOL SpriteSystem_LoadCellResObj(void *, void *, int, int, BOOL, int);
BOOL SpriteSystem_LoadAnimResObj(void *, void *, int, int, BOOL, int);
BOOL SpriteSystem_LoadCharResObjAtEndWithHardwareMappingType(void *, void *, int, int, BOOL, int, int);
Ov80ManagedSprite *SpriteSystem_NewSprite(void *, void *, const Ov80ManagedSpriteTemplate *);
u32 Pokemon_GetIconNaix(void *mon);
u32 Pokemon_GetIconPalette(void *mon);
void Sprite_SetPalOffsetRespectVramOffset(void *sprite, int palette);
void ManagedSprite_TickFrame(Ov80ManagedSprite *sprite);
void ov80_0222EFD0(Ov80PositionCommand *, void *, u16, s32);
void ov80_0222F1D0(Ov80FrontierMap *, u32);
void ov80_0222F210(Ov80FrontierMap *);
void ov80_0222F278(Ov80FrontierMap *);
Ov80ManagedSprite *ov80_0222F29C(Ov80FrontierMap *, void *, s32, s32, s32);

#define PLTTBUF_MAIN_OBJ                2
#define NARC_poketool_icongra_poke_icon 0x14
#define NNS_G2D_VRAM_TYPE_2DMAIN        1
#define GF_GFX_RES_TYPE_CHAR            0
#endif
typedef struct SysTask SysTask;
extern const u8 ov80_0223BD44[];
BOOL ov42_02228188(void *, u32);
void ov42_02229200(void *, u32);
void ov42_022299C0(void *, void *);
void Heap_Free(void *);
void SysTask_Destroy(SysTask *);
void ov80_0222EEC8(SysTask *, void *);
#define R8(p, o)   (*(u8 *)((u8 *)(p) + (o)))
#define R16(p, o)  (*(u16 *)((u8 *)(p) + (o)))
#define RS16(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define RP(p, o)   (*(void **)((u8 *)(p) + (o)))
