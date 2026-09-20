#include "to54_atomic_w19_private.h"

void ov112_021EC8A4(Ov112State *p, int row, int count) {
    void *s;
    if (count == 5 || ov112_021ECA88(p) == -1) {
        s = NewString_ReadMsgData(P(p, 0x1e44c), 0x76);
        ov112_021EA010(p, 0, s, 0, 0x10200);
        String_Delete(s);
        FillWindowPixelBuffer(p->raw + 0x1ebb8, 0);
        CopyWindowToVram(p->raw + 0x1ebb8);
    } else {
        s = NewString_ReadMsgData(P(p, 0x1e44c), ov112_021ECA88(p) + 0x5b);
        ov112_021EA010(p, 0, s, 0, 0x10200);
        String_Delete(s);
        s = NewString_ReadMsgData(P(p, 0x1e44c), ov112_021ECA88(p) + 0x77);
        ov112_021EA010(p, 1, s, 0, 0x10200);
        String_Delete(s);
    }
}
