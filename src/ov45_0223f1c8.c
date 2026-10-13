#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#define W(p, n) (*(int *)((u8 *)(p) + (n)))
#define U(p, n) (*(unsigned *)((u8 *)(p) + (n)))
#define P(p, n) (*(void **)((u8 *)(p) + (n)))

typedef union {
    struct {
        u32 flag : 1;
        u32 pad : 31;
        u32 len;
        char *data;
    } b;

    struct {
        char head;
        char inlinebuf[11];
    } s;
} Str;

extern void *ov45_022577DC;
extern char ov45_022552DC[];
extern char ov45_022552E4[];
extern char ov45_022552EC[];
extern char ov45_022552F0[];
extern char ov45_022552FC[];
void LobbyString_InitFromCStr(Str *, char *);
void LobbyString_Destroy(Str *);
void LobbyString_InitCopy(void *, void *);
void ov45_022376BC(void *);
void ov45_02232D8C(void *, int *, int);
void ov45_02242858(void *);
void DWC_Free(int, void *, int);
void ov45_022436E0(void *, Str *, Str *);
void ov45_0224335C(void *, Str *);
void LobbyString_AssignCStr(Str *, char *);
void *ov45_02242C3C(void *, Str *);
void ov45_022428D8(Str *, void *, Str *);
void ov45_022428C8(Str *, void *, Str *);
int chatInChannelA(void *, char *);
void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void *LobbyMem_Alloc(unsigned);
void *ov45_0223EA34(void *, unsigned, char *);
void ov45_022421E4(void *, unsigned, void *);
char *strncpy(char *, const char *, size_t);
int ov45_022435F8(void *, Str *);
void *ov45_0223EC64(void *p);
void ov45_0223EC80(void *p, unsigned n);
void *ov45_0223ECB0(void *p);
void ov45_0223ED00(void);
void ov45_0223F1C8(void);
void ov45_0223F8F4(void);
void ov45_0223F910(void);
void ov45_0223ED1C(void);
void ov45_0223F2D8(void *a, char *s);
void ov45_0223F6F4(void *a, void *b, char *c, char *d);
void ov45_0223F768(void *a, void *b, void *c, unsigned mode);
void ov45_0223F83C(void *a, char *s, void *b);
void ov45_0223F92C(void *a, int n);
void ov45_0223F9C4(void *a, void *b, char *out);
void ov45_0223FA2C(void *out, void *p);
void ov45_0223FC5C(void *out, void *p);
void ov45_02240F6C(void *out, void *p);
void ov45_02240F7C(void *out, void *p);
int ov45_0223FD6C(void *p);
void *ov45_0223FD74(void *p);
void ov45_0223FD88(void);
void *ov45_0223FDE8(void *c, void *key, void *node, void *best);
void ov45_0223FD8C(void **out, void *tree, void *key);
void ov45_0223FE20(void *a, int n);
void ov45_0223FFE8(void *p, u32 *first, u32 *last);
int ov45_02240850(void);
int ov45_02240A58(void);
void *ov45_02240A74(void *p);
int ov45_02240A80(char *s);
void ov45_0223F1E4(void *a, char *s, char *name, void *arg, int mode);
void ov45_02240620(void *a, char *s, char *name, void *arg, void *extra);

void ov45_0223F1C8(void) {
    if (ov45_022577DC && W(ov45_022577DC, 4) == 5) {
        return;
    }
}

void ov45_0223F1E4(void *a, char *s, char *name, void *arg, int mode) {
    Str x, y, z, t;
    int id;
    void *p;
    if (ov45_022577DC && W(ov45_022577DC, 4) == 5) {
        return;
    }
    if (mode != 3) {
        return;
    }
    LobbyString_InitFromCStr(&x, name);
    id = ov45_022435F8(ov45_022577DC, &x);
    LobbyString_Destroy(&x);
    LobbyString_InitFromCStr(&y, s);
    p = ov45_02242C3C(ov45_022577DC, &y);
    LobbyString_Destroy(&y);
    LobbyString_InitFromCStr(&z, s);
    ov45_022428D8(&t, ov45_022577DC, &z);
    ((void (*)(char *, int, void *, void *))P(p, 8))(!t.b.flag ? t.s.inlinebuf : t.b.data, id, arg, P(p, 20));
    LobbyString_Destroy(&t);
    LobbyString_Destroy(&z);
}

void ov45_0223F2D8(void *a, char *s) {
    Str t;
    if (ov45_022577DC && W(ov45_022577DC, 4) == 5) {
        return;
    }
    LobbyString_InitFromCStr(&t, s);
    ov45_0224335C(ov45_022577DC, &t);
    LobbyString_Destroy(&t);
}
