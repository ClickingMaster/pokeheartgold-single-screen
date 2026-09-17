#include "fade_func_01_internal.h"

extern u8 _0210F64C_call[];

BOOL FadeFunc_02(FadeWork *work) {
    if (work->state == 0) {
        _0210F64C.first = _020F5D60;
        sub_0200FCDC(work->color);
        sub_0201289C(work, _0210F64C_call);
        work->topScreen = 1;
        work->bottomScreen = 0;
        return FALSE;
    }
    return sub_020128E0(work);
}
