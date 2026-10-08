#pragma force_active on
#include "global.h"
// clang-format off
#include <nitro/code32.h>
// clang-format on
#include <nnsys/g3d/binres/res_struct.h>
typedef struct {u16 textureOffset,paletteOffset;NNSG3dResDict dict;} Materials;
typedef struct {u16 offset;u8 count,flags;} Binding;
typedef struct {u8 prefix[0x14];u32 image;u8 settings[8];u16 width,height;fx32 scaleS,scaleT;} Material;
typedef struct {u32 image,dimensions;} TextureEntry;
void bindMdlTex_Internal_(Materials *,Binding *,NNSG3dResTex *,const TextureEntry *);
static inline Material *GetMaterial(Materials *materials,u32 index) {
 NNSG3dResDict *dict;NNSG3dResDictEntryHeader *header;u32 *entry;
 if(materials) {
  dict=&materials->dict;
  if(dict && index<materials->dict.numEntry) {
   header=(NNSG3dResDictEntryHeader*)((u8*)dict+materials->dict.ofsEntry);
   entry=(u32*)((u8*)header+4+header->sizeUnit*index);
  } else entry=0;
  if(entry)return (Material*)((u8*)materials+*entry);
 }
 return 0;
}
// @fn bindMdlTex_Internal_
void bindMdlTex_Internal_(Materials *materials,Binding *binding,NNSG3dResTex *texture,const TextureEntry *item) {
 u32 base; u32 index;u8 *indices=(u8*)materials+binding->offset;
 if((item->image&0x1c000000)!=0x14000000)base=(u16)texture->texInfo.vramKey&~0xe0000000;
 else base=(u16)texture->tex4x4Info.vramKey&~0xe0000000;
for(index=0;index<binding->count;index++){
  Material *material=GetMaterial(materials,indices[index]);u32 width,height;
  material->image|=item->image+base;
  width=item->dimensions&0x7ff;height=(item->dimensions&0x3ff800)>>11;
  material->scaleS=width==material->width?4096:FX_Div(width<<12,material->width<<12);
  material->scaleT=height==material->height?4096:FX_Div(height<<12,material->height<<12);
}
 binding->flags|=1;
}
