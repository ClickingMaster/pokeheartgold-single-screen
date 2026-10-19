#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

#include "ppw_lobby_work.h"

PPWLobbyWebRequest *PPWLobbyWebRequest_Delete(PPWLobbyWebRequest *request) {
    LobbyCharVector_Destroy(&request->body);
    LobbyString_Destroy(&request->page);
    LobbyString_Destroy(&request->key);
    if (request) {
        DWC_Free(0, request, 0);
    }
    return request;
}
