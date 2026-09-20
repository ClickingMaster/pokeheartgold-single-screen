#include "to56_w01_private.h"

BOOL ov07_0222C870(To56W01Work *work) {
    u32 *table;
    BOOL result;
    int i;
    s16 x;
    s16 y;
    int index;

    table = ov07_02222D60(work->handle);
    ov07_0222C850(work, table);
    result = ov07_022222B4(&work->movement);
    for (i = work->start; i <= work->end; i++) {
        x = (s16)table[i];
        y = (s16)(table[i] >> 16);
        y -= ((work->end - i) * work->movement.y) / 100;
        x -= ((work->start + 40 - i) * work->movement.x) / 100;
        index = i - 1;
        if (index < 0) {
            index += 192;
        }
        table[index] = ov07_02222D88((u16)x, (u16)y);
    }
    return result;
}
