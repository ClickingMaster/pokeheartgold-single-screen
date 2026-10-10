#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define W(p, n) (*(int *)((u8 *)(p) + (n)))
#define P(p, n) (*(void **)((u8 *)(p) + (n)))
extern char ov38_022257B8[];
extern char ov38_02225644[];
extern char ov38_02225600[];
extern char ov38_02225878[];
extern char ov38_02225C4C[];
extern char ov38_02225B74[];
extern char ov38_02225AE0[];
extern char ov38_02225CA8[];
extern char ov38_02225B00[];
extern char ov38_02225CAC[];
extern u8 ov38_02226050[];
extern char *ov38_02226028;
extern void *ov38_02226078[3];
extern int ov38_0222606C;
extern int ov38_02226074;
extern void (*ov38_02226070)(void *);
extern void *(*ov38_0222605C)(int, int);
void __msl_assertion_failed(char *, char *, char *, int);
int ov38_0221CD90(void *, void *, int);
int ghiPostDoPosting(void *);
int ghiParseURL(void *);
void ghiCallProgressCallback(void *, int, int);
void ghiCallPostCallback(void *);
void ov38_0221E0DC(void *);
void ov00_021FF3CC(void);
int ov00_021FFA44(char *);
void *SOC_GetHostByName(char *);
int ov00_021FF59C(int, int *, int, int);
int ov00_021FFA60(int);
int sscanf(const char *, const char *, ...);
void ov38_02221734(void);
void ov38_02221748(void);
void ov38_02221730(void);
void ov38_02221784(void);
void ov38_022217D0(void);
int ov38_02221788(void);
int ov38_02222C9C(int);
void ov38_02221AC4(void (*)(void));
int ov38_02221870(char *);
void ov38_0222189C(void *, void *, int);
int ov38_022201AC(int);
int ov38_022201C8(char *, char *, int);
int ov38_0221FFC0(int, char *);
void ov38_0221FF7C(void);
extern char ov38_02225B68[];
extern char ov38_02225B48[];
extern char ov38_02225CB0[];
extern char ov38_02225CB8[];
int ghiAppendDataToBuffer(void *, void *, int);
extern char ov38_02225B9C[];
extern char ov38_02225BB0[];
extern char ov38_02225B14[];
extern char ov38_02225BE8[];
extern char ov38_02224E78[];
int ov38_0221C748(void *);
void ghiResetBuffer(void *);
int ov38_0221CB7C(void *, void *, int *);
int ghiParseStatus(void *);
unsigned ov00_021FF368(void);
void *ov00_021EC3A8(int);
void ov00_021EC3D8(void *);
int ghiProcessIncomingFileData(void *, void *, int);
int ov38_02220100(char *, char *, int);
int ov38_02220148(unsigned);
int strncmp(const char *, const char *, size_t);
char *strstr(const char *, const char *);
int ghiPostFileMemoryStateDoPosting(void *s, void *c);
void ov38_0221EBB0(void *c);
void ghiDoPosting(void *c);
void ov38_0221F330(void *c);
int ghiParseChunkSize(void *c);
void ov38_0221FF7C(void);
int ov38_0221FFC0(int i, char *s);
int ov38_02220090(char *a, char *b, char *c);
int ov38_02220100(char *d, char *s, int n);
int ov38_02220148(unsigned x);
int ov38_022201A0(int x);
int ov38_022201AC(int x);
char *ov38_022203E8(char *s);
void ov38_022205DC(void);
void ov38_02220620(void (*f)(void));
int ov38_02220648(void);
int ov38_0221F624(void *c, void *p, int n);
void ghiAppendToChunkHeaderBuffer(void *c, void *p, int n);
void ghiDoSecuringSession(void *c);
void ov38_0221F4FC(void *c);
void ov38_0221FEA8(void *c);
int ov38_022201C8(char *d, char *s, int n);
int ov38_022202C0(char *d, char *s, int n);

void ghiDoPosting(void *c) {
    int r, before = W(c, 0x148);
    r = ghiPostDoPosting(c);
    if (r == 0) {
        if (!W(c, 0xfc) || !W(c, 0x38)) {
            __msl_assertion_failed(ov38_02225C4C, ov38_02225B74, ov38_02225AE0, 0x200);
        }
        ov38_0221E0DC(c);
        return;
    }
    if (before != W(c, 0x148)) {
        ghiCallPostCallback(c);
    }
    if (r != 1) {
        return;
    }
    ov38_0221E0DC(c);
    W(c, 0x10) = 5;
    ghiCallProgressCallback(c, 0, 0);
}

void ov38_0221F330(void *c) {
    int x;
    if (ov00_021FF59C(W(c, 0x48), &x, 0, 0) == -1) {
        W(c, 0xfc) = 1;
        W(c, 0x38) = 5;
        W(c, 0x4c) = ov00_021FFA60(W(c, 0x48));
        return;
    }
    if (x == 0) {
        return;
    }
    W(c, 0x10) = 6;
    ghiCallProgressCallback(c, 0, 0);
}
