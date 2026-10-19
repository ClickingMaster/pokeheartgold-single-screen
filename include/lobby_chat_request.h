#ifndef POKEHEARTGOLD_LOBBY_CHAT_REQUEST_H
#define POKEHEARTGOLD_LOBBY_CHAT_REQUEST_H

// Lobby chat: the commands waiting for the server's answer. The chat manager keeps each request in its
// request map, keyed by id; the answer callback looks the request up, reports to the caller's callback with the
// caller's parameter and deletes the request.

#include <nitro/types.h>

typedef struct LobbyRequest LobbyRequest;

typedef struct LobbyRequestVtable {
    void (*destroy)(LobbyRequest *request);
    LobbyRequest *(*destroyAndFree)(LobbyRequest *request);
    u32 (*getId)(LobbyRequest *request);
    u32 (*getParam)(LobbyRequest *request);
} LobbyRequestVtable;

// A request with its vtable pointer, for the code that builds requests and lookup keys.
struct LobbyRequest {
    LobbyRequestVtable *vtable;
    u32 id;
    u32 param;
};

typedef void (*LobbyEnterChannelCallback)(BOOL success, int error, const char *channel, u32 param);
typedef void (*LobbyGetChannelModeCallback)(BOOL success, const char *channel, u32 mode, u32 param);
typedef void (*LobbyGetChannelKeysCallback)(BOOL success, const char *channel, int userId, u32 key, u32 value, u32 param);
typedef void (*LobbyEnumChannelsCallback)(BOOL success, u32 count, const char *channels, u32 topics, u32 userCounts, u32 param);
typedef void (*LobbyEnumUsersCallback)(BOOL success, const char *channel, u32 count, void *userIds, u32 param);

// The chat manager's requests by id. The root field is also the map's end node: end() points at it.
typedef struct LobbyRequestMapNode {
    struct LobbyRequestMapNode *left;
    struct LobbyRequestMapNode *right;
    struct LobbyRequestMapNode *parent;
    LobbyRequest *request;
} LobbyRequestMapNode;

typedef struct LobbyRequestMap {
    u32 count;
    LobbyRequestMapNode *root;
    LobbyRequestMapNode *leftmost;
} LobbyRequestMap;

typedef struct LobbyRequestMapInsertResult {
    LobbyRequestMapNode *node;
    u8 inserted;
} LobbyRequestMapInsertResult;

#ifdef __cplusplus
extern "C" {
#endif

extern LobbyRequestVtable gLobbyRequestVtable;
extern LobbyRequestVtable gLobbyGetChannelModeRequestVtable;
extern LobbyRequestVtable gLobbyGetChannelKeysRequestVtable;
extern LobbyRequestVtable gLobbyEnterChannelRequestVtable;
extern LobbyRequestVtable gLobbyEnumUsersRequestVtable;
extern LobbyRequestVtable gLobbyEnumChannelsRequestVtable;

void *LobbyRequestMap_Destroy(LobbyRequestMap *);
void LobbyRequestMap_DestroyNodes(LobbyRequestMap *, LobbyRequestMapNode *);
LobbyRequestMapNode *LobbyRequestMap_InsertNode(LobbyRequestMap *, LobbyRequestMapNode *, int, int, LobbyRequest *const *);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

// A request as a command function builds it: the request with the vtable of its kind and the caller's callback.
struct LobbyCallbackRequest : LobbyRequest {
    u32 callback;
};

// The same request as a C++ object, for the answer callbacks that call its methods.
struct LobbyChatRequest {
    virtual void destroy();
    virtual LobbyChatRequest *destroyAndFree();
    virtual u32 getId();
    virtual u32 getParam();
    u32 id;
    u32 param;
};

struct LobbyEnterChannelRequest : LobbyChatRequest {
    LobbyEnterChannelCallback callback;
};

struct LobbyGetChannelModeRequest : LobbyChatRequest {
    LobbyGetChannelModeCallback callback;
};

struct LobbyGetChannelKeysRequest : LobbyChatRequest {
    LobbyGetChannelKeysCallback callback;
};

struct LobbyEnumChannelsRequest : LobbyChatRequest {
    LobbyEnumChannelsCallback callback;
};

struct LobbyEnumUsersRequest : LobbyChatRequest {
    LobbyEnumUsersCallback callback;
};

struct LobbyRequestMapIterator {
    LobbyRequestMapNode *node;

    LobbyRequestMapIterator() {
    }

    LobbyRequestMapIterator(LobbyRequestMapNode *node)
        : node(node) {
    }
};

extern "C" {

void LobbyRequestMap_Insert(LobbyRequestMapInsertResult *, LobbyRequestMap *, LobbyCallbackRequest **);
LobbyRequestMapIterator LobbyRequestMap_Find(LobbyRequestMap *, LobbyRequest *const &);
void LobbyRequestMap_DeleteKey(LobbyRequestMap *, u32);
}

#endif // __cplusplus

#endif // POKEHEARTGOLD_LOBBY_CHAT_REQUEST_H
