#include "unk_0208805C.h"

#include "global.h"

#include "gf_gfx_loader.h"
#include "screen_fade.h"
#include "system.h"

u8 StepNumberWithWrap(s16 *value, u16 max, s16 step);

static const u16 sOamCharSizes[] = {
    0x20,
    0x80,
    0x200,
    0x800,
    0x40,
    0x80,
    0x100,
    0x400,
    0x40,
    0x80,
    0x100,
    0x400,
};

u16 GetOamCharSize(int a0) {
    return sOamCharSizes[a0];
}

u8 CalculateHpBarPixelsLength(u32 hp, u32 maxHp, u8 pixelsWide) {
    u8 ret = hp * pixelsWide / maxHp;
    if (ret == 0 && hp != 0) {
        ret = 1;
    }
    return ret;
}

u8 HpBar_GetColorIdx(u32 pixelsCur, u32 pixelsWide) {
    pixelsCur <<= 8;
    pixelsWide <<= 8;

    if (pixelsCur > pixelsWide / 2) {
        return 3; // green
    } else if (pixelsCur > pixelsWide / 5) {
        return 2; // yellow
    } else if (pixelsCur > 0) {
        return 1; // red
    }

    return 0; // fainted
}

u8 CalculateHpBarColor(u16 hp, u16 maxHp, u32 pixelsWide) {
    if (hp == maxHp) {
        return 4;
    }
    return HpBar_GetColorIdx(CalculateHpBarPixelsLength(hp, maxHp, pixelsWide), pixelsWide);
}

void BeginAppScreenFade(u8 fadeOut, enum HeapID heapID) {
    if (fadeOut == 0) {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_BRIGHTNESS_IN, FADE_TYPE_BRIGHTNESS_IN, RGB_BLACK, 6, 1, heapID);
    } else {
        BeginNormalPaletteFade(FADE_BOTH_SCREENS, FADE_TYPE_UNK_8, FADE_TYPE_BRIGHTNESS_OUT, RGB_BLACK, 6, 1, heapID);
    }
}

u8 StepNumberWithWrap(s16 *value, u16 max, s16 step) {
    s16 prev = *value;
    switch (step) {
    case -1:
        --(*value);
        if (*value <= 0) {
            *value = max;
        }
        if (*value == prev) {
            return 0;
        }
        return 2;
    case -10:
        *value -= 10;
        if (*value <= 0) {
            *value = 1;
        }
        if (*value == prev) {
            return 0;
        }
        return 2;
    case 1:
        ++(*value);
        if (*value > max) {
            *value = 1;
        }
        if (*value == prev) {
            return 0;
        }
        return 1;
    case 10:
        *value += 10;
        if (*value > max) {
            *value = max;
        }
        if (*value == prev) {
            return 0;
        }
        return 1;
    }

    return 0;
}

int HandleNumberSelectInput(s16 *value, u16 max) {
    if (gSystem.newAndRepeatedKeys & PAD_KEY_UP) {
        return StepNumberWithWrap(value, max, 1);
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_DOWN) {
        return StepNumberWithWrap(value, max, -1);
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_LEFT) {
        return StepNumberWithWrap(value, max, -10);
    }
    if (gSystem.newAndRepeatedKeys & PAD_KEY_RIGHT) {
        return StepNumberWithWrap(value, max, 10);
    }
    return 0;
}

void BgConfig_LoadAssetFromOpenNarc(BgConfig *bgConfig, enum HeapID heapID, NARC *narc, NarcId unused, int fileId, GFBgLayer layer, BgGfxAssetType kind, u32 szByte, u32 offset) {
    switch (kind) {
    case GF_BG_GFX_TYPE_CHAR:
        GfGfxLoader_LoadCharDataFromOpenNarc(narc, fileId, bgConfig, layer, offset, szByte, FALSE, heapID);
        break;
    case GF_BG_GFX_TYPE_SCRN:
        GfGfxLoader_LoadScrnDataFromOpenNarc(narc, fileId, bgConfig, layer, offset, szByte, FALSE, heapID);
        break;
    case GF_BG_GFX_TYPE_PLTT:
        if (layer <= GF_BG_LYR_MAIN_3) {
            GfGfxLoader_GXLoadPalFromOpenNarc(narc, fileId, GF_PAL_LOCATION_MAIN_BG, (enum GFPalSlotOffset)offset, szByte, heapID);
        } else {
            GfGfxLoader_GXLoadPalFromOpenNarc(narc, fileId, GF_PAL_LOCATION_SUB_BG, (enum GFPalSlotOffset)offset, szByte, heapID);
        }
        break;
    }
}
