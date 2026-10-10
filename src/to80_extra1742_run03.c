#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
#include <string.h>

typedef struct Host {
    u8 pad[0x80c];
    void *channels;
} Host;

typedef struct Channel {
    u8 pad0[0x120];
    void *callback;
    u8 pad124[12];
    void *user, *members;
} Channel;

typedef struct Member {
    u8 pad[0xdc];
    int flags;
} Member;

extern char ov45_022559B8[];
extern char ov45_02255918[];
extern char ov45_022556D8[];
extern char ov45_02255B44[];
extern char ov45_02255B54[];
extern char ov45_02255B64[];
extern char ov45_022559D4[];
extern char ov45_022559E4[];
extern char ov45_02255AF4[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void *ov00_021FF0C0(void *, const char *);
extern void ov45_02244D30(Host *, int, void *, void *, void *, int, const char *, int);
void ov45_02249194(Host *, const char *, const char *, int, int);

void ov45_02249194(Host *h, const char *name, const char *channel, int flags, int set) {
    struct Frame {
        const char *channel, *name;
        int flags;
        char member[128];
        char gap[96];
        char channelName[257];
        char tail[225];
    } f;

    Channel *c;
    Member *m;
    if (!h) {
        __msl_assertion_failed(ov45_022559B8, ov45_02255918, ov45_022556D8, 923);
    }
    if (!name) {
        __msl_assertion_failed(ov45_02255B44, ov45_02255918, ov45_022556D8, 925);
    }
    if (!*name) {
        __msl_assertion_failed(ov45_02255B54, ov45_02255918, ov45_022556D8, 925);
    }
    if (strlen(name) >= 128) {
        __msl_assertion_failed(ov45_02255B64, ov45_02255918, ov45_022556D8, 925);
    }
    if (!channel) {
        __msl_assertion_failed(ov45_022559D4, ov45_02255918, ov45_022556D8, 926);
    }
    if (!*channel) {
        __msl_assertion_failed(ov45_022559E4, ov45_02255918, ov45_022556D8, 926);
    }
    if (flags < 0 || flags > 3) {
        __msl_assertion_failed(ov45_02255AF4, ov45_02255918, ov45_022556D8, 927);
    }
    strncpy(f.channelName, channel, 257);
    f.channelName[256] = 0;
    c = ov00_021FF0C0(h->channels, f.channelName);
    if (!c) {
        return;
    }
    strncpy(f.member, name, 128);
    f.member[127] = 0;
    m = ov00_021FF0C0(c->members, f.member);
    if (!m) {
        return;
    }
    if (set) {
        m->flags |= flags;
    } else {
        m->flags &= ~flags;
    }
    if (!c->callback) {
        return;
    }
    f.channel = channel;
    f.name = name;
    f.flags = m->flags;
    ov45_02244D30(h, 11, c->callback, &f.channel, c->user, 0, channel, 12);
}
