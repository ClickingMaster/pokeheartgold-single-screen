#pragma force_active on
#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

void *LobbyRequest_Delete(void *p) {
    if (p != NULL) {
        DWC_Free(0, p, 0);
    }
    return p;
}

void LobbyGetChannelModeRequest_Destroy(void) {
}

void *LobbyGetChannelModeRequest_Delete(void *p) {
    if (p != NULL) {
        DWC_Free(0, p, 0);
    }
    return p;
}

void LobbyGetChannelKeysRequest_Destroy(void) {
}

void *LobbyGetChannelKeysRequest_Delete(void *p) {
    if (p != NULL) {
        DWC_Free(0, p, 0);
    }
    return p;
}

void LobbyEnterChannelRequest_Destroy(void) {
}

void *LobbyEnterChannelRequest_Delete(void *p) {
    if (p != NULL) {
        DWC_Free(0, p, 0);
    }
    return p;
}

void LobbyEnumUsersRequest_Destroy(void) {
}

void *LobbyEnumUsersRequest_Delete(void *p) {
    if (p != NULL) {
        DWC_Free(0, p, 0);
    }
    return p;
}

void LobbyEnumChannelsRequest_Destroy(void) {
}

void *LobbyEnumChannelsRequest_Delete(void *p) {
    if (p != NULL) {
        DWC_Free(0, p, 0);
    }
    return p;
}

void LobbyRequestMap_Insert(InsertResult *out, Tree *t, Obj **val) {
    Node *pos;
    u8 a;
    u8 b;
    Node *parent = LobbyRequestMap_FindInsertPos(t, val, &pos, &a, &b);
    if (pos == NULL || pos->key.obj->id < (*val)->id) {
        out->node = LobbyRequestMap_InsertNode(t, parent, a, b, val);
        out->inserted = ov45_02255228[1];
        return;
    }
    out->node = pos;
    out->inserted = ov45_02255228[2];
}
