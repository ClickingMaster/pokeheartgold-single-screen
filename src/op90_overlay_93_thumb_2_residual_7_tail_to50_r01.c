#pragma force_active on
#include "global.h"
#include "sprite_system.h"
#include "palette.h"
typedef struct {u8 pad0[0x2c];u8 ids[4];u8 count;} Config;
typedef struct {
    Config *config;
    u8 pad4[0x24];
    SpriteManager *spriteManager;
    u8 pad2c[0x8c - 0x2c];
    PaletteData *plttData;
} Work;
extern u16 ov93_02262C72[];
extern u16 ov93_02262DA4[][4];
int sub_0203769C(void);
int ov93_0225E3C4(Work *work, int id);

static inline int FindPlayerIndex(Work *work, int id) {
    int i;
    for (i = 0; i < work->config->count; i++) {
        if (id == work->config->ids[i]) {
            break;
        }
    }
    return i;
}

static inline int GetPaletteIndex(Work *work, int i) {
    u8 count = work->config->count;
    int pos = ov93_0225E3C4(work, work->config->ids[i]);
    return ov93_02262DA4[count][pos];
}
void ov93_02260BF0(Work *work);
// @fn ov93_02260BF0
void ov93_02260BF0(Work *work) {
    u16 colors[4][3];
    int i;
    int j;
    u16 *unfaded;
    u16 *faded;
    for (i = 0; i < work->config->count; i++) {
        int base = ov93_02262C72[i];
        for (j = 0; j < 3; j++) {
            colors[i][j] = PaletteData_GetBufferColorAtIndex(work->plttData, PLTTBUF_SUB_BG, PLTTSEL_OPAQUE, base + j);
        }
    }
    FindPlayerIndex(work, sub_0203769C());
    unfaded = PaletteData_GetUnfadedBuf(work->plttData, PLTTBUF_SUB_BG);
    faded = PaletteData_GetFadedBuf(work->plttData, PLTTBUF_SUB_BG);
    for (i = 0; i < work->config->count; i++) {
        int index = GetPaletteIndex(work, i);
        int player = i;
        int k;
        for (k = 0; k < 3; k++, index++) {
            unfaded[index] = colors[player][k];
            faded[index] = colors[player][k];
        }
    }
}
