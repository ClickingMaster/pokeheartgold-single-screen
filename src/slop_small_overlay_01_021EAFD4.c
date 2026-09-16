#include "global.h"

#include "filesystem.h"
#include "heap.h"

extern u32 sub_020208DC(void *a0, void *a1);
extern u32 sub_020209E0(void *a0, void *a1);
extern int sprintf(char *str, const char *format, ...);

typedef struct {
    u32 unk0;
    u32 unk4;
    void *unk8;
    void *unkc;
    void *unk10;
    u16 unk14;
    u16 unk16;
} UnkStruct_ov01_021EAFD4_sub;

typedef struct {
    void *unk0;
    void *unk4;
    UnkStruct_ov01_021EAFD4_sub entries[16];
    u32 unk188;
} UnkStruct_ov01_021EAFD4;

char ov01_02209898[] = "data/fld_anime%d.bin";
int ov01_021EB058(UnkStruct_ov01_021EAFD4 *work, void *a1, int memberIdx, NARC *narc);

int ov01_021EB058(UnkStruct_ov01_021EAFD4 *work, void *a1, int memberIdx, NARC *narc) {
    char buf[40];
    int slot;
    if (work == NULL) {
        return -1;
    }
    for (slot = 0; slot < 16; slot++) {
        if (work->entries[slot].unkc == NULL) {
            break;
        }
    }
    if (slot == 16 || a1 == NULL) {
        return -1;
    }
    work->entries[slot].unk0 = sub_020208DC(a1, (u8 *)work->unk4 + 4 + memberIdx * 0x34);
    if (work->entries[slot].unk0 == 0) {
        return -1;
    }
    work->entries[slot].unk4 = sub_020209E0(a1, (u8 *)work->unk4 + 4 + memberIdx * 0x34);
    sprintf(buf, ov01_02209898, memberIdx);
    work->entries[slot].unk10 = (u8 *)work->unk4 + 4 + memberIdx * 0x34;
    work->entries[slot].unkc = NARC_AllocAndReadWholeMember(narc, memberIdx + 1, HEAP_ID_FIELD1);
    work->entries[slot].unk8 = NNS_G3dGetTex(work->entries[slot].unkc);
    return slot;
}
