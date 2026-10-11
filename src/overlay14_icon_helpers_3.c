#include "overlay14_icon_helpers_private.h"
#define U8(p,o) (*(u8 *)((u8 *)(p)+(o)))
#define S8(p,o) (*(s8 *)((u8 *)(p)+(o)))
#define U32(p,o) (*(u32 *)((u8 *)(p)+(o)))
typedef struct {u8 pad[0x2FC]; void **sprites[64];} SpriteCtx;
typedef struct {u8 pad[0x4094];u8 ids[64];} IdCtx;

typedef struct {void *sprite;u8 pad[3];u8 counter;u32 scale;} ScaleWork;

void PCBox_SetIconImage(void *p,void *src,int i,int pal){PCBox_LoadSpriteCharsMain(p,i,src,0x200);ManagedSprite_SetPaletteOverride(((SpriteCtx *)p)->sprites[i],pal+3);}

void PCBox_LoadMonIcon(void *p,void *mon,int i){void *data;void *alloc=PCBox_LoadMonIconChars((void *)U32(p,0x34),mon,&data);int pal=Boxmon_GetIconPalette(mon);PCBox_SetIconImage((void *)U32(p,0x34),(void *)U32(data,0x14),i,pal);Heap_Free(alloc);}
