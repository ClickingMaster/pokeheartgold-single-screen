#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern char ov45_02256848[];
extern char ov45_022565D0[];
extern char ov45_022562B4[];
void __msl_assertion_failed(const char *cond, const char *file, const char *func, int line);

typedef struct {
    int unk0;
    int unk4;
    int unk8;
} Key;

typedef struct {
    Key key;
    int type;
    int pid;
    int pad;
} Query;

typedef struct {
    int a, b, c, d, e;
} Reply5;

typedef struct {
    int unk0;
    int pid;
} Profile;

typedef struct {
    u8 pad[0x20];
    Profile *profile;
    int state;
} MatchNode;

typedef struct {
    int type;
} Entry;

Entry *ov45_022496AC(void *ctx, int kind, Query *q);
Entry *ov45_0224983C(void *ctx, int pid);
void ov45_02249A0C(void *ctx, Entry *e, void *reply);

static inline void InitKey(Key *k) {
    k->unk0 = 0;
    k->unk4 = 0;
    k->unk8 = 0;
}

void ov45_0224ED80(void *ctx, MatchNode *node);

void ov45_0224ED80(void *ctx, MatchNode *node) {
    Query q;
    Key r3a;
    Key r3b;
    Reply5 r5;
    Entry *e;
    int pid;
    if (node->state != 3) {
        __msl_assertion_failed(ov45_02256848, ov45_022565D0, ov45_022562B4, 0x1090);
    }
    if (node->state != 3) {
        return;
    }
    pid = node->profile->pid;
    {
        int *w = (int *)&q.key;
        w[1] = 0;
        w[0] = 0;
        w[2] = 0;
    }
    q.key.unk0 = 1;
    q.key.unk4 = pid;
    q.type = 5;
    q.pid = pid;
    e = ov45_022496AC(ctx, 2, &q);
    if (e != NULL) {
        if (e->type == 1) {
            r3a.unk0 = 0;
            r3a.unk4 = 1;
            r3a.unk8 = pid;
            ov45_02249A0C(ctx, e, &r3a);
            return;
        }
        if (e->type == 5) {
            r3b.unk0 = 0;
            r3b.unk4 = pid;
            r3b.unk8 = 0;
            ov45_02249A0C(ctx, e, &r3b);
            return;
        }
    }
    e = ov45_0224983C(ctx, pid);
    if (e == NULL) {
        return;
    }
    r5.a = 0;
    r5.b = 0;
    r5.c = 0;
    r5.d = 0;
    r5.e = 0;
    ov45_02249A0C(ctx, e, &r5);
}
