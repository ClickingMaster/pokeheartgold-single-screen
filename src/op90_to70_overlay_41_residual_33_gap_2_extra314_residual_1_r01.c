#pragma force_active on
#include "global.h"
#include "assert.h"

typedef struct Ov41Template {
    void *unk0;
    void *unk4;
    void *res0;
    void *res1;
    u32 unk10;
    u32 unk14;
    int index;
    int value;
} Ov41Template;

typedef struct Ov41Entry {
    u8 unk0[0xC];
    void *unkC;
} Ov41Entry;

typedef struct Ov41Table {
    Ov41Entry *entries;
    u8 unk4[8];
} Ov41Table;

typedef struct Ov41Unk {
    u8 unk0[4];
    void *unk4;
    void *unk8;
    Ov41Table tables[3];
    u8 unk30[0x14];
    void *unk44;
    void **res0;
    void **res1;
    u8 *values;
    u8 unk54[4];
    void *unk58;
} Ov41Unk;

int ov41_022484E8(int mode, int index, void *a2);
void *DressUpGfx_AddSlotObject(Ov41Template *tmpl);
void *ov41_022499F0(void *a0, void *obj, int mode);
void ov41_02249A50(void *a0, void *a1);
void ov41_02249B94(void *a0, int *x, int *y);
void ov41_02248584(int mode, int index, int *outX, int *outY, int x, int y, void *a6);
void ov41_02249AF4(void *a0, int x, int y);
void ov41_02248B20(Ov41Unk *data, void *obj, int mode, int a3);
void ov41_022485DC(Ov41Unk *data, int mode, int index);

// @fn ov41_022485DC
void ov41_022485DC(Ov41Unk *data, int mode, int index) {
    Ov41Template tmpl;
    int outX, outY;
    int x, y;
    void *obj;
    void *work;
    int res0Index;
    int res1Index;
    int entry;

    tmpl.unk0 = data->unk58;
    tmpl.index = index;
    tmpl.unk4 = data->unk44;
    tmpl.unk10 = 0;
    tmpl.unk14 = 0;
    switch (mode) {
    case 0:
        res0Index = index;
        res1Index = 0;
        tmpl.value = data->values[index];
        break;
    case 1:
        res0Index = index;
        res0Index += 100;
        res1Index = index + 1;
        tmpl.value = 0;
        break;
    case 2:
        res0Index = index;
        res0Index += 100;
        res1Index = index + 1;
        tmpl.value = 0;
        break;
    default:
        GF_AssertFail();
        break;
    }
    tmpl.res0 = data->res0[res0Index];
    tmpl.res1 = data->res1[res1Index];
    GF_ASSERT(tmpl.res0 != NULL);
    GF_ASSERT(tmpl.res1 != NULL);
    entry = ov41_022484E8(mode, index, data->unk4);
    obj = DressUpGfx_AddSlotObject(&tmpl);
    work = ov41_022499F0(data->unk8, obj, mode);
    ov41_02249A50(work, data->tables[mode].entries[entry].unkC);
    ov41_02249B94(work, &x, &y);
    ov41_02248584(mode, index, &outX, &outY, x, y, data->unk4);
    ov41_02249AF4(work, outX, outY);
    ov41_02248B20(data, obj, mode, entry);
}
