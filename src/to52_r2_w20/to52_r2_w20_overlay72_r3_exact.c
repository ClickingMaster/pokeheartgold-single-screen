#include "private.h"

void ov72_02238164(void) {
    W20TransferTemplate template = ov72_0223B344;
    ObjCharTransfer_Init(&template);
    ObjPlttTransfer_Init(0x14, 0x43);
    ObjCharTransfer_ClearBuffers();
    ObjPlttTransfer_Reset();
}
