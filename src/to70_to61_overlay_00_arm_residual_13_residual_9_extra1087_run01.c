#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u32 ov00_0221A0B8[];
void ov00_02212008(int id, u32 value);
extern int sprintf(char *, const char *, ...);

typedef struct {
    char data[2048];
    int count;
} Buffer;

extern int ov00_0221B878;
extern u32 ov00_0221B87C[];
extern char ov00_02219F24[];
extern char ov00_02219F30[];
extern char ov00_02219F3C[];
extern char ov00_02219F44[];
extern char ov00_02219F48[];
extern char ov00_02219F4C[];
extern char ov00_02219F5C[];
extern char ov00_02219F68[];
extern char ov00_02219F74[];
extern void ov00_022111D8(Buffer *, int, void *);
extern void ov00_02210DFC(Buffer *, const char *);
extern void ov00_02210DC8(Buffer *, int);
extern void ov00_022115B0(void *, Buffer *, int, int, int, int, int, int);
extern void ov00_021FF984(u32, void *, int, int, void *, int);
extern char *SOC_InetNtoA(u32);
extern int ov00_021FF368(void);
void ov00_02211DFC(u8 *, int);

void ov00_02211DFC(u8 *w, int mode) {
    Buffer b;
    char text[20];
    int i;
    b.count = 0;
    ov00_022111D8(&b, 3, w + 0x84);
    for (i = 0; i < ov00_0221B878; i++) {
        sprintf(text, ov00_02219F24, i);
        ov00_02210DFC(&b, text);
        ov00_02210DFC(&b, SOC_InetNtoA(ov00_0221B87C[i]));
    }
    ov00_02210DFC(&b, ov00_02219F30);
    ov00_02210DC8(&b, *(int *)(w + 0xc0));
    ov00_02210DFC(&b, ov00_02219F3C);
    ov00_02210DFC(&b, *(int *)(w + 0xc8) ? ov00_02219F44 : ov00_02219F48);
    if (mode) {
        ov00_02210DFC(&b, ov00_02219F4C);
        ov00_02210DC8(&b, mode);
    }
    ov00_02210DFC(&b, ov00_02219F5C);
    ov00_02210DFC(&b, (char *)w + 4);
    if (*(int *)(w + 0xa8)) {
        ov00_02210DFC(&b, ov00_02219F68);
        ov00_02210DC8(&b, *(int *)(w + 0x104));
        ov00_02210DFC(&b, ov00_02219F74);
        ov00_02210DC8(&b, *(u16 *)(w + 0x108));
    }
    if (mode != 2) {
        ov00_022115B0(w, &b, 255, 0, 255, 0, 255, 0);
    } else if (2048 - b.count >= 1) {
        int n = b.count;
        b.count = n + 1;
        b.data[n] = 0;
    }
    ov00_021FF984(*(u32 *)w, b.data, b.count, 0, w + 0xcc, 8);
    {
        int now = ov00_021FF368();
        *(int *)(w + 0xac) = now;
        *(int *)(w + 0xb0) = now;
    }
    if (mode) {
        *(int *)(w + 0xb4) = 0;
    }
}

void ov00_02212008(int id, u32 value) {
    if (id < 50 || id > 254) {
        return;
    }
    ov00_0221A0B8[id] = value;
}
