#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
extern int strncmp(const char *, const char *, unsigned long);
extern char *strstr(const char *, const char *);
extern int sprintf(char *, const char *, ...);
#define W(p, o) (*(int *)((u8 *)(p) + (o)))
#define S(p, o) ((char *)(p) + (o))

typedef struct {
    int a, b;
} Pair;

extern char ov00_022188D0[];
extern char ov00_022188D8[];
extern char ov00_022188E8[];
extern char ov00_0221891C[];
extern char ov00_02218924[];
extern char ov00_02218930[];
extern char ov00_0221893C[];
extern char ov00_0221894C[];
extern char ov00_02218954[];
extern char ov00_02218958[];
extern char ov00_02218964[];
extern char ov00_02218970[];
extern char ov00_0221897C[];
extern char ov00_02218988[];
extern char ov00_02218990[];
extern char ov00_0221899C[];
extern char ov00_0221B16C[];
extern char ov00_022189A8[];
extern char ov00_022189B8[];
extern char ov00_022189C0[];
extern void ov00_02202E08(char *, int);
extern void ov00_021FF31C(char *, int, char *);
extern void ov00_022020EC(void *, void *, char *);
extern void ov00_0220211C(void *, void *, int);
extern int ov00_02207EB4(void *, char *, char *, Pair **);
int ov00_02203428(void *, void *);
extern char ov00_02218A00[];
extern char ov00_02218A08[];
extern char ov00_02218A10[];
extern char ov00_02218A18[];
extern char ov00_02218A48[];
extern char ov00_02218A50[];
extern char ov00_02218A80[];
extern char ov00_02218A88[];
extern char ov00_02218A94[];
extern char ov00_02218A9C[];
extern char ov00_02218AA4[];
extern char ov00_022188C0[];
extern int ov00_0220A3C8(void *, char *, int);
extern int ov00_0220A4A0(char *, char *, char *, int);
extern int ov00_022037CC(void *, void *);
extern int ov00_02202A84(void *, Pair, void *, void *, int);
extern int ov00_02203428(void *, void *);
extern void ov00_02207E0C(void *, int);
extern void ov00_0220A86C(void *, int, void *);
extern void ov00_02202990(void *, int, int);
extern void ov00_0220A348(void *, void *, int);
extern void ov00_022066D0(void *, void *);
extern void ov00_0220A890(void *, char *);
extern void *ov00_02207CEC(void *, int);
extern void *ov00_021EC3A8(int);
extern long atol(const char *);

typedef struct {
    u32 pad;
    void *req;
    u32 pad2, a, b;
    int state;
} Job;

typedef struct {
    int a, b;
    char code[21];
    u8 pad[3];
} Object;

int ov00_02203A58(void *, Job *, char *);
#define ERROR(sym)                  \
    {                               \
        ov00_0220A86C(ctx, 1, sym); \
        ov00_02202990(ctx, 3, 1);   \
        return 3;                   \
    }

int ov00_02203A58(void *ctx, Job *job, char *input) {
    void *req;
    void *c = *(void **)ctx;

    struct {
        char code[21], hash[33], auth[82], text[512];
    } tmp;

    Pair callback;
    if (ov00_0220A3C8(ctx, input, 0)) {
        if (W(c, 0x418) == 0x106 && W(c, 0x1a0)) {
            ov00_02207E0C(ctx, W(c, 0x1a0));
            W(c, 0x19c) = 0;
            W(c, 0x1a0) = 0;
        } else if (W(c, 0x418) == 0x201) {
            if (ov00_0220A4A0(input, ov00_02218A00, tmp.text, 512)) {
                W(c, 0x1a0) = atol(tmp.text);
            }
        }
        if (strstr(input, ov00_02218A08)) {
            ov00_0220A86C(ctx, W(c, 0x418), c);
            ov00_02202990(ctx, 4, 1);
            return 4;
        } else {
            ov00_0220A86C(ctx, W(c, 0x418), c);
            ov00_02202990(ctx, 4, 0);
            return 4;
        }
    }
    req = job->req;
    switch (job->state) {
    case 1:
        if (strncmp(input, ov00_02218A10, 5)) {
            ERROR(ov00_02218A18);
        }
        if (!ov00_0220A4A0(input, ov00_02218924, req, 128)) {
            ERROR(ov00_02218A18);
        }
        if (W(req, 0x304)) {
            int result = ov00_022037CC(ctx, req);
            if (result) {
                return result;
            }
            job->state = 3;
        } else {
            int result = ov00_02203428(ctx, req);
            if (result) {
                return result;
            }
            job->state = 2;
        }
        break;
    case 3:
        if (strncmp(input, ov00_02218A48, 5)) {
            ERROR(ov00_02218A18);
        }
        if (!ov00_0220A4A0(input, ov00_02218958, tmp.text, 512)) {
            ERROR(ov00_02218A50);
        }
        W(c, 0x19c) = atol(tmp.text);
        if (!ov00_0220A4A0(input, ov00_02218964, tmp.text, 512)) {
            ERROR(ov00_02218A50);
        }
        W(c, 0x1a0) = atol(tmp.text);
        {
            int result = ov00_02203428(ctx, req);
            if (result) {
                return result;
            }
        }
        job->state = 2;
        break;
    case 2:
        if (strncmp(input, ov00_02218A80, 5)) {
            ERROR(ov00_02218A18);
        }
        if (!ov00_0220A4A0(input, ov00_02218A88, tmp.text, 512)) {
            ERROR(ov00_02218A50);
        }
        W(c, 0x198) = atol(tmp.text);
        if (!ov00_0220A4A0(input, ov00_02218958, tmp.text, 512)) {
            ERROR(ov00_02218A50);
        }
        W(c, 0x19c) = atol(tmp.text);
        if (!ov00_0220A4A0(input, ov00_02218964, tmp.text, 512)) {
            ERROR(ov00_02218A50);
        }
        W(c, 0x1a0) = atol(tmp.text);
        if (!ov00_0220A4A0(input, ov00_0221893C, tmp.code, 21)) {
            tmp.code[0] = 0;
        }
        if (!ov00_0220A4A0(input, ov00_02218A94, S(c, 0x474), 25)) {
            *S(c, 0x474) = 0;
        }
        {
            char *user;
            if (*S(req, 0xc2)) {
                user = S(req, 0xc2);
            } else if (*S(c, 0x12f)) {
                user = S(c, 0x12f);
            } else {
                sprintf(tmp.auth, ov00_022188D0, S(c, 0x110), S(c, 0x144));
                user = tmp.auth;
            }
            sprintf(tmp.text, ov00_022188D8, S(req, 0xa1), ov00_022188E8, user, req, S(req, 0x80), S(req, 0xa1));
        }
        ov00_021FF31C(tmp.text, strlen(tmp.text), tmp.hash);
        if (!ov00_0220A4A0(input, ov00_02218A9C, tmp.text, 512)) {
            ERROR(ov00_02218A50);
        }
        if (memcmp(tmp.hash, tmp.text, 32)) {
            ov00_0220A86C(ctx, 0x108, ov00_02218AA4);
            ov00_02202990(ctx, 3, 1);
            return 3;
        }
        if (W(c, 0x100)) {
            Pair *p = ov00_02207CEC(ctx, W(c, 0x1a0));
            p->a = W(c, 0x1a0);
            p->b = W(c, 0x19c);
        }
        W(c, 0x1d8) = 3;
        callback.a = job->a;
        callback.b = job->b;
        if (callback.a) {
            Object *object = ov00_021EC3A8(32);
            int result;
            if (!object) {
                ov00_0220A890(ctx, ov00_022188C0);
                return 1;
            }
            memset(object, 0, 32);
            object->b = W(c, 0x1a0);
            object->a = 0;
            ov00_0220A348(object->code, tmp.code, 21);
            result = ov00_02202A84(ctx, callback, object, job, 0);
            if (result) {
                return result;
            }
        }
        ov00_022066D0(ctx, job);
        break;
    }
    return 0;
}
