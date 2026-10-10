#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#pragma inlineintrinsics on

typedef struct {
    char *a, *b, *c, *d, *e, *f;
    int age;
    char tag[11], region[3];
    u8 pad[2];
    float x, y;
    char description[128];
    int flagsA, flagsB, flagsC, selector, unknown;
    char *nickname;
    int values[9];
} Parsed;

typedef struct Profile {
    u8 p[0x18];
    char *text;
} Profile;

typedef struct Job {
    int state;
    u8 p[8];
    int id;
    u8 q[0x3c - 0x10];
    struct Job *next;
} Job;

typedef struct {
    u8 p[0x100];
    int flag;
    u8 q[0x434 - 0x104];
    Job *jobs;
} Root;

typedef struct {
    Root *root;
} Context;

typedef struct {
    u8 p[12];
    void *callback;
    void *arg;
} Request;

typedef struct {
    void *callback, *arg;
} CB;

typedef struct {
    int state, id;
    u8 p[0x204 - 8];
} Block;

extern char ov00_02218B74[];
extern char ov00_02218B7C[];
extern char ov00_02218BAC[];
extern char ov00_02218BB8[];
extern char ov00_02218B40[];
extern char ov00_02218B2C[];
extern char ov00_02218BC8[];
extern char ov00_02218BD0[];
extern char ov00_02218BE0[];
extern char ov00_02218BE8[];
extern char ov00_02218BF4[];
extern char ov00_02218C00[];
extern char ov00_02218C0C[];
extern char ov00_02218C18[];
extern char ov00_02218C24[];
extern char ov00_02218C34[];
extern char ov00_02218C3C[];
extern char ov00_02218C44[];
extern char ov00_02218C4C[];
extern char ov00_02218C58[];
extern char ov00_02218C60[];
extern char ov00_02218C68[];
extern char ov00_02218C70[];
extern char ov00_02218C78[];
extern char ov00_02218C80[];
extern char ov00_02218C88[];
extern char ov00_02218C90[];
extern char ov00_02218C98[];
extern char ov00_02218CA0[];
extern char ov00_02218CA8[];
extern char ov00_02218CB0[];
extern char ov00_02218CB8[];
extern char ov00_02218CC0[];
extern int ov00_0220A3C8(Context *, char *, int);
extern int strncmp(const char *, const char *, u32);
extern void ov00_0220A86C(Context *, int, char *);
extern void ov00_02202990(Context *, int, int);
extern int ov00_0220A4A0(char *, char *, char *, int);
extern int atol(const char *);
extern double atod(const char *);
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov00_02207DD0(Context *, int, Profile **);
extern int ov00_022045AC(Context *, int, int *, int *, int *);
extern Profile *ov00_02207CEC(Context *, int);
extern void ov00_021EC3D8(void *);
extern char *ov00_021FF3D4(char *);
extern void ov00_0220608C(Context *, Profile *, Parsed *);
extern Block *ov00_021EC3A8(int);
extern void ov00_0220A890(Context *, char *);
extern void ov00_02204654(Parsed *, Block *);
extern int ov00_02202A84(Context *, CB, Block *, Request *, int);
extern void ov00_022066D0(Context *, Request *);
extern void *memset(void *, int, u32);
#define ERROR()                               \
    do {                                      \
        ov00_0220A86C(ctx, 1, ov00_02218B7C); \
        ov00_02202990(ctx, 3, 1);             \
        return 3;                             \
    } while (0)
int ov00_0220484C(Context *, Request *, char *);

int ov00_0220484C(Context *ctx, Request *request, char *input) {
    Parsed parsed;
    char str6[76], temp[64], str1[31], str2[21], str3[51], str4[31], str5[31], nickname[51];
    Profile *profile;
    Root *root = ctx->root;
    Job *job;
    int changed, result, id;
    CB cb;
    Block *block;
    if (ov00_0220A3C8(ctx, input, 1)) {
        return 4;
    }
    if (strncmp(input, ov00_02218B74, 4)) {
        ERROR();
    }
    if (!ov00_0220A4A0(input, ov00_02218BAC, temp, 64)) {
        ERROR();
    }
    id = atol(temp);
    if (id <= 0) {
        __msl_assertion_failed(ov00_02218BB8, ov00_02218B40, ov00_02218B2C, 346);
    }
    ov00_02207DD0(ctx, id, &profile);
    memset(&parsed, 0, sizeof(parsed));
    parsed.a = str1;
    parsed.b = str2;
    parsed.c = str3;
    parsed.d = str4;
    parsed.e = str5;
    parsed.f = str6;
    parsed.nickname = nickname;
    if (!ov00_0220A4A0(input, ov00_02218BC8, parsed.a, 31)) {
        parsed.a[0] = 0;
    }
    if (!ov00_0220A4A0(input, ov00_02218BD0, parsed.b, 21)) {
        parsed.b[0] = 0;
    }
    if (!ov00_0220A4A0(input, ov00_02218BE0, parsed.c, 51)) {
        parsed.c[0] = 0;
    }
    if (!ov00_0220A4A0(input, ov00_02218BE8, parsed.d, 31)) {
        parsed.d[0] = 0;
    }
    if (!ov00_0220A4A0(input, ov00_02218BF4, parsed.e, 31)) {
        parsed.e[0] = 0;
    }
    if (!ov00_0220A4A0(input, ov00_02218C00, temp, 64)) {
        parsed.age = -1;
    } else {
        parsed.age = atol(temp);
    }
    if (!ov00_0220A4A0(input, ov00_02218C0C, parsed.f, 76)) {
        parsed.f[0] = 0;
    }
    if (!ov00_0220A4A0(input, ov00_02218C18, parsed.tag, 11)) {
        parsed.tag[0] = 0;
    }
    if (!ov00_0220A4A0(input, ov00_02218C24, parsed.region, 3)) {
        parsed.region[0] = 0;
    }
    if (!ov00_0220A4A0(input, ov00_02218C34, temp, 64)) {
        parsed.x = 0;
    } else {
        parsed.x = atod(temp);
    }
    if (!ov00_0220A4A0(input, ov00_02218C3C, temp, 64)) {
        parsed.y = 0;
    } else {
        parsed.y = atod(temp);
    }
    if (!ov00_0220A4A0(input, ov00_02218C44, parsed.description, 128)) {
        parsed.description[0] = 0;
    }
    if (!ov00_0220A4A0(input, ov00_02218C4C, temp, 64)) {
        parsed.flagsA = 0;
        parsed.flagsB = 0;
        parsed.flagsC = 0;
    } else {
        result = ov00_022045AC(ctx, atol(temp), &parsed.flagsA, &parsed.flagsB, &parsed.flagsC);
        if (result) {
            return result;
        }
    }
    if (!ov00_0220A4A0(input, ov00_02218C58, temp, 64)) {
        parsed.selector = 1282;
    } else if (temp[0] == '0') {
        parsed.selector = 1280;
    } else if (temp[0] == '1') {
        parsed.selector = 1281;
    } else {
        parsed.selector = 1282;
    }
    if (!ov00_0220A4A0(input, ov00_02218C60, temp, 64)) {
        parsed.unknown = -1;
    } else {
        parsed.unknown = atol(temp);
    }
    if (!ov00_0220A4A0(input, ov00_02218C68, parsed.nickname, 51)) {
        parsed.nickname[0] = 0;
    }
    if (!ov00_0220A4A0(input, ov00_02218C70, temp, 64)) {
        parsed.values[0] = 0;
    } else {
        parsed.values[0] = atol(temp);
    }
    if (!ov00_0220A4A0(input, ov00_02218C78, temp, 64)) {
        parsed.values[1] = 0;
    } else {
        parsed.values[1] = atol(temp);
    }
    if (!ov00_0220A4A0(input, ov00_02218C80, temp, 64)) {
        parsed.values[2] = 0;
    } else {
        parsed.values[2] = atol(temp);
    }
    if (!ov00_0220A4A0(input, ov00_02218C88, temp, 64)) {
        parsed.values[3] = 0;
    } else {
        parsed.values[3] = atol(temp);
    }
    if (!ov00_0220A4A0(input, ov00_02218C90, temp, 64)) {
        parsed.values[4] = 0;
    } else {
        parsed.values[4] = atol(temp);
    }
    if (!ov00_0220A4A0(input, ov00_02218C98, temp, 64)) {
        parsed.values[5] = 0;
    } else {
        parsed.values[5] = atol(temp);
    }
    if (!ov00_0220A4A0(input, ov00_02218CA0, temp, 64)) {
        parsed.values[6] = 0;
    } else {
        parsed.values[6] = atol(temp);
    }
    if (!ov00_0220A4A0(input, ov00_02218CA8, temp, 64)) {
        parsed.values[7] = 0;
    } else {
        parsed.values[7] = atol(temp);
    }
    if (!ov00_0220A4A0(input, ov00_02218CB0, temp, 64)) {
        parsed.values[8] = 0;
    } else {
        parsed.values[8] = atol(temp);
    }
    if (!ov00_0220A4A0(input, ov00_02218CB8, temp, 64)) {
        ERROR();
    }
    job = root->jobs;
    changed = root->flag;
    while (job) {
        if (job->id == id && job->state == 101) {
            if (!profile) {
                profile = ov00_02207CEC(ctx, id);
            }
            job->state = 102;
            changed = 1;
        }
        job = job->next;
    }
    if (!profile && root->flag) {
        profile = ov00_02207CEC(ctx, id);
    }
    if (changed) {
        ov00_021EC3D8(profile->text);
        profile->text = 0;
        profile->text = ov00_021FF3D4(temp);
    }
    if (root->flag) {
        ov00_0220608C(ctx, profile, &parsed);
    }
    cb.callback = request->callback;
    cb.arg = request->arg;
    if (cb.callback) {
        block = ov00_021EC3A8(516);
        if (!block) {
            ov00_0220A890(ctx, ov00_02218CC0);
            return 1;
        }
        ov00_02204654(&parsed, block);
        block->state = 0;
        block->id = id;
        result = ov00_02202A84(ctx, cb, block, request, 0);
        if (result) {
            return result;
        }
    }
    ov00_022066D0(ctx, request);
    return 0;
}
