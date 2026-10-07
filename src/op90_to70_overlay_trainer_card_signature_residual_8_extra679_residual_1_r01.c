#pragma force_active on
#include "global.h"
typedef struct { u8 b[17]; } Sig17;
void ov52_021E9260(u8 *w);
// @fn ov52_021E9260
void ov52_021E9260(u8 *w) {
    *(Sig17 *)(w + 0x432D) = *(Sig17 *)(w + 0x431C);
}
