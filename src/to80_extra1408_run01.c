#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct Request {
    int unused, type;
    void (*callback)(int, int, int);
    int notify;
    void *source;
    int size;
    void *buffer, *first, *second;
} Request;

extern void (*ov38_02226070)(void *);
extern void *(*ov38_0222605C)(int, int);
extern int ov38_0222606C;
void ov38_02220B60(Request *, void *);
int ov38_02221D9C(void *, char *, const void *);

extern char ov38_02224ECC[];
extern char ov38_02225EA8[];
extern char ov38_02225EB0[];
int ov38_022218C0(char *, int);
int ov38_022201AC(int);
int ov38_022201C8(char *, char *, int);
int ov38_02220B8C(Request **, void **, const void *, void *, int);
int ov38_02220EB8(Request *, void *);
int ov38_02220F8C(void *, int, int);

int ov38_02220F8C(void *source, int a, int count) {
    int size = count << 8, n;
    char *buffer, *first, *second;
    Request *r;
    void *d;
    char number[12];
    if (size < 128) {
        size = 128;
    }
    buffer = ov38_0222605C(size, 4);
    if (!buffer) {
        ov38_0222606C = 1;
        return 0;
    }
    n = ov38_022218C0(number, a);
    first = ov38_0222605C(ov38_022201AC(n) + 1, 4);
    if (!first) {
        ov38_02226070(buffer);
        ov38_0222606C = 1;
        return 0;
    }
    first[ov38_022201C8(first, number, n)] = 0;
    n = ov38_022218C0(number, count);
    second = ov38_0222605C(ov38_022201AC(n) + 1, 4);
    if (!second) {
        ov38_02226070(first);
        ov38_02226070(buffer);
        ov38_0222606C = 1;
        return 0;
    }
    second[ov38_022201C8(second, number, n)] = 0;
    if (!ov38_02220B8C(&r, &d, ov38_02224ECC, buffer, size)) {
        ov38_02226070(second);
        ov38_02226070(first);
        ov38_02226070(buffer);
        return 0;
    }
    if (ov38_02221D9C(d, ov38_02225EA8, first) && ov38_02221D9C(d, ov38_02225EB0, second)) {
        r->type = 2;
        r->source = source;
        r->first = first;
        r->second = second;
        r->size = count;
        r->buffer = buffer;
        if (ov38_02220EB8(r, d)) {
            return 1;
        }
        ov38_02226070(second);
        ov38_02226070(first);
        ov38_02226070(buffer);
        return 0;
    }
    ov38_02226070(second);
    ov38_02226070(first);
    ov38_02226070(buffer);
    ov38_02220B60(r, d);
    ov38_0222606C = 1;
    return 0;
}
