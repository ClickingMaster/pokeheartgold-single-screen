#ifndef POKEHEARTGOLD_LOBBY_CHAT_MANAGER_H
#define POKEHEARTGOLD_LOBBY_CHAT_MANAGER_H

// Lobby chat: the chat session and the channels it is in, for the C and C++ chat sources.

#include <nitro/types.h>

#include "lobby_chat_request.h"
#include "lobby_string.h"
#include "lobby_timer.h"
#include "lobby_tree.h"

enum LobbyChatState {
    LOBBY_CHAT_STATE_INIT,
    LOBBY_CHAT_STATE_CONNECTING,
    LOBBY_CHAT_STATE_CONNECTED, // logged in, waits 60 frames before READY
    LOBBY_CHAT_STATE_READY,
    LOBBY_CHAT_STATE_DISCONNECTING,
    LOBBY_CHAT_STATE_ERROR,
};

// What the code that started the session is told about the connection, with its parameter.
typedef struct LobbyChatConnectCallbacks {
    void (*onConnected)();
    void (*onError)();
    void *param;
} LobbyChatConnectCallbacks;

// What a channel reports to the code that entered it, with that code's parameter.
typedef struct LobbyChannelCallbacks {
    void (*onUserJoined)(const char *channel, int userId, void *param);
    void (*onUserParted)(const char *channel, int userId, void *param);
    void (*onChannelMessage)(const char *channel, int userId, void *message, void *param);
    void (*onBroadcastKeyChanged)(const char *channel, int userId, void *key, void *value, void *param);
    void (*onTopicChanged)(const char *channel, void *topic, void *param);
    void *param;
} LobbyChannelCallbacks;

// A channel the session is in: its users, the user limit it was entered with and its callbacks.
typedef struct LobbyChannel {
    BOOL usersEnumerated;
    LobbyUserIdSet users;
    int userLimit;
    LobbyChannelCallbacks callbacks;
} LobbyChannel;

// The channel map: full channel name -> channel.
typedef struct LobbyChannelMapEntry {
    Str name;
    LobbyChannel channel;
} LobbyChannelMapEntry;

typedef struct LobbyChannelMapNode {
    struct LobbyChannelMapNode *left;
    struct LobbyChannelMapNode *right;
    u32 parent;
    LobbyChannelMapEntry entry;
} LobbyChannelMapNode;

typedef struct LobbyChannelMap {
    int count;
    LobbyChannelMapNode *root;
    u8 padding_08[4];
    LobbyChannelMapNode *first;
} LobbyChannelMap;

typedef struct LobbyChannelMapInsertResult {
    LobbyChannelMapNode *node;
    u8 inserted;
} LobbyChannelMapInsertResult;

// The chat session: the CHAT connection, the strings it logged in with and the user, channel, request and timer maps.
typedef struct LobbyChatManager {
    void *connection;
    int state;
    int userId;
    Str nick;
    Str user;
    Str realName;
    Str gameName;
    Str gameKey;
    Str channelPrefix;
    u32 unk54;
    int lastError;
    int connectedFrames;
    int disconnected;
    LobbyChatConnectCallbacks connectCallbacks;
    LobbyUserMap userMap;
    LobbyChannelMap channelMap;
    LobbyRequestMap requestMap;
    LobbyTimerMap timerMap;
} LobbyChatManager;

#ifdef __cplusplus
extern "C" {
#endif

extern LobbyChatManager *gLobbyChatManager;

void *LobbyChannelMap_Destroy(LobbyChannelMap *);
void LobbyChannelMap_DestroyNodes(LobbyChannelMap *, LobbyChannelMapNode *);
void LobbyChannelMap_Erase(Iterator *, LobbyChannelMap *, Iterator);
int LobbyChannelMap_EraseKey(LobbyChannelMap *, Str *);
void LobbyChannelMap_Find(Iterator *, LobbyChannelMap *, Str *);
LobbyChannelMapNode *LobbyChannelMap_FindInsertPos(LobbyChannelMap *, Str *, LobbyChannelMapNode **, u8 *, u8 *);
void LobbyChannelMap_Insert(LobbyChannelMapInsertResult *, LobbyChannelMap *, LobbyChannelMapEntry *);
LobbyChannelMapNode *LobbyChannelMap_InsertNode(LobbyChannelMap *, LobbyChannelMapNode *, u8, u8, LobbyChannelMapEntry *);

#ifdef __cplusplus
}
#endif

#endif // POKEHEARTGOLD_LOBBY_CHAT_MANAGER_H
