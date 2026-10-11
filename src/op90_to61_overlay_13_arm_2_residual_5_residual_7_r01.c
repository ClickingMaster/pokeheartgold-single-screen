#pragma force_active on
#include "global.h"
#include <nitro/code32.h>

typedef void (*InitFunc)(char *);
typedef struct { InitFunc f[5]; } InitTable;
typedef struct { u8 v[5]; } Bytes5;
typedef struct {
    u32 attr01;
    u16 attr2;
} Oam;
typedef struct {
    void *bg;
    Oam *oam;
    u8 data[12];
    u8 result;
} Work1B18;
void ov13_0223A2B0(char *buf);
void ov13_0223A2F0(char *buf);
void ov13_0223A330(char *buf);
void ov13_0223A374(char *buf);
void ov13_0223A3B8(char *buf);
extern Work1B18 *ov13_0224F544;
extern const u8 ov13_022463E0[];
void *WifiSetupHeap_AllocZeroed(u32 size, u32 align);
void ov13_0222B948(int *out, int a);
void ov13_02231C8C(void);
void WifiSetupBackground_LoadHeader(int a);
void WifiSetupMessage_Show(int a, int b, int c);
void ov13_02227964(int a);
void *WifiSetupText_InitBgCanvas(int a, int b);
Oam *WifiSetupOamTemplate_AllocOam(int a, int b);
void ov13_02232440(void);
void ov13_0223237C(void);
void ov13_0222B8E8(void (*fn)(void));
void ov13_02231D48(void);
void ov13_02231B18(void);
extern const struct S80 { u8 pad[8]; Bytes5 vals; u8 rest[11]; } ov13_02245480;
extern const InitTable ov13_02245498;
// @fn ov13_02231B18
void ov13_02231B18(void) {
    InitTable funcs = ov13_02245498;
    Bytes5 vals = ov13_02245480.vals;
    int idx;
    Work1B18 *work;
    ov13_0224F544 = WifiSetupHeap_AllocZeroed(sizeof(Work1B18), 4);
    ov13_0222B948(&idx, 0);
    funcs.f[idx]((char *)ov13_0224F544->data);
    work = ov13_0224F544;
    if (memcmp(work->data, ov13_022463E0, 3) != 0) {
        work->result = 3;
    } else {
        MI_CpuFill8(work->data, 0, 12);
        ov13_0224F544->result = 0;
    }
    ov13_02231C8C();
    WifiSetupBackground_LoadHeader(idx + 11);
    WifiSetupMessage_Show(vals.v[idx], -1, 0);
    ov13_02227964(2);
    ov13_0224F544->bg = WifiSetupText_InitBgCanvas(0, 0);
    ov13_0224F544->oam = WifiSetupOamTemplate_AllocOam(0, 0x3f);
    ov13_0224F544->oam->attr2 = (ov13_0224F544->oam->attr2 & ~0xc00) | 0xc00;
    ov13_02232440();
    ov13_0223237C();
    ov13_0222B8E8(ov13_02231D48);
}

const struct S80 ov13_02245480 = {{0x32,0x35,0x35,0,0xb,0,0x10,0},{{0x3F,0x40,0x41,0x42,0x43}},{0,0,0,0x29}};
const InitTable ov13_02245498 = {{ov13_0223A2B0, ov13_0223A2F0, ov13_0223A330, ov13_0223A374, ov13_0223A3B8}};
