#include "overlay08_exact_bank_private.h"

#pragma force_active on

void ov08_02223454(u8 *work) {
    WindowArray_Delete(*(void **)(work + 0x2C), work[0x30]);
}
