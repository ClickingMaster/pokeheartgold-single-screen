#include "global.h"
#include "bg_window.h"
#include "palette.h"
#include "sprite_system.h"
#include "filesystem.h"
#pragma force_active on
typedef struct Ov89PartyEntry {u16 value;u16 species;u32 personality;u8 form;u8 padding[3];} Ov89PartyEntry;
typedef struct{u8 pad[8];BgConfig *bg;PaletteData *palette;u8 pad2[0x160-0x10];NARC *mainArchive,*alternateArchive;u8 pad3[0x8d8-0x168];Ov89PartyEntry items[6];u8 first;u8 pad4[3];ManagedSprite *sprites[6];ManagedSprite *secondary[6];ManagedSprite *icons[6];u8 pad5[0x19e0-0x96c];void *network;} State;
extern const u16 _0225C9B0[4];
extern BOOL ov89_02259A3C(int,int,ManagedSprite*,NARC*,NARC*,int,void*);
extern void ov89_0225A1D8(void*,int);
void ov89_022598D0(void*);
void ov89_022598D0(void *context){State *s=context;int i;u16*unf;int found=0;u8 *item,*cursor;int column;u16*pal,*faded;int j;unf=PaletteData_GetUnfadedBuf(s->palette,PLTTBUF_MAIN_OBJ);faded=PaletteData_GetFadedBuf(s->palette,PLTTBUF_MAIN_OBJ);for(i=0,column=0,item=(u8*)s,cursor=(u8*)s,pal=unf;i<6;item+=12,cursor+=4,column+=4,pal++,faded++,i++){if(((Ov89PartyEntry*)(item+0x8d8))->species==0||((Ov89PartyEntry*)(item+0x8d8))->species>493){ManagedSprite_SetDrawFlag(*(ManagedSprite**)(cursor+0x924),0);ManagedSprite_SetDrawFlag(*(ManagedSprite**)(cursor+0x93c),0);ManagedSprite_SetDrawFlag(*(ManagedSprite**)(cursor+0x954),0);for(j=0;j<4;j++)FillBgTilemapRect(s->bg,2,_0225C9B0[j],column,j+20,4,1,17);}else{ov89_02259A3C(((Ov89PartyEntry*)(item+0x8d8))->species,((Ov89PartyEntry*)(item+0x8d8))->form,*(ManagedSprite**)(cursor+0x954),s->mainArchive,s->alternateArchive,1,s->network);pal[17]=((Ov89PartyEntry*)(item+0x8d8))->value;faded[17]=((Ov89PartyEntry*)(item+0x8d8))->value;BlendPalette(unf+(i+17),unf+(i+33),1,12,0);faded[33]=pal[33];if(!found){found++;s->first=(u8)i;}}}ov89_0225A1D8(s,0);ScheduleBgTilemapBufferTransfer(s->bg,2);}
