#ifndef POKEHEARTGOLD_LOBBY_MEM_H
#define POKEHEARTGOLD_LOBBY_MEM_H

// The lobby library's allocator: DWC_Alloc with an out-of-memory handler that drops the reserve block and
// disconnects the chat session.

#include <nitro/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void *LobbyMem_Alloc(u32);
void LobbyMem_ReleaseReserve(void);

#ifdef __cplusplus
}
#endif

#endif // POKEHEARTGOLD_LOBBY_MEM_H
