#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

typedef struct {
    u32 *data;
    u32 start, size, capacity;
} Queue;

extern u32 ov45_022577D0;
extern u32 ov45_022551A4;
extern void LobbyEventDequeMap_LengthError(void);

static inline u32 Count(Queue *q) {
    return q->capacity == 0 ? 0 : q->capacity - 1;
}

static inline u32 Limit(void) {
    if (!(ov45_022577D0 & 1)) {
        ov45_022551A4 = 0x3ffffffe;
        ov45_022577D0 |= 1;
    }
    return ov45_022551A4;
}

static inline const u32 *Max(const u32 *a, const u32 *b) {
    return *a > *b ? a : b;
}

u32 LobbyEventDequeMap_RecommendSize(Queue *, u32, ...);

typedef struct {
    Queue q;
    u32 *capacity;
} Temp;

typedef struct {
    u32 *current, *begin, *end, *limit;
} Iter;

typedef struct {
    u8 byte;
} Empty;

extern void LobbyEventDequeMapBuffer_Allocate(Temp *, u32, Empty);
extern void LobbyEventDequeMapBuffer_AppendRange(Temp *, Iter *, Iter *, Empty);
extern void LobbyEventDequeMapBuffer_PrependRange(Temp *, Iter *, Iter *, Empty);
extern void LobbyEventDequeMapBuffer_Destroy(Temp *);

static inline void Init(Temp *t, Queue *q) {
    t->q.data = 0;
    t->q.start = 0;
    t->q.size = 0;
    t->q.capacity = 0;
    t->capacity = &q->capacity;
}

static inline Empty MakeEmpty(void) {
    Empty e;
    e.byte = 0;
    return e;
}

static inline void Begin(Iter *i, Queue *q) {
    i->current = q->data + (q->capacity ? (q->start + q->size) % q->capacity : 0);
    {
        u32 *data = q->data;
        i->begin = data;
        i->end = data + q->start;
        i->limit = data + q->capacity;
    }
}

static inline void End(Iter *i, Queue *q) {
    i->current = q->data + q->start;
    {
        u32 *data = q->data;
        i->begin = data;
        i->end = data + q->start;
        i->limit = data + q->capacity;
    }
}

static inline void Swap(Queue *a, Queue *b) {
    u32 *p;
    u32 n;
    p = a->data;
    a->data = b->data;
    b->data = p;
    n = a->start;
    a->start = b->start;
    b->start = n;
    n = a->size;
    a->size = b->size;
    b->size = n;
    {
        u32 *last = &b->capacity;
        n = a->capacity;
        a->capacity = *last;
        *last = n;
    }
}

void LobbyEventDequeMap_Reserve(Queue *, u32);
void LobbyEventDequeMap_PushBack(Queue *, u32 *);

u32 LobbyEventDequeMap_RecommendSize(Queue *q, u32 extra, ...) {
    u32 size, limit, first, second;
    limit = Limit();
    size = Count(q);
    if (extra > limit - size) {
        LobbyEventDequeMap_LengthError();
    }
    if (size < limit / 3) {
        first = (size + 1) * 3 / 5;
        return size + *Max(&extra, &first);
    } else if (size < limit / 3 * 2) {
        second = (size + 1) / 2;
        return size + *Max(&extra, &second);
    }
    return limit;
}
