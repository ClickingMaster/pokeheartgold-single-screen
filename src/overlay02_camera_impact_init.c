#include "overlay02_4_landing_private.h"

WIP_LOCAL void ov02_0224D9C0(void *a0, void *a1, void *data) {
    VecFx32 offset;
    VecFx32 target;
    VecFx32 pos;
    memset(data, 0, 0x114);
    HeapExp_FndInitAllocator((NNSFndAllocator *)((u8 *)data + 0xdc), HEAP_ID_FIELD1, 0x20);
    ov02_0224D0C8(data, 0x13, 0x11, 2, (u8 *)data + 0xdc);
    ov02_0224D044(*(void **)((u8 *)a1 + 0x40), data);
    target = Camera_GetLookAtCamTarget(*(Camera **)((u8 *)a1 + 0x24));
    *(VecFx32 *)((u8 *)data + 0xf8) = target;
    pos = Camera_GetLookAtCamPos(*(Camera **)((u8 *)a1 + 0x24));
    *(VecFx32 *)((u8 *)data + 0xec) = pos;
    {
        int *p = (int *)&offset;
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
    }
    switch (PlayerAvatar_GetFacingDirection(*(PlayerAvatar **)((u8 *)a1 + 0x40))) {
    case 0:
        offset.z -= 0x8000;
        break;
    case 1:
        offset.z += 0x8000;
        break;
    case 2:
        offset.x -= 0x8000;
        break;
    case 3:
        offset.x += 0x8000;
        break;
    }
    *(VecFx32 *)((u8 *)data + 0x104) = offset;
    PlaySE(SEQ_SE_GS_ZUTUKI);
    *(u8 *)((u8 *)data + 0x113) = 0;
}

WIP_LOCAL void ov02_0224DAA4(void *a0, void *a1, void *data) {
    VecFx32 offset;
    VecFx32 target;
    VecFx32 pos;
    memset(data, 0, 0x114);
    HeapExp_FndInitAllocator((NNSFndAllocator *)((u8 *)data + 0xdc), HEAP_ID_FIELD1, 0x20);
    ov02_0224D0C8(data, 0x13, 0x11, 2, (u8 *)data + 0xdc);
    ov02_0224CFD8(*(void **)((u8 *)a1 + 0x3c), 0xfd, data);
    target = Camera_GetLookAtCamTarget(*(Camera **)((u8 *)a1 + 0x24));
    *(VecFx32 *)((u8 *)data + 0xf8) = target;
    pos = Camera_GetLookAtCamPos(*(Camera **)((u8 *)a1 + 0x24));
    *(VecFx32 *)((u8 *)data + 0xec) = pos;
    {
        int *p = (int *)&offset;
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
    }
    switch (PlayerAvatar_GetFacingDirection(*(PlayerAvatar **)((u8 *)a1 + 0x40))) {
    case 0:
        offset.z -= 0x8000;
        break;
    case 1:
        offset.z += 0x8000;
        break;
    case 2:
        offset.x -= 0x8000;
        break;
    case 3:
        offset.x += 0x8000;
        break;
    }
    *(VecFx32 *)((u8 *)data + 0x104) = offset;
    PlaySE(SEQ_SE_GS_ZUTUKI);
    *(u8 *)((u8 *)data + 0x113) = 0;
}
