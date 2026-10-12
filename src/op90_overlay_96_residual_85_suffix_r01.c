#pragma force_active on
#include "global.h"
#include <nitro/gx/gx.h>
#include "heap.h"
#include "system.h"
#include "pokeathlon/pokeathlon.h"
#include "bg_window.h"
#include "obj_char_transfer.h"
#include "font.h"
#include "oam_manager.h"

typedef struct Member {u8 p0[8];float factor,other;u8 p1[7];u8 tag,mode;u8 p2[3];} Member;
typedef struct Actor {void *sprites[3];u8 p0[12];Member members[3];void *sprite6c;u8 p1[8];void *sprite78;fx32 x,y;u8 p2[7];u8 current;VecFx32 velocity;u8 collision[4];u8 p3;u8 blocked;u8 p4[6];u8 tag;u8 p5[4];u8 cooldown;u8 p6[0x26];u8 participant;u8 p7[3];} Actor;
typedef struct ActorB {void*sprites[3];u8 pad0c[0x4c];fx32 x,y;u8 pad60[0x38];int participant;u8 pad9c[0x14];u8 current;u8 tail[7];} ActorB;
typedef struct State {void*bg;u8 p04[0x10];u32 heap;u8 p18[8];void*renderer;ActorB actors[4];u8 p304[0x30];void*future[4];void*resource;void*icons;u8 p34c[0x20];void*secondary;void*screen;u32 random,ready;u8 p37c[0x190];u16 duration;u8 tail[0x1b2];} State;



typedef struct MonInfo {u16 species,form;u8 pad[3],gender;u32 flags,personality;} MonInfo;
typedef struct Point {u16 x,y;} Point;

typedef struct Params {int a,b,c,d,e;u32 ability[12];} Params;

extern void GfGfx_DisableEngineAPlanes(void);
extern void GfGfx_DisableEngineBPlanes(void);
extern void PokeathlonRelayRun_SetGfxBanks(void);
extern void MI_CpuFill8(void *,u8,u32);

extern void PokeathlonCourse_SetHudPlanes(u8 *,u32);
extern void PokeathlonCourse_InitObjTransfer(int,int,int,int,int);


extern void PokeathlonRelayRun_InitBgs(void *);
extern void PokeathlonRelayRun_AddResultWindow(State *);

extern void GfGfx_SwapDisplay(void);
extern void *PokeathlonRelayRunHud_New(u32,void *,void *);
extern void *ov96_02200E3C(u32,int,int);
extern void *PokeathlonCourseSprites_CreateResources(u32,int,int);
extern VecFx32 ov96_0221C5E4;
extern void *PokeathlonCourseSpritePool_New(u32,VecFx32 *);
extern void PokeathlonCourseSpritePool_SetSurfaceCoords(void *,int,int,int,int);
extern void *PokeathlonCourseSpritePool_GetSpriteList(void *);
extern void *PokeathlonCourseMonObjects_Create(enum HeapID,void*,void*,void *,void *);
extern void PokeathlonCourseSpritePool_SetResourceId(void *,int,int);
extern void PokeathlonRelayRunHud_LoadSpriteGraphics(void *,void *);
extern void PokeathlonRelayRun_LoadSpriteGraphics(void *);
extern void PokeathlonCourseSpritePool_TransferResources(void *);
extern void **ov96_021E6290(void *,int,void *,void *);
extern void Sprite_SetDrawPriority(void *,int);
extern void PokeathlonRelayRun_CreateSprites(State *,void *);
extern void PokeathlonRelayRunHud_CreateSprites(void *,void *,void *);
extern void PokeathlonCourse_GetMonInfo(void *,int,int,MonInfo *);
extern void *PokeathlonCourse_GetPerformance(void *,int,int);
extern u32 ov96_021E6108(void *);
extern void PokeathlonRelayRunHud_LoadMonIcons(void *,Point *);
extern void PokeathlonRelayRunHud_LoadMonPics(void *,MonInfo *);
extern void PokeathlonRelayRunHud_ShowMonPic(void *,int,int);
extern void PokeathlonCourseMonObjects_Load(void *,int,MonInfo *,Params *,int,int);
extern void PokeathlonRelayRunHud_SetDrawPriorities(void *,int);
extern int PokeathlonCourseMonObjects_IsLoaded(void *);
extern void ReadWholeNarcMemberByIdPair(void *,int,int);
extern void *PokeathlonCourseMonObjects_Get(void *,u8);
extern void PokeathlonCourseMonObject_SetDrawFlag(void *,int);
extern u32 ov96_021E6138(void *);
extern void PokeathlonCourseObject_SetCollision(void *,int,int);
extern void PokeathlonCourseMonObject_SetAnimState(void *,int);
extern void PokeathlonCourseObject_SetGroundPosition(void *,int,int);
extern int ov96_021E6104(void);
extern void PokeathlonCourseObject_SetTouchRadius(void *,int);
extern void PokeathlonCourseObject_GroundToCenter(void *,int,int,int *,int *);
extern void PokeathlonRelayRunTeam_SetDrawPriority(Actor *,u8,int);
extern void PokeathlonRelayRunHud_SetTimer(void *,int);
extern void PokeathlonRelayRun_InitMember(void *,void *,u8,u8,Actor *);
extern void ov96_02200E78(void *,u8,Actor *);
extern void PokeathlonRelayRun_PackPacket(State *,void *);
extern void ov96_021E634C(void *,int,void *,void *,int,int,Point *);
extern void PokeathlonRelayRun_LoadBgGraphics(State *);


extern void WirelessStatusIcon_CreateOnScreenIfConnected(int);
extern u16 LCRandom(void);
extern void ov96_02200E80(void *,void *);
static inline int TeamNum(int i){return i/3;}
static inline int SlotNum(int i){return i%3;}
static inline void *GetMember(void *course,int i){return PokeathlonCourse_GetPerformance(course,i/3,i%3);}

extern void ov96_02204F20(void),PokeathlonPennantCapture_InitBgs(void *),PokeathlonPennantCapture_InitScoreWindow(u8 *);
extern void *PokeathlonPennantCaptureDisplay_New(u32,void *,void *),*PokeathlonPennantCaptureCpu_New(u32,int,int);
extern VecFx32 ov96_0221CAD4;
extern void PokeathlonPennantCaptureDisplay_LoadSpriteResources(void *,void *),PokeathlonPennantCapture_LoadSpriteResources(void *),PokeathlonPennantCapture_CreateSprites(State *,void *),PokeathlonPennantCaptureDisplay_CreateSprites(void *,void *,void *);
extern void Sprite_SetDrawPriority(void *,int);
extern void PokeathlonPennantCaptureDisplay_LoadIcons(void *,Point *),PokeathlonPennantCaptureDisplay_LoadMonPics(void *,MonInfo *),PokeathlonPennantCaptureDisplay_LoadMonPicToVram(void *,int,int),PokeathlonPennantCaptureDisplay_BringToFront(void *,int),PokeathlonPennantCaptureDisplay_SetTimer(void *,int),PokeathlonPennantCapture_ResetPennantSpots(State *),PokeathlonPennantCapture_SetMoveParams(void *,void *,u8,u8,void *),PokeathlonPennantCaptureCpu_SetPlayer(void *,u8,void *),PokeathlonPennantCapture_PackFrame(State *,void *),PokeathlonPennantCapture_LoadBgGfx(State *),PokeathlonCourseMonObject_SetDrawPriorityStep(void *,int);

int ov96_022047EC(void *);
static inline void ReadCompetitor(void *course,MonInfo *info,Params *params,Point *points,int i){int slot=SlotNum(i);int team=TeamNum(i);PokeathlonCourse_GetMonInfo(course,team,slot,info+i);params->ability[i]=ov96_021E6108(PokeathlonCourse_GetPerformance(course,team,slot));points[i].x=info[i].species;points[i].y=info[i].form;}
static inline State *CreateCourseState(void *course){State *data=PokeathlonCourse_AllocPtr4FromHeap(course,0x6c0);MI_CpuFill8(data,0,0x6c0);data->bg=BgConfig_Alloc((enum HeapID)139);return data;}
static inline void InitializeCourseScene(void *course,ObjCharTransferTemplate *cfg){
 Heap_Create((enum HeapID)92,(enum HeapID)139,0x68000);Main_SetVBlankIntrCB(NULL,NULL);Main_SetHBlankIntrCB(NULL,NULL);GfGfx_DisableEngineAPlanes();GfGfx_DisableEngineBPlanes();
 GX_SetVisiblePlane(0);GXS_SetVisiblePlane(0);ov96_02204F20();
 State *data=CreateCourseState(course);
 PokeathlonCourse_SetField3A4(course,(u32)&data->random,(u32)&data->ready,4);PokeathlonCourse_SetHudPlanes(course,8);
 cfg->maxTasks=202;cfg->sizeMain=0x40000;cfg->sizeSub=0x4000;cfg->heapID=(enum HeapID)139;PokeathlonCourse_InitObjTransfer((int)cfg,22,139,0x300010,16);
 NNS_G2dInitOamManagerModule();OamManager_Create(0,126,0,32,0,126,0,32,(enum HeapID)139);data->heap=139;FontID_Alloc(4,(enum HeapID)139);PokeathlonPennantCapture_InitBgs(data->bg);PokeathlonPennantCapture_InitScoreWindow((u8*)data);gSystem.screensFlipped=1;GfGfx_SwapDisplay();PokeathlonCourse_IncrementField1ED(course);}
static inline void Place3(ActorB *actor, void *sprite, int base, u32 x, int team4, int own, int slot4, Point *selected, int *x1, int *y1) { actor->current=1;PokeathlonCourseMonObject_SetAnimState(sprite,1);x+=128;PokeathlonCourseObject_SetGroundPosition(sprite,x,96);PokeathlonCourseObject_SetTouchRadius(sprite,ov96_021E6104());base+=208;actor->x=(fx32)((u32)base<<12);actor->y=0x1a0000;PokeathlonCourseMonObject_SetDrawPriorityStep(sprite,3000);
 if(team4==own && slot4==0){PokeathlonCourseObject_GroundToCenter(sprite,x,96,x1,y1);selected->x=128;selected->y=*y1;} }
// @fn ov96_022047EC
int ov96_022047EC(void *course){
 MonInfo info[12];Params params;u8 setup[100];ObjCharTransferTemplate config;VecFx32 pos;Point points[12];int memberCoords[6];Point selected;int x1,y1;
 int team4;int team3;State *s;int own;s32 i2;int slot3;int slot4;
 s=PokeathlonCourse_GetHeapAllocPtr4(course);
 switch(PokeathlonCourse_GetField1ED(course)) {
 case 0:InitializeCourseScene(course,&config);break;
 case 1:{s->screen=PokeathlonPennantCaptureDisplay_New(s->heap,s->bg,course);int count=PokeathlonCourse_GetParticipantCount(course);int mode=PokeathlonCourse_GetMode(course);s->secondary=PokeathlonPennantCaptureCpu_New(s->heap,4-count,mode);s->resource=PokeathlonCourseSprites_CreateResources(s->heap,0x2bf,1);PokeathlonCourse_IncrementField1ED(course);break;}
 case 2:{pos=ov96_0221CAD4;s->renderer=PokeathlonCourseSpritePool_New(s->heap,&pos);PokeathlonCourseSpritePool_SetSurfaceCoords(s->renderer,0,0,0,0x2bc000);
 s->icons=PokeathlonCourseMonObjects_Create((enum HeapID)s->heap,(void*)12,(void*)2,s->resource,PokeathlonCourseSpritePool_GetSpriteList(s->renderer));
 PokeathlonCourseSpritePool_SetResourceId(s->renderer,0,101);PokeathlonCourseSpritePool_SetResourceId(s->renderer,1,102);PokeathlonCourseSpritePool_SetResourceId(s->renderer,2,103);PokeathlonCourseSpritePool_SetResourceId(s->renderer,3,104);
 PokeathlonPennantCaptureDisplay_LoadSpriteResources(s->screen,s->renderer);PokeathlonPennantCapture_LoadSpriteResources(s->renderer);PokeathlonCourseSpritePool_TransferResources(s->renderer);Sprite_SetDrawPriority(*ov96_021E6290(course,0,s->resource,s->renderer),1);
 PokeathlonPennantCapture_CreateSprites(s,s->renderer);PokeathlonPennantCaptureDisplay_CreateSprites(s->screen,s->renderer,s->resource);PokeathlonCourse_IncrementField1ED(course);break;}
 case 3:{int i;
 for(i=0;i<12;i++){slot3=SlotNum(i);team3=TeamNum(i);PokeathlonCourse_GetMonInfo(course,team3,slot3,info+i);params.ability[i]=ov96_021E6108(PokeathlonCourse_GetPerformance(course,team3,slot3));points[i].x=info[i].species;points[i].y=info[i].form;}
 PokeathlonPennantCaptureDisplay_LoadIcons(s->screen,points);u8 team=ov96_021E5F24(course);PokeathlonPennantCaptureDisplay_LoadMonPics(s->screen,info+team*3);PokeathlonPennantCaptureDisplay_LoadMonPicToVram(s->screen,0,1);PokeathlonPennantCaptureDisplay_LoadMonPicToVram(s->screen,1,2);
 params.a=0;params.b=2;params.c=0;params.d=1;params.e=1;PokeathlonCourseMonObjects_Load(s->icons,12,info,&params,0,0);PokeathlonPennantCaptureDisplay_BringToFront(s->screen,ov96_021E5F24(course));PokeathlonCourse_IncrementField1ED(course);break;}
 case 4:{if(!PokeathlonCourseMonObjects_IsLoaded(s->icons))break;
 own=ov96_021E5F24(course);PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData*)s->bg);PokeathlonCourse_SetField1F4(course,1);ReadWholeNarcMemberByIdPair(memberCoords,170,14);s32 i;int j;
 for(i=0;i<12;i++){void *sprite=PokeathlonCourseMonObjects_Get(s->icons,(u8)i);slot4=i%3;if(slot4==0)PokeathlonCourseMonObject_SetDrawFlag(sprite,1);
 u32 member=ov96_021E6138(GetMember(course,i));PokeathlonCourseObject_SetCollision(sprite,memberCoords[(u32)(member-1)*2],memberCoords[(u32)(member-1)*2+1]);
 sprite=PokeathlonCourseMonObjects_Get(s->icons,(u8)i);team4=i/3;ActorB *actor=s->actors+team4;
 actor->sprites[slot4]=sprite;Place3(actor,sprite,team4*32,team4*32,team4,own,slot4,&selected,&x1,&y1);}
 ReadWholeNarcMemberByIdPair(setup,170,4);s->duration=1800;PokeathlonPennantCaptureDisplay_SetTimer(s->screen,s->duration);PokeathlonPennantCapture_ResetPennantSpots(s);
 for(i2=0;i2<4;i2++){s->future[i2]=NULL;ActorB *actor=s->actors+i2;for(j=0;j<3;j++)PokeathlonPennantCapture_SetMoveParams(course,setup,(u8)i2,(u8)j,actor);
 if(ov96_021E5F24(course)==0)PokeathlonPennantCaptureCpu_SetPlayer(s->secondary,(u8)i2,actor);s->actors[i2].participant=i2;}
 if(ov96_021E5F24(course)==0)PokeathlonPennantCapture_PackFrame(s,ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(course)+40));
 ov96_021E634C(course,0,s->resource,s->renderer,1,1,&selected);PokeathlonPennantCapture_LoadBgGfx(s);GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineBTogglePlanes(16,1);
 ActorB *actor=s->actors+own;int y=actor->y/4096;int ax=actor->x/4096;ScheduleSetBgPosText(s->bg,0,BG_POS_OP_SET_X,ax-128);ScheduleSetBgPosText(s->bg,0,BG_POS_OP_SET_Y,y-96);
 WirelessStatusIcon_CreateOnScreenIfConnected(1);PokeathlonCourse_IncrementField1ED(course);break;}
 case 5:if(ov96_021E5F24(course)==0)s->random=LCRandom()%20;PokeathlonCourse_IncrementField1ED(course);break;
 case 6:return 1;
 }
 return 0;
}
