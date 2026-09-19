#include "overlay07_callbacks_w07_private.h"

#define WORD(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define HALF(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))

void ov07_0222DCD8(void *owner, void *data) {
    switch (WORD(data, 0xC)) {
    case 0:
        ov07_0222DBD8(data);
        if (ov07_0222DC54(data)) {
            WORD(data, 0xC)++;
        }
        break;
    case 1:
        Sprite_DeleteAndFreeResources(PTR(data, 0x3C));
        ov07_0221C448(PTR(data, 0), owner);
        Heap_Free(data);
        return;
    }
    SpriteSystem_DrawSprites(PTR(data, 8));
}
