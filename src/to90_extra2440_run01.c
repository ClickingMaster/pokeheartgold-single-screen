#pragma force_active on
#include "global.h"
#pragma force_active on

typedef struct{int x,y,w,h;} W31795Rect;typedef struct{u16 narc,file,palette,gap;u32 pid,extra;} W31795Pic;
typedef struct{u16 species,form;u8 pad4[2],shiny,gender;u32 pad8,pid;} W31795Data;
extern const W31795Rect ov96_0221B1AC;extern void ManagedSprite_SetDrawFlag(void*,BOOL),GetMonSpriteCharAndPlttNarcIdsEx(W31795Pic*,u16,u8,u8,u8,u8,u32);
extern void*Heap_AllocAtEnd(u32,u32);extern void Heap_Free(void*),sub_02014510(u16,int,u32,W31795Rect*,void*,u32,BOOL,int,int),ov96_021EED64(void*,void*,u32),ov96_021EED70(void*,u16,u16,u32,u32,u32);
void ov96_021EEC0C(void*,W31795Data*,u32,u32,u32,BOOL);
void ov96_021EEC0C(void*sprite,W31795Data*data,u32 arg,u32 palette,u32 heap,BOOL loadPalette){W31795Pic pic;W31795Rect rect;void*buffer;if(data->species==0){ManagedSprite_SetDrawFlag(sprite,FALSE);return;}GetMonSpriteCharAndPlttNarcIdsEx(&pic,data->species,data->gender,2,data->shiny,(u8)data->form,data->pid);rect=ov96_0221B1AC;buffer=Heap_AllocAtEnd(heap,3200);sub_02014510(pic.narc,pic.file,heap,&rect,buffer,data->pid,FALSE,2,data->species);ov96_021EED64(*(void**)sprite,buffer,3200);if(loadPalette)ov96_021EED70(sprite,pic.narc,pic.palette,arg,palette,heap);ManagedSprite_SetDrawFlag(sprite,TRUE);Heap_Free(buffer);}
