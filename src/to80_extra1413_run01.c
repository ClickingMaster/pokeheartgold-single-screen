#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern u32 wcslen(const u16 *);
extern int ov00_021E99A4(void *, char *, void *, int);
extern int ov00_021E98C8(void *, char *, void *);
extern int ov00_021E9B44(void *, void *, int);
extern int ov00_021E9C84(void *, int);
extern int ov00_021E9808(void *);

struct Params {
    int arg, a, b;
    void *(*alloc)(char *, int);
    int p[3];
};

struct Work {
    u8 base[0x1000];
    u8 flag;
    u8 pad[3];
    struct Params params;
    int status;
    u8 gap[0x9A8];
    void *a, *b;
    u8 more[0x5C];
    int one, two;
};

extern char ov00_02216680[];
extern char ov00_02216698[];
int ov00_021E8C5C(void *, char *, int), ov00_021E8FA0(struct Work *, struct Params *);
extern char ov00_022165A0[];
extern char ov00_022165AC[];
extern char ov00_022165B4[];
extern char ov00_022165BC[];
extern char ov00_022165C4[];
extern char ov00_022165CC[];
extern char ov00_022165D4[];
extern char ov00_022165DC[];
extern char ov00_022165E4[];
extern char ov00_022165EC[];
extern char ov00_022165F4[];
extern char ov00_022165FC[];
extern char ov00_02216604[];
extern char ov00_0221660C[];
extern char ov00_02216614[];
extern char ov00_0221661C[];
extern char ov00_02216634[];
extern char ov00_02216640[];

int ov00_021E8C5C(void *w, char *info, int mode) {
    char temp[33];
    OS_SNPrintf(temp, 33, ov00_022165A0, 2, 2);
    if (ov00_021E99A4(w, ov00_022165AC, temp, strlen(temp))) {
        return 0;
    }
    if (ov00_021E99A4(w, ov00_022165B4, info + 0, strlen(info + 0))) {
        return 0;
    }
    if (ov00_021E99A4(w, ov00_022165BC, info + 14, strlen(info + 14))) {
        return 0;
    }
    if (ov00_021E99A4(w, ov00_022165C4, info + 65, strlen(info + 65))) {
        return 0;
    }
    if (ov00_021E99A4(w, ov00_022165CC, info + 111, strlen(info + 111))) {
        return 0;
    }
    if (ov00_021E99A4(w, ov00_022165D4, info + 21, strlen(info + 21))) {
        return 0;
    }
    if (ov00_021E99A4(w, ov00_022165DC, info + 26, strlen(info + 26))) {
        return 0;
    }
    if (ov00_021E99A4(w, ov00_022165E4, info + 29, strlen(info + 29))) {
        return 0;
    }
    if (ov00_021E99A4(w, ov00_022165EC, info + 31, strlen(info + 31))) {
        return 0;
    }
    if (ov00_021E99A4(w, ov00_022165F4, info + 44, strlen(info + 44))) {
        return 0;
    }
    if (ov00_021E99A4(w, ov00_022165FC, info + 47, strlen(info + 47))) {
        return 0;
    }
    if (ov00_021E99A4(w, ov00_02216604, info + 52, strlen(info + 52))) {
        return 0;
    }
    if (ov00_021E99A4(w, ov00_0221660C, info + 126, wcslen((u16 *)(info + 126)) * 2)) {
        return 0;
    }
    if (mode == 1) {
        if (ov00_021E99A4(w, ov00_02216614, info + 78, strlen(info + 78))) {
            return 0;
        }
    }
    OS_SNPrintf(temp, 33, ov00_0221661C, 2, 2);
    if (ov00_021E98C8(w, ov00_02216634, temp)) {
        return 0;
    }
    return ov00_021E98C8(w, ov00_02216640, info + 21) == 0;
}

int ov00_021E8FA0(struct Work *w, struct Params *p) {
    void *(*alloc)(char *, int) = p->alloc;
    MI_CpuFill8(w, 0, 0x1C14);
    w->one = -1;
    w->two = -1;
    w->params = *p;
    w->a = alloc(ov00_02216680, 0xB68);
    if (!w->a) {
        w->status = 1;
        return 1;
    }
    w->b = alloc(ov00_02216698, 0x5EA);
    if (!w->b) {
        w->status = 1;
        return 1;
    }
    if (!ov00_021E9B44(w, (u8 *)w + 0x1A08, w->params.b)) {
        w->status = 1;
        return 1;
    }
    if (!ov00_021E9C84(w, p->arg)) {
        w->status = 1;
        return 1;
    }
    w->status = ov00_021E9808(w);
    if (!w->status) {
        w->flag = 255;
    }
    return w->status;
}
