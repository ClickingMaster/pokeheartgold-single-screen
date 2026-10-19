#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
extern long atol(const char *);

typedef union {
    u32 words[3];

    struct {
        u8 flags;
        char small[11];
    } local;

    struct {
        u32 long_ : 1;
        u32 rest : 31;
        u32 length;
        char *text;
    } heap;
} Text;

typedef struct {
    Text *items;
    volatile u32 count;
    u32 capacity;
} Vector;

typedef struct {
    void (*onMessage)(int, void *, void *);
    void (*onExcessFlood)(int, void *);
    u32 param;
} Callbacks;

typedef struct {
    void *connection;
    int state;
    u8 unk_08[0x5C];
    Callbacks callbacks;
} Work;

extern Work *gLobbyChatManager;
extern char sStr_SERVER[];
extern char sStr_ExcessFlood[];
extern char sStr_SpaceColon[];
extern void LobbyString_InitFromCStr(Text *, char *);
extern int LobbyString_CompareNoCase(Text *, Text *, int);
extern void LobbyString_Destroy(Text *);
extern void LobbyStringVector_Destroy(Vector *);
extern void LobbyChat_SplitString(Vector *, Text *, Text *, int);
extern int LobbyChat_FindUserIdByNick(Work *, Text *);

static inline char *TextChars(Text *t) {
    if (t->heap.long_ == 0) {
        return t->local.small;
    } else {
        return t->heap.text;
    }
}

static inline int Compare(char *key, char *str, Text *strText, Text *keyText, int len) {
    int result;
    LobbyString_InitFromCStr(strText, str);
    LobbyString_InitFromCStr(keyText, key);
    result = LobbyString_CompareNoCase(strText, keyText, len);
    LobbyString_Destroy(keyText);
    LobbyString_Destroy(strText);
    return result;
}

#pragma opt_propagation off
void LobbyChat_OnPrivateMessage(void *, char *, char *, int, void *);

// Messages from the server only matter as excess flood notices; user messages are passed on by user ID.
void LobbyChat_OnPrivateMessage(void *chat, char *nick, char *message, int type, void *param) {
    struct {
        Text messageStr, floodStr, nickStr, serverStr;
        Callbacks messageCallbacks;
        Text senderNick;
        Callbacks floodCallbacks;
        Text separator, text;
        Vector vector;
    } frame;

    char *serverKey, *floodKey;
    int serverCmp, floodCmp, userId;
    if (gLobbyChatManager && gLobbyChatManager->state == 5) {
        return;
    }
    serverKey = sStr_SERVER;
    serverCmp = Compare(serverKey, nick, &frame.nickStr, &frame.serverStr, 6);
    if (serverCmp == 0) {
        floodKey = sStr_ExcessFlood;
        floodCmp = Compare(floodKey, message, &frame.messageStr, &frame.floodStr, 12);
        if (floodCmp != 0) {
            return;
        }
        LobbyString_InitFromCStr(&frame.text, message);
        LobbyString_InitFromCStr(&frame.separator, sStr_SpaceColon);
        LobbyChat_SplitString(&frame.vector, &frame.text, &frame.separator, 0);
        LobbyString_Destroy(&frame.separator);
        LobbyString_Destroy(&frame.text);
        if (frame.vector.count == 0) {
            LobbyStringVector_Destroy(&frame.vector);
            return;
        }
        {
            Text *last = &frame.vector.items[frame.vector.count - 1];
            userId = atol(TextChars(last));
        }
        if (userId == 0) {
            LobbyStringVector_Destroy(&frame.vector);
            return;
        }
        frame.floodCallbacks = gLobbyChatManager->callbacks;
        frame.floodCallbacks.onExcessFlood(userId, param);
        LobbyStringVector_Destroy(&frame.vector);
        return;
    }
    if (type != 3) {
        return;
    }
    LobbyString_InitFromCStr(&frame.senderNick, nick);
    userId = LobbyChat_FindUserIdByNick(gLobbyChatManager, &frame.senderNick);
    LobbyString_Destroy(&frame.senderNick);
    if (userId == -1) {
        return;
    }
    frame.messageCallbacks = gLobbyChatManager->callbacks;
    frame.messageCallbacks.onMessage(userId, message, param);
}
