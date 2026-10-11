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
extern u16 ov93_02262C82[];
extern u16 ov93_02262D54[][4];
int Comm_GetLocalNetId(void);
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

static inline int GetPaletteIndex(Work *work, int i, int offset) {
    return offset * 16 + ov93_02262D54[work->config->count][ov93_0225E3C4(work, work->config->ids[i])];
}
void ov93_02260E1C(Work *work);
// @fn ov93_02260E1C
void ov93_02260E1C(Work *work) {
    u16 colors[4];
    int i;
    u16 *unfaded;
    u16 *faded;
    int offset;
    offset = SpriteManager_FindPlttResourceOffset(work->spriteManager, 10006, 2);
    for (i = 0; i < work->config->count; i++) {
        colors[i] = PaletteData_GetBufferColorAtIndex(work->plttData, PLTTBUF_SUB_OBJ, PLTTSEL_OPAQUE, ov93_02262C82[i] + offset * 16);
    }
    FindPlayerIndex(work, Comm_GetLocalNetId());
    unfaded = PaletteData_GetUnfadedBuf(work->plttData, PLTTBUF_SUB_OBJ);
    faded = PaletteData_GetFadedBuf(work->plttData, PLTTBUF_SUB_OBJ);
    for (i = 0; i < work->config->count; i++) {
        int index = GetPaletteIndex(work, i, offset);
        s32 player = i;
        unfaded[index] = colors[player];
        faded[index] = colors[player];
    }
}
