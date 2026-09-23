#include "to58_overlay112_mid_private.h"
int ov112_021EB568(Ov112State *p) {
    ov112_021EA570(p);
    switch (W(p,0x1ec50)) {
    case 0: W(p,4)=3; W(p,0x10)=1; break;
    case 1: W(p,4)=3; W(p,0x10)=2; break;
    case 2: W(p,4)=11; break;
    default: if ((s32)W(p,0x1ec50)>=3) GF_AssertFail(); break;
    }
    return 0x1a;
}
