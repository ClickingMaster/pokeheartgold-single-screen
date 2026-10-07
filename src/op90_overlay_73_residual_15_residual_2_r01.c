#pragma force_active on
#include "global.h"

typedef struct Record {
    u32 unk00[6];
    u8 unk18;
    u8 unk19;
    u8 unk1A[2];
    u32 unk1C[11];
} Record;

BOOL ov73_021E7A8C(int idx, int val);
int ov73_021E7CD8(Record *records, int count, Record *record, int idx);

// @fn ov73_021E7CD8
int ov73_021E7CD8(Record *records, int count, Record *record, int idx) {
    int i;
    if (record->unk18 == 0) {
        return 0;
    }
    if (!ov73_021E7A8C(idx, record->unk18)) {
        return -1;
    }
    for (i = 0; i < count; i++) {
        if (records[i].unk18 == 0) {
            records[i] = *record;
            return 1;
        }
    }
    for (i = 0; i < count; i++) {
        if (records[i].unk19 >= 3) {
            records[i] = *record;
            return 1;
        }
    }
    return 0;
}
