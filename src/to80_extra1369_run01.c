#pragma force_active on
#include "global.h"
#define V(w,o) (*(u32 *)((u8 *)(w)+(o)))
typedef struct{void *sprite;u8 rest[48];} Entry;
extern VecFx32 *Sprite_GetMatrixPtr(void *);
extern void Sprite_SetAnimCtrlSeq(void *,int),Sprite_SetDrawPriority(void *,int),Sprite_SetMatrix(void *,VecFx32 *),Sprite_SetDrawFlag(void *,int),sub_02094C08(void *,void *,int,void *,void *,void *,int),sub_02094758(void);
extern void *NARC_New(int,int),*Heap_AllocAtEnd(int,int);
extern void NARC_Delete(void *),Heap_Free(void *);
typedef struct{u32 initial,heap;u8 p[0x4644-8];void(*callback)(void);u8 p2[0x4680-0x4648];u32 active,index;VecFx32 position;u8 p3[8];int kind;Entry *selected;} State;
void sub_020948C4(u8 *,int,int);
void sub_020948C4(u8 *w,int kind,int index){State *state=(State *)w;Entry *entries;state->active=1;state->index=index;state->kind=kind;if(kind==1){entries=(Entry *)(w+0x198);state->selected=entries;}else if(kind==2){entries=(Entry *)(w+0x7e8);state->selected=entries;}else GF_AssertFail();state->position=*Sprite_GetMatrixPtr(entries[index].sprite);Sprite_SetAnimCtrlSeq((void *)V(w,0x8c0),46);Sprite_SetDrawPriority(state->selected[state->index].sprite,3);if(kind==1){VecFx32 *pos=Sprite_GetMatrixPtr(state->selected[state->index].sprite);int i;void *narc,*buffer;{u8 *cur=w;for(i=0;i<30;i++){V(cur,0x8dc)=0;cur+=0x20c;}}narc=NARC_New(20,state->heap);buffer=Heap_AllocAtEnd(state->heap,4096);sub_02094C08((u8 *)&state->selected[state->index]+4,w+0x8d4,0,(void *)V(w,0x7b0),buffer,narc,0x2b1);Sprite_SetMatrix((void *)V(w,0x7b0),pos);Sprite_SetDrawFlag((void *)V(w,0x7b0),1);Heap_Free(buffer);NARC_Delete(narc);state->callback=sub_02094758;}}
