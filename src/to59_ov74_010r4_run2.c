#include "global.h"
#include "main.h"
#include "title_screen.h"
#include "pm_string.h"
#include "text.h"
#include "render_text.h"
#include "system.h"
#include "constants/sndseq.h"
#include "to58_overlay74_010_private.h"
#include "yes_no_prompt.h"
#include "sprite.h"
#include "gf_gfx_planes.h"
extern void ov74_02236034(int);
extern void GF_RunVramTransferTasks(void);
extern void OamManager_ApplyAndResetBuffers(void);
extern void ov74_02232B18(u8 *);
extern void ov74_02231A1C(u8 *, u8 *, int);
extern u32 ov74_0223C9B8[];
extern void ov74_02232F5C(u8 *);
extern void PlaySE(u32);

int ov74_02233060(u8 *work);
int ov74_022330D0(u8 *work, int index);
int ov74_02233134(u8 *work, int index);
int ov74_0223319C(u8 *work, int input);

int ov74_02233060(u8 *work) {
    MigratedPokemon *migrated = Save_MigratedPokemon_Get(*(SaveData **)(work + 0x10));
    int count = 0;
    int box;
    for (box = 0; box < 14; box++) {
        int slot;
        for (slot = 0; slot < 30; slot++) {
            u8 *boxes = *(u8 **)(work + 0xE880);
            if (AGB_GetBoxMonData(boxes + 4 + box * 0x960 + slot * 0x50, 5, NULL) != 0) {
                count++;
            }
        }
    }
    if (count < 6) return 5;
    if (MigratedPokemon_CountPokemon(migrated) != 0) return 6;
    return 0;
}

int ov74_022330D0(u8 *work, int index) {
    if (*(u32 *)(work + 4) != 0) {
        ov74_02232B18(work);
        TextFlags_SetCanTouchSpeedUpPrint(1);
        *(u32 *)(work + 0x460) = ov74_0223C9B8[index];
        *(u32 *)(work + 0x470) = 1;
        ov74_02231A1C(work, work + 0x42c, 0x18);
        *(u32 *)(work + 4) = 0;
    } else if (!TextPrinterCheckActive((u8)*(u32 *)(work + 0x474))) {
        TextFlags_SetCanTouchSpeedUpPrint(0);
        return 1;
    }
    return 0;
}

int ov74_02233134(u8 *work, int index) {
    if (*(u32 *)(work + 4) != 0) {
        ov74_02232B18(work);
        TextFlags_SetCanTouchSpeedUpPrint(1);
        *(u32 *)(work + 0x460) = ov74_0223C9B8[index];
        *(u32 *)(work + 0x470) = 1;
        ov74_02231A1C(work, work + 0x42c, 0x18);
        *(u32 *)(work + 4) = 0;
    } else if (!TextPrinterCheckActive((u8)*(u32 *)(work + 0x474))) {
        ov74_02232F5C(work);
        TextFlags_SetCanTouchSpeedUpPrint(0);
        return 1;
    }
    return 0;
}

int ov74_0223319C(u8 *work, int input) {
    if (*(s32 *)(work + 8) != -1) {
        ov74_02232B18(work);
        TextFlags_SetCanTouchSpeedUpPrint(1);
        *(u32 *)(work + 0x460) = *(u32 *)(work + 8);
        *(u32 *)(work + 0x470) = input;
        ov74_02231A1C(work, work + 0x42c, 0x18);
        *(s32 *)(work + 8) = -1;
        PlaySE(SEQ_SE_DP_SELECT);
    } else if (input != 0) {
        if (!TextPrinterCheckActive((u8)*(u32 *)(work + 0x474))) {
            TextFlags_SetCanTouchSpeedUpPrint(0);
            return 1;
        }
    } else if (gSystem.touchNew != 0 || gSystem.newKeys != 0) {
        TextFlags_SetCanTouchSpeedUpPrint(0);
        PlaySE(SEQ_SE_DP_SELECT);
        return 1;
    }
    return 0;
}
