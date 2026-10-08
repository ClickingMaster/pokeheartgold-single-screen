#pragma force_active on
#include "global.h"
#include "palette.h"
#include "save_vars_flags.h"
#include "sys_vars.h"

typedef struct Ov40PalOffsets {
    int offsets[5];
} Ov40PalOffsets;

typedef struct Ov40Unk {
    u8 unk0[0x28];
    PaletteData *plttData;
    u8 unk2C[0x804];
    SaveData *saveData;
} Ov40Unk;

extern const Ov40PalOffsets ov40_02245174;

void ov40_022334F8(u16 *buf, int start, int count);
void ov40_02233550(Ov40Unk *data);

// @fn ov40_02233550
void ov40_02233550(Ov40Unk *data) {
    PaletteData *plttData = data->plttData;
    SaveVarsFlags *varsFlags = Save_VarsFlags_Get(data->saveData);
    int progress[5];
    Ov40PalOffsets offsets;
    int i;

    progress[0] = Save_VarsFlags_GetBattleTowerPrintProgress(varsFlags);
    progress[1] = Save_VarsFlags_GetBattleFactoryPrintProgress(varsFlags);
    progress[2] = Save_VarsFlags_GetBattleArcadePrintProgress(varsFlags);
    progress[3] = Save_VarsFlags_GetBattleCastlePrintProgress(varsFlags);
    progress[4] = Save_VarsFlags_GetBattleHallPrintProgress(varsFlags);
    for (i = 0; i < 5; i++) {
        if (progress[i] != 0) {
            offsets = ov40_02245174;
            if (progress[i] == 2 || progress[i] == 3) {
                ov40_022334F8(PaletteData_GetFadedBuf(data->plttData, PLTTBUF_MAIN_OBJ), (u16)((offsets.offsets[i] + 4) * 16), 16);
            }
        }
    }
    PaletteData_SetAutoTransparent(plttData, TRUE);
}
