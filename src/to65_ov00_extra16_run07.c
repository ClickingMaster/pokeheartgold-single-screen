#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#define W(p, o) (*(int *)((char *)(p) + (o)))
#define P(p, o) (*(void **)((char *)(p) + (o)))
#define B(p, o) (*(u8 *)((char *)(p) + (o)))
#define A(p, o) ((void *)((char *)(p) + (o)))

typedef struct {
    char pad[0xf4];
    int ids[1];
} IdList;

extern void *ov00_021F955C(void);
extern void ov00_021F6100(int);
extern void ov00_02210CBC(void *);
extern void ov00_021F954C(int);
extern void ov00_021F7AD8(void);
extern void ov00_021F125C(void);
extern void ov00_021F956C(int);
extern void ov00_022136E4(void *);
extern void ov00_021EC2EC(int, void *, int);
extern void ov00_021F381C(int);
extern void ov00_02212008(int, char *);
extern int ov00_021F3398(int, int);
extern char ov00_0221A928[];
extern char ov00_0221B080[];
extern char ov00_02217AAC[];
extern char ov00_02217AB4[];
extern char ov00_02217AC0[];
extern char ov00_02217ACC[];
extern char ov00_02217AD8[];
extern void ov00_021F2EC4(void);
extern void ov00_021F2FA0(void);
extern void ov00_021F3310(int n);
extern int ov00_021F3334(int x, int n);
extern int ov00_021F3550(void);
extern int ov00_021F3570(void);
extern void ov00_021F3590(void);
extern int ov00_021F35E4(void **out);
extern int ov00_021F3618(void **out);
extern void ov00_021F378C(void);
extern int ov00_021F3804(void);
extern void ov00_021F3BB4(int a, int b, void *c, void *d);
extern void ov00_021F3C5C(void);

#include <string.h>
extern char *strchr(const char *, int);
extern unsigned long strtoul(const char *, char **, int);
extern int ov00_021F4740(int, void *, int, int, u32 *, int);
extern int ov00_021F04A0(int, void *, int);
extern void ov00_021ED470(char *, char *, char *, int);
extern void ov00_021ED4A8(char *, char *, char *, int);
extern int ov00_02200880(void *, void *, void *);
extern char ov00_02217A64[];
extern char ov00_02217AA4[];
extern char ov00_02217AA8[];
extern char ov00_02217A70[];
extern char ov00_02217AF8[];
extern char ov00_02217B6C[];
extern char ov00_02217B5C[];
extern char ov00_02217B64[];
extern int ov00_021F46A4(char *, char *, int);
extern int ov00_021F2E40(int unused, void *x, u8 *s);
extern int ov00_021F36BC(void);
extern int ov00_021F3EF4(void *out, int a, int b, int c);
extern int ov00_021F45E0(void *x, int id, void *y, char *text);
extern int ov00_021F46A4(char *out, char *s, int index);
extern void ov00_021F7128(void);
extern void ov00_021EC24C(void *, int);
extern int ov00_021F0404(void *);
extern char ov00_02217A60[];
extern void ov00_021F2EE4(void *, int);

int ov00_021F46A4(char *out, char *s, int index) {
    char *end = strchr(s, 0), *stop;
    int i, n;
    for (i = 0; i < index; i++) {
        stop = strchr(s, 47);
        if (!stop) {
            return -1;
        }
        s = stop + 1;
    }
    stop = strchr(s, 47);
    if (!stop) {
        stop = end;
    }
    if (s == stop) {
        return -1;
    }
    n = stop - s;
    MI_CpuCopy8(s, out, n);
    out[n] = 0;
    return n;
}
