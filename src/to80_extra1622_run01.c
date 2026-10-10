#pragma force_active on
#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define I(p, o) (*(int *)((u8 *)(p) + (o)))
#define U(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define B(p, o) (*((u8 *)(p) + (o)))
#define V(p, o) (*(void **)((u8 *)(p) + (o)))

typedef struct {
    u8 x[13];
} Block13;

typedef struct {
    u8 x[8];
} Block8;

typedef struct {
    u8 x[4];
} Block4;

typedef struct {
    u8 unused, kind;
    u16 port;
    u32 address;
} Address;

extern Block13 ov00_0221A4B0;
extern Block8 ov00_0221A4C0;
extern void ov00_022122F4(void *);
extern u32 ov00_021FF368(void);
extern int ov00_021FF984(int, void *, int, int, Address *, int);
void ov00_02212404(u8 *, u8 *);

void ov00_02212404(u8 *p, u8 *q) {
    Address addr;
    u8 data[256];
    int size, i;
    ov00_022122F4(p + 8);
    U(q, 28) = ov00_021FF368();
    addr.kind = 2;
    if (B(q, 20) & 32) {
        return;
    }
    if (I(p, 0) == 1) {
        data[0] = 254;
        data[1] = 253;
        data[2] = 0;
        memcpy(data + 3, q + 28, 4);
        if (B(q, 20) & 4) {
            data[7] = I(p, 64);
            i = 0;
            if (I(p, 64) > 0) {
                u8 *out = data;
                do {
                    out[8] = B(p + i, 44);
                    i++;
                    out++;
                } while (i < I(p, 64));
            }
            data[8 + I(p, 64)] = 0;
            data[9 + I(p, 64)] = 0;
            size = I(p, 64) + 10;
        } else {
            data[7] = 255;
            data[8] = 255;
            data[9] = 255;
            size = 10;
        }
    } else if (B(q, 20) & 4) {
        memcpy(data, &ov00_0221A4B0, 13);
        size = 13;
    } else {
        memcpy(data, &ov00_0221A4C0, 8);
        size = 8;
    }
    size = size;
    u32 ip = U(q, 0);
    if (ip == U(p, 40) && (B(q, 21) & 2)) {
        addr.address = U(q, 8);
        addr.port = H(q, 12);
    } else {
        addr.address = ip;
        addr.port = H(q, 4);
    }
    ov00_021FF984(I(p, 32), data, size, 0, &addr, 8);
}
