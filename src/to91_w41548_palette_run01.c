// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#include <nnsys/g3d/binres/res_struct.h>
#include <nnsys/g3d/binres/res_struct_accessor_inline.h>
#pragma force_active on
typedef struct {u16 texOffset,paletteOffset;} Materials;
typedef struct {u16 offset;u8 count,flags;} Binding;
typedef struct {u32 header,diffuse,specular,polygon,polygonMask,texture,reserved18;u16 palette,flags,width,height;fx32 ratioW,ratioH;} Material;
typedef struct {u16 offset,flags;} Palette;
static inline Material *GetMaterial(Materials *materials,u32 index){if(materials){u32 *offset=NNS_G3dGetResDataByIdx((NNSG3dResDict*)((u8*)materials+4),index);if(offset)return (Material*)((u8*)materials+*offset);}return 0;}
void bindMdlPltt_Internal_(void*,void*,void*,void*);
void bindMdlPltt_Internal_(void *materialData,void *bindingData,void *textureData,void *paletteData){Materials *materials=materialData;Binding *binding=bindingData;NNSG3dResTex *texture=textureData;u16 flags,value;u32 key;u16 base;u8 *ids;u32 i;Palette *palette;palette=paletteData;key=(u16)texture->plttInfo.vramKey;flags=palette->flags;ids=(u8*)materials+binding->offset;base=(u16)(key&~0xe0000000);value=palette->offset;if(!(flags&1)){value=(u16)(value>>1);base=(u16)(base>>1);}i=0;if(i<binding->count)do{Material *material=GetMaterial(materials,ids[i]);material->palette=(u16)(value+base);i++;}while(i<binding->count);binding->flags|=1;}
