#pragma force_active on
#include "global.h"
typedef struct {int heap,pad;void *task,*menu,*pad2,*manager;u8 pad3[8];void *sprites[4];u8 pad4[0x58];void *memory;u8 pad5[8];int counter;u8 pad6[0x18];u8 active,course,player,pad7,wait,state,pad8,count;int finished;} Work;
extern Work *PokeathlonCourse_GetHeapAllocPtr4(void *);extern int PokeathlonCourse_GetMode(void *),ov96_021EE324(void *),ov96_021EEA80(void *),IsPaletteFadeFinished(void),ov96_021ED8A4(void *,int),ov96_021EE54C(void *),ov96_021EC2E0(void *);
extern void ov96_021EE830(void *),BufferPokeathlonCourseName(void *,int,int),sub_0200FC20(int),PlaySE(int),BeginNormalPaletteFade(int,int,int,int,int,int,int),ov96_021EE908(void *,int),ov96_021EDF3C(void *,void *,int,int),BufferPlayersName(void *,int,void *),ov96_021ED524(void *,int,u8,int),ov96_021ECB38(void *,void *,int,int,int),ManagedSprite_SetAnim(void *,int),PlaySE_SetPitch(u16,s16),Heap_Free(void *),SysTask_Destroy(void *),ov96_021EE944(void *),PokeathlonCourse_SetStateField07(void *,int),GF_AssertFail(void),ov96_021EE8CC(void *,int),ov96_021EAC0C(void *,int),ov96_021EAC5C(void *,int);
extern void *ov96_021EE97C(void *),*PokeathlonCourse_GetParticipantData(void *,int),*ov96_021ED86C(void *,int),*PokeathlonCourse_GetPlayerProfileFromData(void *,int),*ov96_021EAA04(void *,u8),*ov96_021EAA20(void *),*ov96_021E8BAC(void *);
extern void *Sprite_GetCellAnim(void *);
extern int ov96_021ED838(void *,int);extern int ov96_0221B088[];
int ov96_021EB730(void *);
int ov96_021EB730(void *ctx) {
 int i;Work *w=PokeathlonCourse_GetHeapAllocPtr4(ctx);int blocked=0;if(PokeathlonCourse_GetMode(ctx)==1)blocked=ov96_021EE324(ctx);
 ov96_021EE830(w->menu);if(!ov96_021EEA80(w->menu)&&!blocked){void *fmt=ov96_021EE97C(w->menu);int text=-1;
 switch(w->state){
 case 0:text=0;BufferPokeathlonCourseName(fmt,0,w->course);w->state=1;break;
 case 1:sub_0200FC20(0x7fff);PlaySE(2268);w->state=2;break;
 case 2:if(w->wait<4)w->wait++;else{w->wait=0;w->state=3;}break;
 case 3:BeginNormalPaletteFade(0,1,1,0x7fff,24,1,w->heap);w->state=4;break;
 case 4:if(IsPaletteFadeFinished())w->state=5;break;
 case 5:if(*(int*)PokeathlonCourse_GetParticipantData(ctx,w->player)>0){ov96_021EE908(w->menu,ov96_021ED838(ctx,w->player));}else{text=ov96_021ED838(ctx,w->player);ov96_021EDF3C(fmt,ov96_021ED86C(ctx,w->player),7,1);}w->state=6;break;
 case 6:{BufferPlayersName(fmt,0,PokeathlonCourse_GetPlayerProfileFromData(ctx,w->player));int i;for(i=0;i<3;i++)ov96_021ED524(ctx,w->player,(u8)i,i+1);ov96_021ECB38(w->sprites,ctx,w->player,2,w->heap);ManagedSprite_SetAnim(w->sprites[w->player],1);u8 sound=ov96_021ED8A4(ctx,w->player);int id;if(sound<5)id=2268;else id=2269;PlaySE_SetPitch((u16)id,(s16)ov96_0221B088[sound]);w->active=1;text=1;w->state=7;break;}
 case 7:if(w->active==0){if(w->player>=4){w->finished=1;text=2;w->state=8;}else w->state=5;}break;
 case 8:if(PokeathlonCourse_GetMode(ctx)==0 || ov96_021EE54C(ctx)){if(ov96_021EC2E0(w)){Heap_Free(w->memory);SysTask_Destroy(w->task);}ov96_021EE944(w->menu);PokeathlonCourse_SetStateField07(ctx,2);return 0;}break;
 default:GF_AssertFail();break;
 }
 if(text!=-1){ov96_021EE8CC(w->menu,text);w->count++;}
 }
 if(w->active){int index=(u8)(w->player*3);if(w->counter++==0){for(i=0;i<3;i++,index++){void *p=ov96_021EAA04(w->manager,(u8)index);ov96_021EAC0C(p,2);ov96_021EAC5C(p,16);}}
 else if(w->counter++>=60){int i;for(i=0;i<3;index++,i++){void *p=ov96_021EAA04(w->manager,(u8)index);ov96_021EAC0C(p,1);ov96_021EAC5C(p,0);u8 *anim=(u8 *)Sprite_GetCellAnim(ov96_021E8BAC(ov96_021EAA20(p)));*(int *)(anim+16)=i*3<<12;}
 ov96_021ECB38(w->sprites,ctx,w->player,1,w->heap);w->active=0;w->player++;w->counter=0;
 }}return 0;
}
