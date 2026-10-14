#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u8 unused;
} Tag;

typedef struct {
    u32 a, b;
} Entry;

typedef struct {
    Entry *p;
} Iter;

static inline int Equal(const Iter *first, Entry *last) {
    return first->p == last;
}

static inline long Distance(Iter first, Iter last) {
    Iter *a = &first, *b = &last;
    return b->p - a->p;
}

typedef struct {
    u32 start, size, field8, fieldC;
    int capacity;
} Vector;

static inline u32 End(Vector *v) {
    if (v->capacity) {
        return (v->capacity - 1) << 7;
    }
    return 0;
}

typedef struct {
    u32 x[5];
} Position;

void LobbyEventDeque_AddBackCapacity(Vector *, u32);
Position LobbyEventDeque_IteratorAt(Vector *, u32);
Position LobbyEventDeque_UninitializedCopy(Iter, Iter, Position *, u32 *, u32 *);
void LobbyEventDeque_AppendRange(Vector *, Iter *, Iter *, Tag);

void LobbyEventDeque_AppendRange(Vector *v, Iter *first, Iter *last, Tag tag) {
    Position result, pos;
    u32 count, available;
    if (Equal(first, last->p)) {
        return;
    }
    count = Distance(*first, *last);
    if (v->capacity == 0) {
        available = 0;
    } else {
        available = (v->capacity - 1) << 7;
    }
    available -= v->start + v->size;
    if (count > available) {
        LobbyEventDeque_AddBackCapacity(v, count - available);
    }
    pos = LobbyEventDeque_IteratorAt(v, v->size);
    result = LobbyEventDeque_UninitializedCopy(*first, *last, &pos, &v->field8, &v->size);
}
