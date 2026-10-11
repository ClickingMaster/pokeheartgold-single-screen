#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 pad[0x36c];
    char name[1];
    u8 rest[0x800 - 0x36d];
    void *callback;
    u32 gap;
    void *arg;
} Context;

typedef struct {
    u8 pad[8];
    char *channel;
    u8 rest[20];
    char **pair;
    int kind;
} Event;

typedef struct {
    void *callback;
    u8 pad[40];
    void *arg;
} Handler;

typedef struct {
    char *channel, *message;
    int action;
} Local;

typedef struct {
    char *sender, *channel, *message;
    int action;
} Remote;

extern const char ov45_022565C0[];
extern const char ov45_022565D0[];
extern const char ov45_02255FA4[];
extern const char ov45_022566F0[];
extern int strnicmp(const char *, const char *);
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov45_02244D30(Context *, int, void *, void *, void *, int, char *, int);
extern Handler *ov45_02247E98(Context *, char *);
void ov45_0224B310(Context *, Event *);

void ov45_0224B310(Context *c, Event *e) {
    char *sender, *message;
    Local l;
    Remote r;
    Handler *h;
    if (!c) {
        __msl_assertion_failed(ov45_022565C0, ov45_022565D0, ov45_02255FA4, 1514);
    }
    if (e->kind != 2) {
        __msl_assertion_failed(ov45_022566F0, ov45_022565D0, ov45_02255FA4, 1520);
    }
    if (e->kind != 2) {
        return;
    }
    sender = e->pair[0];
    message = e->pair[1];
    if (strnicmp(sender, c->name) == 0) {
        if (!c->callback) {
            return;
        }
        if (e->channel) {
            l.channel = e->channel;
        } else {
            l.channel = NULL;
        }
        l.message = message;
        l.action = 2;
        ov45_02244D30(c, 2, c->callback, &l, c->arg, 0, NULL, 12);
    } else {
        h = ov45_02247E98(c, sender);
        if (!h || !h->callback) {
            return;
        }
        r.sender = sender;
        if (e->channel) {
            r.channel = e->channel;
        } else {
            r.channel = NULL;
        }
        r.message = message;
        r.action = 2;
        ov45_02244D30(c, 4, h->callback, &r, h->arg, 0, sender, 16);
    }
}
