#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

int LobbyString_FindFirstOfChars(Str *s, const char *chars, u32 pos, u32 n) {
    const char *data;
    u32 len;
    const char *end;
    const char *it;
    if (s->l.isLong) {
        data = s->l.ptr;
        len = s->l.size;
    } else {
        len = s->s.len;
        data = s->s.buf;
    }
    if (pos < len && n != 0) {
        end = data + len;
        it = data + pos;
        if (it < end) {
            do {
                const char *c = chars;
                u32 i;
                for (i = 0; i < n; i++, c++) {
                    if (*it == *c) {
                        return it - data;
                    }
                }
                it++;
            } while (it < end);
        }
    }
    return -1;
}

// Find the parent for a new node with this key, the last node not greater than it, and which side it goes on.
LobbyRequestMapNode *LobbyRequestMap_FindInsertPos(LobbyRequestMap *map, LobbyRequest **key, LobbyRequestMapNode **pos, u8 *insertLeft, u8 *isLeftmost) {
    LobbyRequestMapNode *parent;
    LobbyRequestMapNode *n;
    *pos = NULL;
    n = map->root;
    *insertLeft = 1;
    *isLeftmost = 1;
    parent = (LobbyRequestMapNode *)&map->root;
    while (n != NULL) {
        parent = n;
        if ((*key)->id < n->request->id) {
            n = n->left;
            *insertLeft = 1;
        } else {
            *pos = n;
            n = n->right;
            *insertLeft = 0;
            *isLeftmost = 0;
        }
    }
    return parent;
}

void *LobbyMem_Alloc(u32 size) {
    void *p;
    if (size == 0) {
        size = 1;
    }
    for (;;) {
        VoidFunc f;
        p = DWC_Alloc(0, size);
        if (p != NULL) {
            return p;
        }
        f = gLobbyMemOutOfMemoryHandler;
        gLobbyMemOutOfMemoryHandler = f;
        if (f == NULL) {
            break;
        }
        f();
    }
    return NULL;
}

void LobbyMem_OnOutOfMemoryDisconnect(void) {
    chatDisconnect(ov45_022577E8.manager->connection);
    gLobbyMemOutOfMemoryHandler = NULL;
}

void LobbyMem_OnOutOfMemory(void) {
    LobbyChatManager *manager;
    if (ov45_022577E8.buf != NULL) {
        DWC_Free(0, ov45_022577E8.buf, 0);
        ov45_022577E8.buf = NULL;
    }
    manager = ov45_022577E8.manager;
    manager->lastError = 2;
    manager->state = LOBBY_CHAT_STATE_ERROR;
    gLobbyMemOutOfMemoryHandler = LobbyMem_OnOutOfMemoryDisconnect;
}

BOOL LobbyMem_InstallOutOfMemoryHandler(LobbyChatManager *manager) {
    void *buf;
    ov45_022577E8.manager = manager;
    if (ov45_022577E8.buf != NULL) {
        DWC_Free(0, ov45_022577E8.buf, 0);
        ov45_022577E8.buf = NULL;
    }
    buf = DWC_Alloc(0, 0x400);
    ov45_022577E8.buf = buf;
    if (buf == NULL) {
        return FALSE;
    }
    gLobbyMemOutOfMemoryHandler = LobbyMem_OnOutOfMemory;
    return TRUE;
}

void LobbyMem_ReleaseReserve(void) {
    if (ov45_022577E8.buf != NULL) {
        DWC_Free(0, ov45_022577E8.buf, 0);
        ov45_022577E8.buf = NULL;
    }
}

BOOL LobbyRequestMap_DeleteKey(LobbyRequestMap *map, u32 id) {
    LobbyRequest key;
    Iter tmp;
    LobbyRequest *pk;
    Iter it;
    Iter res;
    key.vtable = &gLobbyRequestVtable;
    key.id = id;
    key.param = 0;
    pk = &key;
    LobbyRequestMap_Find(&it, map, &pk);
    tmp = it;
    if (it.current == &map->root) {
        return FALSE;
    }
    if (((LobbyRequestMapNode *)it.current)->request != NULL) {
        ((LobbyRequestMapNode *)it.current)->request->vtable->destroyAndFree(((LobbyRequestMapNode *)it.current)->request);
    }
    LobbyRequestMap_Erase(&res, map, tmp);
    return TRUE;
}

void LobbyRequestMap_DeleteAll(LobbyRequestMap *map) {
    Iter it;
    Iter tmp;
    Iter res;
    it.current = map->leftmost;
    while (it.current != &map->root) {
        LobbyRequest *request = ((LobbyRequestMapNode *)it.current)->request;
        if (request != NULL) {
            request->vtable->destroyAndFree(request);
        }
        tmp.current = it.current;
        ov45_02236E58(&it);
        LobbyRequestMap_Erase(&res, map, tmp);
    }
}

void LobbyRequestMap_Erase(Iter *out, LobbyRequestMap *map, Iter pos) {
    LobbyRequestMapNode *n = pos.current;
    if (n == map->leftmost) {
        ov45_02236E58(&pos);
        map->leftmost = pos.current;
    }
    out->current = LobbyTree_EraseRebalance(n, map->root);
    if (n != NULL) {
        DWC_Free(0, n, 0);
    }
    map->count--;
}
