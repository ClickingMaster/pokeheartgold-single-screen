#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    int key;
    unsigned value;
} Pair;

typedef struct {
    Pair *data;
    int count;
} Allocation;

typedef struct {
    int capacity, count;
    Pair *data;
} Buffer;

typedef struct {
    Pair *position;
    Buffer *state;
} Out;

typedef struct {
    unsigned value;
} Tag;

typedef struct {
    Pair *ptr;
} Cursor;

extern Allocation ov45_0223AA80(int);
extern void ov45_02239294(Cursor, Cursor, Out, Out, Tag *);
extern void _ZdaPv(void *);
void ov45_022391EC(Cursor, Cursor, Tag);

void ov45_022391EC(Cursor first, Cursor last, Tag tag) {
    int n = last.ptr - first.ptr;
    Out begin;
    Out end;
    Allocation a;
    Buffer b;
    if (n <= 20) {
        n = 0;
    }
    a = ov45_0223AA80(n);
    b.capacity = a.count;
    b.count = 0;
    b.data = a.data;
    end.position = a.data + a.count;
    end.state = &b;
    begin.position = a.data;
    begin.state = &b;
    ov45_02239294(first, last, begin, end, &tag);
    _ZdaPv(b.data);
}
