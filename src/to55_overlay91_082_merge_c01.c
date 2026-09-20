#include "to55_overlay91_merge_private.h"

#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

void ov91_02261790(void *p, void *manager) {
    TextOBJ_Destroy(PTR_AT(p, 0x48));
    SpriteTransfer_DeletePlttTransferTask(PTR_AT(p, 0x60));
    DestroySingle2DGfxResObj(PTR_AT(manager, 0x14c), PTR_AT(p, 0x60));
    sub_02021B5C((u8 *)p + 0x50);
    sub_02013938(PTR_AT(p, 0x4c));
    String_Delete(PTR_AT(p, 0x5c));
    Sprite_Delete(PTR_AT(p, 0x34));
    SpriteTransfer_DeleteCharTransferTask(PTR_AT(p, 0));
    SpriteTransfer_DeletePlttTransferTask(PTR_AT(p, 4));
    DestroySingle2DGfxResObj(PTR_AT(manager, 0x148), PTR_AT(p, 0));
    DestroySingle2DGfxResObj(PTR_AT(manager, 0x14c), PTR_AT(p, 4));
    DestroySingle2DGfxResObj(PTR_AT(manager, 0x150), PTR_AT(p, 8));
    DestroySingle2DGfxResObj(PTR_AT(manager, 0x154), PTR_AT(p, 0xc));
}

void ov91_02261808(void *p, void *manager, int value) {
    String16_FormatInteger(PTR_AT(p, 0x5c), value, 2, 2, TRUE);
    AddTextWindowTopLeftCorner(PTR_AT(manager, 0), (u8 *)p + 0x38, 3, 2, 0, 0);
    AddTextPrinterParameterizedWithColor((u8 *)p + 0x38, 0, PTR_AT(p, 0x5c), 0, 0, 0xff, 0x000f0e00, NULL);
    TextOBJ_CopyFromBGWindow(PTR_AT(p, 0x48), PTR_AT(p, 0x4c), (u8 *)p + 0x38, *(u32 *)((u8 *)p + 0x9c));
    RemoveWindow((u8 *)p + 0x38);
    TextOBJ_SetSpritesDrawFlag(PTR_AT(p, 0x48), TRUE);
    Sprite_SetDrawFlag(PTR_AT(p, 0x34), TRUE);
    U16_AT(p, 0x98) = 1;
    U16_AT(p, 0x9a) = 0;
}

void ov91_02261890(void *p) {
    u16 state = U16_AT(p, 0x98);
    if (state != 3) {
        if (state == 2) {
            U16_AT(p, 0x9a) = 12;
        }
        U16_AT(p, 0x98) = 3;
    }
}

void ov91_022618B0(void *p, void *manager, int value) {
    if (U16_AT(p, 0x98) != 3) {
        String16_FormatInteger(PTR_AT(p, 0x5c), value, 2, 2, TRUE);
        AddTextWindowTopLeftCorner(PTR_AT(manager, 0), (u8 *)p + 0x38, 3, 2, 0, 0);
        AddTextPrinterParameterizedWithColor((u8 *)p + 0x38, 0, PTR_AT(p, 0x5c), 0, 0, 0xff, 0x000f0e00, NULL);
        TextOBJ_CopyFromBGWindow(PTR_AT(p, 0x48), PTR_AT(p, 0x4c), (u8 *)p + 0x38, *(u32 *)((u8 *)p + 0x9c));
        RemoveWindow((u8 *)p + 0x38);
        U16_AT(p, 0x96) = 0;
    }
}

void ov91_02261928(void *p) {
    switch (U16_AT(p, 0x98)) {
    case 0:
        break;
    case 1:
        if (S16_AT(p, 0x94) < 8) {
            S16_AT(p, 0x94)
            ++;
        } else {
            U16_AT(p, 0x98) = 2;
        }
        break;
    case 2:
        break;
    case 3:
        if (S16_AT(p, 0x9a) > 0) {
            S16_AT(p, 0x9a)
            --;
        } else if (S16_AT(p, 0x94) > 0) {
            S16_AT(p, 0x94)
            --;
        } else {
            U16_AT(p, 0x98) = 0;
            TextOBJ_SetSpritesDrawFlag(PTR_AT(p, 0x48), FALSE);
            Sprite_SetDrawFlag(PTR_AT(p, 0x34), FALSE);
        }
        break;
    }

    if (S16_AT(p, 0x96) < 4) {
        S16_AT(p, 0x96)
        ++;
        S32_AT(p, 0x8c) = S16_AT(p, 0x96);
        ov91_0225D46C((u8 *)p + 0x7c);
    }
    S32_AT(p, 0x74) = S16_AT(p, 0x94);
    ov91_0225D46C((u8 *)p + 0x64);
    {
        u32 mtx[3];
        mtx[0] = *(u32 *)((u8 *)p + 0x64);
        mtx[1] = *(u32 *)((u8 *)p + 0x7c);
        Sprite_SetMatrix(PTR_AT(p, 0x34), mtx);
    }
    sub_02013728(PTR_AT(p, 0x48));
}
