#include "to54_atomic_w14_overlay07_callbacks_w14_private.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define U32_AT(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct DrawTaskData {
    void *owner;
    void *manager;
    u32 unused8;
    SysTask *task;
} DrawTaskData;

typedef struct Counts24 {
    u32 v[6];
} Counts24;
extern const Counts24 ov07_02234BF0;

void ov07_0221D718(void *p) {
    void *pc = PTR_AT(p, 0x18);
    pc = (u8 *)pc + 4;
    pc = (u8 *)pc + 4;
    PTR_AT(p, 0x18) = pc;
    MI_CpuFill8(BgGetCharPtr(2), 0, 0x1900);
    ov07_0221D55C(p, 4);
}

void ov07_0221D740(void *p) {
    Counts24 counts = ov07_02234BF0;
    U32_AT(p, 0x18) += 4;
    PTR_AT(p, 0x138) = SpriteManager_New(PTR_AT(PTR_AT(p, 0xc0), 0xac));
    SpriteSystem_InitSprites(PTR_AT(PTR_AT(p, 0xc0), 0xac), PTR_AT(p, 0x138), 5);
    G2dRenderer_SetSubSurfaceCoords(SpriteSystem_GetRenderer(PTR_AT(PTR_AT(p, 0xc0), 0xac)), 0, 0x110000);
    SpriteSystem_InitManagerWithCapacities(PTR_AT(PTR_AT(p, 0xc0), 0xac), PTR_AT(p, 0x138), &counts);
}

void ov07_0221D7B8(void *p) {
    u32 index;
    u32 resourceId;
    U32_AT(p, 0x18) += 4;
    index = U32_AT(PTR_AT(p, 0x18), 0);
    U32_AT(p, 0x18) += 4;
    resourceId = index + 0x4e21 + U16_AT(PTR_AT(p, 0xc0), 0x14) * 0x1388;

    SpriteSystem_LoadCharResObjFromOpenNarc(PTR_AT(PTR_AT(p, 0xc0), 0xac), PTR_AT(p, 0x138), PTR_AT(p, 0x1b4), 0x4c, 0, 1, resourceId);
    SpriteSystem_LoadPaletteBufferFromOpenNarc(PTR_AT(p, 0xc8), 2, PTR_AT(PTR_AT(p, 0xc0), 0xac), PTR_AT(p, 0x138), PTR_AT(p, 0x1b4), 0x4b, 0, 1, 1, resourceId);
    SpriteSystem_LoadCellResObjFromOpenNarc(PTR_AT(PTR_AT(p, 0xc0), 0xac), PTR_AT(p, 0x138), PTR_AT(p, 0x1b4), 0x4d, 0, resourceId);
    SpriteSystem_LoadAnimResObjFromOpenNarc(PTR_AT(PTR_AT(p, 0xc0), 0xac), PTR_AT(p, 0x138), PTR_AT(p, 0x1b4), 0x4e, 0, resourceId);
}
