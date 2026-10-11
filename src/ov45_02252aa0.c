#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <stdarg.h>
#include <string.h>
int vsprintf(char *, const char *, va_list);

typedef struct {
    char *data;
    int used, capacity;
} Buffer;

typedef struct {
    int handle;
    u32 state;
    u8 pad[0x100];
    Buffer recv, send;
    int crypt;
    u8 context[0x102];
    u8 sendContext[1];
} Conn;

void __msl_assertion_failed(const char *, const char *, const char *, int);
void ov45_02252530(int, int *, int *, int);
int ov00_021FF970(int, const void *, int, int);
void ov45_02251DB4(Buffer *, int);
int ov45_02251C50(Buffer *, int);
void ov45_022495DC(void *, int, void *);
void ov45_02252590(Conn *);
void ov45_02252AA0(Conn *);
void ov45_02252800(Conn *);
int ov45_02252CB4(Conn *, const char *);
int ov45_022531D8(signed char *, char **);
void *ov00_021EC3A8(u32);
extern const char ov45_02257334[];
extern const char ov45_02257258[];
extern const char ov45_0225720C[];
extern const char ov45_02257344[];
extern const char ov45_022573C4[];
extern const char ov45_022573E4[];
extern const char ov45_02257408[];
extern const char ov45_02257438[];
extern const char ov45_0225745C[];
extern const char ov45_0225749C[];
extern const char ov45_022574BC[];
extern const char ov45_022574E0[];
extern const char ov45_02257514[];
extern const char ov45_02257538[];
extern const char ov45_022575D8[];
extern const char ov45_022571B4[];
extern const char ov45_02257154[];
extern const char ov45_02257248[];
extern const char ov45_022575FC[];
extern const char ov45_02257148[];
extern const char ov45_022571A4[];
extern char ov45_022577F8[];
int ov45_02252F74(Conn *, const char *, ...);

static inline int nonempty(signed char *str) {
    return str && *str;
}

static inline void validate(Conn *c, const char *fn, int line) {
    if (!c) {
        __msl_assertion_failed(ov45_02257334, ov45_02257258, fn, line);
    }
    if (c->state > 2) {
        __msl_assertion_failed(ov45_02257344, ov45_02257258, fn, line);
    }
    if (!&c->recv) {
        __msl_assertion_failed(ov45_022573C4, ov45_02257258, fn, line);
    }
    if (c->recv.capacity < 0) {
        __msl_assertion_failed(ov45_022573E4, ov45_02257258, fn, line);
    }
    if (c->recv.capacity % 8192) {
        __msl_assertion_failed(ov45_02257408, ov45_02257258, fn, line);
    }
    if (c->recv.used < 0) {
        __msl_assertion_failed(ov45_02257438, ov45_02257258, fn, line);
    }
    if (c->recv.used > c->recv.capacity) {
        __msl_assertion_failed(ov45_0225745C, ov45_02257258, fn, line);
    }
    if (!&c->send) {
        __msl_assertion_failed(ov45_0225749C, ov45_02257258, fn, line);
    }
    if (c->send.capacity < 0) {
        __msl_assertion_failed(ov45_022574BC, ov45_02257258, fn, line);
    }
    if (c->send.capacity % 8192) {
        __msl_assertion_failed(ov45_022574E0, ov45_02257258, fn, line);
    }
    if (c->send.used < 0) {
        __msl_assertion_failed(ov45_02257514, ov45_02257258, fn, line);
    }
    if (c->send.used > c->send.capacity) {
        __msl_assertion_failed(ov45_02257538, ov45_02257258, fn, line);
    }
}

void ov45_02252AA0(Conn *c) {
    validate(c, ov45_022571B4, 0x1be);
    if (c->state != 1) {
        __msl_assertion_failed(ov45_022575D8, ov45_02257258, ov45_022571B4, 0x1bf);
    }
    if (c->state == 2) {
        return;
    }
    ov45_02252590(c);
    ov45_02252800(c);
}

int ov45_02252CB4(Conn *c, const char *text) {
    int len;
    char *base;
    int start;
    validate(c, ov45_02257154, 0x1d8);
    if (c->state != 1) {
        __msl_assertion_failed(ov45_022575D8, ov45_02257258, ov45_02257154, 0x1d9);
    }
    if (!text) {
        __msl_assertion_failed(ov45_02257248, ov45_02257258, ov45_02257154, 0x1da);
    }
    if (c->state == 2) {
        return 1;
    }
    len = strlen(text);
    if (!ov45_02251C50(&c->send, len + 2)) {
        return 0;
    }
    base = c->send.data;
    start = c->send.used;
    memcpy(base + start, text, len);
    c->send.used += len;
    {
        int at = c->send.used++;
        c->send.data[at] = '\r';
    }
    {
        int at = c->send.used++;
        c->send.data[at] = '\n';
    }
    if (c->crypt) {
        ov45_022495DC(base + start, len + 2, c->sendContext);
    }
    return 1;
}

int ov45_02252F74(Conn *c, const char *fmt, ...) {
    va_list args;
    int n;
    validate(c, ov45_022571A4, 0x213);
    if (c->state != 1) {
        __msl_assertion_failed(ov45_022575D8, ov45_02257258, ov45_022571A4, 0x214);
    }
    if (c->state == 2) {
        return 1;
    }
    va_start(args, fmt);
    n = vsprintf(ov45_022577F8, fmt, args);
    if (n != -1) {
        ov45_022577F8[n] = 0;
    } else {
        ov45_022577F8[4095] = 0;
    }
    return ov45_02252CB4(c, ov45_022577F8);
}
