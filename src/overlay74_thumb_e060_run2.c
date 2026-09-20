#include "constants/sndseq.h"

#include "overlay74_thumb_e060_private.h"

int ov74_0222E7EC(void *p) {
    W08Work *work = p;
    W08Entry *entry = work->entries[work->index];

    memcpy(work->copied_entry, entry, 0x358);
    memcpy((u8 *)work + 0x90, (u8 *)entry + 0x104, 0x50);
    work->field_428 = 0;
    work->flags.bit4 = 0;
    work->flags.bit5 = 1;
    ov74_02229CE0((u8 *)work + 4, work->save, (void *)0xf);
    return 0;
}
