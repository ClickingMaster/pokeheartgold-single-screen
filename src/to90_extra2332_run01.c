#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    int state, active, error;
    u8 pad[0x44];
    u32 key;
    void *unused, *out;
    u32 unused2;
    void *input;
    u32 len;
} Work;

extern Work ov45_0225762C;
extern char *ov45_02257628;
extern char ov45_02257728[];
extern char ov45_022576DC[];
extern char ov45_02257704[];
extern void *ov00_021EC2A8(int, int);
extern void ov00_021EC2EC(int, void *, int);
extern int ov00_021FFC18(void *, void *, int, int);
extern void ov45_02253D88(u32);
extern u32 ov45_02253D9C(void);
extern int ov45_02253D50(int);
extern void ov45_02253DDC(char *, char *, u32, u32, u32, u32);
extern void ov00_021FA0B4(int);
extern unsigned long strtoul(const char *, char **, int);
int ov45_022542F4(u8 *, u8 *, int, u8 *, int);
void ov45_0225443C(int, char *);
#pragma force_active on

int ov45_022542F4(u8 *out, u8 *first, int a, u8 *second, volatile int b) {
    u8 *p;
    u32 sum = 0;
    int len = b + a;
    int i;
    p = ov00_021EC2A8(7, len + 4);
    if (!p) {
        return 2;
    }
    for (i = 0; i < a; i++) {
        p[i + 4] = first[i];
    }
    {
        int secondLen = b;

        struct Header {
            u8 head[4];
            u8 bytes[1];
        };

        first = (u8 *)0;
        if (secondLen > 0) {
            struct Header *dest = (struct Header *)(p + a);
            do {
                ((struct Header *)((u8 *)dest + (u32)first))->bytes[0] = second[(u32)first];
                first = (u8 *)((u32)first + 1);
            } while ((s32)first < secondLen);
        }
    }
    for (i = 0; i < len; i++) {
        sum += p[i + 4];
    }
    ov45_02253D88(sum);
    for (i = 0; i < len; i++) {
        p[i + 4] ^= ov45_02253D9C();
    }
    sum ^= ov45_0225762C.key;
    p[0] = sum >> 24;
    p[1] = sum >> 16;
    p[2] = sum >> 8;
    p[3] = sum;
    ov00_021FFC18(p, out, len + 4, 2);
    ov00_021EC2EC(7, p, 0);
    out[ov45_02253D50(len + 4)] = 0;
    return 0;
}
