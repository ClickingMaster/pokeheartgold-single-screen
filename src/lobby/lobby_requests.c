#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "lobby_chat_internal.h"

LobbyRequest *LobbyRequest_Delete(LobbyRequest *request) {
    if (request != NULL) {
        DWC_Free(0, request, 0);
    }
    return request;
}

void LobbyGetChannelModeRequest_Destroy(void) {
}

LobbyRequest *LobbyGetChannelModeRequest_Delete(LobbyRequest *request) {
    if (request != NULL) {
        DWC_Free(0, request, 0);
    }
    return request;
}

void LobbyGetChannelKeysRequest_Destroy(void) {
}

LobbyRequest *LobbyGetChannelKeysRequest_Delete(LobbyRequest *request) {
    if (request != NULL) {
        DWC_Free(0, request, 0);
    }
    return request;
}

void LobbyEnterChannelRequest_Destroy(void) {
}

LobbyRequest *LobbyEnterChannelRequest_Delete(LobbyRequest *request) {
    if (request != NULL) {
        DWC_Free(0, request, 0);
    }
    return request;
}

void LobbyEnumUsersRequest_Destroy(void) {
}

LobbyRequest *LobbyEnumUsersRequest_Delete(LobbyRequest *request) {
    if (request != NULL) {
        DWC_Free(0, request, 0);
    }
    return request;
}

void LobbyEnumChannelsRequest_Destroy(void) {
}

LobbyRequest *LobbyEnumChannelsRequest_Delete(LobbyRequest *request) {
    if (request != NULL) {
        DWC_Free(0, request, 0);
    }
    return request;
}

void LobbyRequestMap_Insert(LobbyRequestMapInsertResult *out, LobbyRequestMap *map, LobbyRequest **val) {
    LobbyRequestMapNode *pos;
    u8 insertLeft;
    u8 isLeftmost;
    LobbyRequestMapNode *parent = LobbyRequestMap_FindInsertPos(map, val, &pos, &insertLeft, &isLeftmost);
    if (pos == NULL || pos->request->id < (*val)->id) {
        out->node = LobbyRequestMap_InsertNode(map, parent, insertLeft, isLeftmost, val);
        out->inserted = ov45_02255228[1];
        return;
    }
    out->node = pos;
    out->inserted = ov45_02255228[2];
}
