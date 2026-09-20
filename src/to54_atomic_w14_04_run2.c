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

void ov07_0221DA74(void *p) {
    void *manager;
    U32_AT(p, 0x18) += 4;
    manager = PTR_AT(p, 0x138);
    if (manager != NULL) {
        SpriteSystem_FreeResourcesAndManager(PTR_AT(PTR_AT(p, 0xc0), 0xac), manager);
    }
    PTR_AT(p, 0x138) = NULL;
}

void ov07_0221DAA0(void *p) {
    u32 index;
    void **sprites;
    U32_AT(p, 0x18) += 4;
    index = U32_AT(PTR_AT(p, 0x18), 0);
    U32_AT(p, 0x18) += 4;
    sprites = (void **)((u8 *)p + 0x13c);
    if (sprites[index] != NULL) {
        Sprite_DeleteAndFreeResources(sprites[index]);
    }
    U32_AT(p, 0x150 + index * 4) = 0;
    sprites[index] = NULL;
}

void ov07_0221DAD0(SysTask *task, DrawTaskData *data) {
    if (data->task == NULL) {
        SysTask_Destroy(task);
        return;
    }
    if (PTR_AT(data->owner, 0x138) != NULL) {
        SpriteSystem_DrawSprites(data->manager);
    }
}
