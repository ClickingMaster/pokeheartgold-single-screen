#include "global.h"

#include "dwc/net_common_internal.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>
#include <stdarg.h>

extern int LobbyCharVector_Destroy();
extern int LobbyString_Destroy();
u8 *PPWLobbyWebRequest_Delete(u8 *p);

u8 *PPWLobbyWebRequest_Delete(u8 *p) {
    LobbyCharVector_Destroy(p + 40);
    LobbyString_Destroy(p + 24);
    LobbyString_Destroy(p + 12);
    if (p) {
        DWC_Free(0, p, 0);
    }
    return p;
}
