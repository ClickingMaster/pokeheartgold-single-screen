#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
typedef struct {u8 pad[8];u32 normalKey;u8 pad2[12];u32 compressedKey;u8 pad3[16];u32 paletteKey;u8 pad4[4];u16 dictOffset;u8 pad5[6];u8 texDict[1];} Texture;
typedef struct {u32 param,size;} TexData;
typedef struct {u16 offset,flags;} PaletteData;
typedef struct {u8 pad[16];u32 texParam,paletteParam;u8 pad2[20];u16 width,height;s32 scaleX,scaleY;} Material;
extern void *NNS_G3dGetResDataByName(void *,const void *);
extern s32 FX_Div(s32,s32);
void SetTexParamaters_(Texture *,const void *,Material *);
void SetPlttParamaters_(Texture *,const void *,Material *);
static inline u32 KeyAddr(u32 key){return (key&65535)<<3;}
static inline u32 RawAddr(const void *address){return (u32)address>>3;}
#pragma force_active on
void SetPlttParamaters_(Texture *texture,const void *name,Material *mat){PaletteData *data=texture&&texture->dictOffset?NNS_G3dGetResDataByName((u8*)texture+texture->dictOffset,name):0;u16 offset=(u16)((u16)texture->paletteKey&0x1fffffff);u16 base=data->offset;if(!(data->flags&1)){base>>=1;offset>>=1;}mat->paletteParam=base+offset;}
