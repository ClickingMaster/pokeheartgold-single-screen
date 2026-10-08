#pragma force_active on
#include "global.h"
#include <nitro/code32.h>
typedef struct {u8 pad[8];u32 normalKey;u8 pad2[12];u32 compressedKey;u8 pad3[32];u8 texDict[1];} Texture;
typedef struct {u32 param,size;} TexData;
typedef struct {u8 pad[16];u32 texParam,paletteParam;u8 pad2[20];u16 width,height;s32 scaleX,scaleY;} Material;
extern void *NNS_G3dGetResDataByName(void *,const void *);
extern s32 FX_Div(s32,s32);
void SetTexParamaters_(Texture *,const void *,Material *);
static inline u32 GetTexAddr_(const Texture *t, const TexData *d){ if ((d->param & 0x1c000000) != 0x14000000) return (u16)t->normalKey & ~0xe0000000; return (u16)t->compressedKey & ~0xe0000000; }
// @fn SetTexParamaters_
void SetTexParamaters_(Texture *texture,const void *name,Material *mat)
{
    TexData *data = texture ? NNS_G3dGetResDataByName(texture->texDict, name) : 0;
    u32 w, h;
    u32 addr;
    addr = GetTexAddr_(texture, data);
    mat->texParam &= 0xc00f0000;
    mat->texParam |= data->param + addr;
    mat->width = data->size & 0x7ff;
    mat->height = (data->size & (0x7ff << 11)) >> 11;
    w = data->size & 0x7ff;
    h = (data->size & (0x7ff << 11)) >> 11;
    mat->scaleX = w == mat->width ? 0x1000 : FX_Div(w << 12, mat->width << 12);
    mat->scaleY = h == mat->height ? 0x1000 : FX_Div(h << 12, mat->height << 12);
}
