#include "to57_overlay112_122_private.h"
#define U32(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define PTR(p, o) (*(void **)((u8 *)(p) + (o)))
int ov112_021EE2D4(void *vp) {
    Ov112State *p = vp;
    PokewalkerApp_LoadTilemap(p, 2, 10);
    PokewalkerApp_PrintString(p, 5, p->field468, 4, 0xF0100);
    PokewalkerApp_PrintExit(p, 1);
    p->selection = 0;
    PokewalkerApp_SetCursorPosition(p, 2);
    return 0x32;
}
int ov112_021EE31C(void *vp) {
    Ov112State *p = vp;
    int choice = PokewalkerApp_GetTouchedButton(1);
    if (choice != -1) {
        ov112_021EC440(p, choice, choice + 2, 0x33);
        PlaySE(0x5DD);
        return 0x41;
    }
    if (*(u32 *)(gSystem + 0x48) & 0x40) {
        PokewalkerApp_SetCursorPosition(p, 2);
        if (p->selection != 0) {
            PlaySE(0x5DC);
        }
        p->selection = 0;
    } else if (*(u32 *)(gSystem + 0x48) & 0x80) {
        PokewalkerApp_SetCursorPosition(p, 3);
        if (p->selection != 1) {
            PlaySE(0x5DC);
        }
        p->selection = 1;
    } else if (*(u32 *)(gSystem + 0x48) & 1) {
        ov112_021EC440(p, p->selection, p->selection + 2, 0x33);
        PlaySE(0x5DD);
        return 0x41;
    } else if (*(u32 *)(gSystem + 0x48) & 2) {
        ov112_021EC440(p, 1, 3, 0x33);
        PlaySE(0x5DC);
        return 0x41;
    }
    return 0x32;
}
int ov112_021EE3E0(void *vp) {
    Ov112State *p = vp;
    PokewalkerApp_HideCursor(p);
    if (p->selection == 0) {
        FillBgTilemapRect(p->bg, 1, 0, 0, 0, 0x20, 0x18, 0x10);
        FillBgTilemapRect(p->bg, 2, 0, 0, 0, 0x20, 0x18, 0x10);
        ClearWindowTilemapAndCopyToVram((u8 *)p + 0x1EA98);
        ClearWindowTilemapAndCopyToVram((u8 *)p + 0x1EAA8);
        BgCommitTilemapBufferToVram(p->bg, 2);
        U32(p, 0x1EC7C) = 0;
        return 0x34;
    }
    U32(p, 4) = 0xB;
    return 0x42;
}
int ov112_021EE464(void *p) {
    void *box = (u8 *)p + 0x1F2E8;
    Pokewalker_TryGetBoxMon(PTR(p, 0x1E440), box);
    ov112_021EA5A4(p, 0);
    BufferBoxMonNickname(PTR(p, 0x1E448), 1, box);
    PokewalkerApp_LoadMonPicture(box, (u8 *)p + 0x1D7AC, (u8 *)p + 0x1D79C, 0);
    ov112_021EC460(p, 2, 0x1A, 0x35);
    return 0x40;
}
int ov112_021EE4B8(void *p) {
    U32(p, 0x1F2D0) = 0;
    PokewalkerApp_HideSprite(p, 8);
    return 0x36;
}
int ov112_021EE4D0(void *vp) {
    Ov112State *p = vp;
    p->counter++;
    if (p->counter > 0xB4) {
        return 0x37;
    }
    if ((u32)p->counter > MTRandom() % 0xB4) {
        PokewalkerApp_SpawnFallingParticle(p);
    }
    ov112_021ED314(p->counter);
    return 0x36;
}
