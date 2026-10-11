#include "overlay18_mon_intro_setup_private.h"

void PokedexEntryPopup_Task(void *unused, Ov18State *state) {
    switch (*(u16 *)&state->raw[0x240]) {
    case 0:
        *(vu16 *)0x04000050 = 0;
        PokedexEntryPopup_ReadMonData(state);
        PokedexEntryPopup_InitBgs(state);
        PokedexEntryPopup_DrawWindows(state);
        PokedexEntryPopup_InitSprites(state);
        PokedexEntryPopup_CreatePokepic(state);
        PokedexEntryPopup_StartFadeIn(state);
        *(u16 *)&state->raw[0x240] = 1;
        break;
    case 1:
        if (PokedexEntryPopup_IsFadeDone(state) == TRUE) {
            *(u16 *)&state->raw[0x240] = 2;
        }
        break;
    case 2:
        PlayCryEx(0xE, *(u32 *)&state->raw[0x244], 0x1FF, 0x1FF, 0x1FF, 0);
        *(u16 *)&state->raw[0x240] = 3;
        break;
    case 3:
        if (IsCryFinished() == FALSE) {
            *(u32 *)&state->raw[0x254] = 1;
            *(u16 *)&state->raw[0x240] = 4;
        }
        break;
    case 4:
        break;
    }
    SpriteList_RenderAndAnimateSprites(*(SpriteList **)&state->raw[0xB4]);
    PokedexEntryPopup_FlashBanner(state);
}

void PokedexEntryPopup_ReadMonData(Ov18State *state) {
    BOOL lock = AcquireMonLock(*(Pokemon **)&state->raw[0xC]);
    *(u32 *)&state->raw[0x244] = GetMonData(*(Pokemon **)&state->raw[0xC], 5, NULL);
    *(u32 *)&state->raw[0x248] = GetMonData(*(Pokemon **)&state->raw[0xC], 0x70, NULL);
    *(u32 *)&state->raw[0x24C] = GetMonData(*(Pokemon **)&state->raw[0xC], 0xB1, NULL);
    *(u32 *)&state->raw[0x250] = GetMonData(*(Pokemon **)&state->raw[0xC], 0xB2, NULL);
    ReleaseMonLock(*(Pokemon **)&state->raw[0xC], lock);
}

void PokedexEntryPopup_InitBgs(Ov18State *state) {
    SetBgPriority(0, 0);
    GfGfx_EngineATogglePlanes(1, 1);
    {
        BgTemplate bg1 = sEntryPopupBg1Template;
        InitBgFromTemplate(*(BgConfig **)&state->raw[0], 1, &bg1, 0);
        BG_ClearCharDataRange(1, 0x20, 0, *(enum HeapID *)&state->raw[0x14]);

        BgTemplate bg2 = sEntryPopupBg2Template;
        InitBgFromTemplate(*(BgConfig **)&state->raw[0], 2, &bg2, 0);

        BgTemplate bg3 = sEntryPopupBg3Template;
        InitBgFromTemplate(*(BgConfig **)&state->raw[0], 3, &bg3, 0);
    }
    GfGfxLoader_LoadCharDataFromOpenNarc(*(NARC **)&state->raw[0x1C], 0x13, *(BgConfig **)&state->raw[0], (GFBgLayer)2, 0, 0, TRUE, *(enum HeapID *)&state->raw[0x14]);
    GfGfxLoader_LoadScrnDataFromOpenNarc(*(NARC **)&state->raw[0x1C], 0x14, *(BgConfig **)&state->raw[0], (GFBgLayer)2, 0, 0, TRUE, *(enum HeapID *)&state->raw[0x14]);
    PaletteData_LoadOpenNarc(*(PaletteData **)&state->raw[4], *(NARC **)&state->raw[0x1C], 0x12, *(enum HeapID *)&state->raw[0x14], (PaletteBufferId)0, 0, 0);
}
