#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Buffer {
    char *data;
    int capacity, length, offset;
} Buffer;

typedef struct Request {
    int kind, fd;
    Buffer input, output;
    char a[31], b[21], c[51], d[31], e[31], f[31], g[68];
    int number, extra, target, unused, done;
} Request;

typedef struct Root {
    u8 opaque[0x198];
    int n198;
    u32 pad19c;
    int n1a0;
    u8 gap[0x274];
    int token;
    u8 gap2[0x50];
    int n46c, n470;
} Root;

typedef struct Context {
    Root *root;
} Context;

typedef void (*Callback)(Context *, void *, void *);

typedef struct Pair {
    Callback fn;
    void *arg;
} Pair;

typedef struct Work {
    int kind;
    Request *request;
    int synchronous;
    Pair callback;
    int state, id, error;
    struct Work *next;
} Work;

typedef struct Item {
    int id;
    char a[31], b[21], c[31], d[31], e[51];
    u8 pad[3];
} Item;

typedef struct List {
    int unused, count, status;
    Item *items;
} List;

typedef struct Names {
    int unused;
    char key[52];
    int count;
    char **names, **values;
} Names;

typedef struct Entry {
    int id;
    char name[31];
    u8 pad;
    int number;
    char data[256];
} Entry;

typedef struct Entries {
    int unused, target, count;
    Entry *entries;
} Entries;

typedef struct Status {
    int unused;
    char name[52];
    int value;
} Status;

typedef struct Two {
    int a, b;
} Two;

typedef struct NameList {
    int unused, count;
    char **names;
} NameList;

typedef struct Items {
    int unused, count;
    Item *items;
} Items;

extern int ov00_0220267C(Context *, int, Buffer *, int *, int, const char *);
extern int ov00_0220A5A8(Context *, int, int *);
extern void ov00_0220A86C(Context *, int, const char *);
extern void ov00_02202990(Context *, int, int);
extern void ov00_022020EC(Context *, void *, const char *);
extern void ov00_0220211C(Context *, void *, int);
extern int ov00_02202454(Context *, int, Buffer *, int *, int *, const char *);
extern int ov00_0220A3C8(Context *, const char *, int);
extern int ov00_0220A69C(Context *, const char *, int *, char *, char *);
extern void *ov00_021EC3BC(void *, int);
extern void *ov00_021EC3A8(int);
extern void ov00_021EC3D8(void *);
extern void ov00_0220A890(Context *, const char *);
extern void ov00_0220A348(char *, const char *, int);
extern int ov00_02208330(Context *, char *, char *, char *, char *, char *, void *, int, int, Callback, void *);
extern int ov00_02202A84(Context *, Pair, void *, Work *, int);
extern int ov00_0220A4A0(const char *, const char *, char *, int);
extern void ov00_021FF3C0(int);
extern int strcmp(const char *, const char *);
extern char *strstr(const char *, const char *);
extern char *strcpy(char *, const char *);
extern long atol(const char *);
extern void *memset(void *, int, u32);
extern void __msl_assertion_failed(const char *, const char *, const char *, int);
typedef char RequestExtent[sizeof(Request) == 0x144 ? 1 : -1];
typedef char WorkExtent[sizeof(Work) == 0x24 ? 1 : -1];
int ov00_0220851C(Context *, Work *);
extern char ov00_022192A8[];
extern char ov00_02219328[];
extern char ov00_022193E4[];
extern char ov00_0221942C[];
extern char ov00_02219430[];
extern char ov00_0221945C[];
extern char ov00_02219468[];
extern char ov00_02219474[];
extern char ov00_02219480[];
extern char ov00_02219490[];
extern char ov00_02219498[];
extern char ov00_022194A8[];
extern char ov00_022194B0[];
extern char ov00_022194BC[];
extern char ov00_022194C8[];
extern char ov00_022194D4[];
extern char ov00_022194DC[];
extern char ov00_022194E4[];
extern char ov00_022194EC[];
extern char ov00_022194F4[];
extern char ov00_02219500[];
extern char ov00_0221950C[];
extern char ov00_02219514[];
extern char ov00_02219520[];
extern char ov00_0221952C[];
extern char ov00_02219534[];
extern char ov00_02219540[];
extern char ov00_02219550[];
extern char ov00_02219560[];
extern char ov00_02219564[];
extern char ov00_02219570[];
extern char ov00_02219578[];
extern char ov00_022195A4[];
extern char ov00_022195AC[];
extern char ov00_022195B4[];
extern char ov00_022195B8[];
extern char ov00_022195C0[];
extern char ov00_022195CC[];
extern char ov00_022195D8[];
extern char ov00_022195E4[];
extern char ov00_022195EC[];
extern char ov00_02219614[];
extern char ov00_02219618[];
extern char ov00_0221961C[];
extern char ov00_02219624[];
extern char ov00_0221962C[];
extern char ov00_02219630[];
extern char ov00_02219638[];
extern char ov00_02219644[];
extern char ov00_02219648[];
extern char ov00_02219650[];
extern char ov00_02219654[];
extern char ov00_0221965C[];
extern char ov00_02219664[];
extern char ov00_02219668[];
extern char ov00_02219670[];
extern char ov00_02219678[];
extern char ov00_0221967C[];
extern char ov00_02219684[];
extern char ov00_0221B16C[];
#define ADD(s) ov00_022020EC(ctx, &r->output, (s))
#define NUM(n) ov00_0220211C(ctx, &r->output, (n))
#define NEXT()                                                       \
    do {                                                             \
        ret = ov00_0220A69C(ctx, r->input.data, &pos, label, value); \
        if (ret)                                                     \
            return ret;                                              \
    } while (0)
#define BAD()                                 \
    do {                                      \
        ov00_0220A86C(ctx, 1, ov00_022195EC); \
        ov00_02202990(ctx, 3, 1);             \
        return 3;                             \
    } while (0)
#define OOM()                              \
    do {                                   \
        ov00_0220A890(ctx, ov00_02219328); \
        return 1;                          \
    } while (0)
#define DISPATCH(pair, obj, type)                           \
    do {                                                    \
        ret = ov00_02202A84(ctx, (pair), (obj), w, (type)); \
        if (ret)                                            \
            return ret;                                     \
    } while (0)

int ov00_0220851C(Context *ctx, Work *w) {
    char label[512], value[512];
    List response;
    int replyState, pos;
    Pair cb1;
    int received, available;
    Pair cb8, cb7, cb6, cb5, cb4, cb3, cb2;
    int finished;
    Root *root = ctx->root;
    Request *r = w->request;
    int again = w->synchronous != 0, ret, saved, stopped;
    do {
        ret = ov00_0220267C(ctx, r->fd, &r->output, &available, 1, ov00_0221942C);
        if (ret) {
            return ret;
        }
        if (w->state == 1) {
            ret = ov00_0220A5A8(ctx, r->fd, &replyState);
            if (ret) {
                return ret;
            }
            if (replyState == 4) {
                ov00_0220A86C(ctx, 0xd01, ov00_02219430);
                ov00_02202990(ctx, 4, 0);
                return 4;
            }
            if (replyState == 3) {
                if (r->kind == 1) {
                    ADD(ov00_0221945C);
                    ADD(ov00_02219468);
                    NUM(root->n198);
                    ADD(ov00_02219474);
                    NUM(root->n1a0);
                    ADD(ov00_02219480);
                    NUM(root->n470);
                    if (r->a[0] != 0) {
                        ADD(ov00_02219490);
                        ADD(r->a);
                    }
                    if (r->b[0] != 0) {
                        ADD(ov00_02219498);
                        ADD(r->b);
                    }
                    if (r->c[0] != 0) {
                        ADD(ov00_022194A8);
                        ADD(r->c);
                    }
                    if (r->d[0] != 0) {
                        ADD(ov00_022194B0);
                        ADD(r->d);
                    }
                    if (r->e[0] != 0) {
                        ADD(ov00_022194BC);
                        ADD(r->e);
                    }
                    if (r->number != 0) {
                        ADD(ov00_022194C8);
                        NUM(r->number);
                    }
                    if (r->extra > 0) {
                        ADD(ov00_022194D4);
                        NUM(r->extra);
                    }
                } else if (r->kind == 2) {
                    ADD(ov00_022194DC);
                    ADD(ov00_022194A8);
                    ADD(r->c);
                } else if (r->kind == 3) {
                    ADD(ov00_022194E4);
                    ADD(ov00_022194A8);
                    ADD(r->c);
                    ADD(ov00_022194EC);
                    ADD(r->f);
                    ADD(ov00_02219480);
                    NUM(root->n470);
                } else if (r->kind == 4) {
                    ADD(ov00_022194F4);
                    ADD(ov00_02219468);
                    NUM(root->n198);
                    ADD(ov00_02219474);
                    NUM(root->n1a0);
                    ADD(ov00_02219500);
                    NUM(r->target);
                } else if (r->kind == 5) {
                    ADD(ov00_0221950C);
                    ADD(ov00_02219490);
                    ADD(r->a);
                    ADD(ov00_022194A8);
                    ADD(r->c);
                    ADD(ov00_022194EC);
                    ADD(r->f);
                } else if (r->kind == 6) {
                    ADD(ov00_02219514);
                    ADD(ov00_02219490);
                    ADD(r->a);
                    ADD(ov00_022194A8);
                    ADD(r->c);
                    ADD(ov00_022194EC);
                    ADD(r->f);
                    ADD(ov00_02219520);
                    NUM(root->n46c);
                    ADD(ov00_02219480);
                    NUM(root->n470);
                    ADD(ov00_02219498);
                    ADD(r->b);
                    if (r->g[0] != 0) {
                        ADD(ov00_0221952C);
                        ADD(r->g);
                    }
                } else if (r->kind == 7) {
                    ADD(ov00_02219534);
                    ADD(ov00_02219468);
                    NUM(root->n198);
                    ADD(ov00_02219474);
                    NUM(root->n1a0);
                    ADD(ov00_02219480);
                    NUM(root->n470);
                } else if (r->kind == 8) {
                    ADD(ov00_02219540);
                    ADD(ov00_02219550);
                    ADD(r->b);
                    ADD(ov00_02219480);
                    NUM(root->n470);
                } else {
                    __msl_assertion_failed(ov00_02219560, ov00_022193E4, ov00_022192A8, 0x275);
                }
                ADD(ov00_02219564);
                ADD(ov00_0221B16C);
                ADD(ov00_02219570);
                w->state = 4;
            }
        } else if (w->state == 4) {
            ret = ov00_02202454(ctx, r->fd, &r->input, &received, &available, ov00_0221942C);
            if (ret) {
                if (ret != 3) {
                    return ret;
                }
                ov00_0220A86C(ctx, 0xd01, ov00_02219578);
                ov00_02202990(ctx, 3, 0);
                return 3;
            }
            if (strstr(r->input.data, ov00_02219570)) {
                pos = 0;
                w->state = 5;
                if (ov00_0220A3C8(ctx, r->input.data, 1)) {
                    r->done = 1;
                    return 4;
                }
                if (r->kind == 1) {
                    finished = 0;
                    response.unused = 0;
                    response.count = 0;
                    response.items = 0;
                    response.status = 0x601;
                    do {
                        NEXT();
                        if (!strcmp(label, ov00_022195A4)) {
                            NEXT();
                            if (!strcmp(label, ov00_022195AC)) {
                                if (strcmp(value, ov00_02219560)) {
                                    response.status = 0x600;
                                }
                            }
                            finished = 1;
                        } else if (!strcmp(label, ov00_022195B4)) {
                            Item *base, *item;
                            int offset;
                            response.count++;
                            base = ov00_021EC3BC(response.items, response.count * 0xac);
                            response.items = base;
                            if (!base) {
                                OOM();
                            }
                            offset = (response.count - 1) * 0xac;
                            item = (Item *)((u8 *)base + offset);
                            memset(item, 0, 0xac);
                            *(int *)((u8 *)base + offset) = atol(value);
                            stopped = 0;
                            do {
                                saved = pos;
                                NEXT();
                                if (!strcmp(label, ov00_022195B8)) {
                                    ov00_0220A348(item->a, value, 31);
                                } else if (!strcmp(label, ov00_022195C0)) {
                                    ov00_0220A348(item->b, value, 21);
                                } else if (!strcmp(label, ov00_022195CC)) {
                                    ov00_0220A348(item->c, value, 31);
                                } else if (!strcmp(label, ov00_022195D8)) {
                                    ov00_0220A348(item->d, value, 31);
                                } else if (!strcmp(label, ov00_022195E4)) {
                                    ov00_0220A348(item->e, value, 51);
                                } else if (!strcmp(label, ov00_022195B4) || !strcmp(label, ov00_022195A4)) {
                                    pos = saved;
                                    stopped = 1;
                                }
                            } while (!stopped);
                        } else {
                            BAD();
                        }
                    } while (!finished);
                    cb1 = w->callback;
                    {
                        int status = response.status;
                        if (cb1.fn) {
                            cb1.fn(ctx, &response, cb1.arg);
                        }
                        if (status == 0x600 && response.status == 0x600) {
                            ret = ov00_02208330(ctx, r->a, r->b, r->c, r->d, r->e, (void *)(u32)r->number, response.count + r->extra, w->synchronous, w->callback.fn, w->callback.arg);
                            if (ret) {
                                return ret;
                            }
                        }
                    }
                    ov00_021EC3D8(response.items);
                    response.items = 0;
                } else if (r->kind == 2) {
                    cb2 = w->callback;
                    if (cb2.fn) {
                        Status *result;
                        NEXT();
                        if (strcmp(label, ov00_02219614)) {
                            BAD();
                        }
                        result = ov00_021EC3A8(0x3c);
                        if (!result) {
                            OOM();
                        }
                        result->unused = 0;
                        ov00_0220A348(result->name, r->c, 51);
                        if (value[0] == '0') {
                            result->value = 0;
                        } else {
                            result->value = 1;
                        }
                        DISPATCH(cb2, result, 0);
                    }
                } else if (r->kind == 3) {
                    cb3 = w->callback;
                    if (cb3.fn) {
                        Names *result = ov00_021EC3A8(0x44);
                        if (!result) {
                            OOM();
                        }
                        result->unused = 0;
                        strcpy(result->key, r->c);
                        result->count = 0;
                        result->names = 0;
                        result->values = 0;
                        NEXT();
                        if (strcmp(label, ov00_02219618)) {
                            BAD();
                        }
                        finished = 0;
                        do {
                            NEXT();
                            if (!strcmp(label, ov00_022195B8)) {
                                char **names = ov00_021EC3BC(result->names, (result->count + 1) * 4);
                                char *name;
                                if (!names) {
                                    OOM();
                                }
                                result->names = names;
                                name = ov00_021EC3A8(31);
                                if (!name) {
                                    OOM();
                                }
                                result->names[result->count] = name;
                                ov00_0220A348(result->names[result->count], value, 31);
                                result->count++;
                            } else if (!strcmp(label, ov00_022195C0)) {
                                if (result->count > 0) {
                                    char **values = ov00_021EC3BC(result->values, result->count * 4);
                                    char *v;
                                    if (!values) {
                                        OOM();
                                    }
                                    result->values = values;
                                    v = ov00_021EC3A8(21);
                                    if (!v) {
                                        OOM();
                                    }
                                    result->values[result->count - 1] = v;
                                    ov00_0220A348(result->values[result->count - 1], value, 21);
                                }
                            } else if (!strcmp(label, ov00_0221961C)) {
                                finished = 1;
                            } else {
                                BAD();
                            }
                        } while (!finished);
                        DISPATCH(cb3, result, 3);
                    }
                } else if (r->kind == 4) {
                    cb4 = w->callback;
                    if (cb4.fn) {
                        Entries *result = ov00_021EC3A8(16);
                        if (!result) {
                            OOM();
                        }
                        result->target = r->target;
                        result->unused = 0;
                        result->count = 0;
                        result->entries = 0;
                        finished = 0;
                        do {
                            NEXT();
                            if (!strcmp(label, ov00_02219624)) {
                                finished = 1;
                            } else if (!strcmp(label, ov00_0221962C)) {
                                Entry *entries;
                                int i;
                                result->count++;
                                entries = result->entries = ov00_021EC3BC(result->entries, result->count * sizeof(Entry));
                                if (!entries) {
                                    OOM();
                                }
                                i = result->count - 1;
                                memset(&entries[i], 0, sizeof(Entry));
                                entries[i].number = 1;
                                entries[i].id = atol(value);
                                stopped = 0;
                                do {
                                    saved = pos;
                                    NEXT();
                                    if (!strcmp(label, ov00_02219630)) {
                                        ov00_0220A348(entries[i].data, value, 256);
                                    } else if (!strcmp(label, ov00_022195B8)) {
                                        ov00_0220A348(entries[i].name, value, 31);
                                    }
                                    if (!strcmp(label, ov00_02219638)) {
                                        entries[i].number = atol(value);
                                    } else if (!strcmp(label, ov00_0221962C) || !strcmp(label, ov00_02219624)) {
                                        pos = saved;
                                        stopped = 1;
                                    }
                                } while (!stopped);
                            } else {
                                BAD();
                            }
                        } while (!finished);
                        DISPATCH(cb4, result, 4);
                    }
                } else if (r->kind == 5) {
                    cb5 = w->callback;
                    if (cb5.fn) {
                        int a, b;
                        Two *result;
                        NEXT();
                        if (strcmp(label, ov00_02219644)) {
                            BAD();
                        }
                        a = atol(value);
                        if (a) {
                            root->token = a;
                            b = 0;
                        } else {
                            if (!ov00_0220A4A0(r->input.data, ov00_02219648, value, 512)) {
                                BAD();
                            }
                            b = atol(value);
                        }
                        result = ov00_021EC3A8(8);
                        if (!result) {
                            OOM();
                        }
                        result->a = a;
                        result->b = b;
                        DISPATCH(cb5, result, 0);
                    }
                } else if (r->kind == 6) {
                    cb6 = w->callback;
                    if (cb6.fn) {
                        int a, b;
                        Two *result;
                        NEXT();
                        if (strcmp(label, ov00_02219650)) {
                            BAD();
                        }
                        a = atol(value);
                        if (a) {
                            root->token = a;
                        }
                        if (!ov00_0220A4A0(r->input.data, ov00_02219648, value, 512)) {
                            if (!a) {
                                BAD();
                            }
                            b = 0;
                        } else {
                            b = atol(value);
                        }
                        result = ov00_021EC3A8(8);
                        if (!result) {
                            OOM();
                        }
                        result->a = a;
                        result->b = b;
                        DISPATCH(cb6, result, 0);
                    }
                } else if (r->kind == 7) {
                    cb7 = w->callback;
                    if (cb7.fn) {
                        Items *result = ov00_021EC3A8(12);
                        if (!result) {
                            OOM();
                        }
                        result->unused = 0;
                        result->count = 0;
                        result->items = 0;
                        NEXT();
                        if (strcmp(label, ov00_02219654)) {
                            BAD();
                        }
                        finished = 0;
                        do {
                            NEXT();
                            if (!strcmp(label, ov00_0221965C)) {
                                finished = 1;
                            } else if (!strcmp(label, ov00_02219664)) {
                                Item *items, *p;
                                int i;
                                p = ov00_021EC3BC(result->items, (result->count + 1) * sizeof(Item));
                                if (!p) {
                                    OOM();
                                }
                                result->items = p;
                                items = result->items;
                                i = result->count;
                                memset(&items[i], 0, sizeof(Item));
                                result->count++;
                                items[i].id = atol(value);
                                stopped = 0;
                                do {
                                    saved = pos;
                                    NEXT();
                                    if (!strcmp(label, ov00_022195B8)) {
                                        ov00_0220A348(items[i].a, value, 31);
                                    } else if (!strcmp(label, ov00_022195C0)) {
                                        ov00_0220A348(items[i].b, value, 21);
                                    } else if (!strcmp(label, ov00_02219668)) {
                                        ov00_0220A348(items[i].c, value, 31);
                                    } else if (!strcmp(label, ov00_02219670)) {
                                        ov00_0220A348(items[i].d, value, 31);
                                    } else if (!strcmp(label, ov00_022195E4)) {
                                        ov00_0220A348(items[i].e, value, 51);
                                    } else if (!strcmp(label, ov00_02219664) || !strcmp(label, ov00_0221965C)) {
                                        pos = saved;
                                        stopped = 1;
                                    }
                                } while (!stopped);
                            } else {
                                BAD();
                            }
                        } while (!finished);
                        DISPATCH(cb7, result, 8);
                    }
                } else if (r->kind == 8) {
                    cb8 = w->callback;
                    if (cb8.fn) {
                        int index = 0;
                        NameList *result = ov00_021EC3A8(12);
                        if (!result) {
                            OOM();
                        }
                        result->unused = 0;
                        result->count = 0;
                        result->names = 0;
                        NEXT();
                        if (strcmp(label, ov00_02219678)) {
                            BAD();
                        }
                        result->count = atol(value);
                        result->names = ov00_021EC3A8(result->count * 4);
                        if (!result->names) {
                            OOM();
                        }
                        finished = index;
                        do {
                            NEXT();
                            if (!strcmp(label, ov00_022195B8)) {
                                result->names[index] = ov00_021EC3A8(21);
                                if (!result->names[index]) {
                                    OOM();
                                }
                                ov00_0220A348(result->names[index], value, 21);
                                index++;
                            } else if (!strcmp(label, ov00_0221967C)) {
                                if (index != result->count) {
                                    __msl_assertion_failed(ov00_02219684, ov00_022193E4, ov00_022192A8, 0x515);
                                }
                                result->count = index;
                                finished = 1;
                            } else {
                                BAD();
                            }
                        } while (!finished);
                        DISPATCH(cb8, result, 9);
                    }
                } else {
                    __msl_assertion_failed(ov00_02219560, ov00_022193E4, ov00_022192A8, 0x52a);
                }
                r->done = 1;
                again = 0;
            }
        }
        if (again) {
            ov00_021FF3C0(10);
        }
    } while (again);
    return 0;
}
