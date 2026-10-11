#pragma force_active on
#include "global.h"
#include "save.h"
typedef struct Args73 {
    int unk00;
    SaveData *saveData;
    int unk08;
    int unk0C;
    void **records;
    void **records2;
} Args73;
typedef struct Entry73 {
    u32 (*getSize)(SaveData *saveData);
    void *unk04;
    void (*func)(Args73 *args);
} Entry73;
extern const Entry73 ov73_021EA744[];
int Comm_GetLocalNetId(void);
BOOL sub_02034818(int idx);
void *sub_020270C4(SaveData *saveData);
void sub_0202E43C(void *p);
void sub_0202E474(void *p);
void ov73_021EA3D0(SaveData *saveData, u8 (*buffer)[0xBC0]);
// @fn ov73_021EA3D0
void ov73_021EA3D0(SaveData *saveData, u8 (*buffer)[0xBC0]) {
    Args73 args;
    void *records[5];
    void *records2[5];
    u32 offsets[3];
    u32 size;
    void *p;
    u32 remaining;
    int i;
    u32 offset;
    int j;
    u32 *o;

    remaining = 3000;
    args.unk00 = 50;
    offset = 0;
    args.saveData = saveData;
    args.unk0C = Comm_GetLocalNetId();
    args.unk08 = 5;
    args.records = records;
    args.records2 = records2;
    offsets[0] = 0;
    offsets[1] = 0;
    offsets[2] = 0;
    for (i = 0; i < 2; i++) {
        offsets[0] += ov73_021EA744[i].getSize(saveData);
    }
    for (i = 0; i < 3; i++) {
        offsets[1] += ov73_021EA744[i].getSize(saveData);
    }
    for (i = 0; i < 4; i++) {
        offsets[2] += ov73_021EA744[i].getSize(saveData);
    }
    for (i = 0; (u32)i < 10; i++) {
        size = ov73_021EA744[i].getSize(saveData);
        GF_ASSERT(remaining > size);
        for (j = 0; j < 5; j++) {
            if (sub_02034818(j)) {
                args.records[j] = buffer[j] + offset;
            } else {
                args.records[j] = NULL;
            }
        }
        o = &offsets[i];
        switch (i) {
        case 7:
        case 8:
        case 9:
            for (j = 0; j < 5; j++) {
                if (sub_02034818(j)) {
                    args.records2[j] = buffer[j] + o[-7];
                } else {
                    args.records2[j] = NULL;
                }
            }
            break;
        }
        if (ov73_021EA744[i].func != NULL) {
            ov73_021EA744[i].func(&args);
        }
        offset += size;
        remaining -= size;
    }
    p = sub_020270C4(saveData);
    sub_0202E43C(p);
    sub_0202E474(p);
}
