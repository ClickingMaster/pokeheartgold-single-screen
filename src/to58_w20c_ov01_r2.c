#include "global.h"
#include "heap.h"
#include "filesystem.h"

extern void GF_AssertFail(void);
void ov01_021F49FC(NARC *narc, int member, u32 *out);
extern int GetMapModelNo(int, void *);
int ov01_021F4A50(u32 *out, void *manager, void *matrix, void *state, int index, int count, int reset);

void ov01_021F49FC(NARC *narc, int member, u32 *out) {
    u32 *buf;
    if (narc != NULL) {
        buf = Heap_AllocAtEnd((enum HeapID)4, 16);
        NARC_ReadFromMember(narc, member, 0, 16, buf);
    } else {
        GF_AssertFail();
        buf = AllocAtEndAndReadFromNarcMemberByIdPair((NarcId)0x41, member, (enum HeapID)4, 0, 16);
    }
    out[0] = buf[0];
    out[3] = buf[1];
    out[1] = buf[2];
    out[2] = buf[3];
    Heap_Free(buf);
}

int ov01_021F4A50(u32 *out, void *manager, void *matrix, void *state, int index, int count, int reset) {
    int model;
    if (reset) {
        if (*(u32 *)((u8 *)state + 0x808) != 0) {
            GF_AssertFail();
        }
        *(u32 *)((u8 *)state + 0x808) = 0;
    }
    if (index < 0 || index >= count) {
        return 0xffff;
    }
    model = GetMapModelNo(index, matrix);
    if (model != 0xffff) {
        ov01_021F49FC(*(NARC **)((u8 *)manager + 0x100), model, out);
    }
    return model;
}
