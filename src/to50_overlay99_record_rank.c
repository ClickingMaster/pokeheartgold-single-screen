#include "to50_overlay99_record_rank_private.h"

typedef union Ov99RecordBits {
    u32 raw;
    struct {
        u32 low : 5;
        u32 value : 9;
        u32 high : 5;
        u32 rest : 13;
    } bits;
} Ov99RecordBits;

u8 ov99_021E7158(const void *work) {
    const Ov99RecordBits *record = (const Ov99RecordBits *)((const u8 *)work + 0x3f4);
    u8 result;

    if (record->bits.high == record->bits.low) {
        result = record->bits.value % 30;
        if (result != 0) {
            return result;
        }
    }
    return 30;
}
