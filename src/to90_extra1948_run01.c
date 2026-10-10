#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern u8 *ov00_021F955C(void);
extern int ov00_02212EAC(void *);
extern u32 ov00_02212E78(void *);
extern u32 ov00_02212E80(void *);
extern u32 ov00_02212EC4(void *);
extern u32 ov00_02212ECC(void *);
extern u32 ov00_022139E8(u32);
extern u32 SOC_GetHostID(void);
extern u32 ov00_021ED5E4(u32);
extern u32 ov00_0220D27C(u32);
extern u32 ov00_0220D294(u32);
extern int ov00_021F4304(int, u32, u32, u32, u32 *, int);
extern int ov00_021F4248(void *);
extern void ov00_021F9104(int, u32, int, void *);

typedef struct Work {
    u8 pad[0xf4];
    u32 friends[32];
    u8 gap[0x9c];
    u32 ips[32];
    u16 ports[32];
} Work;

int ov00_021F3F5C(int, u32, void *);
#define W(p, x) (*(u32 *)((p) + (x)))
#define H(p, x) (*(u16 *)((p) + (x)))
#define B(p, x) (*(u8 *)((p) + (x)))

int ov00_021F3F5C(int mode, u32 value, void *packet) {
    u8 *p = ov00_021F955C();
    int index = B(p, 0x14), retry;
    unsigned result = 0;
    u32 pair[2];
    if (mode == 0) {
        p = ov00_021F955C();
        value = (W(p, 0x200) & 0xffff) | (H(ov00_021F955C(), 0x17e) << 16);
        if (ov00_02212EAC(packet)) {
            u32 ip = ov00_02212E78(packet);
            if (ip != ov00_022139E8(W(ov00_021F955C(), 0xe4))) {
                retry = 1;
            } else {
                p = ov00_021F955C();
                ((Work *)p)->ips[index] = ov00_02212EC4(packet);
                p = ov00_021F955C();
                ((Work *)p)->ports[index] = ov00_02212ECC(packet);
                retry = result;
            }
        } else {
            u32 ip = (u16)SOC_GetHostID();
            if ((u16)ip == 0xa8c0 || ((ip & 0xff) == 0xac && (ip & 0xff00) >= 0x1000 && (ip & 0xff00) <= 0x1f00) || (ip & 0xff) == 10) {
                retry = 1;
            } else {
                p = ov00_021F955C();
                ((Work *)p)->ips[index] = ov00_02212E78(packet);
                p = ov00_021F955C();
                ((Work *)p)->ports[index] = ov00_02212E80(packet);
                retry = 0;
            }
        }
        if (retry) {
            p = ov00_021F955C();
            H(p, 0x17e) = ov00_021ED5E4(0x10000);
            W(ov00_021F955C(), 0x19c) = value;
        } else {
            pair[0] = SOC_GetHostID();
            pair[1] = ov00_0220D27C(**(u32 **)(ov00_021F955C() + 4));
            p = ov00_021F955C();
            value = ov00_02212E78(packet);
            {
                int rc = ov00_021F4304(6, ((Work *)p)->friends[index], value, ov00_02212E80(packet), pair, 2);
                B(ov00_021F955C(), 0x3cd) = 0;
                if (rc) {
                    return 2;
                }
            }
            W(ov00_021F955C(), 0x19c) = 0;
        }
        B(ov00_021F955C(), 0x194) = 0;
        B(ov00_021F955C(), 0x195) = 0;
        p = ov00_021F955C();
        H(p, 0x196) = ov00_02212E80(packet);
        p = ov00_021F955C();
        W(p, 0x198) = ov00_02212E78(packet);
    } else {
        B(ov00_021F955C(), 0x194) = 1;
        B(ov00_021F955C(), 0x195) = result;
        H(ov00_021F955C(), 0x196) = result;
        W(ov00_021F955C(), 0x198) = result;
        W(ov00_021F955C(), 0x19c) = value;
        retry = 1;
    }
    if (retry) {
        result = ov00_021F4248(ov00_021F955C() + 0x194);
    } else {
        u32 id = ov00_0220D294(**(u32 **)(ov00_021F955C() + 4));
        ov00_021F9104(0, id, 0, ov00_021F955C() + 0x194);
        p = ov00_021F955C();
        W(p, 0x18c) = 0;
        W(p, 0x190) = 0;
    }
    return result;
}
