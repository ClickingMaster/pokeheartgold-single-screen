#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
typedef void (*Fn)(void *);

typedef struct {
    Fn fn[5];
} Tab;

typedef struct {
    void *bg;
    void *obj;
    u8 b[16];
} A;

typedef union {
    u8 bytes[72];
    void *words[18];
} B;

typedef struct {
    u8 a, b, c, d;
    u16 pos, pad;
    B *p;
} C;

extern void *ov13_0224128C(int, int);
extern void *ov13_022408A0(int, int);
extern void *ov13_02240184(int, int);
extern void *ov13_02242528(int, void *, int, int);
extern char ov13_02234B48[];
extern char ov13_022350A8[];
extern s8 ov13_022455D4[];
extern u8 ov13_022455F4[];
extern u8 ov13_02245480[];
extern u8 ov13_02245526[];
extern void *ov13_0224F540[];
extern int FX_DivS32(int, int);
extern int swprintf(u16 *, u32, const u16 *, ...);
extern A *ov13_0224F544;
extern C ov13_0224F54C;
extern int ov13_0224F548;
extern Tab ov13_022454AC;
extern u8 ov13_022454C0[];
extern u16 ov13_02245536[];
extern u16 ov13_022464E4[];
extern char ov13_02246440[];
extern char ov13_02246458[];
extern u8 *ov13_02239FF0(void);
extern int ov13_02227228();
extern int ov13_02227258();
extern int ov13_0222726C();
extern int ov13_022272A0();
extern int ov13_022272CC();
extern int ov13_022272E4();
extern int ov13_022277B8();
extern int ov13_0222788C();
extern int ov13_02227924();
extern int ov13_02227964();
extern int ov13_022279B4();
extern int ov13_02227E58();
extern int ov13_0222B8E8();
extern int ov13_0222B8F8();
extern int ov13_0222B934();
extern int ov13_0222B948();
extern int ov13_0222B970();
extern int ov13_02231D9C();
extern int ov13_0223287C();
extern int ov13_02233000();
extern int ov13_022332E8();
extern int ov13_02233810();
extern int ov13_02233AD8();
extern int ov13_022344F0();
extern int ov13_02234700();
extern int ov13_02234974();
extern int ov13_022349B8();
extern int ov13_02234BCC();
extern int ov13_02234D54();
extern int ov13_02235074();
extern int ov13_022350FC();
extern int ov13_022351C0();
extern int ov13_02235260();
extern int ov13_022352C0();
extern int ov13_022368D8();
extern int ov13_022392B4();
extern int ov13_022392CC();
extern int ov13_0223943C();
extern int ov13_0223959C();
extern int ov13_0223A004();
extern int ov13_0223A018();
extern int ov13_0223ABB8();
extern int ov13_0223BE6C();
extern int ov13_0223C198();
extern int ov13_0223C1DC();
extern int ov13_0223C1F0();
extern int ov13_022400F0();
extern int ov13_02240394();
extern int ov13_022403B8();
extern int ov13_022405B8();
extern int ov13_02240B04();
extern int ov13_02240C14();
extern int ov13_02240CC0();
extern int ov13_02240D80();
extern int ov13_02240E2C();
extern int ov13_02240EA8();
extern int ov13_02240EFC();
extern int ov13_022412B0();
extern int ov13_02241694();
extern int ov13_02241D98();
extern int ov13_02242074();
void ov13_022322D8(void);
void ov13_0223237C(void);
void ov13_02232440(void);
void ov13_02232494(void);
void ov13_022324F0(void);
void ov13_022325A4(void);
void ov13_02232770(void);
void ov13_0223279C(void);
void ov13_022327E0(void);
void ov13_022327FC(void);
void ov13_02232BD8(void);
void ov13_02232C0C(void);
void ov13_02232C48(void);
void ov13_02232C70(void);
void ov13_02232C84(void);
void ov13_02232CA4(void);
void ov13_02232CA8(void);
void ov13_02232CD0(void);
void ov13_02232CF0(void);
void ov13_02232D40(void);
void ov13_02232DF4(int x);
void ov13_02232E04(void);
void ov13_02233214(void);
void ov13_02233250(void);
void ov13_022332A4(void);
void ov13_022332D8(void);
int ov13_02233568(void);
void ov13_022335EC(int x);
void ov13_0223376C(u32 x);
void ov13_022337D8(u32 x);
void ov13_02233A6C(void);
void ov13_02233A8C(void);
void ov13_02233DCC(void);
void ov13_02233E8C(int x, int y);
void ov13_02234028(int x, int y);
int ov13_0223417C(int x);
void ov13_02234210(int x, int z, int y);
void ov13_02234314(u8 *p, int y);
void ov13_02234468(u8 *p, int x);

void ov13_02234314(u8 *p, int y) {
    u16 buf[17];
    int n, i, m;
    MI_CpuFill8(buf, 0, 34);
    n = ov13_02242074(p, 32);
    m = n <= 16 ? n : 16;
    for (i = 0; i < m; i++) {
        int c = p[i];
        if (c == 32) {
            buf[i] = 0xe01d;
        } else {
            buf[i] = c;
        }
    }
    y = y * 29 + 2;
    if (n <= 16) {
        y += 5;
    }
    ov13_02240C14(ov13_0224F54C.p->words[5], 72, y, 2, 8, buf, 1);
    if (n <= 16) {
        return;
    }
    MI_CpuFill8(buf, 0, 34);
    n -= 16;
    for (i = 0; i < n; i++) {
        int c = p[i + 16];
        if (c == 32) {
            buf[i] = 0xe01d;
        } else {
            buf[i] = c;
        }
    }
    ov13_02240C14(ov13_0224F54C.p->words[5], 72, y + 12, 2, 8, buf, 1);
}

void ov13_02234468(u8 *p, int x) {
    u16 buf[16];
    swprintf(buf, 16, ov13_022464E4, p[0], p[1], p[2], p[3]);
    ov13_02240C14(ov13_0224F54C.p->words[5], 95, x * 29 + 8, 2, 7, buf, 1);
}
