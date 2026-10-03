#pragma force_active on
#include "global.h"
#pragma force_active on
#pragma opt_dead_assignments off
#include "overlay_56_helpers_internal.h"
typedef NNSG2dCharacterData CharD;
typedef NNSG2dPaletteData PalD;
typedef NNSG2dScreenData ScrD;
typedef struct Args {u8 pad[19];u8 kind;} Args;
typedef struct Work {u32 heap;u8 pad[7];u8 frame,mode;u8 pad2[7];u8 c14,c15;u8 pad3[2];void *bg;Args *args;u8 pad4[16];void *pal;int (*cb1)(void);int (*cb2)(void *);void *raw,*raw2;ScrD *scr,*scr2;} Work;
extern void *NARC_New(int,u32);
extern u32 NARC_GetMemberSize(void *,int);
extern void NARC_ReadWholeMember(void *,int,void *);
extern void NARC_Delete(void *);
extern void LoadUserFrameGfx1(void *,int,int,int,int,u32);
extern void LoadUserFrameGfx2(void *,int,int,int,int,u32);
extern void *Heap_AllocAtEnd(u32,u32);
extern void *Heap_Alloc(u32,u32);
extern void BG_LoadCharTilesData(void *,int,void *,u32,int);
extern void BG_LoadPlttData(int,void *,u16,int);
extern void *PaletteData_Init(u32);
extern void PaletteData_AllocBuffers(void *,int,u32,u32);
extern void PaletteData_LoadPalette(void *,void *,int,int,u32);
extern void PaletteData_LoadNarc(void *,int,int,u32,int,u32,int);
extern void PaletteData_SetAutoTransparent(void *,int);
extern void FillBgTilemapRect(void *,int,u16,int,int,int,int,int);
extern void CopyToBgTilemapRect(void *,int,int,int,int,int,void *,int,int,int,int);
extern void ScheduleBgTilemapBufferTransfer(void *,int);
extern void *SysTask_CreateOnMainQueue(void (*)(void *,void *),void *,int);
void ov56_021E6650(void *p) {
 Work *w=p; void *n; void *b; int kind; CharD *ch; PalD *pal;
 kind=w->args->kind;
 int scrid=kind; scrid=kind+24;
 n=NARC_New(79,w->heap);
 LoadUserFrameGfx1(w->bg,0,1,4,0,w->heap);
 LoadUserFrameGfx2(w->bg,0,10,6,w->frame,w->heap);
 int charid=kind; charid=kind+12;
 b=Heap_AllocAtEnd(w->heap,NARC_GetMemberSize(n,charid));
 NARC_ReadWholeMember(n,charid,b);NNS_G2dGetUnpackedCharacterData(b,&ch);
 BG_LoadCharTilesData(w->bg,1,ch->pRawData,ch->szByte,0);
 BG_LoadCharTilesData(w->bg,4,ch->pRawData,ch->szByte,0);Heap_Free(b);
 b=Heap_AllocAtEnd(w->heap,NARC_GetMemberSize(n,kind));
 NARC_ReadWholeMember(n,kind,b);NNS_G2dGetUnpackedPaletteData(b,&pal);
 BG_LoadPlttData(4,pal->pRawData,(u16)pal->szByte,0);
 w->pal=PaletteData_Init(w->heap);
 PaletteData_AllocBuffers(w->pal,0,224,w->heap);
 PaletteData_AllocBuffers(w->pal,2,96,w->heap);
 PaletteData_LoadPalette(w->pal,pal->pRawData,0,0,96);
 if(w->mode==1)PaletteData_LoadPalette(w->pal,(u8 *)pal->pRawData+96,0,16,32);
 PaletteData_LoadNarc(w->pal,20,0,w->heap,2,96,0);
 PaletteData_LoadNarc(w->pal,16,7,w->heap,0,32,48);
 PaletteData_LoadNarc(w->pal,16,8,w->heap,0,32,80);
 PaletteData_LoadNarc(w->pal,38,25,w->heap,0,32,64);
 PaletteData_LoadNarc(w->pal,38,w->frame+26,w->heap,0,32,96);
 PaletteData_BlendPalette(w->pal,0,0,112,16,0);
 PaletteData_BlendPalette(w->pal,2,0,48,16,0);
 PaletteData_SetAutoTransparent(w->pal,1);PaletteData_PushTransparentBuffers(w->pal);Heap_Free(b);
 w->raw=Heap_Alloc(w->heap,NARC_GetMemberSize(n,scrid));
 NARC_ReadWholeMember(n,scrid,w->raw);NNS_G2dGetUnpackedScreenData(w->raw,&w->scr);
 w->raw2=Heap_Alloc(w->heap,NARC_GetMemberSize(n,36));
 NARC_ReadWholeMember(n,36,w->raw2);NNS_G2dGetUnpackedScreenData(w->raw2,&w->scr2);NARC_Delete(n);
 FillBgTilemapRect(w->bg,4,0x2001,0,0,32,32,17);
 CopyToBgTilemapRect(w->bg,3,0,0,32,24,(u8 *)w->scr+12,0,0,(u8)(w->scr->screenWidth>>3),(u8)(w->scr->screenHeight>>3));
 ScheduleBgTilemapBufferTransfer(w->bg,3);ScheduleBgTilemapBufferTransfer(w->bg,4);
 if(w->mode){
 CopyToBgTilemapRect(w->bg,2,0,0,32,24,(u8 *)w->scr2+12,0,0,(u8)(w->scr2->screenWidth>>3),(u8)(w->scr2->screenHeight>>3));
 ScheduleBgTilemapBufferTransfer(w->bg,2);w->c14=0;w->c15=0;
 SysTask_CreateOnMainQueue(ov56_021E63FC,w,0);w->cb1=ov56_021E5D40;w->cb2=ov56_021E5D34;
 }
}
