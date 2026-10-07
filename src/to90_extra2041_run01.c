#pragma force_active on
#include "global.h"
#include "heap.h"
#include "pokemon.h"
#include "sprite_system.h"
#include "obj_pltt_transfer.h"
#include "unk_02013FDC.h"
#pragma force_active on
typedef struct {u32 a;PaletteData *palette;u8 pad[0x2c];SpriteSystem *system;SpriteManager *manager;u8 pad2[0x44];ManagedSprite *sprites[1];} State;
void ov80_0222F030(State *,Pokemon *,enum HeapID,int,int,int,int,int,int,u16);
void ov80_0222F030(State *s,Pokemon *mon,enum HeapID heap,int id,int x,int y,int priority,int bgPriority,int blend,u16 color){
 PaletteData *palette=s->palette;SpriteSystem *system=s->system;SpriteManager *manager=s->manager;
 NARC *narc=NARC_New((NarcId)8,heap);PokepicTemplate pic;ManagedSpriteTemplate t;
 ManagedSprite *sprite;void *buffer;u32 pid;int species,i;NNSG2dImageProxy *image;u32 offset;
 SpriteSystem_LoadCharResObjFromOpenNarc(system,manager,narc,112,0,1,id);
 SpriteSystem_LoadPaletteBufferFromOpenNarc(palette,(PaletteBufferId)2,system,manager,narc,111,0,1,1,id);
 SpriteSystem_LoadCellResObjFromOpenNarc(system,manager,narc,113,0,id);
 SpriteSystem_LoadAnimResObjFromOpenNarc(system,manager,narc,114,0,id);NARC_Delete(narc);
 MI_CpuFill8(&t,0,sizeof(t));t.x=x;t.y=y;t.z=0;t.animation=0;t.drawPriority=priority;t.pal=0;t.vram=(NNS_G2D_VRAM_TYPE)1;t.bgPriority=bgPriority;t.vramTransfer=0;
 for(i=0;i<6;i++)t.resIdList[i]=id;
 sprite=SpriteSystem_NewSprite(system,manager,&t);ManagedSprite_TickFrame(sprite);
 buffer=Heap_Alloc(heap,3200);pid=GetMonData(mon,0,0);species=GetMonData(mon,5,0);
 GetPokemonSpriteCharAndPlttNarcIds(&pic,mon,2);
 sub_02014494((NarcId)pic.narcID,pic.charDataID,heap,0,0,10,10,buffer,pid,0,2,species);
 image=Sprite_GetImageProxy(sprite->sprite);DC_FlushRange(buffer,4);GX_LoadOBJ(buffer,image->vramLocation.baseAddrOfVram[1],3200);
 offset=ObjPlttTransfer_GetPaletteVramOffset(Sprite_GetPaletteProxy(sprite->sprite),(NNS_G2D_VRAM_TYPE)1)*16;
 PaletteData_LoadNarc(palette,(NarcId)pic.narcID,pic.palDataID,heap,(PaletteBufferId)2,32,(u16)offset);
 if(blend>0)PaletteData_BlendPalette(palette,(PaletteBufferId)2,(u16)offset,16,(u8)blend,color);
 Heap_Free(buffer);GF_ASSERT(s->sprites[id-50000]==0);s->sprites[id-50000]=sprite;
}
