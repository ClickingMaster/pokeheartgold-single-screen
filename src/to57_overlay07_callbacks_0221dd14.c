#include "to57_overlay07_callbacks_0221dd14_private.h"

extern int _02237840;

void ov07_0221DD14(Ov07Slot **slots, int kind) {
    int i;
    for (i = 0; i < 4; i++, slots++) {
        Ov07Slot *slot = *slots;
        if (slot != NULL && (kind == 5 || kind == slot->kind)) {
            slot->active = TRUE;
        }
    }
}

void ov07_0221DD38(Ov07Slot **slots, Ov07Slot *slot, int kind) {
    u8 i;
    for (i = 0; i < 4; i++) {
        if (slots[i] != NULL && slots[i]->kind == kind) {
            GF_AssertFail();
        }
    }
    for (i = 0; i < 4; i++) {
        if (slots[i] == NULL) {
            slots[i] = slot;
            slots[i]->kind = kind;
            return;
        }
    }
    if (&_02237840 != NULL) {
        GF_AssertFail();
    }
}

Ov07Slot *ov07_0221DD8C(Ov07Slot **slots, int kind) {
    u8 i;
    for (i = 0; i < 4; i++) {
        Ov07Slot *slot = slots[i];
        if (kind != slot->kind) {
            continue;
        }
        return slot;
    }
    return NULL;
}

BOOL ov07_0221DDB0(Ov07Slot **slots, Ov07Context *ctx, int index) {
    int first = ov07_0223192C(ctx, *(u16 *)((u8 *)ctx->config + 0x14));
    int second = ov07_0223192C(ctx, *(u16 *)((u8 *)ctx->config + 0x16));
    int kind = *(int *)((u8 *)slots + index * 4 + 0x1C);

    if (kind == 2) {
        if (first == second) {
            return second != 3;
        }
        if (second == 3) {
            return TRUE;
        }
        goto no;
    }
    if (kind != 0 && second == 3) {
        return TRUE;
    }
no:
    return FALSE;
}

void ov07_0221DE04(Ov07Slot **slots, Ov07Context *ctx, int layer, void *resource) {
    int member;
    int screen;
    int kind;

    member = ov07_0221FB7C(resource, 0);
    GfGfxLoader_LoadCharData(7, member, ctx->bgConfig, layer, 0, 0, 1, (int)ctx->narc);
    screen = ov07_0221FB7C(resource, 1);
    PaletteData_LoadNarc(ctx->palette, 7, screen, (int)ctx->narc, 0, 0x20, 0x90);
    BgClearTilemapBufferAndCommit(ctx->bgConfig, (u8)layer);
    kind = 2;
    if (ov07_0221BFC0(ctx) == 1) {
        kind = 4;
    } else if (ov07_0221DDB0(slots, ctx, 7) == 1) {
        kind = 3;
    }
    member = ov07_0221FB7C(resource, kind);
    GfGfxLoader_LoadScrnData(7, member, ctx->bgConfig, layer, 0, 0, 1, (int)ctx->narc);
}
