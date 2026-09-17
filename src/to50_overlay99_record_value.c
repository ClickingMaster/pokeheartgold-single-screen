#include "to50_overlay99_record_value_private.h"

typedef union Ov99R6Packed {
    u32 raw;
    struct {
        u32 low : 5;
        u32 value : 9;
        u32 row : 5;
        u32 column : 8;
        u32 rest : 5;
    } bits;
} Ov99R6Packed;

u16 ov99_021E71B0(const Ov99R6Work *work) {
    const Ov99R6Packed *packed = (const Ov99R6Packed *)&work->packed;
    int index = packed->bits.column + packed->bits.row * 30;

    if (index >= 493) {
        return 0;
    }
    return work->values[index];
}
