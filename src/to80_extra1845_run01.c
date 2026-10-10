#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on
typedef struct Conn Conn;

struct Conn {
    int state;
    u32 pad4;
    void *array;
    u8 padc[0x70];
    u8 *data;
    int size;
    void *items[255];
    int progress, total;
    void (*callback)(Conn *, int, int, void *);
    u8 pad48c[8];
    void *user;
    u8 pad498[8];
    u8 address[4];
    u8 pad4a4[4];
    u16 port;
    u8 pad4aa[0x11a];
    int flags, stage;
};

typedef struct Item {
    void *value;
    int kind;
} Item;

extern int ov00_0221B9D4;
extern char ov00_0221A644[];
extern char ov00_0221A5C8[];
extern char ov00_0221A588[];
extern void __msl_assertion_failed(char *, char *, char *, int);
extern void ov00_022149A4(Conn *, u8 *, int);
extern void ov00_022122B8(void *, u8 *, int);
extern int ov00_02213FD4(u8 *, int);
extern void ov00_0221505C(Conn *, u8 *);
extern void *ov00_021FE5B4(int, int, int);
extern int ArrayLength(void *);
extern void *ov00_02213EE8(Conn *, u8 *);
extern void ov00_021FE754(void *, Item *);
extern int ov00_02214F2C(Conn *, u8 *, int);
int ov00_02215064(Conn *);

int ov00_02215064(Conn *c) {
    struct {
        u8 *ptr;
        int remaining;
    } cursor = { c->data, c->size };

    Item item;
    switch (c->stage) {
    case 0: {
        int pos, total, len;
        if (cursor.remaining < 1) {
            break;
        }
        pos = (*cursor.ptr ^ 0xec) + 2;
        if (cursor.remaining < pos) {
            break;
        }
        len = cursor.ptr[pos - 1] ^ 0xea;
        total = pos + len;
        if (cursor.remaining < total) {
            break;
        }
        ov00_022149A4(c, cursor.ptr + pos, len);
        cursor.ptr += total;
        cursor.remaining -= total;
        c->stage = 1;
        ov00_022122B8((u8 *)c + 0x4bc, cursor.ptr, cursor.remaining);
    }
    case 1:
        if (cursor.remaining < 6) {
            break;
        }
        memcpy(c->address, cursor.ptr, 4);
        c->callback(c, 6, ov00_0221B9D4, c->user);
        memcpy(&c->port, cursor.ptr + 4, 2);
        if (c->port == 0xffff) {
            if (ov00_02213FD4(cursor.ptr + 6, cursor.remaining - 6) == -1) {
                break;
            }
            ov00_0221505C(c, cursor.ptr + 6);
            c->callback(c, 5, ov00_0221B9D4, c->user);
            if (!c->data) {
                break;
            }
        }
        cursor.ptr += 6;
        cursor.remaining -= 6;
        if ((c->flags & 2) || c->port == 0xffff) {
            c->stage = 5;
            c->state = 2;
            break;
        }
        c->stage = 2;
        c->total = -1;
    case 2: {
        int len;
        if (c->total == -1) {
            if (cursor.remaining < 1) {
                break;
            }
            c->total = *cursor.ptr;
            c->array = ov00_021FE5B4(8, c->total, 0);
            if (!c->array) {
                return 5;
            }
            cursor.ptr++;
            cursor.remaining--;
        }
        while (c->total > ArrayLength(c->array)) {
            if (cursor.remaining < 2) {
                break;
            }
            len = ov00_02213FD4(cursor.ptr + 1, cursor.remaining - 1);
            if (len == -1) {
                break;
            }
            item.kind = *cursor.ptr;
            item.value = ov00_02213EE8(c, cursor.ptr + 1);
            ov00_021FE754(c->array, &item);
            cursor.ptr += len + 1;
            cursor.remaining -= len + 1;
        }
        if (c->total > ArrayLength(c->array)) {
            break;
        }
        c->stage = 3;
        c->total = -1;
    }
    case 3: {
        int len;
        if (c->total == -1) {
            if (cursor.remaining < 1) {
                break;
            }
            c->total = *cursor.ptr++;
            cursor.remaining--;
            c->progress = 0;
        }
        while (c->total > c->progress) {
            len = ov00_02213FD4(cursor.ptr, cursor.remaining);
            if (len == -1) {
                break;
            }
            c->items[c->progress++] = ov00_02213EE8(c, cursor.ptr);
            cursor.ptr += len;
            cursor.remaining -= len;
        }
        if (c->total > c->progress) {
            break;
        }
        c->stage = 4;
    }
    case 4: {
        int len;
        if (cursor.remaining < 5) {
            break;
        }
        do {
            len = ov00_02214F2C(c, cursor.ptr, cursor.remaining);
            if (len == -2) {
                return 5;
            }
            if (len == -1) {
                c->stage = 5;
                c->state = 2;
                cursor.remaining -= 5;
                cursor.ptr += 5;
                c->callback(c, 3, ov00_0221B9D4, c->user);
                break;
            }
            cursor.ptr += len;
            cursor.remaining -= len;
            if (!c->data) {
                len = 0;
            }
        } while (len);
    }
    }
    if (cursor.remaining < 0) {
        __msl_assertion_failed(ov00_0221A644, ov00_0221A5C8, ov00_0221A588, 0x4af);
    }
    if (!c->data) {
        return 0;
    }
    if (cursor.remaining) {
        memmove(c->data, cursor.ptr, cursor.remaining);
    }
    c->size = cursor.remaining;
    return 0;
}
