#include "global.h"
#include "system.h"
#include "touchscreen.h"

BOOL ov80_0222D2AC(void *ctx);

BOOL ov80_0222D2AC(void *ctx) {
    if (gSystem.newKeys & 3 || System_GetTouchNew()) {
        return TRUE;
    }
    return FALSE;
}
