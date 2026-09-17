#include "touch_sprite_motion_alloc_private.h"

UnkStruct02087284 *sub_02087284(volatile int count, volatile enum HeapID heapId, volatile int resBase, volatile int unk1C, fx32 yOffset, SpriteSystem *spriteSys, SpriteManager *spriteMan, PaletteData *plttData) {
    enum HeapID heap = heapId;
    UnkStruct02087284 *motion = Heap_Alloc(heap, sizeof(UnkStruct02087284));

    motion->count = count;
    motion->heapId = heap;
    motion->resBase = resBase;
    motion->unk_1C = unk1C;
    motion->yOffset = yOffset;
    motion->spriteSys = spriteSys;
    motion->spriteMan = spriteMan;
    motion->plttData = plttData;
    motion->state = 0;
    motion->unk_FC = 15;
    motion->unk_100 = 15;
    motion->task = NULL;
    motion->unk_F0 = 0;
    return motion;
}

const int _02102814[] = {
    0x61,
    0x62,
    0x63,
    0x64,
    0x65,
    0x80,
    0x85,
};
