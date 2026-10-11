#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u32 exists;
    u8 pad[24];
    char name[1];
} Client;

typedef struct {
    int enabled;
    const char *name;
    void *data;
} Args;

extern const char ov45_02256BA0[];
extern const char ov45_02256B94[];
extern const char ov45_02256AE0[];
extern const char ov45_02256E9C[];
extern const char ov45_02256EAC[];
extern const char ov45_02256FA0[];
extern const char ov45_02256FE8[];
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern void ov45_02244D30(Client *, int, void *, Args *, void *, int, int, int);
extern void ov45_0224F770(Client *, int);
extern void ov00_021FF3C0(int);
extern void ov45_02252F74(char *, const char *, const char *);
extern int ov45_02248238(Client *, const char *);
extern int ov45_02248288(Client *, const char *, void *);
extern int ov45_0224A5E4(Client *);
extern int ov45_0224F660(Client *, int);
extern int ov45_0224A9F8(Client *, const char *, void *, void *);
void ov45_02250C80(Client *, const char *, void *, void *, int);

void ov45_02250C80(Client *c, const char *name, void *callback, void *user, int wait) {
    u8 info[32];
    Args args;
    int id;
    if (!c) {
        __msl_assertion_failed(ov45_02256BA0, ov45_02256B94, ov45_02256AE0, 1730);
    }
    if (!c || !c->exists) {
        return;
    }
    if (!name) {
        __msl_assertion_failed(ov45_02256E9C, ov45_02256B94, ov45_02256AE0, 1733);
    }
    if (!*name) {
        __msl_assertion_failed(ov45_02256EAC, ov45_02256B94, ov45_02256AE0, 1733);
    }
    if (!callback) {
        __msl_assertion_failed(ov45_02256FA0, ov45_02256B94, ov45_02256AE0, 1734);
    }
    if (ov45_02248238(c, name) && ov45_02248288(c, name, info)) {
        id = ov45_0224A5E4(c);
        args.enabled = 1;
        args.data = info;
        args.name = name;
        ov45_02244D30(c, 17, callback, &args, user, id, 0, 12);
        if (!wait) {
            return;
        }
        do {
            ov45_0224F770(c, id);
            ov00_021FF3C0(10);
        } while (ov45_0224F660(c, id));
    } else {
        ov45_02252F74(c->name, ov45_02256FE8, name);
        id = ov45_0224A9F8(c, name, callback, user);
        if (!wait) {
            return;
        }
        do {
            ov45_0224F770(c, id);
            ov00_021FF3C0(10);
        } while (ov45_0224F660(c, id));
    }
}
