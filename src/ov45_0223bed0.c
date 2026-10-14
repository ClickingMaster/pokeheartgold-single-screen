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
extern char ov45_022551C4[];
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

u32 LobbyEventVector_RecommendSize(Vec *p, u32 n) {
    u32 used = p->capacity, a, b;
    if (n > 0x1fffffff - used) {
        _ZNSt15__vector_commonILb1EE18throw_length_errorEv();
    }
    if (used < 0xaaaaaaa) {
        a = (used + 1) * 3 / 5;
        return used + *(n > a ? &n : &a);
    }
    if (used < 0x15555554) {
        b = (used + 1) / 2;
        return used + *(n > b ? &n : &b);
    }
    return 0x1fffffff;
}
