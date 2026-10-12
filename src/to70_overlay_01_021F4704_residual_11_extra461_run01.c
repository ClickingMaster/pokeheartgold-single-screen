#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x85c];void *a;u32 pad2;u32 enabled;void *props;u8 pad3[0x204];void *extra;} Chunk;
typedef struct {u32 pad;u8 slots[2][0x30];u8 pad2[0x2c];Chunk *chunks[4];u8 pad3[0x10];u32 flag,mode;u8 pad4[0x38];u32 final;} W;
extern void sub_0201A430(void),ov01_021F477C(void *),Bdhc_Free(void *),MapPropManager_Free(void *),Heap_Free(void *);
extern void NNS_GfdResetLnkTexVramState(void),NNS_GfdResetLnkPlttVramState(void);
void MapLoadManager_End(W *);
extern void NNS_GfdResetLnkTexVramState(void),NNS_GfdResetLnkPlttVramState(void);
void MapLoadManager_End(W *w){u8 i;sub_0201A430();NNS_GfdResetLnkTexVramState();NNS_GfdResetLnkPlttVramState();for(i=0;i<2;i++)ov01_021F477C(w->slots[i]);for(i=0;i<4;i++){w->chunks[i]->enabled=0;Bdhc_Free(w->chunks[i]->a);MapPropManager_Free(w->chunks[i]->props);if(w->chunks[i]->extra){Heap_Free(w->chunks[i]->extra);w->chunks[i]->extra=0;}Heap_Free(w->chunks[i]);w->chunks[i]=0;}w->mode=3;w->flag=1;w->final=0;}
