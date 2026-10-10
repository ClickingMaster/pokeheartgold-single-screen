#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <string.h>
#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*(u8 *)((u8 *)(p) + (o)))
extern u32 *ov00_0221A784;
extern void ov00_021EF1B4(void);
extern void ov00_021F2974(void);
extern void ov00_021F2880(void);
extern u32 ov00_021ED5E4(u32);
extern u32 ov00_0220F724(u32, u16, u32);
extern int ov00_0220CF98(void *, u32, u32, u32, void *);
extern int ov00_021EE9D0(void);
extern void ov00_0220CFF4(u32, void *);
extern void ov00_0220D29C(u32, void *);
int ov00_021EE584(void);

int ov00_021EE584(void) {
    u16 port;
    u32 addr;
    u32 *c;
    int result;
    if (ov00_0221A784[0]) {
        return 0;
    }
    port = (u16)(ov00_021ED5E4(0x4000) + 0xc000);
    c = ov00_0221A784;
    addr = ov00_0220F724(0, port, 0);
    result = ov00_0220CF98(ov00_0221A784, addr, c[5], c[6], ov00_021EF1B4);
    if (ov00_021EE9D0()) {
        return result;
    }
    ov00_0220CFF4(ov00_0221A784[0], ov00_021F2974);
    ov00_0220D29C(ov00_0221A784[0], ov00_021F2880);
    return result;
}
