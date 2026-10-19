#ifndef POKEHEARTGOLD_LOBBY_TREE_H
#define POKEHEARTGOLD_LOBBY_TREE_H

// The red-black trees behind the lobby's user id sets and user maps (overlay 45). A tree keeps its node count, its
// root and its leftmost node. The root field is also the tree's end node: end() points at it and the root's parent
// is its address. A node's parent field holds the parent's address with the node's colour in bit 0 (set: red).

#include <nitro/types.h>

#include "lobby_string.h"

#include "../lib/NitroDWC/src/tree-iterator4.h"

typedef struct LobbyUserIdSetNode {
    struct LobbyUserIdSetNode *left;
    struct LobbyUserIdSetNode *right;
    u32 parent;
    int userId;
} LobbyUserIdSetNode;

typedef struct LobbyUserIdSet {
    int count;
    LobbyUserIdSetNode *root;
    LobbyUserIdSetNode *first;
} LobbyUserIdSet;

typedef struct LobbyUserIdSetInsertResult {
    LobbyUserIdSetNode *node;
    u8 inserted;
} LobbyUserIdSetInsertResult;

// The user map: user id -> nickname.
typedef struct LobbyUserMapEntry {
    int userId;
    Str nick;
} LobbyUserMapEntry;

typedef struct LobbyUserMapNode {
    struct LobbyUserMapNode *left;
    struct LobbyUserMapNode *right;
    u32 parent;
    LobbyUserMapEntry entry;
} LobbyUserMapNode;

typedef struct LobbyUserMap {
    int count;
    LobbyUserMapNode *root;
    u8 padding_08[4];
    LobbyUserMapNode *first;
} LobbyUserMap;

typedef struct LobbyUserMapInsertResult {
    LobbyUserMapNode *node;
    u8 inserted;
} LobbyUserMapInsertResult;

#ifdef __cplusplus
extern "C" {
#endif

void LobbyUserIdSet_Assign(LobbyUserIdSet *, LobbyUserIdSet *);
void LobbyUserIdSet_CopyNodes(LobbyUserIdSet *, LobbyUserIdSetNode *, LobbyUserIdSetNode *, BOOL);
void *LobbyUserIdSet_Destroy(LobbyUserIdSet *);
void *LobbyUserIdSet_Destroy2(LobbyUserIdSet *);
void LobbyUserIdSet_DestroyNodes(LobbyUserIdSet *, LobbyUserIdSetNode *);
void LobbyUserIdSet_Erase(Iterator *, LobbyUserIdSet *, Iterator);
int LobbyUserIdSet_EraseKey(LobbyUserIdSet *, void *);
LobbyUserIdSetNode *LobbyUserIdSet_FindInsertPos(LobbyUserIdSet *, int *, LobbyUserIdSetNode **, u8 *, u8 *);
void LobbyUserIdSet_Insert(LobbyUserIdSetInsertResult *, LobbyUserIdSet *, int *);
LobbyUserIdSetNode *LobbyUserIdSet_InsertNode(LobbyUserIdSet *, LobbyUserIdSetNode *, int, int, const int *);
void LobbyUserIdSet_Swap(LobbyUserIdSet *, LobbyUserIdSet *);
void LobbyUserMap_DestroyNodes(LobbyUserMap *, LobbyUserMapNode *);
void LobbyUserMap_Erase(Iterator *, LobbyUserMap *, Iterator);
LobbyUserMapNode *LobbyUserMap_FindInsertPos(LobbyUserMap *, int *, LobbyUserMapNode **, u8 *, u8 *);
void LobbyUserMap_Insert(LobbyUserMapInsertResult *, LobbyUserMap *, LobbyUserMapEntry *);
LobbyUserMapNode *LobbyUserMap_InsertNode(LobbyUserMap *, LobbyUserMapNode *, int, int, LobbyUserMapEntry *);

#ifdef __cplusplus
}
#endif

#endif // POKEHEARTGOLD_LOBBY_TREE_H
