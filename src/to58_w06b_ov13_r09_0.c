#include "global.h"
extern int SOC_Cleanup(void);
extern int ov13_02222D78(void);
extern u32 ov13_02222A1C(u32);
extern int SOC_Startup(void *);
extern void OS_Sleep(u32);
extern u8 ov13_022459B4[];
extern u8 ov13_022459BC[];
extern volatile u32 _021D4670;
int ov13_022228CC(u32, u32, u32);
int ov13_022228CC(u32 a, u32 b, u32 c) {
    *(u32 *)(ov13_022459B4 + 0x18) = ov13_02222A1C(a);
    *(u32 *)(ov13_022459B4 + 0x1C) = ov13_02222A1C(b);
    *(u32 *)(ov13_022459B4 + 0x20) = ov13_02222A1C(c);
    if (SOC_Startup(ov13_022459BC) < 0) return -1;
    while (_021D4670 == 0) OS_Sleep(100);
    return 0;
}
int ov13_02222924(void);
int ov13_02222948(const u8 *a, const u8 *b, int n);
int ov13_02222924(void) { if (SOC_Cleanup() < 0) return -1; return -!!ov13_02222D78(); }
int ov13_02222948(const u8 *a, const u8 *b, int n) {
    int diff = 0;
    goto check;
    do {
        a++;
        b++;
    check:
        if (n-- <= 0) break;
        diff = *a - *b;
    } while (diff == 0);
    return diff;
}
