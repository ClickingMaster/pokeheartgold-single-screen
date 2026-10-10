#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    u8 bytes[301];
} Text;

typedef struct {
    u8 header[0x45];
    Text result;
    u8 remaining[2];
} Request;

extern u8 _0222AB84[];
extern u8 _0222AA58[];
extern int ov00_021ECC6C(const char *, void *);
extern int ov00_021ECCE0(void);
extern int ov00_021EC11C(int *, int *);
extern void ov39_022298FC(void);
extern void ov39_02229910(void);
extern void ov39_02229FC0(int);
int ov39_02229FCC(void);

// Native keeps the destination pointer in its own register for the 301-byte struct copy, so the copy's
// destination is a local that copy propagation must not fold back into the assignment.
#pragma push
#pragma opt_propagation off

int ov39_02229FCC(void) {
    int c, a;
    Request payload;
    if (*(u32 *)(_0222AB84 + 0x138) == 1) {
        return 1;
    }
    if (!ov00_021ECC6C((const char *)_0222AA58, &payload)) {
        *(u32 *)(_0222AB84 + 0x250) = 14;
        return 0;
    }
    for (;;) {
        int status = ov00_021ECCE0();
        if (status == 3) {
            Text *dst = (Text *)(_0222AB84 + 6);
            *dst = payload.result;
            *(u32 *)(_0222AB84 + 0x138) = 1;
            return 1;
        }
        if (status == 4) {
            ov00_021EC11C(&a, &c);
            *(u32 *)(_0222AB84 + 0x250) = 14;
            return 0;
        }
        if (status == 5) {
            *(u32 *)(_0222AB84 + 0x250) = 14;
            return 0;
        }
        ov39_022298FC();
        if (*(u32 *)(_0222AB84 + 0x268) == 1) {
            *(u32 *)(_0222AB84 + 0x250) = 13;
            ov39_02229910();
            return 0;
        }
        ov39_02229910();
        ov39_02229FC0(1);
    }
}

#pragma pop
