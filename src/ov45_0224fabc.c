#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u32 d[5];
} Config;

typedef struct {
    int result, state;
    u32 pad;
    void *cb1;
    void *cb2;
    void *cb3;
    void *cb4;
    u8 engine[0x36c - 0x1c];
    char name[64], q[128], p[128];
    int count;
    char r[64], key[32], login[64], pass[32], sa[256], sb[256];
    u32 padS;
    char s[128];
    int param;
    Config config;
    u8 pad2[16];
    int active;
    u32 pad3;
    int zero;
    char sd[128];
    int kind;
    u32 tail[2];
} Work;

extern char ov45_02256C2C[];
extern char ov45_02256C30[];
extern char ov45_02256B94[];
extern char ov45_02256A08[];
extern char ov45_02256C48[];
extern char ov45_02256C5C[];
extern char ov45_02256C74[];
extern char ov45_02256C84[];
extern char ov45_02256C94[];
extern char ov45_02256CAC[];
extern char ov45_02256CBC[];
extern char ov45_02256CD8[];
extern char ov45_02256CF0[];
extern char ov45_02256D04[];
extern char ov45_02256D18[];
extern char ov45_02256D38[];
extern char ov45_02256D54[];
extern char ov45_02256D7C[];
extern char ov45_02256D84[];
extern char ov45_02256D9C[];
extern int ov45_0225699C;
extern char ov45_02256DB8[];
extern char ov45_02256DC8[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov00_021FF3CC(void);
extern Work *ov00_021EC3A8(int);
extern void ov00_021EC3D8(void *);
extern void ov00_021FF3D0(void);
extern int ov45_02247968(Work *);
extern int ov45_02244B9C(Work *);
extern void ov45_022479F0(Work *);
extern int ov45_02251EF0(void *, char *);
extern void ov45_02244BCC(Work *);
extern int ov45_02251F6C(void *, char *, int);
extern void ov45_022522B4(void *);
extern void ov45_02252F74(void *, char *, int, char *);
extern void ov45_02252CB4(void *, char *);
extern void ov45_0224F9A0(Work *);
extern void ov45_0224F770(Work *, int);
extern void ov00_021FF3C0(int);
extern void ov45_022504E0(Work *);
#define ASSERT(c, s, l)                                                 \
    do {                                                                \
        if (!(c))                                                       \
            __msl_assertion_failed(s, ov45_02256B94, ov45_02256A08, l); \
    } while (0)
Work *ov45_0224FABC(int, char *, int, char *, char *, char *, int, char *, char *, char *, char *, char *, char *, char *, char *, Config *, void *, void *, void *, void *, int);

Work *ov45_0224FABC(int kind, char *s, int param, char *name, char *p, char *q, int count, char *r, char *key, char *login, char *pass, char *sa, char *sb, char *sc, char *sd, Config *config, void *cb1, void *cb2, void *cb3, void *cb4, int blocking) {
    char *token = ov45_02256C2C;
    Work *w;
    ASSERT(s != 0, ov45_02256C30, 347);
    ASSERT(config != 0, ov45_02256C48, 348);
    ASSERT(cb3 != 0, ov45_02256C5C, 349);
    if (kind == 0) {
        ASSERT(name != 0, ov45_02256C74, 355);
        ASSERT(name[0] != 0, ov45_02256C84, 355);
        ASSERT(strlen(name) < 64, ov45_02256C94, 355);
        if (!name || !name[0]) {
            return 0;
        }
        token = name;
    } else if (kind == 1) {
        ASSERT(count > 0, ov45_02256CAC, 362);
        if (count <= 0) {
            return 0;
        }
        ASSERT(login && login[0], ov45_02256CBC, 365);
        if (!login || !login[0]) {
            return 0;
        }
        ASSERT(pass && pass[0], ov45_02256CD8, 368);
        if (!pass || !pass[0]) {
            return 0;
        }
        token = login;
    } else if (kind == 2) {
        ASSERT(count >= 0, ov45_02256CF0, 375);
        if (count < 0) {
            return 0;
        }
        ASSERT(r && r[0], ov45_02256D04, 378);
        if (!r || !r[0]) {
            return 0;
        }
        ASSERT(key && key[0], ov45_02256D18, 381);
        if (!key || !key[0]) {
            return 0;
        }
        ASSERT(pass && pass[0], ov45_02256CD8, 384);
        if (!pass || !pass[0]) {
            return 0;
        }
        token = key;
    } else if (kind == 3) {
        ASSERT(sa && sa[0], ov45_02256D38, 391);
        if (!sa || !sa[0]) {
            return 0;
        }
        ASSERT(sb && sb[0], ov45_02256D54, 394);
        if (!sb || !sb[0]) {
            return 0;
        }
        token = ov45_02256D7C;
    }
    if (kind != 0) {
        ASSERT(sc && sc[0], ov45_02256D84, 401);
        if (!sc || !sc[0]) {
            return 0;
        }
        ASSERT(sd && sd[0], ov45_02256D9C, 404);
        if (!sd || !sd[0]) {
            return 0;
        }
    }
    ov00_021FF3CC();
    w = ov00_021EC3A8(0x8b4);
    if (!w) {
        return 0;
    }
    memset(w, 0, 0x8b4);
    w->kind = kind;
    if (name) {
        strncpy(w->name, name, 64);
        w->name[63] = 0;
    }
    if (p) {
        strncpy(w->p, p, 128);
        w->p[127] = 0;
    }
    if (q) {
        strncpy(w->q, q, 128);
        w->q[127] = 0;
    }
    w->count = count;
    if (r) {
        strncpy(w->r, r, 64);
        w->r[63] = 0;
    }
    if (key) {
        strncpy(w->key, key, 32);
        w->key[31] = 0;
    }
    if (login) {
        strncpy(w->login, login, 64);
        w->login[63] = 0;
    }
    if (pass) {
        strncpy(w->pass, pass, 32);
        w->pass[31] = 0;
    }
    if (sa) {
        strncpy(w->sa, sa, 256);
        w->sa[255] = 0;
    }
    if (sb) {
        strncpy(w->sb, sb, 256);
        w->sb[255] = 0;
    }
    strncpy(w->s, s, 128);
    w->s[127] = 0;
    w->param = param;
    w->config = *config;
    w->active = 1;
    w->state = 1;
    w->zero = 0;
    if (!ov45_02247968(w)) {
        ov00_021EC3D8(w);
        ov00_021FF3D0();
        return 0;
    }
    if (!ov45_02244B9C(w)) {
        ov45_022479F0(w);
        ov00_021EC3D8(w);
        ov00_021FF3D0();
        return 0;
    }
    if (!ov45_02251EF0(w->engine, token)) {
        ov45_02244BCC(w);
        ov45_022479F0(w);
        ov00_021EC3D8(w);
        ov00_021FF3D0();
        return 0;
    }
    if (!ov45_02251F6C(w->engine, s, param)) {
        ov45_022522B4(w->engine);
        ov45_02244BCC(w);
        ov45_022479F0(w);
        ov00_021EC3D8(w);
        ov00_021FF3D0();
        return 0;
    }
    w->cb1 = cb1;
    w->cb2 = cb2;
    w->cb3 = cb3;
    w->cb4 = cb4;
    if (sc && sc[0] && sd && sd[0]) {
        strncpy(w->sd, sd, 128);
        w->sd[127] = 0;
        ov45_02252F74(w->engine, ov45_02256DB8, ov45_0225699C, sc);
    } else if (w->cb2) {
        ov45_02252CB4(w->engine, ov45_02256DC8);
    } else {
        ov45_0224F9A0(w);
    }
    if (blocking) {
        do {
            ov45_0224F770(w, 0);
            ov00_021FF3C0(10);
        } while (w->state);
        if (!w->result) {
            ov45_022504E0(w);
            w = 0;
        }
    }
    return w;
}
