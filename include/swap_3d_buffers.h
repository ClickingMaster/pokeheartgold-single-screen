#ifndef POKEHEARTGOLD_SWAP_3D_BUFFERS_H
#define POKEHEARTGOLD_SWAP_3D_BUFFERS_H

#include "global.h"

void Init3DBufferSwapRequestState(void);
void Thunk_G3X_Reset(void);
void RequestSwap3DBuffers(GXSortMode sortMode, GXBufferMode bufferMode);
void Handle3DBufferSwapRequest(void);

#endif // POKEHEARTGOLD_SWAP_3D_BUFFERS_H
