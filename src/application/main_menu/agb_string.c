#include "global.h"

#include "application/main_menu/agb_cartridge.h"

// Conversion of Ruby/Sapphire/Emerald/FireRed/LeafGreen strings to this game's characters (Migrate from GBA).

u32 AgbString_GetSpaceChar(u32 language);
u16 AgbString_GetOpenQuoteChar(u32 language);
u16 AgbString_GetCloseQuoteChar(u32 language);

extern const u16 sAgbToDpCharMap[][2]; // GBA character -> character of this game (Japanese, other languages)

// The space
u32 AgbString_GetSpaceChar(u32 language) {
    u32 result;
    if (language == LANGUAGE_JAPANESE) {
        result = 1;
    } else {
        result = 0x1DE;
    }
    return (u16)result;
}

// The opening double quote
u16 AgbString_GetOpenQuoteChar(u32 language) {
    switch (language) {
    case LANGUAGE_JAPANESE:
    default:
        return 0xEA;
    case LANGUAGE_ENGLISH:
    case LANGUAGE_ITALIAN:
    case LANGUAGE_SPANISH:
        return 0x1B4;
    case LANGUAGE_FRENCH:
        return 0x1B7;
    case LANGUAGE_GERMAN:
        return 0x1B6;
    }
}

// The closing double quote
u16 AgbString_GetCloseQuoteChar(u32 language) {
    switch (language) {
    case LANGUAGE_JAPANESE:
    default:
        return 0xEB;
    case LANGUAGE_ENGLISH:
    case LANGUAGE_ITALIAN:
    case LANGUAGE_SPANISH:
        return 0x1B5;
    case LANGUAGE_FRENCH:
        return 0x1B8;
    case LANGUAGE_GERMAN:
        return 0x1B4;
    }
}

BOOL ConvertRSStringToDPStringInternational(const u8 *src, u16 *dst, u32 length, u32 language) {
    BOOL nonJa = (language != LANGUAGE_JAPANESE);
    u32 i;
    for (i = 0; i < (length - 1); i++) {
        if (src[i] == 0xFF) { // EOS
            break;
        }
        if (src[i] >= 0xF7) { // control characters: the string becomes "?????" (up to 10 characters)
            int invalidStrLen = (length - 1 < 10) ? length - 1 : 10;
            int j;
            for (j = 0; j < invalidStrLen; j++) {
                dst[j] = 0x1AC;
            }
            dst[j] = 0xFFFF;
            return FALSE;
        }
        switch (sAgbToDpCharMap[src[i]][nonJa]) {
        case 1:
            dst[i] = AgbString_GetSpaceChar(language);
            break;
        case 0xEA:
            dst[i] = AgbString_GetOpenQuoteChar(language);
            break;
        case 0xEB:
            dst[i] = AgbString_GetCloseQuoteChar(language);
            break;
        default:
            dst[i] = sAgbToDpCharMap[src[i]][nonJa];
            break;
        }
    }
    dst[i] = 0xFFFF;
    return TRUE;
}
