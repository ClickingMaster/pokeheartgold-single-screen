#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Summary {void *mon,*options;u8 pad0[9];u8 f11,f12,f13,f14,pad15,selection;u8 pad1;u16 move;u8 pad2[14];u32 field28;u8 pad3[4];u32 field30;} Summary;
typedef struct Work {void *bg,*window,*messages,*format,*string,*palette,*picman,*pic0,*pic1,*party,*mon,*options,*scene,*vram,*manager;Summary *summary;void *field40,*anim,*pokedex,*bag,*stats,*field54,*gfx;u32 heap;u16 species,newSpecies;u8 state,printer,timer,done;u32 cursor;u16 newMove;u8 selection,pad6f;u8 oscillate,step,x0,y0,x1,y1;u8 pad76[6];u32 flags;u8 cryMode,pad81[3];void *narc;} Work;
typedef struct ParticleConfig {u32 heap;int resource;} ParticleConfig;
extern void Pokepic_AddAttr(void *,int,int);
extern int Pokepic_GetAttr(void *,int);
extern void Pokepic_SetAttr(void *,int,int);
extern void PaletteData_BeginPaletteFade(void *,int,u16,int,int,int,int);
extern u8 gSystem[];
extern void SetMasterBrightnessNeutral(int);
extern void PaletteBlink_SetState(void *,int);
extern int PaletteData_GetSelectedBuffersBitmask(void *);
extern u8 sub_020772F8(Work *,int);
extern int TextPrinterCheckActive(u8);
extern void sub_0207294C(void *,void *,void *,u16,int,int,int);
extern void NARC_ReadPokepicAnimScript(void *,void *,u16,int);
extern void Pokepic_SetAnimScript(void *,void *);
extern void Pokepic_StartAnim(void *,int);
extern void PlayCry(u16,int);
extern void *Mon_GetBoxMon(void *);
extern void BufferBoxMonNickname(void *,int,void *);
extern void BufferBoxMonSpeciesName(void *,int,void *);
extern int IsCryFinished(void);
extern int sub_02017068(void *,int);
extern int Pokepic_IsAnimFinished(void *);
extern void sub_02005D10(int);
extern void *sub_02077604(ParticleConfig *);
extern void sub_02077634(void *,int);
extern int sub_02077650(void *);
extern void sub_02077664(void *);
extern void Pokepic_StartPaletteFade(void *,int,int,int,u16);
extern void Pokepic_StartPaletteFadeAll(void *,int,int,int,u16);
extern u32 HeapExp_FndGetTotalFreeSize(u32);
extern void PlaySE(u16);
extern void SetMonData(void *,int,const void *);
extern void UpdateMonAbility(void *);
extern void CalcMonLevelAndStats(void *);
extern void Pokedex_SetMonCaughtFlag(void *,void *);
extern void GameStats_Inc(void *,int);
extern void GameStats_AddScore(void *,int);
extern u32 GetMonData(void *,int,void *);
extern u32 MonTryLearnMoveOnLevelUp(void *,u32 *,u16 *);
extern void BufferMoveName(void *,int,u32);
extern void sub_0207584C(Work *,int);
extern int sub_02075A04(Work *);
extern void SetMasterBrightnessByColor(int,int);
extern void sub_020771A0(void *);
extern void sub_02075770(Work *);
extern const u8 _020FFEC0[];
extern void sub_02089D40(Summary *,const void *);
extern void sub_02077394(Work *);
extern int OverlayManager_Run(void *);
extern void OverlayManager_Delete(void *);
extern void sub_02076E64(void *,void *);
extern void sub_02075630(Work *);
extern void DrawFrameAndWindow2(void *,int,int,int);
extern void Pokepic_ScheduleReloadFromNarc(void *);
extern void WirelessStatusIcon_Create(void);
extern void MonSetMoveInSlot(void *,u16,u8);
extern int IsFanfarePlaying(void);
extern void sub_02076C90(Work *);
extern void StopBGM(int,int);
void sub_02075E14(Work *);
void sub_02075E14(Work *w){
 if(w->oscillate){
 if((w->oscillate&1)==0){Pokepic_AddAttr(w->pic0,12,-w->step);Pokepic_AddAttr(w->pic0,13,-w->step);Pokepic_AddAttr(w->pic1,12,w->step);Pokepic_AddAttr(w->pic1,13,w->step);if(Pokepic_GetAttr(w->pic0,12)==0)w->oscillate^=1;}
 else{Pokepic_AddAttr(w->pic0,12,w->step);Pokepic_AddAttr(w->pic0,13,w->step);Pokepic_AddAttr(w->pic1,12,-w->step);Pokepic_AddAttr(w->pic1,13,-w->step);if(Pokepic_GetAttr(w->pic1,12)==0){w->oscillate^=1;if(w->step<64)w->step*=2;}}
 }
 if((w->flags&1)&&w->state==8&&(*(u32 *)&gSystem[0x48]&2)){PaletteData_BeginPaletteFade(w->palette,15,0xf3ff,0,0,16,0x7fff);w->state=41;}
 switch(w->state){
 case 0:if(--w->timer==0)w->state++;break;
 case 1:SetMasterBrightnessNeutral(0);SetMasterBrightnessNeutral(1);PaletteBlink_SetState(w->gfx,0);if(w->flags&2)w->state=2;else w->state=4;break;
 case 2:if(PaletteData_GetSelectedBuffersBitmask(w->palette)==0){w->printer=sub_020772F8(w,0x394);w->state++;}break;
 case 3:if(TextPrinterCheckActive(w->printer))break;w->state=4;break;
 default:break;
 case 4:if(PaletteData_GetSelectedBuffersBitmask(w->palette)==0){
 u32 script[10];sub_0207294C(w->narc,w->anim,w->pic0,w->species,2,0,0);NARC_ReadPokepicAnimScript(w->narc,script,w->species,1);Pokepic_SetAnimScript(w->pic0,script);Pokepic_StartAnim(w->pic0,0);PlayCry(w->species,w->cryMode);BufferBoxMonNickname(w->format,0,Mon_GetBoxMon(w->mon));if(w->flags&2)w->printer=sub_020772F8(w,0x395);else w->printer=sub_020772F8(w,0x393);w->state=5;}break;
 case 5:if(!TextPrinterCheckActive(w->printer)&&!IsCryFinished()&&sub_02017068(w->anim,0)==1&&!Pokepic_IsAnimFinished(w->pic0)){PaletteBlink_SetState(w->gfx,1);sub_02005D10(0x3f3);w->timer=20;w->state=6;}break;
 case 6:if(--w->timer==0){ParticleConfig cfg;cfg.heap=w->heap;cfg.resource=0;w->scene=sub_02077604(&cfg);sub_02077634(w->scene,0);Pokepic_StartPaletteFade(w->pic0,0,16,4,0x7fff);Pokepic_StartPaletteFade(w->pic1,0,16,4,0x7fff);GF_ASSERT(HeapExp_FndGetTotalFreeSize(w->heap)>0x8000);PlaySE(0x5f8);w->timer=40;w->state++;}break;
 case 7:if(w->y0<40){w->y0+=2;w->y1-=2;}if(--w->timer==0){sub_02077634(w->scene,1);sub_02077634(w->scene,2);sub_02077634(w->scene,7);sub_02077634(w->scene,8);sub_02077634(w->scene,9);sub_02077634(w->scene,11);PlaySE(0x5f9);w->oscillate=16;w->step=8;w->state++;}break;
 case 8:if(!sub_02077650(w->scene)){sub_02077634(w->scene,3);sub_02077634(w->scene,4);sub_02077634(w->scene,5);sub_02077634(w->scene,6);sub_02077634(w->scene,10);PaletteData_BeginPaletteFade(w->palette,15,0xf3ff,2,0,16,0x7fff);Pokepic_SetAttr(w->pic0,12,0);Pokepic_SetAttr(w->pic0,13,0);Pokepic_SetAttr(w->pic1,12,256);Pokepic_SetAttr(w->pic1,13,256);PlaySE(0x5fa);w->oscillate=0;w->timer=8;w->state++;}break;
 case 9:if(w->y0){w->y0-=2;w->y1+=2;}if(PaletteData_GetSelectedBuffersBitmask(w->palette)==0&&--w->timer==0){sub_02077634(w->scene,12);PaletteData_BeginPaletteFade(w->palette,15,0xf3ff,4,16,0,0x7fff);Pokepic_StartPaletteFadeAll(w->picman,16,0,3,0x7fff);PlaySE(0x5fb);w->state++;}break;
 case 10:if(PaletteData_GetSelectedBuffersBitmask(w->palette)==0&&!sub_02077650(w->scene)){u32 script[10];sub_0207294C(w->narc,w->anim,w->pic1,w->newSpecies,2,0,0);NARC_ReadPokepicAnimScript(w->narc,script,w->newSpecies,1);Pokepic_SetAnimScript(w->pic1,script);Pokepic_StartAnim(w->pic1,0);PlayCry(w->newSpecies,w->cryMode);w->state++;}break;
 case 11:if(!IsCryFinished()&&sub_02017068(w->anim,0)==1&&!Pokepic_IsAnimFinished(w->pic1)){SetMonData(w->mon,5,&w->newSpecies);UpdateMonAbility(w->mon);CalcMonLevelAndStats(w->mon);BufferBoxMonNickname(w->format,0,Mon_GetBoxMon(w->mon));BufferBoxMonSpeciesName(w->format,1,Mon_GetBoxMon(w->mon));w->printer=sub_020772F8(w,0x396);w->timer=40;w->state++;}break;
 case 12:if(!TextPrinterCheckActive(w->printer)&&--w->timer==0){Pokedex_SetMonCaughtFlag(w->pokedex,w->mon);GameStats_Inc(w->stats,13);GameStats_AddScore(w->stats,21);if(GetMonData(w->mon,77,NULL)==0)SetMonData(w->mon,179,NULL);w->state++;}break;
 case 13:{u16 move;u32 result=MonTryLearnMoveOnLevelUp(w->mon,&w->cursor,&move);switch(result){case 0:w->state=39;break;case 0xfffe:break;case 0xffff:w->newMove=move;w->state=14;break;default:BufferBoxMonNickname(w->format,0,Mon_GetBoxMon(w->mon));BufferMoveName(w->format,1,move);w->printer=sub_020772F8(w,4);w->timer=30;w->state=37;break;}break;}
 case 14:BufferBoxMonNickname(w->format,0,Mon_GetBoxMon(w->mon));BufferMoveName(w->format,1,w->newMove);w->printer=sub_020772F8(w,0x4a9);w->timer=30;w->state++;break;
 case 16:BufferBoxMonNickname(w->format,0,Mon_GetBoxMon(w->mon));w->printer=sub_020772F8(w,0x4aa);w->timer=30;w->state++;break;
 case 18:w->printer=sub_020772F8(w,0x4ab);w->timer=1;w->state++;break;
 case 15:case 17:case 19:case 26:case 28:case 30:case 33:if(!TextPrinterCheckActive(w->printer)&&--w->timer==0)w->state++;break;
 case 20:sub_0207584C(w,1);w->state++;break;
 case 21:{int result=sub_02075A04(w);switch(result){case 1:w->state=22;PaletteData_BeginPaletteFade(w->palette,15,0xffff,1,0,16,0);Pokepic_StartPaletteFadeAll(w->picman,0,16,0,0);break;case 2:w->state=32;break;default:break;}break;}
 case 22:if(PaletteData_GetSelectedBuffersBitmask(w->palette)==0){SetMasterBrightnessByColor(0,0);SetMasterBrightnessByColor(1,0);sub_020771A0(w->bg);sub_02075770(w);Pokepic_SetAttr(w->pic0,6,1);Pokepic_SetAttr(w->pic1,6,1);w->summary->mon=w->mon;w->summary->options=w->options;w->summary->f11=0;w->summary->f14=0;w->summary->f13=1;w->summary->move=w->newMove;w->summary->f12=2;w->summary->field28=0;w->summary->field30=0;sub_02089D40(w->summary,_020FFEC0);sub_02077394(w);w->state++;}break;
 case 23:if(OverlayManager_Run(w->manager)){OverlayManager_Delete(w->manager);w->manager=NULL;sub_02076E64(w,w->bg);sub_02075630(w);DrawFrameAndWindow2(w->window,0,1,10);Pokepic_SetAttr(w->pic0,6,0);Pokepic_SetAttr(w->pic1,6,0);Pokepic_ScheduleReloadFromNarc(w->pic0);Pokepic_ScheduleReloadFromNarc(w->pic1);PaletteData_BeginPaletteFade(w->palette,15,0xffff,1,16,0,0);Pokepic_StartPaletteFadeAll(w->picman,16,0,0,0);WirelessStatusIcon_Create();w->state++;}break;
 case 24:SetMasterBrightnessNeutral(0);SetMasterBrightnessNeutral(1);if(PaletteData_GetSelectedBuffersBitmask(w->palette)==0){if(w->summary->selection==4)w->state=32;else{w->selection=w->summary->selection;w->state=25;}}break;
 case 32:BufferMoveName(w->format,0,w->newMove);w->printer=sub_020772F8(w,0x4ad);w->timer=1;w->state++;break;
 case 34:sub_0207584C(w,0);w->state++;break;
 case 35:{int result=sub_02075A04(w);switch(result){case 1:BufferBoxMonNickname(w->format,0,Mon_GetBoxMon(w->mon));BufferMoveName(w->format,1,w->newMove);w->printer=sub_020772F8(w,0x4ae);w->timer=30;w->state=36;break;case 2:w->state=14;break;default:break;}break;}
 case 36:if(!TextPrinterCheckActive(w->printer)&&--w->timer==0)w->state=13;break;
 case 25:w->printer=sub_020772F8(w,0x4af);w->timer=30;w->state++;break;
 case 27:BufferBoxMonNickname(w->format,0,Mon_GetBoxMon(w->mon));BufferMoveName(w->format,1,GetMonData(w->mon,w->selection+54,NULL));w->printer=sub_020772F8(w,0x4b0);w->timer=30;w->state++;break;
 case 29:w->printer=sub_020772F8(w,0x4b1);w->timer=30;w->state++;break;
 case 31:BufferBoxMonNickname(w->format,0,Mon_GetBoxMon(w->mon));BufferMoveName(w->format,1,w->newMove);w->printer=sub_020772F8(w,0x4b2);w->timer=0;SetMonData(w->mon,w->selection+62,&w->timer);MonSetMoveInSlot(w->mon,w->newMove,w->selection);w->timer=30;w->state=37;break;
 case 37:if(!TextPrinterCheckActive(w->printer))w->state++;break;
 case 38:if(!IsFanfarePlaying()&&--w->timer==0)w->state=13;break;
 case 39:PaletteData_BeginPaletteFade(w->palette,15,0xffff,1,0,16,0);Pokepic_StartPaletteFadeAll(w->picman,0,16,0,0);w->state++;break;
 case 40:if(PaletteData_GetSelectedBuffersBitmask(w->palette)==0){sub_02077664(w->scene);sub_02076C90(w);w->done=1;}break;
 case 41:if(PaletteData_GetSelectedBuffersBitmask(w->palette)==0){Pokepic_SetAttr(w->pic0,12,256);Pokepic_SetAttr(w->pic0,13,256);Pokepic_SetAttr(w->pic1,12,0);Pokepic_SetAttr(w->pic1,13,0);Pokepic_SetAttr(w->pic1,6,1);PaletteData_BeginPaletteFade(w->palette,15,0xf3ff,0,16,0,0x7fff);Pokepic_StartPaletteFadeAll(w->picman,16,0,0,0x7fff);w->x0=0;w->y0=0;w->x1=255;w->y1=160;w->oscillate=0;StopBGM(0x3f3,0);sub_02077664(w->scene);w->state++;}break;
 case 42:if(PaletteData_GetSelectedBuffersBitmask(w->palette)==0){u32 script[10];sub_0207294C(w->narc,w->anim,w->pic0,w->species,2,0,0);NARC_ReadPokepicAnimScript(w->narc,script,w->species,1);Pokepic_SetAnimScript(w->pic0,script);Pokepic_StartAnim(w->pic0,0);PlayCry(w->species,w->cryMode);w->state++;}break;
 case 43:if(!IsCryFinished()&&sub_02017068(w->anim,0)==1&&!Pokepic_IsAnimFinished(w->pic0)){BufferBoxMonNickname(w->format,0,Mon_GetBoxMon(w->mon));w->printer=sub_020772F8(w,0x397);w->timer=20;w->state++;}break;
 case 44:if(!TextPrinterCheckActive(w->printer)&&--w->timer==0){PaletteData_BeginPaletteFade(w->palette,15,0xffff,1,0,16,0);Pokepic_StartPaletteFadeAll(w->picman,0,16,0,0);w->state++;}break;
 case 45:if(PaletteData_GetSelectedBuffersBitmask(w->palette)==0)w->done=1;break;
 }
}
