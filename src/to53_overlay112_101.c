#include "to53_overlay112_w13_private.h"

#define PRINTER_STATE(p, inactive, active) \
    (!TextPrinterCheckActive(*(u32 *)((u8 *)(p) + 0x1E524)) ? (inactive) : (active))

#define LOAD_PRINTER(p, a, b, next)                                \
    do {                                                           \
        *(void **)((u8 *)(p) + 0x1E524) = ov112_021EA08C(p, a, b); \
        return next;                                               \
    } while (0)


void ov112_021ED180(void *task, void *p) {
    u16 *value = (u16 *)((u8 *)p + 0x1F2E0);
    *value += 16;
    if (*value > 256) {
        *value = 256;
        SysTask_Destroy(task);
    }
}
