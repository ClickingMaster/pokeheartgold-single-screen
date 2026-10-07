#pragma force_active on
#include <nitro/types.h>
#include <nitro/gx/g3imm.h>
#include <nitro/code32.h>
typedef struct TexFlags {u32 format:4,width:4,height:4,repeat:2,flip:2,zero:1,shared:1,sharedidx:8,pad:6;} TexFlags;
typedef struct Tex {void *data;u32 addr,pltt;TexFlags flags;u16 width,height;} Tex;
void spl_set_tex(Tex *tex);
static inline void Scale(fx32 x, fx32 y, fx32 z){ reg_G3_MTX_SCALE = x; reg_G3_MTX_SCALE = y; reg_G3_MTX_SCALE = z; }
static inline void PlttBase(u32 addr, GXTexFmt fmt){ reg_G3_TEXPLTT_BASE = GX_PACK_TEXPLTTBASE_PARAM(addr, fmt); }
void b1(Tex *tex);
// @fn spl_set_tex
void spl_set_tex(Tex *tex) { TexFlags f = tex->flags; G3_TexImageParam((GXTexFmt)f.format, GX_TEXGEN_TEXCOORD, (GXTexSizeS)f.width, (GXTexSizeT)f.height, (GXTexRepeat)f.repeat, (GXTexFlip)f.flip, (GXTexPlttColor0)f.zero, tex->addr); G3_TexPlttBase(tex->pltt, (GXTexFmt)f.format); G3_MtxMode(GX_MTXMODE_TEXTURE); G3_Identity(); Scale(tex->width << FX32_SHIFT, tex->height << FX32_SHIFT, 0); G3_MtxMode(GX_MTXMODE_POSITION); }
