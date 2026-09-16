#ifndef POKEHEARTGOLD_SLOP_PORT_OVERLAY02_2_PRIVATE_H
#define POKEHEARTGOLD_SLOP_PORT_OVERLAY02_2_PRIVATE_H

#include "global.h"

#include "pokemon.h"
#include "sprite.h"
#include "sys_task_api.h"

void ov02_0224A7A8(void *a0, PokepicTemplate *tmpl);
void *ov02_0224A7B8(Pokemon *mon, void *arg1, enum HeapID heapID);
void ov02_0224A8D4(void *work);
void ov02_0224AB58(void *work);
BOOL ov02_0224ABCC(void *a0, void *a1);
void ov02_0224AC04(void *a0, u8 *sm);
void ov02_0224AC38(void *work);
int ov02_0224ACE0(void *work);
void *ov02_0224B298(void *mgr, void *arg1);
void ov02_0224B2CC(void *work);
void ov02_0224B314(void *work);
void ov02_0224B364(void *a0, void *work);
void ov02_0224B3B0(void *a0, void *work);
void ov02_0224B45C(SysTask *task, void *sm);
int ov02_0224B4AC(void *work);
int ov02_0224B5F0(void *work);
int ov02_0224B638(void *work);
int ov02_0224B664(void *work);
BOOL ov02_0224B6D0(void *a0, void *out);
void ov02_0224B6E4(void *a0, void *work);
void ov02_0224B72C(void *work);
void ov02_0224B784(void *work);
BOOL ov02_0224B7CC(void *a0, void **out);
void ov02_0224B808(void *a0, void *work);

#endif // POKEHEARTGOLD_SLOP_PORT_OVERLAY02_2_PRIVATE_H
