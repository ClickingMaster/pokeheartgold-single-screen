#pragma force_active on
#include "global.h"
#include "hall_of_fame.h"
#include "bg_window.h"
#include "sprite_system.h"
#include "gf_gfx_loader.h"
#include "unk_02013FDC.h"
#include "font.h"
#include "text.h"
#include "msgdata.h"
#include "heap.h"
typedef UnkStruct_02014E30 Config;
typedef PokepicTemplate Info;
typedef struct{HallOfFame*save;u32 pad;Window windows[16];u8 p[0x118-0x108];MsgData*msg;MsgData*speciesMsg;MsgData*moveMsg;MessageFormat*format;String*str;String*str2;void*sys;void*manager;ManagedSprite*sprites[2];u8 q[0x184-0x140];NARC*pic;SHOW_HOFMON mon;u8 r[0x1ac-0x1a4];u32 count;u32 pad2;u32 archive;u8 s[0x1c4-0x1b8];u16 screen;} Work;
extern const Config ov64_021E6E98;
extern void ov64_021E6754(Work*);
extern u8 GetGenderBySpeciesAndPersonality(u16,u32);
extern u8 CalcShininessByOtIdAndPersonality(u32,u32);
extern u8 GetMonPicHeightBySpeciesGenderForm(u16,u8,u8,u8,u32);
extern void GetMonSpriteCharAndPlttNarcIdsEx(PokepicTemplate*,u16,u8,u8,u8,u8,u32);
/* Native caller supplies these three consumed inputs; selected fourth formal is unused. */
extern void sub_020729D8(NARC*,s8*,u16);
extern void ov64_021E5AC8(const void*,u32,u32);
void ov64_021E677C(Work*,u32);
static inline void DrawMonLabels(Work*w,s32 start,u32 gender){u32 i,width;for(i=0;i<7;i++)FillWindowPixelBuffer(&w->windows[start+i],0);AddTextPrinterParameterizedWithColor(&w->windows[start],0,w->mon.nickname,0,0,255,0xF0200,0);ReadMsgDataIntoString(w->speciesMsg,w->mon.species,w->str);AddTextPrinterParameterizedWithColor(&w->windows[start+1],0,w->str,0,0,255,0xF0200,0);if(w->mon.species!=29&&w->mon.species!=32){if(gender==0){ReadMsgDataIntoString(w->msg,3,w->str2);width=FontID_String_GetWidth(0,w->str,0);AddTextPrinterParameterizedWithColor(&w->windows[start+1],0,w->str2,width+8,0,255,0xF0200,0);}else if(gender==1){ReadMsgDataIntoString(w->msg,4,w->str2);width=FontID_String_GetWidth(0,w->str,0);AddTextPrinterParameterizedWithColor(&w->windows[start+1],0,w->str2,width+8,0,255,0xF0200,0);}}ReadMsgDataIntoString(w->msg,1,w->str);BufferIntegerAsString(w->format,0,w->mon.level,3,PRINTING_MODE_LEFT_ALIGN,TRUE);StringExpandPlaceholders(w->format,w->str2,w->str);AddTextPrinterParameterizedWithColor(&w->windows[start+1],0,w->str2,0,16,255,0xF0200,0);ReadMsgDataIntoString(w->msg,2,w->str);AddTextPrinterParameterizedWithColor(&w->windows[start+2],0,w->str,0,0,255,0xF0200,0);width=FontID_String_GetWidth(0,w->str,0);AddTextPrinterParameterizedWithColor(&w->windows[start+2],0,w->mon.otname,width,0,255,0xF0200,0);for(i=0;i<4;i++){u16 move=w->mon.moves[i];if(move){ReadMsgDataIntoString(w->moveMsg,move,w->str);width=FontID_String_GetWidth(0,w->str,0);AddTextPrinterParameterizedWithColor(&w->windows[start+3+i],0,w->str,56-(width>>1),0,255,0xF0200,0);}}for(i=0;i<7;i++){CopyWindowPixelsToVram_TextMode(&w->windows[start+i]);ScheduleWindowCopyToVram(&w->windows[start+i]);}}
// @fn ov64_021E677C
void ov64_021E677C(Work*w,u32 idx){Info info;Config config=ov64_021E6E98;s8 offsets[4];s32 start;u32 gender;u32 slot;ManagedSprite **spriteBase;int height,shiny;void*buf;u32 personality;if(idx>=w->count){ov64_021E6754(w);return;}ManagedSprite_SetDrawFlag(w->sprites[0],0);ManagedSprite_SetDrawFlag(w->sprites[1],0);if(w->screen==0){slot=0;start=0;}else{slot=1;start=8;}Save_HOF_GetMonStatsByIndexPair(w->save,w->archive,idx,&w->mon);gender=GetGenderBySpeciesAndPersonality(w->mon.species,w->mon.personality);sub_020729D8(w->pic,offsets,w->mon.species);height=GetMonPicHeightBySpeciesGenderForm(w->mon.species,(u8)gender,2,w->mon.form,w->mon.personality);spriteBase=w->sprites;ManagedSprite_SetPositionXYWithSubscreenOffset(w->sprites[slot],64,(s16)(height+64-offsets[0]),0x200000);shiny=CalcShininessByOtIdAndPersonality(w->mon.otid,w->mon.personality)!=0;buf=Heap_AllocAtEnd((enum HeapID)59,3200);personality=w->mon.personality;GetMonSpriteCharAndPlttNarcIdsEx(&info,w->mon.species,GetGenderBySpeciesAndPersonality(w->mon.species,personality),2,(u8)shiny,w->mon.form,personality);sub_02014510((NarcId)info.narcID,info.charDataID,(enum HeapID)59,&config,buf,w->mon.personality,0,2,w->mon.species);ov64_021E5AC8(buf,NNS_G2dGetImageLocation(Sprite_GetImageProxy(spriteBase[slot]->sprite),(NNS_G2D_VRAM_TYPE)2),3200);Heap_Free(buf);GfGfxLoader_GXLoadPal((NarcId)info.narcID,info.palDataID,GF_PAL_LOCATION_SUB_OBJ,(enum GFPalSlotOffset)NNS_G2dGetImagePaletteLocation(Sprite_GetPaletteProxy(spriteBase[slot]->sprite),(NNS_G2D_VRAM_TYPE)2),32,(enum HeapID)59);ManagedSprite_SetDrawFlag(spriteBase[slot],1);DrawMonLabels(w,start,gender);w->screen^=1;}
