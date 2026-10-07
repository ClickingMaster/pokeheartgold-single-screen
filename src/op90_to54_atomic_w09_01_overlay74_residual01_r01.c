#pragma force_active on
#include "global.h"

typedef struct RecordId {
    u32 value;
} RecordId;

typedef struct RecordKind {
    u16 value;
} RecordKind;

typedef struct RecordName {
    u16 chars[12];
} RecordName;

typedef struct Record {
    RecordId id;
    RecordName nameA;
    RecordName nameB;
    RecordKind kind;
    u8 flag;
    u8 pad;
} Record;

typedef struct Work {
    u8 unk00[0x12];
    u16 flag;
} Work;

typedef struct Input {
    u32 unk00;
    u8 data[0x54];
    RecordName nameA;
    RecordName nameB;
} Input;

extern Record *ov74_02231154(void);
extern u32 ov74_0222FE5C(void *data);
extern int ov74_0222FE68(void *data);
int ov74_0223026C(Work *work, Input *input);

static inline RecordId GetRecordId(Input *input) {
    RecordId id;
    id.value = ov74_0222FE5C(input->data);
    return id;
}

static inline RecordKind GetRecordKind(Input *input) {
    RecordKind kind;
    kind.value = ov74_0222FE68(input->data);
    return kind;
}

// @fn ov74_0223026C
int ov74_0223026C(Work *work, Input *input) {
    Record *records = ov74_02231154();
    RecordId id = GetRecordId(input);
    RecordKind kind = GetRecordKind(input);
    int i = 0;
    Record *cursor = records;

    for (; i < 8; i++, cursor++) {
        RecordKind *curKind = &cursor->kind;
        if (cursor->id.value == id.value && curKind->value == kind.value) {
            return 1;
        }
        if (cursor->id.value == 0) {
            records[i].id.value = id.value;
            records[i].nameA = input->nameA;
            records[i].nameB = input->nameB;
            records[i].kind.value = kind.value;
            records[i].flag = work->flag;
            return 1;
        }
    }
    return 0;
}
