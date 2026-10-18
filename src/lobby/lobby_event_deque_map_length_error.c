#include "global.h"

// The code below is compiled as ARM (32-bit) code.
#include <nitro/code32.h>

#include "ppw_lobby_internal.h"

extern char sStr_CdequeLengthError[];

void LobbyEventDequeMap_LengthError(void) {
    printf(sStr_CdequeLengthError);
    abort();
}
