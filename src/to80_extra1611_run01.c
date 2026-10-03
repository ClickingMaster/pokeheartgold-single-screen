#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Card {u8 kind,pad[2],gender;u8 f0:1,f1:1,f2:1,f3:1,rest:4;u8 variant;u16 flags;} Card;
typedef struct Work {void *bg;u8 p0[0xe0];void *args;Card *card;u8 p1[0x3000];void *charraw;NNSG2dCharacterData *ch;u8 p2[0x2bc];void *scrraw;NNSG2dScreenData *scr;void *scrraw2;NNSG2dScreenData *scr2;u8 p3[0x7a];u8 bit0:1,bit1:1,rest:6;} Work;
extern void *GfGfxLoader_GetPlttData(int,int,NNSG2dPaletteData **,u32);
extern void *GfGfxLoader_LoadFromNarc(int,u32,BOOL,u32,BOOL);
extern void *GfGfxLoader_GetScrnData(int,int,int,NNSG2dScreenData **,u32);
extern void GfGfxLoader_LoadCharData(int,int,void *,int,int,int,int,u32);
extern void GfGfxLoader_LoadScrnData(int,int,void *,int,int,int,int,u32);
extern void Heap_Free(void *);
extern void ov51_021E60F4(int,int,int);
extern void ov51_021E6200(int);
extern void ov51_021E6C6C(Work *);
extern void CopyToBgTilemapRect(void *,int,int,int,int,int,void *,int,int,int,int);
extern void BgCommitTilemapBufferToVram(void *,int);
extern void ov51_021E6CF0(void *,void *);
extern const u32 ov51_021E7F08[];
void ov51_021E6354(Work *);
void ov51_021E6354(Work *w){
 NNSG2dPaletteData *p1,*p2;void *raw;
 raw=GfGfxLoader_GetPlttData(49,0,&p1,25);DC_FlushRange(p1->pRawData,512);GXS_LoadBGPltt(p1->pRawData,0,512);Heap_Free(raw);
 ov51_021E60F4(w->card->gender,w->card->f3,w->card->kind);
 raw=GfGfxLoader_GetPlttData(49,28,&p2,25);DC_FlushRange(p2->pRawData,512);GX_LoadBGPltt(p2->pRawData,0,512);Heap_Free(raw);
 if(w->card->variant==255){
 w->charraw=GfGfxLoader_LoadFromNarc(49,44,FALSE,25,FALSE);GF_ASSERT(w->charraw!=NULL);GF_ASSERT(NNS_G2dGetUnpackedBGCharacterData(w->charraw,&w->ch));
 if(w->card->f2==0)w->scrraw=GfGfxLoader_GetScrnData(49,54,0,&w->scr,25);else w->scrraw=GfGfxLoader_GetScrnData(49,55,0,&w->scr,25);
 }else{
 u8 variant=w->card->variant;u32 index=ov51_021E7F08[variant];w->charraw=GfGfxLoader_LoadFromNarc(49,index,FALSE,25,FALSE);GF_ASSERT(w->charraw!=NULL);GF_ASSERT(NNS_G2dGetUnpackedBGCharacterData(w->charraw,&w->ch));
 w->scrraw=GfGfxLoader_GetScrnData(49,61,0,&w->scr,25);ov51_021E6200(w->card->variant);
 }
 ov51_021E6C6C(w);
 GfGfxLoader_LoadCharData(49,41,w->bg,6,0,0,0,25);GfGfxLoader_LoadScrnData(49,47,w->bg,6,0,0,0,25);
 GfGfxLoader_LoadCharData(49,42,w->bg,5,0,0,0,25);GfGfxLoader_LoadScrnData(49,49,w->bg,5,0,0,0,25);
 GfGfxLoader_LoadCharData(49,43,w->bg,2,0,0,0,25);
 w->scrraw2=GfGfxLoader_GetScrnData(49,53,0,&w->scr2,25);
 if(w->bit0)GfGfxLoader_LoadScrnData(49,52,w->bg,2,0,0,0,25);else GfGfxLoader_LoadScrnData(49,51,w->bg,2,0,0,0,25);
 GfGfxLoader_LoadScrnData(49,50,w->bg,3,0,0,0,25);
 if(!w->bit0&&w->card->flags>=255)CopyToBgTilemapRect(w->bg,2,0,7,7,9,(u8 *)w->scr2+12,0,0,(u8)(w->scr2->screenWidth>>3),(u8)(w->scr2->screenHeight>>3));
 else if(w->card->flags==65535)CopyToBgTilemapRect(w->bg,2,0,14,7,9,(u8 *)w->scr2+12,7,0,(u8)(w->scr2->screenWidth>>3),(u8)(w->scr2->screenHeight>>3));
 BgCommitTilemapBufferToVram(w->bg,2);ov51_021E6CF0((u8 *)w->card+0x68,(u8 *)w+0xec);
}
