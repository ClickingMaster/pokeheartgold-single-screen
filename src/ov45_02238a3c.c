#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>
#pragma force_active on
#pragma warn_uninitializedvar off
#define V(p, o) (*(volatile u32 *)((p) + (o)))
#define W(p, o) (*(u32 *)((p) + (o)))
#define H(p, o) (*(u16 *)((p) + (o)))

typedef union S {
    struct {
        u32 flag : 1;
        u32 cap : 31;
        u32 len;
        char *ptr;
    } l;

    struct {
        u8 flag : 1;
        u8 len : 7;
        char data[11];
    } s;
} S;

typedef struct Vec {
    S *data;
    u32 len, cap, z, extra;
} Vec;

typedef struct Node {
    void *left, *right;
    u32 parent;
    u32 key;
} Node;

typedef struct Small {
    u8 x;
} Small;

typedef struct E {
    int key, val;
} E;

extern void ov45_02237800(Vec *, u32, Small);
extern int LobbyString_CompareChars(const u8 *, u32, u32, const void *, u32);
extern S *LobbyString_ReplaceRange(S *, u32, u32, char *, char *, Small);
extern void *LobbyMem_Alloc(u32);
extern void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
extern u8 *LobbyByteVector_Data(S *);
extern int DWC_Free();
extern int LobbyCharVector_Grow();
extern int LobbyString_Destroy();
#include "overlay45_string_search.h"
void LobbyString_Compare(S *p, S *q);
void LobbyString_CompareCStr(S *p, char *q);
void *LobbyTimerMap_FindInsertPos(Node *p, u32 *key, Node **out, u8 *less, u8 *empty);
void LobbyString_AssignRange(S *p, char *a, char *b);
void ov45_02238558(void *ctx, Node *p);
void ov45_022387E4(Vec *p, u32 n, Small tag);
void ov45_02238ACC(Vec *p, u32 n, Small tag);
Vec *ov45_0223895C(Vec *p);
Vec *ov45_022389B0(Vec *p);
void ov45_022389CC(Vec *p, u32 n, Small tag);
void ov45_02238A3C(Vec *p, u32 n, Small tag);
u8 *ov45_02238B08(u8 *p);
S *ov45_02238C08(Vec *p);
void ov45_02238C1C(S *p, u8 *q);
int PPWLobbySchedule_Copy(u8 *p, u8 *out, u32 *size);
u32 ov45_022385A4(Vec *p, u32 n, ...);
void LobbySort_LowerBound(E **out, E *first, E *last, int *key, ...);
void LobbySort_UpperBound(E **out, E *first, E *last, int *key, ...);
void ov45_022383E0(Vec *p, char *first, char *last);
u8 *ov45_02238508(u8 *p);
u8 *ov45_022389EC(u8 *p);
Vec *ov45_02238A74(Vec *p);

#pragma optimization_level 3

void ov45_02238A3C(Vec *p, u32 n, Small tag) {
    if (n > 0x15555555) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    p->data = LobbyMem_Alloc(n * 12);
    p->cap = n;
}
