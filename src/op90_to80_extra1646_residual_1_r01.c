#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef enum { RES_FAIL, RES_OK } Res;
typedef struct {
    u8 pad[0x820];
    void *handle;
} Ctx;
extern void ov45_022444C8(void);
void *ov00_021FE5B4(int a, int b, void (*cb)(void));
Res ov45_02244B9C(Ctx *ctx);
// @fn ov45_02244B9C
Res ov45_02244B9C(Ctx *ctx) {
    ctx->handle = ov00_021FE5B4(0x18, 0x80, ov45_022444C8);
    return ctx->handle == NULL ? RES_FAIL : RES_OK;
}
