#include "global.h"

#include "lobby_tree.h"
#include "lobby_vector.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

typedef struct {
    Iterator advance;
    void *other;
} Iter;

void ov45_02236E58(Iterator *);
u32 LobbyU32Vector_RecommendSize(LobbyU32Vector *, u32);
void LobbyU32Vector_Reallocate(LobbyU32Vector *, u32, u32);
void LobbyU32Vector_AssignFromSet(LobbyU32Vector *, Iter, Iter);

typedef struct {
    volatile u8 fillFlag, reserveFlag;
    u8 pad[2];

    volatile struct {
        LobbyUserIdSetNode *a;
        int b;
        LobbyUserIdSetNode *c;
    } tail;

    Iter fill, count;
} Frame;

void LobbyU32Vector_AssignFromSet(LobbyU32Vector *v, Iter first, Iter last) {
    Frame frame;
    LobbyUserIdSetNode *node;
    int value;
    u32 *out;
    u32 n, capacity;
    frame.count = first;
    Iter *counter = &frame.count;
    n = 0;
    while (frame.count.advance.current != last.advance.current) {
        n++;
        ov45_02236E58(&counter->advance);
    }
    capacity = v->capacity;
    if (n > capacity) {
        frame.reserveFlag = 0;
        u32 p = LobbyU32Vector_RecommendSize(v, n - capacity);
        LobbyU32Vector_Reallocate(v, n, p);
    }
    out = v->data;
    frame.fill = first;
    if (frame.fill.advance.current != last.advance.current) {
        do {
            node = *(void *volatile *)&frame.fill.advance.current;
            frame.fillFlag = 0;
            value = *(volatile int *)&node->userId;
            *out++ = value;
            ov45_02236E58(&frame.fill.advance);
        } while (frame.fill.advance.current != last.advance.current);
        frame.tail.b = value;
        frame.tail.a = node;
        frame.tail.c = node;
    }
    v->size = n;
}
