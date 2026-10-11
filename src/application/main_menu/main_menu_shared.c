#include "application/main_menu/main_menu_shared.h"

#include "global.h"

#include "constants/sndseq.h"

#include "application/main_menu/main_menu_util.h"
#include "library/nitrocrypto_rc4.h"

#include "bg_window.h"
#include "gf_gfx_loader.h"
#include "gf_gfx_planes.h"
#include "heap.h"
#include "list_menu.h"
#include "save.h"
#include "sound_playback.h"
#include "sprite.h"
#include "system.h"

u16 MATH_CalcCRC16(const MATHCRC16Table *table, const void *data, u32 dataLength);
u8 *sub_02035754(int index);
BOOL CTRDG_IsAgbCartridge(void);
void CTRDG_TerminateForPulledOut(void);

extern enum HeapID sMainMenuUtilCryptoHeapId;
extern u32 ov74_0223E2D4[]; // = gMainMenuUtilWork + 0xE80

#pragma force_active on
// The first 0x34 bytes of the MainMenuUtilWork (up to .sprites); the rest of it is .bss of asm/to57_overlay_74_data.s
u32 gMainMenuUtilWork[13];

#define sMainMenuUtilWork ((MainMenuUtilWork *)gMainMenuUtilWork)

// Sub screen of the card screens: background, palette by gift type, and the gift sprite (hidden)
void MainMenuUtil_ShowCardSubScreen(BgConfig *bgConfig, WonderCard *card) {
    MainMenuUtilWork *work = sMainMenuUtilWork;
    u16 tag = card->tag;
    int palette = MainMenuUtil_GetGiftPalette(tag);
    void *file;
    NNSG2dScreenData *screen;

    GfGfxLoader_LoadCharData(NARC_a_1_1_3, 34, bgConfig, GF_BG_LYR_SUB_1, 0, 0x1400, TRUE, sMainMenuUtilWork->heapId);
    file = GfGfxLoader_LoadFromNarc(NARC_a_1_1_3, 35, TRUE, sMainMenuUtilWork->heapId, TRUE);
    NNS_G2dGetUnpackedScreenData(file, &screen);
    BG_LoadScreenTilemapData(bgConfig, GF_BG_LYR_SUB_1, screen->rawData, 0x600);
    Heap_Free(file);
    BgTilemapRectChangePalette(bgConfig, GF_BG_LYR_SUB_1, 0, 0, 32, 24, palette + 8);
    ScheduleBgTilemapBufferTransfer(bgConfig, GF_BG_LYR_SUB_1);
    ov74_0223E2D4[9] = (u32)MainMenuUtil_LoadCardSubScreenPalette; // = work->vblankCallback
    sMainMenuUtilWork->bgConfig = bgConfig;
    switch (tag) {
    case MG_TAG_BATTLE_RULES:
    case MG_TAG_SECRET_BASE_DECO:
    case MG_TAG_MON_DECO:
    case MG_TAG_POKETCH_APP:
        MainMenuUtil_ShowGiftBox(work, tag, card);
        break;
    case MG_TAG_MANAPHY_EGG:
        work->giftSpriteDelay = 120;
    case MG_TAG_POKEMON:
    case MG_TAG_EGG:
    case MG_TAG_POKEMON_MOVIE:
        MainMenuUtil_ShowGiftPokemon(work, tag, card);
        break;
    case MG_TAG_ITEM:
    case MG_TAG_MEMBER_CARD:
    case MG_TAG_OAKS_LETTER:
    case MG_TAG_AZURE_FLUTE:
    case MG_TAG_SECRET_KEY:
    case MG_TAG_POKEWALKER_COURSE:
    case MG_TAG_MEMORIAL_PHOTO:
        MainMenuUtil_ShowGiftItem(work, tag, card);
        break;
    case MG_TAG_INVALID:
        break;
    }
    Sprite_SetDrawFlag(work->giftSprite, FALSE);
}

// Encrypts the Wonder Card that follows the header (RC4, the key mixes the CRC-16 of the header with our MAC address)
void MainMenuUtil_EncryptWonderCard(UnkWonderCardSubstruct_104 *header, void *dest, enum HeapID heapID) {
    MATHCRC16Table *table = Heap_Alloc(heapID, sizeof(MATHCRC16Table));
    u16 crc;
    u16 key[4];
    u16 prev;
    int i;
    u16 *p;
    CRYPTORC4Context *rc4;

    MATHi_CRC16InitTableRev(table, 0xA001);
    crc = MATH_CalcCRC16(table, header, sizeof(UnkWonderCardSubstruct_104));
    Heap_Free(table);
    OS_GetMacAddress((u8 *)key);
    key[3] = key[1];
    key[1] = crc;
    prev = 0xD679;
    for (i = 0, p = key; i < 4; i++, p++) {
        *p ^= prev;
        prev = *p;
    }
    rc4 = Heap_Alloc(heapID, sizeof(CRYPTORC4Context));
    CRYPTO_RC4Init(rc4, key, 8);
    CRYPTO_RC4Encrypt(rc4, header + 1, sizeof(WonderCard), dest);
    Heap_Free(rc4);
}

// Decrypts a received Wonder Card (the same key, with the MAC address of the sender)
void MainMenuUtil_DecryptWonderCard(UnkWonderCardSubstruct_104 *header, WonderCard *dest, enum HeapID heapID) {
    MATHCRC16Table *table;
    u16 crc;
    const u8 *mac;
    u16 key[4];
    u8 *bytes;
    u16 prev;
    int i;
    CRYPTORC4Context *rc4;

    table = Heap_Alloc(heapID, sizeof(MATHCRC16Table));
    MATHi_CRC16InitTableRev(table, 0xA001);
    crc = MATH_CalcCRC16(table, header, sizeof(UnkWonderCardSubstruct_104));
    Heap_Free(table);
    mac = sub_02035754(0);
    bytes = (u8 *)key;
    bytes[0] = mac[4];
    bytes[1] = mac[5];
    bytes[2] = mac[6];
    bytes[3] = mac[7];
    bytes[4] = mac[8];
    bytes[5] = mac[9];
    key[3] = key[1];
    key[1] = crc;
    prev = 0xD679;
    for (i = 0; i < 4; i++) {
        *(u16 *)bytes ^= prev;
        prev = *(u16 *)bytes;
        bytes += 2;
    }
    rc4 = Heap_Alloc(heapID, sizeof(CRYPTORC4Context));
    CRYPTO_RC4Init(rc4, key, 8);
    CRYPTO_RC4Encrypt(rc4, header + 1, sizeof(WonderCard), dest);
    Heap_Free(rc4);
}

#pragma force_active reset

// Cursor callback of the overlay's list menus
void MainMenuUtil_ListMenuCursorCallback(ListMenu *list, s32 index, u8 onInit) {
    switch (onInit) {
    case FALSE:
        PlaySE(SEQ_SE_DP_SELECT);
        break;
    case TRUE:
        break;
    }
}

#pragma force_active on

// Cartridge interrupt: the GBA cartridge was pulled out
void MainMenuUtil_CartridgeIrqHandler(void) {
    if (((*(vu16 *)HW_BUTTON_XY_BUF & 0x8000) >> 15) == 0 && !CTRDG_IsAgbCartridge()) {
        CTRDG_TerminateForPulledOut();
    }
}

void MainMenuUtil_EnableCartridgePullOutCheck(BOOL enable) {
    if (enable == TRUE) {
        OS_DisableIrq();
        OS_SetIrqFunction(OS_IE_CARTRIDGE, MainMenuUtil_CartridgeIrqHandler);
        OS_EnableIrqMask(OS_IE_CARTRIDGE);
        OS_EnableIrq();
    } else {
        OS_DisableIrqMask(OS_IE_CARTRIDGE);
    }
}

#pragma force_active reset

void MainMenuUtil_IgnoreCartridgeIrqHandler(void) {
}

#pragma force_active on

// Cartridge interrupt that does nothing
void MainMenuUtil_IgnoreCartridgeIrq(void) {
    OS_DisableIrq();
    OS_SetIrqFunction(OS_IE_CARTRIDGE, MainMenuUtil_IgnoreCartridgeIrqHandler);
    OS_EnableIrqMask(OS_IE_CARTRIDGE);
    OS_EnableIrq();
}

#pragma force_active reset

// Asynchronous save: MainMenuUtil_RunSave runs it step by step; the state is SAVE_STATE_*
void MainMenuUtil_StartSave(SaveData *saveData) {
    sMainMenuUtilWork->saveData = saveData;
    sMainMenuUtilWork->saveState = SAVE_STATE_START;
}

#pragma force_active on

int MainMenuUtil_RunSave(void) {
    // the work through the preceding label (second literal-pool entry of the same address, matching)
    MainMenuUtilWork *work = (MainMenuUtilWork *)(&sMainMenuUtilCryptoHeapId + 1);
    int result;

    switch (sMainMenuUtilWork->saveState) {
    case SAVE_STATE_START:
        sub_0201A728(4);
        Save_PrepareForAsyncWrite(work->saveData, 2);
        work->saveState++;
        break;
    case SAVE_STATE_WRITING:
        result = Save_WriteFileAsync(work->saveData);
        if (result == WRITE_STATUS_TOTAL_FAIL) {
            work->saveState = SAVE_STATE_FAILED;
        } else if (result == WRITE_STATUS_SUCCESS) {
            work->saveState = SAVE_STATE_DONE;
        } else if (result == WRITE_STATUS_NEXT) {
            work->saveState = SAVE_STATE_PAUSED;
        }
        if (result == WRITE_STATUS_SUCCESS || result == WRITE_STATUS_TOTAL_FAIL) {
            sub_0201A738(4);
        }
        return result;
    case SAVE_STATE_DONE:
    case SAVE_STATE_FAILED:
    case SAVE_STATE_PAUSED:
        break;
    }
    return WRITE_STATUS_CONTINUE;
}

void MainMenuUtil_ResumeSave(void) {
    MainMenuUtilWork *work = (MainMenuUtilWork *)(&sMainMenuUtilCryptoHeapId + 1);

    if (sMainMenuUtilWork->saveState == SAVE_STATE_PAUSED) {
        work->saveState = SAVE_STATE_WRITING;
    }
}

#pragma force_active reset

void MainMenuUtil_CancelSave(void) {
    Save_Cancel(sMainMenuUtilWork->saveData);
    sMainMenuUtilWork->saveState = SAVE_STATE_FAILED;
    sub_0201A738(4);
}

int MainMenuUtil_GetSaveState(void) {
    return sMainMenuUtilWork->saveState;
}
