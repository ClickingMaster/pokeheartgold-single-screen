#pragma force_active on
#include "global.h"
#include "sprite.h"

typedef struct SpriteManager SpriteManager;
void ManagedSprite_SetDrawPriority(ManagedSprite *sprite, int priority);
BOOL SpriteManager_UnloadCharObjById(SpriteManager *spriteManager, int resId);

typedef struct {
    u8 unk0[0x80];
    u16 species[12];
    u8 forms[12];
} Ov40Party;

typedef struct {
    int resId;
    ManagedSprite *sprite;
} Ov40IconSlot;

typedef struct {
    u8 unk0[0x2080];
    void *unk50C;
    int row;
    int resCounter;
    Ov40IconSlot slots[5][6];
} Ov40IconData;

typedef struct {
    u8 unk0[0x1C];
    SpriteManager *spriteMan;
    u8 unk20[0x4A4 - 0x20];
    s16 cursor;
    u8 unk4A6[0x860 - 0x4A6];
    Ov40IconData *iconData;
    u8 unk864[0x2608 - 0x864];
    Ov40Party *parties[1];
} Ov40Work;

typedef struct {
    int v[2][6];
} Ov40Order;

extern const Ov40Order ov40_0224551C;
BOOL ov40_022303B8(u16 *species);
ManagedSprite *ov40_0222FEA0(Ov40Work *work, void *a1, int resId, int species, int form, int a5);
void ov40_0222FF64(Ov40Work *work, int resId, ManagedSprite *sprite);
void ov40_02230964(Ov40Work *work, int a1);
void ov40_0223A3BC(Ov40Work *work);
void ov40_0223D244(Ov40Work *work);

// @fn ov40_0223D244
void ov40_0223D244(Ov40Work *work) {
    int row = 0;
    int freed[6] = {0};
    Ov40IconData *data = work->iconData;
    int i;
    int j;
    int idx;
    int count;
    int mode;

    if (data->row == work->cursor) {
        return;
    }
    ov40_02230964(work, 1);
    if (data->row > work->cursor) {
        row = 4;
    }
    for (j = 0; j < 6; j++) {
        if (data->slots[row][j].sprite != NULL) {
            ov40_0222FF64(work, data->slots[row][j].resId, data->slots[row][j].sprite);
            freed[j] = data->slots[row][j].resId;
            data->slots[row][j].sprite = NULL;
        }
    }
    if (row != 0) {
        for (j = 4; j >= 1; j--) {
            for (mode = 0; mode < 6; mode++) {
                data->slots[j][mode].sprite = data->slots[j - 1][mode].sprite;
                data->slots[j][mode].resId = data->slots[j - 1][mode].resId;
            }
        }
        row = 0;
    } else {
        for (i = 1; i < 5; i++) {
            for (j = 0; j < 6; j++) {
                data->slots[i - 1][j].sprite = data->slots[i][j].sprite;
                data->slots[i - 1][j].resId = data->slots[i][j].resId;
            }
        }
        row = 4;
    }
    idx = work->cursor + row;
    {
        Ov40Order order = ov40_0224551C;
        int species[6] = {0};
        int forms[6] = {0};

        mode = 0;
        if (ov40_022303B8(work->parties[idx]->species)) {
            mode = 1;
        }
        count = 0;
        {
        int n;
        for (n = 0; n < 6; n++) {
            forms[n] = species[n] = 0;
        }
        }
        for (j = 0; j < 3; j++) {
            if (work->parties[idx]->species[order.v[mode][j]] != 0) {
                species[count] = work->parties[idx]->species[order.v[mode][j]];
                forms[count] = work->parties[idx]->forms[order.v[mode][j]];
                count++;
            }
        }
        if (ov40_022303B8(work->parties[idx]->species)) {
            count = 3;
        }
        for (j = 3; j < 6; j++) {
            if (work->parties[idx]->species[order.v[mode][j]] != 0) {
                species[count] = work->parties[idx]->species[order.v[mode][j]];
                forms[count] = work->parties[idx]->forms[order.v[mode][j]];
                count++;
            }
        }
        for (j = 0; j < 6; j++) {
            int sp = species[j];
            int fm = forms[j];
            data->slots[row][j].resId = data->resCounter;
            data->slots[row][j].sprite = ov40_0222FEA0(work, data->unk50C, data->slots[row][j].resId, sp, fm, 0);
            if (data->slots[row][j].sprite != NULL) {
                ManagedSprite_SetDrawPriority(data->slots[row][j].sprite, 6 - j);
            }
            data->resCounter++;
        }
    }
    ov40_0223A3BC(work);
    data->row = work->cursor;
    for (j = 0; j < 6; j++) {
        if (freed[j] != 0) {
            SpriteManager_UnloadCharObjById(work->spriteMan, freed[j] + 100000);
        }
    }
    ov40_02230964(work, 0);
}
