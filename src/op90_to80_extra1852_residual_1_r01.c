#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef enum { RES_FAIL, RES_OK } Res;
int ov45_0224739C(void);
Res ov45_022474FC(void);
// @fn ov45_022474FC
Res ov45_022474FC(void) {
    return ov45_0224739C() == -1 ? RES_FAIL : RES_OK;
}
