#include "global.h"

void GF_AssertFail(void);
void CopyU16ArrayToString(void *, const u16 *);
void *ov99_021E9518(void *work, int course, int name);

void *ov99_021E9518(void *work, int course, int name) {
    u8 *record;
    void *string;

    if (work == NULL) {
        GF_AssertFail();
    }
    record = *(u8 **)(*(u8 **)work + 4) + course * 0xA4;
    string = *(void **)((u8 *)work + 0x1BC);
    CopyU16ArrayToString(string, (const u16 *)(record + 0x30 + name * 0x18));
    return *(void **)((u8 *)work + 0x1BC);
}
