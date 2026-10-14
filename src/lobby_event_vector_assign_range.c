#pragma force_active on
#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
int printf(const char *, ...);
void abort(void);
#define W(p, n) (*(u32 *)((u8 *)(p) + (n)))
#define P(p, n) (*(void **)((u8 *)(p) + (n)))

typedef struct {
    int a, b;
} Pair;

typedef struct {
    Pair *p;
} Iter;

typedef struct {
    u8 x;
} Empty;

typedef struct {
    Pair *data;
    u32 size, capacity;
} Vec;

extern u32 ov45_02255198[];
extern u32 ov45_022577D0;
extern u32 ov45_022551A4;
extern char _ZSt7nothrow[];
extern char sStr_CdequeLengthError[];
void *_ZnamRKSt9nothrow_t(u32, void *);
void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);
void DWC_Free(int, void *, int);
void *PPWLobby_GetWork(void);
int PPWLobbyPlayers_IsFull(void *);
void DWC_GetDateTime(RTCDate *, RTCTime *);
void LobbyEventDeque_LengthError(const char *);
void LobbyEventDequeMap_LengthError(void);
void LobbySort_GetTemporaryBuffer(void *, int);
int PPWLobbySchedule_IsSizeValid(void *, void *, u32);
int PPWLobbySchedule_ValidateVipRecords(void *, void *, u32);
int PPWLobbySchedule_GetQuestionnaire(void *, void *);
int PPWLobbySchedule_SetQuestionnaire(void *, void *);
int PPWLobbySchedule_CanStart(void *);
int PPWLobbySchedule_Start(void *);
void LobbyEventDeque_EraseAtEnd(void *, u32);
void LobbyEventDeque_EraseAtBegin(void *, u32);
u32 LobbyEventDequeMap_RecommendSize(void *, u32);
u32 LobbyVipVector_RecommendSize(Vec *, u32);
u32 LobbyEventVector_RecommendSize(Vec *, u32);
void LobbySort_InsertionSort(Iter, Iter);
void LobbyEventVector_AssignRange(Vec *, Pair *, Pair *);
void LobbyVipVector_AssignRange(Vec *, Pair *, Pair *);
void LobbyEventVector_Reallocate(Vec *, u32, u32);
void LobbyVipVector_Reallocate(Vec *, u32, u32);
void LobbyVipVector_Allocate(Vec *, u32, Empty);
void LobbyEventVector_Allocate(Vec *, u32, Empty);
int PPWLobbySchedule_SetVipRecords(void *, void *, u32);
void LobbyEventDeque_IteratorAt(void *, void *, u32);
void LobbyEventDequeMapIterator_Advance(void *, u32);

void LobbySort_GetTemporaryBuffer(void *p, int n) {
    P(p, 0) = 0;
    W(p, 4) = ov45_02255198[1];
    while (n > 0) {
        P(p, 0) = _ZnamRKSt9nothrow_t(n * 8, _ZSt7nothrow);
        if (P(p, 0)) {
            W(p, 4) = n;
            return;
        }
        n /= 2;
    }
}

void LobbyEventVector_AssignRange(Vec *p, Pair *first, Pair *last) {
    u32 n = last - first, growth;
    Pair *end, *dest;
    volatile u8 a, c, d, b;
    if (p->capacity < n) {
        a = 0;
        growth = LobbyEventVector_RecommendSize(p, n - p->capacity);
        goto alloc;
    }
    end = first;
    end += *(p->size < n ? &p->size : &n);
    dest = p->data;
    while (first < end) {
        dest->a = first->a;
        dest->b = first->b;
        ++first;
        ++dest;
    }
    if (n < p->size) {
        u32 old = p->size;
        c = 0;
        p->size = *(volatile u32 *)&p->size - (old - n);
        return;
    }
    if (p->size >= n) {
        return;
    }
    d = 0;
    dest = *(Pair *volatile *)&p->data + *(volatile u32 *)&p->size;
    while (end != last) {
        if (dest) {
            *dest = *end;
        }
        ++p->size;
        ++end;
        ++dest;
    }
    return;
alloc:
    LobbyEventVector_Reallocate(p, n, growth);
    b = 0;
    dest = p->data + *(volatile u32 *)&p->size;
    for (; first != last; ++first, ++dest) {
        if (dest) {
            *dest = *first;
        }
        ++p->size;
    }
}

int PPWLobbySchedule_IsSizeValid(void *p, void *d, u32 n) {
    if (n < 16) {
        return 0;
    }
    return n >= ((*(u16 *)((u8 *)d + 14) - 1) * 8 + 24);
}
