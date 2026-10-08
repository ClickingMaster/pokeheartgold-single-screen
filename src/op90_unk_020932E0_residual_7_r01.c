#pragma force_active on
#include "global.h"
#include "sprite.h"
#include "filesystem.h"
#include "heap.h"
typedef struct Data30 { u32 words[12]; } Data30;
typedef struct Elem { Sprite *sprite; Data30 data; } Elem;
typedef struct Big { u32 unk0; u32 unk4; u32 unk8; u8 pad[0x20C - 12]; } Big;
typedef struct W {
    u32 unk0;
    enum HeapID heapId;
    u8 pad8[0x198 - 8];
    Elem a[31];
    u8 pad7E4[4];
    Elem b[3];
    u8 pad884[0x8D4 - 0x884];
    Big big[30];
    u8 pad463C[8];
    void (*callback)(void *);
} W;
extern void sub_02094C08(Data30 *data, Big *big, int idx, Sprite *sprite, void *buf, NARC *narc, int fileId);
extern void sub_02094758(void *);
void sub_02094668(W *w, u8 idx, int src, BOOL fromB);
// @fn sub_02094668
void sub_02094668(W *w, u8 idx, int src, BOOL fromB){void *buf; NARC *narc; int i; narc = NARC_New((NarcId)20, w->heapId); buf = Heap_AllocAtEnd(w->heapId, 0x1000); for (i = 0; i < 30; i++) { w->big[i].unk8 = 0; } if (fromB) { w->b[idx].data = w->b[src].data; sub_02094C08(&w->b[src].data, w->big, idx, w->b[idx].sprite, buf, narc, 0x222); } else { w->b[idx].data = w->a[src].data; sub_02094C08(&w->a[src].data, w->big, idx, w->b[idx].sprite, buf, narc, 0x222); } Heap_Free(buf); NARC_Delete(narc); w->callback = sub_02094758;}
