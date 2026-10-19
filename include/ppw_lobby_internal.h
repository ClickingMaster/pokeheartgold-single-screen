#ifndef POKEHEARTGOLD_PPW_LOBBY_INTERNAL_H
#define POKEHEARTGOLD_PPW_LOBBY_INTERNAL_H

// PPW lobby (overlay 45): the lobby player sessions, the event deques and vectors behind them, and the
// functions shared between the PPW lobby source files.

#include "global.h"

#include "dwc/net_common_internal.h"

#include "lobby_mem.h"
#include "lobby_string.h"

#include "../lib/NitroDWC/src/tree-iterator4.h"

#include "ppw_lobby_work.h"

typedef struct DequeBuf {
    u32 **ptr;
    u32 begin;
    u32 size;
    u32 cap;
} DequeBuf;

typedef struct Iter {
    u32 *cur;
    u32 *first;
    u32 *mid;
    u32 *last;
} Iter;

typedef struct Vec {
    void *ptr;
    u32 size;
    u32 cap;
} Vec;

typedef struct VObj VObj;

typedef struct VTable {
    void (*f0)(VObj *);
    void (*dtor)(VObj *);
} VTable;

struct VObj {
    VTable *vt;
};

extern u32 gLobbyEventDequeMapBufferMaxSizeSet;
extern u32 gLobbyEventDequeMapBufferMaxSize;
extern u32 gLobbyEventDequeMapMaxSizeSet;
extern u32 gLobbyEventDequeMapMaxSize;
extern unsigned char __lower_mapC[];
void LobbyEventDequeMap_LengthError(void);
void _ZNSt15__vector_commonILb1EE18throw_length_errorEv(void);

typedef struct Tag {
    u8 c;
} Tag;

void LobbyString_ReplaceRange(Str *, u32, u32, const char *, const char *, Tag);
int LobbyString_CompareChars(const u8 *, u32, u32, const void *, u32);

typedef struct Session {
    VTable *vt;
    u32 channel;
    s64 time;
    u32 unk10;
    u32 systemProfileChanged;
    u32 userProfileData;
    u32 userProfileSize;
    u32 userProfileCapacity;
    u32 userProfileChanged;
    int state;
    int id;
    u32 systemProfileReceived;
    u32 userProfileReceived;
    u32 entered;
} Session;

typedef struct Node {
    void *left;
    void *right;
    u32 parent;
    Session *val;
} Node;

typedef struct List {
    u32 count;
    void *root;
    void *leftmost;
} List;

typedef Iterator NodeIter;

void LobbyCharVector_AppendFillSlow(LobbyCharVector *, u32, const char *);
u32 LobbyU32Vector_RecommendSize(Vec *, u32);
void LobbyU32Vector_AppendFillSlow(Vec *, u32, u32 *);
void LobbyTreeIterator_Advance(NodeIter *);
Node *LobbyTree_EraseRebalance(Node *, void *);
extern char gLobbyStr_TreeInsertLengthError_2[];
int printf(const char *, ...);
void abort(void);
void LobbyTree_InsertRebalance(Node *, void *);

typedef struct InsertResult {
    Node *current;
    u8 inserted;
} InsertResult;

extern u8 ov45_02255200[];
Node *PPWLobbyPlayerSet_FindInsertPos(List *l, Session **key, Node **parent, u8 *left, u8 *flag);
Node *PPWLobbyPlayerSet_InsertNode(List *l, Node *parent, u8 left, u8 flag, Session **key);

typedef struct SessionMgr {
    u32 unk0;
    List list;
    Session self;
} SessionMgr;

void PPWLobbyPlayerSet_Erase(NodeIter *out, List *l, NodeIter it);

typedef struct FindState {
    int id;
    Node *end;
    NodeIter it;
} FindState;

void LobbyU32Vector_Reserve(Vec *v, u32 n);
void LobbyU32Vector_PushBackSlow(Vec *v, u32 *x, Tag unused);

static inline void VecReserve(Vec *v, u32 n) {
    if (n > v->cap) {
        LobbyU32Vector_Reserve(v, n);
    }
}

void PPWLobbyPlayers_GetEnteredIds(Vec *out, SessionMgr *m);
void LobbyU32Vector_Destroy(Vec *v);
void LobbyU32Vector_Allocate(Vec *v, u32 n, Tag unused);
Vec *LobbyU32Vector_Destroy2(Vec *v);

static inline void SwapU32(u32 *a, u32 *b) {
    u32 t = *a;
    *a = *b;
    *b = t;
}

static inline void SwapPtr(void **a, void **b) {
    void *t = *a;
    *a = *b;
    *b = t;
}

void PPWLobbyPlayer_UpdateState(Session *s);
extern VTable gPPWLobbyPlayerVtable;
void PPWLobbyPlayerSet_Insert(InsertResult *out, List *l, Session **key);
extern char gLobbyStr_SpaceSpace[];
extern char gLobbyStr_Space_2[];
extern char gLobbyStr_B[];
extern char gLobbyStr_S[];
extern char gLobbyStr_Empty_16[];
extern char gLobbyFmt_Hex_3[];
extern char gLobbyStr_A[];
extern char gLobbyStr__[];
extern char gLobbyFmt_Str_3[];
int DWC_Base64Encode(const void *src, u32 len, char *dst, u32 dstLen);
void MI_CpuFill8(void *dst, u8 val, u32 size);
void LobbyEventDequeMap_Allocate(DequeBuf *d, u32 n, Tag unused);
int LobbyEventDequeMapIterator_Distance2(Iter *a, Iter *b);
void LobbyEventDequeMap_AppendRange(DequeBuf *d, Iter *a, Iter *b, Tag unused);

static inline void TouchTag(void) {
    volatile Tag t = { 0 };
}

typedef struct SplitBuf {
    u32 *ptr;
    u32 size;
    u32 cap;
    u32 *alloc;
    u32 start;
} SplitBuf;

void LobbyU32VectorBuffer_Allocate(SplitBuf *b, u32 cap, Tag unused);
SplitBuf *LobbyU32VectorBuffer_Destroy(SplitBuf *b);
BOOL PPWLobbyPlayers_Add(SessionMgr *m, int id);
BOOL PPWLobbyPlayers_Remove(SessionMgr *m, int id);
Session *PPWLobbyPlayers_Find(SessionMgr *m, int id);

void LobbyCharVector_PushBackSlow(LobbyCharVector *s, const char *c);
void LobbyEventDequeMapBuffer_Allocate(DequeBuf *d, u32 n);
void LobbyEventDequeMapBuffer_AppendRange(DequeBuf *d, Iter *a, Iter *b);
Vec *LobbyEventDequeMapBuffer_Destroy(Vec *v);
void LobbyEventDequeMapBuffer_PrependRange(DequeBuf *d, Iter *a, Iter *b);
void LobbyEventDequeMapIterator_Advance(Iter *it, int n);
Iter *LobbyEventDequeMapIterator_Advance2(Iter *it, int n);
void LobbyEventDequeMap_InitCopy(DequeBuf *d, Iter a, Iter *b);
void LobbyEventVector_Allocate(Vec *v, u32 n);
int LobbyString_CompareSubstring(const u8 *s, u32 a, u32 b, const u8 *o);
int LobbyString_ToLowerChar(int c);
void LobbyVipVector_Allocate(Vec *v, u32 n);
BOOL PPWLobbyPlayer_HasProfiles(Session *s);
BOOL PPWLobbyPlayers_IsFull(SessionMgr *m);
BOOL PPWLobby_EncodeMessage(int type, int mode, u32 value, const void *data, u32 len, Str *out);

// The state of the lobby library (PPWLobbyState), read from the current work.
static inline int PPWLobby_GetState(void) {
    PPWLobbyWork *volatile *work = &gPPWLobbyWork;
    return (*work)->state;
}

// The web requests and the event schedule the server sends (the work's own functions are in ppw_lobby_work.h).
int PPWLobbySchedule_IsSizeValid(void *schedule, void *data, u32 size);
void LobbyEventDeque_EraseAtBegin(void *deque, u32 n);
void LobbyEventDeque_LengthError(const char *message);
void LobbySort_GetTemporaryBuffer(void *buffer, int n);

#endif // POKEHEARTGOLD_PPW_LOBBY_INTERNAL_H
