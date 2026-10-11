#pragma force_active on
#include "global.h"
#include "sprite.h"
void ManagedSprite_SetDrawPriority(ManagedSprite *sprite, int priority);

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
    u8 unk0[0x50C];
    void *unk50C;
    u8 unk510[4];
    int resCounter;
    Ov40IconSlot slots[5][6];
} Ov40IconData;

typedef struct {
    u8 unk0[0x4A4];
    s16 cursor;
    u8 unk4A6[0x860 - 0x4A6];
    Ov40IconData *iconData;
    u8 unk864[0x2608 - 0x864];
    Ov40Party *parties[1];
} Ov40Work;

typedef struct {
    int v[2][6];
} Ov40Order;

extern const Ov40Order ov40_02245808;
BOOL ov40_022303B8(u16 *species);
ManagedSprite *VsRecorder_NewMonIcon(Ov40Work *work, void *a1, int resId, int species, int form, int a5);
void ov40_0223D618(Ov40Work *work);
void ov40_0223D68C(Ov40Work *work);

// @fn ov40_0223D68C
void ov40_0223D68C(Ov40Work *work) {
    int i;
    Ov40IconData *data = work->iconData;
    Ov40Order order = ov40_02245808;
    int species[6] = {0};
    int forms[6] = {0};
    int j;
    int count;
    int mode;
    int cursor;
    int k;

    cursor = work->cursor;
    for (i = 0; i < 5; i++) {
        if (work->parties[cursor + i] == NULL) {
            continue;
        }
        if (ov40_022303B8(work->parties[cursor + i]->species)) {
            mode = 1;
        } else {
            mode = 0;
        }
        count = 0;
        for (j = 0; j < 6; j++) {
            species[j] = 0;
            forms[j] = 0;
        }
        for (j = 0; j < 3; j++) {
            if (work->parties[cursor + i]->species[order.v[mode][j]] != 0) {
                species[count] = work->parties[cursor + i]->species[order.v[mode][j]];
                forms[count] = work->parties[cursor + i]->forms[order.v[mode][j]];
                count++;
            }
        }
        if (ov40_022303B8(work->parties[cursor + i]->species)) {
            count = 3;
        }
        for (j = 3; j < 6; j++) {
            if (work->parties[cursor + i]->species[order.v[mode][j]] != 0) {
                species[count] = work->parties[cursor + i]->species[order.v[mode][j]];
                forms[count] = work->parties[cursor + i]->forms[order.v[mode][j]];
                count++;
            }
        }
        for (k = 0; k < 6; k++) {
            int sp = species[k];
            int fm = forms[k];
            data->slots[i][k].resId = data->resCounter;
            data->slots[i][k].sprite = VsRecorder_NewMonIcon(work, data->unk50C, data->slots[i][k].resId, sp, fm, 0);
            if (data->slots[i][k].sprite != NULL) {
                ManagedSprite_SetDrawPriority(data->slots[i][k].sprite, 6 - k);
            }
            data->resCounter++;
        }
    }
    ov40_0223D618(work);
}
