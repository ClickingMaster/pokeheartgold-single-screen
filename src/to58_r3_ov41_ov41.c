#include "global.h"

void *ov41_02245EE0(void *work);

void *ov41_0224607C(void *);
u32 ov41_022460A8(void *);
void GF_AssertFail(void);

void *ov41_02245EE0(void *work)
{
    u32 *record = ov41_0224607C(*(void **)work);
    if (record == NULL) {
        GF_AssertFail();
    }
    record[0] = *(u32 *)((u8 *)work + 0x18);
    record[1] = ov41_022460A8(work);
    return record;
}
