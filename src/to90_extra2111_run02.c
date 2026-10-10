#pragma force_active on
#include "global.h"

#include "to90_login_callback_private.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define W(p, n) (*(u32 *)((u8 *)(p) + (n)))

typedef struct {
    u32 zero;
    u8 *s;
} Global;

extern Global ov00_0221A90C;
extern void DWCi_Acc_LoginIdToUserName(void *, u32, char *);
extern void DWCi_Acc_SetLoginIdToUserData(void *, void *, u32);
extern void ov00_021FFF24(void *);
extern void ov00_021EF640(void);
extern void ov00_021EF508(GPConnection *, void *, void *);
extern int ov00_021EF494(u32);
extern int ov00_022000FC(GPConnection *, int, int);
extern void ov00_021EF6F4(void (*)(void), int);
extern int strcmp(const char *, const char *);
void ov00_021EFAC8(GPConnection *, void *, void *);
extern int ov00_02200020(GPConnection *, int, int, int, GPCallback, void *);

void ov00_021EFAC8(GPConnection *p, void *reply, void *parameter) {
    u8 *m = reply;
    int error;
    char a[21], b[21], c[21];
    if (W(m, 0)) {
        return;
    }
    if (W(ov00_0221A90C.s, 4) == 3) {
        if (*(s8 *)(m + 0x8e) == 0) {
            DWCi_Acc_LoginIdToUserName((u8 *)W(ov00_0221A90C.s, 0x1c) + 4, W(ov00_0221A90C.s, 12), a);
            error = ov00_022000FC(p, 0x705, (int)a);
            if (ov00_021EF494(error)) {
                return;
            }
            W(ov00_0221A90C.s, 4) = 4;
            error = ov00_02200020(p, W(m, 4), 0, 0, ov00_021EFAC8, 0);
            if (ov00_021EF494(error)) {
                return;
            }
        } else {
            ov00_021FFF24(p);
            ov00_021EF6F4(ov00_021EF640, 0);
            W(ov00_0221A90C.s, 4) = 1;
        }
    } else if (W(ov00_0221A90C.s, 4) == 4) {
        DWCi_Acc_LoginIdToUserName((u8 *)W(ov00_0221A90C.s, 0x1c) + 4, W(ov00_0221A90C.s, 12), b);
        if (strcmp((char *)m + 0x8e, b) == 0) {
            DWCi_Acc_LoginIdToUserName(ov00_0221A90C.s + 0x3c, W(ov00_0221A90C.s, 12), c);
            DWCi_Acc_SetLoginIdToUserData((void *)W(ov00_0221A90C.s, 0x1c), ov00_0221A90C.s + 0x3c, W(m, 4));
            W(ov00_0221A90C.s, 4) = 2;
            ov00_021EF508(p, ov00_0221A90C.s + 0x260, 0);
        } else {
            error = ov00_02200020(p, W(m, 4), 0, 0, ov00_021EFAC8, 0);
            if (ov00_021EF494(error)) {
                return;
            }
        }
    }
}
