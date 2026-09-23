#include "global.h"

typedef struct {
    u32 words[11];
} Ov73Record;

extern void GF_AssertFail(void);
extern int sub_0202C8C4(void *);
extern int sub_0202C8E4(void *, void *);
void ov73_021E7964(Ov73Record *, const Ov73Record *);
void ov73_021E79A8(Ov73Record *, int);
void ov73_021E79F4(int, int, Ov73Record *, void **);

void ov73_021E7964(Ov73Record *records, const Ov73Record *incoming) {
    int i;
    for (i = 5; i > 2; --i) {
        records[i] = records[i - 1];
    }
    records[2] = *incoming;
}

void ov73_021E79A8(Ov73Record *records, int index) {
    int i;
    if (index < 2 || index > 5) {
        GF_AssertFail();
    }
    for (i = index + 1; i <= 5; ++i) {
        records[i - 1] = records[i];
    }
    u8 *last = (u8 *)records + 0xDC;
    *(u16 *)last = 0xFFFF;
    *(u16 *)(last + 0x10) = 0xFFFF;
}

void ov73_021E79F4(int count, int skip, Ov73Record *records, void **input) {
    int i;
    for (i = 0; i < count; i++) {
        void *record = input[i];
        if (i == skip || record == NULL || sub_0202C8C4(record) == 1 || sub_0202C8E4(record, records) != 0) {
            continue;
        }
        int j;
        for (j = 0; j < 4; ++j) {
            if (sub_0202C8E4(record, &records[j + 2]) != 0) {
                ov73_021E79A8(records, j + 2);
            }
        }
        ov73_021E7964(records, record);
    }
}
