#ifndef POKEHEARTGOLD_BILLBOARD_LISTS_H
#define POKEHEARTGOLD_BILLBOARD_LISTS_H

#include "constants/heap.h"

#include "sprite.h"

void BillboardLists_Create(int count, enum HeapID heapID);
void BillboardLists_Delete(void);
void BillboardLists_Draw(void);
void Billboard_ClearPendingFree(void *data);
void BillboardObject_SetVisible(Sprite *sprite, int draw);
NNSG3dResMdl *BillboardObject_GetModel(void *object);

#endif // POKEHEARTGOLD_BILLBOARD_LISTS_H
