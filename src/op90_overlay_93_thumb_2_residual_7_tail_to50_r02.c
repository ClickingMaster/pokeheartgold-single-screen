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
extern u16 ov93_02262C8A[];
extern u16 ov93_02262D04[][4];
int Comm_GetLocalNetId(void);
int WobbuffetPop_GetPlayerSeat(Work *work, int id);

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
    u8 count = work->config->count;
    int pos = WobbuffetPop_GetPlayerSeat(work, work->config->ids[i]);
    return offset * 16 + ov93_02262D04[count][pos];
}
void ov93_02260CF8(Work *work);
// @fn ov93_02260CF8
void ov93_02260CF8(Work *work) {
    u16 colors[4][16];
    int i;
    int j;
    u16 *unfaded;
    u16 *faded;
    int offset;
    offset = SpriteManager_FindPlttResourceOffset(work->spriteManager, 10006, 2);
    for (i = 0; i < work->config->count; i++) {
        int base = ov93_02262C8A[i] + offset * 16;
        for (j = 0; j < 16; j++) {
            colors[i][j] = PaletteData_GetBufferColorAtIndex(work->plttData, PLTTBUF_SUB_OBJ, PLTTSEL_OPAQUE, base);
            base++;
        }
    }
    FindPlayerIndex(work, Comm_GetLocalNetId());
    unfaded = PaletteData_GetUnfadedBuf(work->plttData, PLTTBUF_SUB_OBJ);
    faded = PaletteData_GetFadedBuf(work->plttData, PLTTBUF_SUB_OBJ);
    for (i = 0; i < work->config->count; i++) {
        int index = GetPaletteIndex(work, i, offset);
        int player = i;
        int k;
        for (k = 0; k < 16; k++, index++) {
            unfaded[index] = colors[player][k];
            faded[index] = colors[player][k];
        }
    }
}
