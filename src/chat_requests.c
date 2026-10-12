#pragma force_active on
#include <string.h>

#include "global.h"

#include "dwc/chat_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#pragma force_active on

// Requests sent to the chat server whose answers are still awaited: the ChatConnection.requests list, the results
// they collect, completion (queues the user callback) and cancellation, and the channel mode strings.

extern void __msl_assertion_failed(const char *, const char *, const char *, int);
extern long atol(const char *);
extern const char ov45_02255E60[];
extern const char ov45_02255E6C[];
extern const char ov45_02255E78[];
extern const char ov45_02255EB4[];
extern const char ov45_02255EC4[];
extern const char ov45_02255F74[];
extern const char ov45_02255F84[];
extern const char ov45_02255F94[];
extern const char ov45_02255FC4[];
extern const char ov45_02255FD4[];
extern const char ov45_02256010[];
extern const char ov45_0225609C[];
extern const char ov45_022560B0[];
extern const char ov45_02256128[];
extern const char ov45_022565C0[];
extern const char ov45_022565D0[];
extern const char ov45_022565E0[];
extern const char ov45_022565F0[];
extern const char ov45_022565F8[];
extern const char ov45_02256630[];
extern const char ov45_02256638[];
extern const char ov45_02256644[];
extern const char ov45_02256654[];
extern const char ov45_0225665C[];
extern const char ov45_02256690[];
extern const char ov45_02256694[];
extern const char ov45_022566A0[];
extern const char ov45_022566A8[];
extern const char ov45_022566BC[];
extern const char ov45_022566DC[];
extern const char ov45_022566E0[];

#define ASSERT(cond, message, func, line)                           \
    if (!(cond)) {                                                  \
        __msl_assertion_failed(message, ov45_022565D0, func, line); \
    }
#define ASSERT_CHAT(chat, func, line)          ASSERT(chat, ov45_022565C0, func, line)
#define ASSERT_EXPR(cond, message, func, line) ((cond) ? (void)0 : __msl_assertion_failed(message, ov45_022565D0, func, line))

#define REQUEST_TIMEOUT 60000 // ms

// Finds the request matching one of the keys (names compared without case) and extends its timeout.
ChatRequest *ov45_022496AC(ChatConnection *chat, int count, const ChatRequestKey *keys) {
    int i;
    const char *name;
    const char *name2;
    ChatRequest *request;
    ASSERT_EXPR(chat != NULL, ov45_022565C0, ov45_02255EB4, 0x15B);
    ASSERT_EXPR(count > 0, ov45_022565E0, ov45_02255EB4, 0x15D);
    ASSERT_EXPR(keys != NULL, ov45_022565F0, ov45_02255EB4, 0x15E);
    for (request = chat->requests; request != NULL; request = request->next) {
        for (i = 0; i < count; i++) {
            const ChatRequestKey *key = &keys[i];
            ASSERT_EXPR(key->type >= 0 && key->type < CHAT_REQUEST_TYPE_COUNT, ov45_022565F8, ov45_02255EB4, 0x164);
            if (request->type != key->type) {
                continue;
            }
            name = key->name;
            name2 = key->name2;
            if (!((name == NULL && request->name == NULL) || (name != NULL && request->name != NULL && strnicmp(name, request->name) == 0))) {
                continue;
            }
            if (!((name2 == NULL && request->name2 == NULL) || (name2 != NULL && request->name2 != NULL && strnicmp(name2, request->name2) == 0))) {
                continue;
            }
            request->expireTime = ov00_021FF368() + REQUEST_TIMEOUT;
            return request;
        }
    }
    return NULL;
}

// Finds the request of type 12 for the nick.
ChatRequest *ov45_0224983C(ChatConnection *chat, const char *nick) {
    ChatRequest *request;
    ASSERT_CHAT(chat, ov45_0225609C, 0x17F);
    ASSERT(nick, ov45_02256630, ov45_0225609C, 0x181);
    ASSERT(*nick, ov45_02256638, ov45_0225609C, 0x182);
    request = chat->requests;
    while (request) {
        if (request->type == 12 && strnicmp(((ChatKeysData *)request->data)->arg.nick, nick) == 0) {
            return request;
        }
        request = request->next;
    }
    return NULL;
}

void ov45_02249904(ChatRequest *request) {
    ASSERT(request, ov45_02256644, ov45_02255FC4, 403);
    ov00_021EC3D8(request->data);
    ov00_021EC3D8(request->name);
    ov00_021EC3D8(request->name2);
    ov00_021EC3D8(request);
}

// Unlinks and frees the request.
void ov45_02249958(ChatConnection *chat, ChatRequest *target) {
    ChatRequest *previous = NULL, *head, *request;
    ASSERT_CHAT(chat, ov45_02255F74, 0x19F);
    ASSERT(target, ov45_02256644, ov45_02255F74, 0x1A1);
    head = chat->requests;
    request = head;
    while (request) {
        if (request == target) {
            if (head == request) {
                chat->requests = request->next;
            }
            if (chat->lastRequest == request) {
                chat->lastRequest = previous;
            }
            if (previous) {
                previous->next = request->next;
            }
            ov45_02249904(request);
            return;
        }
        previous = request;
        request = request->next;
    }
}

// Completes the request: queues its callback with the results (callback types 14 .. 31, by request type), frees
// the collected data and the request.
void ov45_02249A0C(ChatConnection *chat, ChatRequest *request, u32 *results) {
    int i, valid;
    ChatRequestData *data;
    ASSERT(request, ov45_02256654, ov45_02255F84, 0x1C1);
    valid = 0;
    if (request->type >= 0 && request->type < CHAT_REQUEST_TYPE_COUNT) {
        valid = 1;
    }
    ASSERT(valid, ov45_0225665C, ov45_02255F84, 0x1C2);
    if (request->type == 0) {
        data = request->data;
        if (request->callback2) {
            ov45_02244D30(chat, 14, request->callback2, results, request->param, request->id, NULL, 20);
        }
        for (i = 0; i < data->channels.count; i++) {
            ov00_021EC3D8(data->channels.names[i]);
            ov00_021EC3D8(data->channels.topics[i]);
        }
        ov00_021EC3D8(data->channels.names);
        ov00_021EC3D8(data->channels.topics);
        ov00_021EC3D8(data->channels.numUsers);
    } else if (request->type == 1) {
        if (request->callback) {
            ov45_02244D30(chat, 15, request->callback, results, request->param, request->id, NULL, 12);
        }
    } else if (request->type == 2) {
        i = results[1];
        if (request->callback) {
            ov45_02244D30(chat, 16, request->callback, results, request->param, request->id, (char *)i, 12);
        }
    } else if (request->type == 3) {
        data = request->data;
        if (request->callback) {
            ov45_02244D30(chat, 19, request->callback, results, request->param, request->id, NULL, 20);
        }
        for (i = 0; i < data->nicks.count; i++) {
            ov00_021EC3D8(data->nicks.nicks[i]);
        }
        ov00_021EC3D8(data->nicks.nicks);
        ov00_021EC3D8(data->nicks.modes);
    } else if (request->type == 4) {
        data = request->data;
        if (request->callback) {
            ov45_02244D30(chat, 20, request->callback, results, request->param, request->id, NULL, 28);
        }
        for (i = 0; i < data->strings.count; i++) {
            ov00_021EC3D8(data->strings.strings[i]);
        }
        ov00_021EC3D8(data->strings.strings);
        ov00_021EC3D8(data->strings.unk_04);
        ov00_021EC3D8(data->strings.unk_08);
        ov00_021EC3D8(data->strings.unk_00);
    } else if (request->type == 10) {
        if (request->callback) {
            ov45_02244D30(chat, 21, request->callback, results, request->param, request->id, NULL, 16);
        }
    } else if (request->type == 11) {
        if (request->callback) {
            ov45_02244D30(chat, 22, request->callback, results, request->param, request->id, NULL, 16);
        }
    } else if (request->type == 5) {
        if (request->callback) {
            ov45_02244D30(chat, 17, request->callback, results, request->param, request->id, NULL, 12);
        }
    } else if (request->type == 6) {
        if (request->callback) {
            ov45_02244D30(chat, 23, request->callback, results, request->param, request->id, NULL, 16);
        }
    } else if (request->type == 7) {
        ov00_021EC3D8(*(void **)request->data);
    } else if (request->type == 8) {
        data = request->data;
        if (request->callback) {
            ov45_02244D30(chat, 24, request->callback, results, request->param, request->id, NULL, 16);
        }
        for (i = 0; i < data->keys.count; i++) {
            ov00_021EC3D8(data->keys.keys[i]);
        }
    } else if (request->type == 9) {
        if (request->callback) {
            ov45_02244D30(chat, 26, request->callback, results, request->param, request->id, NULL, 12);
        }
    } else if (request->type == 12) {
        data = request->data;
        if (request->callback) {
            ov45_02244D30(chat, 29, request->callback, results, request->param, request->id, NULL, 20);
        }
        ov00_021EC3D8(data->keys.arg.nick);
        for (i = 0; i < data->keys.count; i++) {
            ov00_021EC3D8(data->keys.keys[i]);
        }
        ov00_021EC3D8(data->keys.keys);
    } else if (request->type == 13) {
        data = request->data;
        if (request->callback) {
            ov45_02244D30(chat, 30, request->callback, results, request->param, request->id, NULL, 24);
        }
        for (i = 0; i < data->keys.count; i++) {
            ov00_021EC3D8(data->keys.keys[i]);
        }
        ov00_021EC3D8(data->keys.keys);
    } else if (request->type == 14) {
        data = request->data;
        if (request->callback) {
            ov45_02244D30(chat, 30, request->callback, results, request->param, request->id, NULL, 24);
        }
        for (i = 0; i < data->keys.count; i++) {
            ov00_021EC3D8(data->keys.keys[i]);
        }
        ov00_021EC3D8(data->keys.keys);
    } else if (request->type == 15) {
        data = request->data;
        if (request->callback) {
            ov45_02244D30(chat, 27, request->callback, results, request->param, request->id, NULL, 16);
        }
        for (i = 0; i < data->nicks.count; i++) {
            ov00_021EC3D8(data->nicks.nicks[i]);
        }
        ov00_021EC3D8(data->nicks.nicks);
        ov00_021EC3D8(data->nicks.modes);
    } else if (request->type == 16) {
        if (request->callback) {
            ov45_02244D30(chat, 31, request->callback, results, request->param, request->id, NULL, 8);
        }
    } else {
        __msl_assertion_failed(ov45_02256690, ov45_022565D0, ov45_02255F84, 0x25C);
    }
    ov45_02249958(chat, request);
}

// Ends the request without an answer: completes it with failed (zeroed) results.
void ov45_0224A14C(ChatConnection *chat, ChatRequest *request) {
    BOOL valid;
    ASSERT(request, ov45_02256654, ov45_02256010, 0x268);
    valid = FALSE;
    if (request->type >= 0 && request->type < CHAT_REQUEST_TYPE_COUNT) {
        valid = TRUE;
    }
    ASSERT(valid, ov45_0225665C, ov45_02256010, 0x269);
    if (request->type == 0) {
        u32 results[5];
        results[0] = 0;
        results[1] = 0;
        results[2] = 0;
        results[3] = 0;
        results[4] = 0;
        ov45_02249A0C(chat, request, results);
        return;
    }
    if (request->type == 1) {
        u32 results[3];
        results[0] = 0;
        results[1] = (u32)7;
        results[2] = (u32)request->name;
        ov45_02249A0C(chat, request, results);
        return;
    }
    if (request->type == 2) {
        u32 results[3];
        results[0] = 0;
        results[1] = (u32)request->name;
        results[2] = 0;
        ov45_02249A0C(chat, request, results);
        return;
    }
    if (request->type == 3) {
        u32 results[5];
        results[0] = 0;
        results[1] = (u32)request->name;
        results[2] = 0;
        results[3] = 0;
        results[4] = 0;
        ov45_02249A0C(chat, request, results);
        return;
    }
    if (request->type == 4) {
        u32 results[7];
        results[0] = 0;
        results[1] = (u32)request->name;
        results[2] = 0;
        results[3] = 0;
        results[4] = 0;
        results[5] = 0;
        results[6] = 0;
        ov45_02249A0C(chat, request, results);
        return;
    }
    if (request->type == 10) {
        u32 results[4];
        results[0] = 0;
        results[1] = (u32)request->name;
        results[2] = 0;
        results[3] = 0;
        ov45_02249A0C(chat, request, results);
        return;
    }
    if (request->type == 11) {
        u32 results[5];
        results[0] = 0;
        results[1] = (u32)request->name;
        results[2] = 0;
        results[3] = 0;
        results[4] = 0;
        ov45_02249A0C(chat, request, results);
        return;
    }
    if (request->type == 5) {
        u32 results[3];
        results[0] = 0;
        results[1] = (u32)request->name;
        results[2] = 0;
        ov45_02249A0C(chat, request, results);
        return;
    }
    if (request->type == 6) {
        u32 results[4];
        results[0] = 0;
        results[1] = (u32)request->name2;
        results[2] = (u32)request->name;
        results[3] = 0;
        ov45_02249A0C(chat, request, results);
        return;
    }
    if (request->type == 7) {
        ov45_02249A0C(chat, request, NULL);
        return;
    }
    if (request->type == 8) {
        u32 results[4];
        results[1] = (u32)request->name;
        results[2] = 0;
        results[3] = 0;
        ov45_02249A0C(chat, request, results);
        return;
    }
    if (request->type == 9) {
        u32 results[3];
        results[0] = 0;
        results[1] = (u32)request->name;
        results[2] = (u32)request->name2;
        ov45_02249A0C(chat, request, results);
        return;
    }
    if (request->type == 12) {
        u32 results[5];
        results[0] = 0;
        results[1] = 0;
        results[2] = 0;
        results[3] = 0;
        results[4] = 0;
        ov45_02249A0C(chat, request, results);
        return;
    }
    if (request->type == 13) {
        u32 results[6];
        results[0] = 0;
        results[1] = 0;
        results[2] = 0;
        results[3] = 0;
        results[4] = 0;
        results[5] = 0;
        ov45_02249A0C(chat, request, results);
        return;
    }
    if (request->type == 14) {
        u32 results[6];
        results[0] = 0;
        results[1] = 0;
        results[2] = 0;
        results[3] = 0;
        results[4] = 0;
        results[5] = 0;
        ov45_02249A0C(chat, request, results);
        return;
    }
    if (request->type == 15) {
        ov45_02249958(chat, request);
        return;
    }
    if (request->type == 16) {
        u32 results[2];
        results[0] = 0;
        results[1] = (u32)ov45_02256694;
        ov45_02249A0C(chat, request, results);
        return;
    }
    __msl_assertion_failed(ov45_02256690, ov45_022565D0, ov45_02256010, 0x311);
}

// Ends every request whose timeout has passed.
void ov45_0224A578(ChatConnection *chat) {
    ChatRequest *request, *next;
    u32 now;
    ASSERT_CHAT(chat, ov45_02255EC4, 794);
    now = ov00_021FF368();
    for (request = chat->requests; request; request = next) {
        next = request->next;
        if (now > request->expireTime) {
            ov45_0224A14C(chat, request);
        }
    }
}

// Returns a new request id (1 .. 0x7FFFFFFF, wrapping).
int ov45_0224A5E4(ChatConnection *chat) {
    ASSERT_CHAT(chat, ov45_02255E60, 813);
    u32 id = chat->nextRequestId;
    u32 next;
    if (id == 0x7FFFFFFF) {
        next = 1;
    } else {
        next = id + 1;
    }
    chat->nextRequestId = next;
    return id;
}

// Returns whether the request with this id is still waiting.
BOOL ov45_0224A630(ChatConnection *chat, int id) {
    ChatRequest *request;
    ASSERT_CHAT(chat, ov45_022560B0, 832);
    ASSERT(id > 0, ov45_022566A0, ov45_022560B0, 834);
    for (request = chat->requests; request; request = request->next) {
        if (request->id == id) {
            return TRUE;
        }
    }
    return FALSE;
}

// Ends all requests.
void ov45_0224A6B4(ChatConnection *chat) {
    ASSERT_CHAT(chat, ov45_02255FD4, 851);
    while (chat->requests) {
        ov45_0224A14C(chat, chat->requests);
    }
}

// Appends a request (names are copied) and returns its id, 0 when out of memory.
int ov45_0224A708(ChatConnection *chat, int type, const char *name, const char *name2, void *callback, void *callback2, void *param, void *data) {
    ChatRequest *request;
    ASSERT_CHAT(chat, ov45_02255E6C, 860);
    request = ov00_021EC3A8(sizeof(ChatRequest));
    if (!request) {
        return 0;
    }
    memset(request, 0, sizeof(ChatRequest));
    request->type = type;
    request->expireTime = ov00_021FF368() + REQUEST_TIMEOUT;
    request->callback = callback;
    request->callback2 = callback2;
    request->param = param;
    request->data = data;
    request->name = name ? ov00_021FF3D4(name) : NULL;
    request->name2 = name2 ? ov00_021FF3D4(name2) : NULL;
    request->id = ov45_0224A5E4(chat);
    if (!chat->requests) {
        chat->requests = request;
    } else {
        chat->lastRequest->next = request;
    }
    chat->lastRequest = request;
    return request->id;
}

int ov45_0224A7F0(ChatConnection *chat, void *callbackAll, void *callbackEach, void *param) {
    void *data = ov00_021EC3A8(sizeof(ChatChannelListData));
    if (!data) {
        return 0;
    }
    memset(data, 0, sizeof(ChatChannelListData));
    return ov45_0224A708(chat, 0, NULL, NULL, callbackAll, callbackEach, param, data);
}

int ov45_0224A874(ChatConnection *chat, const char *channel, void *callback, void *param, const ChatChannelCallbacks *callbacks, const char *password) {
    ChatEnterData *data;
    int id;
    ASSERT(password, ov45_022566A8, ov45_02255F94, 913);
    ASSERT(strlen(password) < 32, ov45_022566BC, ov45_02255F94, 914);
    data = ov00_021EC3A8(sizeof(ChatEnterData));
    if (!data) {
        return 0;
    }
    memset(data, 0, sizeof(ChatEnterData));
    data->callbacks = *callbacks;
    strncpy(data->password, password, 32);
    data->password[31] = 0;
    id = ov45_0224A708(chat, 1, channel, NULL, callback, NULL, param, data);
    if (!id) {
        ov00_021EC3D8(data);
    }
    return id;
}

int ov45_0224A978(ChatConnection *chat, const char *channel, ChatEnumUsersFunc callback, void *param) {
    void *data = ov00_021EC3A8(sizeof(ChatNickListData));
    if (!data) {
        return 0;
    }
    memset(data, 0, sizeof(ChatNickListData));
    return ov45_0224A708(chat, 3, channel, NULL, callback, NULL, param, data);
}

int ov45_0224A9F8(ChatConnection *chat, const char *channel, void *callback, void *param) {
    return ov45_0224A708(chat, 5, channel, NULL, callback, NULL, param, NULL);
}

// Copies the keys, without the empty ones.
int ov45_0224AA2C(ChatConnection *chat, const char *cookie, int count, const char **keys, BOOL allUsers, BOOL broadcastKeys, void *callback, void *param) {
    int i, j;
    ChatKeysData *data = ov00_021EC3A8(sizeof(ChatKeysData));
    if (!data) {
        return 0;
    }
    memset(data, 0, sizeof(*data));
    data->broadcastKeys = broadcastKeys;
    data->count = count;
    data->arg.allUsers = allUsers;
    if (broadcastKeys) {
        data->count--;
    }
    data->keys = ov00_021EC3A8(data->count * 4);
    if (!data->keys) {
        ov00_021EC3D8(data);
        return 0;
    }
    i = 0;
    j = i;
    for (; i < count; i++) {
        const char *key = keys[i];
        if (strcmp(key, ov45_022566DC) != 0) {
            data->keys[j] = ov00_021FF3D4(key);
            if (!data->keys[j]) {
                for (j--; j >= 0; j--) {
                    ov00_021EC3D8(data->keys[j]);
                }
                ov00_021EC3D8(data->keys);
                ov00_021EC3D8(data);
                return 0;
            }
            j++;
        }
    }
    data->count = j;
    return ov45_0224A708(chat, 13, cookie, NULL, callback, NULL, param, data);
}

int ov45_0224ABA0(ChatConnection *chat, const char *cookie, int count, const char **keys, BOOL broadcastKeys, void *callback, void *param) {
    int i, j;
    ChatKeysData *data = ov00_021EC3A8(12);
    const char *key;
    if (!data) {
        return 0;
    }
    {
        u8 *p = (u8 *)data;
        p[0] = 0;
        p[1] = 0;
        p[2] = 0;
        p[3] = 0;
        p[4] = 0;
        p[5] = 0;
        p[6] = 0;
        p[7] = 0;
        p[8] = 0;
        p[9] = 0;
        p[10] = 0;
        p[11] = 0;
    }
    data->arg.broadcastKeys = broadcastKeys;
    data->count = count;
    if (broadcastKeys) {
        data->count--;
    }
    if (data->count != 0) {
        data->keys = ov00_021EC3A8(data->count * 4);
        if (!data->keys) {
            ov00_021EC3D8(data);
            return 0;
        }
        for (i = 0, j = 0; i < count; i++) {
            key = keys[i];
            if (strcmp(key, ov45_022566DC) == 0) {
                continue;
            }
            data->keys[j] = ov00_021FF3D4(key);
            if (!data->keys[j]) {
                for (--j; j >= 0; j--) {
                    ov00_021EC3D8(data->keys[j]);
                }
                ov00_021EC3D8(data->keys);
                ov00_021EC3D8(data);
                return 0;
            }
            j++;
        }
        data->count = j;
    }
    return ov45_0224A708(chat, 14, cookie, NULL, callback, NULL, param, data);
}

// Parses a channel mode string ("+" / "-" then mode letters, "+" / "-" may appear again; letters b, k, l, o and v
// take the next argument) into a 0-terminated array of changes (heap).
#pragma opt_strength_reduction off

ChatModeChange *ov45_0224AD24(const char *modes, const char **args, int argCount) {
    ChatModeChange *changes = NULL;
    char c;
    int type;
    BOOL set;
    BOOL hasArg;
    int count;
    count = 0;
    hasArg = FALSE;
    ASSERT(modes != NULL, ov45_022566E0, ov45_02255E78, 0x494);
    if (*modes == '+') {
        set = TRUE;
    } else if (*modes == '-') {
        set = FALSE;
    } else {
        return NULL;
    }
    modes++;
    do {
        c = *modes++;
        switch (c) {
        case '+':
            set = TRUE;
            type = -1;
            break;
        case '-':
            set = FALSE;
            type = -1;
            break;
        case '\0':
            type = CHAT_MODE_END;
            hasArg = FALSE;
            break;
        case 'i':
            type = CHAT_MODE_INVITE_ONLY;
            hasArg = FALSE;
            break;
        case 'l':
            type = CHAT_MODE_LIMIT;
            hasArg = TRUE;
            break;
        case 'p':
            type = CHAT_MODE_PRIVATE;
            hasArg = FALSE;
            break;
        case 's':
            type = CHAT_MODE_SECRET;
            hasArg = FALSE;
            break;
        case 'k':
            type = CHAT_MODE_KEY;
            hasArg = TRUE;
            break;
        case 'm':
            type = CHAT_MODE_MODERATED;
            hasArg = FALSE;
            break;
        case 'n':
            type = CHAT_MODE_NO_EXTERNAL_MESSAGES;
            hasArg = FALSE;
            break;
        case 't':
            type = CHAT_MODE_ONLY_OPS_CHANGE_TOPIC;
            hasArg = FALSE;
            break;
        case 'o':
            type = CHAT_MODE_OP;
            hasArg = TRUE;
            break;
        case 'v':
            type = CHAT_MODE_VOICE;
            hasArg = TRUE;
            break;
        case 'b':
            type = CHAT_MODE_BAN;
            hasArg = TRUE;
            break;
        case 'u':
            type = CHAT_MODE_U;
            hasArg = FALSE;
            break;
        case 'w':
            type = CHAT_MODE_W;
            hasArg = FALSE;
            break;
        case 'e':
            type = CHAT_MODE_E;
            hasArg = FALSE;
            break;
        default:
            type = -1;
            break;
        }
        if (type != -1) {
            ChatModeChange *change;
            ChatModeChange *grown = ov00_021EC3BC(changes, (count + 1) * sizeof(ChatModeChange));
            if (grown == NULL) {
                ov00_021EC3D8(changes);
                return NULL;
            }
            changes = grown;
            change = &changes[count++];
            memset(change, 0, sizeof(ChatModeChange));
            change->set = set;
            change->type = type;
            if (hasArg) {
                if (argCount > 0) {
                    argCount--;
                    change->arg = *args++;
                } else {
                    change->arg = NULL;
                }
            }
        }
    } while (c != '\0');
    return changes;
}

#pragma opt_strength_reduction reset

// (read through a volatile view, as the original does)
#define MODE_TYPE(change) (((volatile ChatModeChange *)(change))->type)

// Applies the channel mode changes (up to the CHAT_MODE_END entry) to mode.
void ov45_0224AFD8(ChatChannelMode *mode, ChatModeChange *changes) {
    if (MODE_TYPE(changes) == CHAT_MODE_END) {
        return;
    }
    do {
        switch (MODE_TYPE(changes)) {
        case CHAT_MODE_BAN:
            break;
        case CHAT_MODE_INVITE_ONLY:
            mode->inviteOnly = changes->set;
            break;
        case CHAT_MODE_LIMIT:
            if (changes->set && changes->arg) {
                mode->limit = atol(changes->arg);
            } else {
                mode->limit = 0;
            }
            break;
        case CHAT_MODE_PRIVATE:
            mode->private = changes->set;
            break;
        case CHAT_MODE_SECRET:
            mode->secret = changes->set;
            break;
        case CHAT_MODE_KEY:
            break;
        case CHAT_MODE_MODERATED:
            mode->moderated = changes->set;
            break;
        case CHAT_MODE_NO_EXTERNAL_MESSAGES:
            mode->noExternalMessages = changes->set;
            break;
        case CHAT_MODE_ONLY_OPS_CHANGE_TOPIC:
            mode->onlyOpsChangeTopic = changes->set;
            break;
        case CHAT_MODE_OP:
        case CHAT_MODE_VOICE:
        case CHAT_MODE_U:
        case CHAT_MODE_W:
            break;
        case CHAT_MODE_E:
            mode->unk_18 = changes->set;
            break;
        default:
            __msl_assertion_failed(ov45_02256690, ov45_022565D0, ov45_02256128, 0x56C);
            break;
        }
        changes++;
    } while (MODE_TYPE(changes));
}
