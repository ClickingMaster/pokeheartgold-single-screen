#pragma force_active on
#include "global.h"
#include "heap.h"
typedef struct Member {u8 p0[8];float factor,other;u8 p1[7];u8 tag,mode;u8 p2[3];} Member;
typedef struct Actor {void *sprites[3];u8 p0[12];Member members[3];void *sprite6c;u8 p1[8];void *sprite78;fx32 x,y;u8 p2[4];u8 mode;u8 p2tail[2];u8 current;VecFx32 velocity;u8 collision[4];u8 p3;u8 blocked;u8 p4[6];u8 tag;u8 p5[4];u8 cooldown;u8 p6[0x26];u8 participant;u8 p7[3];} Actor;
typedef struct State {void*bg;u8 p04[0x10];u32 heap;void*renderer;u8 p1c[0x14];Actor actors[4];u8 p380[0x30];void*future[4];u32 p3c0;void*resource;void*icons;u8 p3cc[0x10];void*secondary;void*screen;u8 stage[120],copy[120];u8 p4d4[0x16c];u16 duration;u8 tail[6];} State;



typedef struct MonInfo {u16 species,form,special;u8 kind,field7;u32 participant,fieldC;} MonInfo;
typedef struct Point {u16 x,y;} Point;
typedef struct {s32 x,y;} MemberCoordinates;
typedef char ActorStride[(sizeof(Actor)==0xd4)?1:-1];
typedef char StateSize[(sizeof(State)==0x648)?1:-1];
typedef char MonInfoSize[(sizeof(MonInfo)==16)?1:-1];
typedef struct Config {u32 file,size,size2,heap;} Config;
typedef struct Params {int a,b,c,d,e;u32 ability[12];} Params;
extern void *PokeathlonCourse_GetHeapAllocPtr4(void *);
extern u8 PokeathlonCourse_GetField1ED(void *);

extern void Main_SetVBlankIntrCB(void *,void *);
extern void Main_SetHBlankIntrCB(void *,void *);
extern void GfGfx_DisableEngineAPlanes(void);
extern void GfGfx_DisableEngineBPlanes(void);
extern void ov96_021FCEE0(void);
extern State *PokeathlonCourse_AllocPtr4FromHeap(void *,u32);
extern void MI_CpuFill8(void *,u8,u32);
extern void *BgConfig_Alloc(int);
extern void PokeathlonCourse_SetField3A4(void *,u32,u32,u32);
extern void PokeathlonCourse_SetHudPlanes(void *,int);
extern void PokeathlonCourse_InitObjTransfer(Config *,int,int,u32,int);
extern void OamManager_Create(int,int,int,int,int,int,int,int,int);
extern void FontID_Alloc(int,int);
extern void ov96_021FCF00(void *);
extern void ov96_021FFD4C(State *);
extern u8 gSystem[];
extern void GfGfx_SwapDisplay(void);
extern void PokeathlonCourse_IncrementField1ED(void *);
extern void *ov96_021FFF3C(u32,void *,void *);
extern u8 PokeathlonCourse_GetParticipantCount(void *);
extern u32 PokeathlonCourse_GetMode(void *);
extern void *ov96_02200E3C(u32,int,int);
extern void *PokeathlonCourseSprites_CreateResources(u32,int,int);
extern VecFx32 ov96_0221C5E4;
extern void *PokeathlonCourseSpritePool_New(u32,VecFx32 *);
extern void PokeathlonCourseSpritePool_SetSurfaceCoords(void *,int,int,int,int);
extern void *PokeathlonCourseSpritePool_GetSpriteList(void *);
extern void *PokeathlonCourseMonObjects_Create(u32,int,int,void *,void *);
extern void PokeathlonCourseSpritePool_SetResourceId(void *,int,int);
extern void ov96_022000E4(void *,void *);
extern void ov96_021FD0E4(void *);
extern void PokeathlonCourseSpritePool_TransferResources(void *);
extern void **ov96_021E6290(void *,int,void *,void *);
extern void Sprite_SetDrawPriority(void *,int);
extern void ov96_021FD128(State *,void *);
extern void ov96_02200180(void *,void *,void *);
extern void PokeathlonCourse_GetMonInfo(void *,int,int,MonInfo *);
typedef struct W40970Mini {u8 p0[6];u8 type6,p7,type8;u8 tail[11];}W40970Mini;
typedef char W40970MiniStride[(sizeof(W40970Mini)==20)?1:-1];
extern W40970Mini *PokeathlonCourse_GetPerformance(void *,int,int);
extern int ov96_021E6108(void *);
extern int ov96_021E5F24(void *);
extern void ov96_022002F8(void *,Point *);
extern void ov96_022003E8(void *,MonInfo *);
extern void ov96_02200454(void *,int,int);
extern void PokeathlonCourseMonObjects_Load(void *,int,MonInfo *,Params *,int,int);
extern void ov96_02200B04(void *,int);
extern int PokeathlonCourseMonObjects_IsLoaded(void *);
extern void PokeathlonCourse_SetVBlankIntrCB(void *);
extern void PokeathlonCourse_SetField1F4(void *,int);
extern void ReadWholeNarcMemberByIdPair(void *,int,int);
extern void *PokeathlonCourseMonObjects_Get(void *,u8);
extern void PokeathlonCourseMonObject_SetDrawFlag(void *,int);
extern int ov96_021E6138(void *);
extern void PokeathlonCourseObject_SetCollision(void *,int,int);
extern void PokeathlonCourseMonObject_SetAnimState(void *,int);
extern void PokeathlonCourseObject_SetGroundPosition(void *,int,int);
extern int ov96_021E6104(void);
extern void PokeathlonCourseObject_SetTouchRadius(void *,int);
extern void PokeathlonCourseObject_GroundToCenter(void *,int,int,int *,int *);
extern void ov96_021FFB7C(Actor *,u8,int);
extern void ov96_02200A64(void *,int);
extern void ov96_021FEFE8(void *,void *,u8,u8,Actor *);
extern void ov96_02200E78(void *,u8,Actor *);
extern void *PokeathlonCourse_GetDataCopyArea(void *);
extern void *ov96_021E8A20(void *);
extern void ov96_021FDA30(State *,void *);
extern void ov96_021E634C(void *,int,int,void *,u32,u8,u32);
extern void ov96_021FD060(State *);
extern void GfGfx_EngineATogglePlanes(int,int);
extern void GfGfx_EngineBTogglePlanes(int,int);
extern void WirelessStatusIcon_CreateOnScreenIfConnected(int);
extern int LCRandom(void);
extern void ov96_02200E80(void *,void *);

static inline int TeamNum(int i){return i/3;}
static inline int SlotNum(int i){return i%3;}

#pragma push
#pragma opt_common_subs off
int ov96_021FC768(void *);
// @fn ov96_021FC768
int ov96_021FC768(void *course){
 State *s=PokeathlonCourse_GetHeapAllocPtr4(course);
 switch(PokeathlonCourse_GetField1ED(course)){
 case 0:{
 Heap_Create((enum HeapID)92,(enum HeapID)144,0x68000);Main_SetVBlankIntrCB(NULL,NULL);Main_SetHBlankIntrCB(NULL,NULL);GfGfx_DisableEngineAPlanes();GfGfx_DisableEngineBPlanes();
 *(vu32 *)0x04000000&=0xffffe0ff;*(vu32 *)0x04001000&=0xffffe0ff;ov96_021FCEE0();
 State *data=PokeathlonCourse_AllocPtr4FromHeap(course,0x648);MI_CpuFill8(data,0,0x648);data->bg=BgConfig_Alloc(144);
 PokeathlonCourse_SetField3A4(course,(u32)data->stage,(u32)data->copy,120);PokeathlonCourse_SetHudPlanes(course,8);
 Config config;config.file=151;config.size=0x40000;config.size2=0x4000;config.heap=144;PokeathlonCourse_InitObjTransfer(&config,22,144,0x300010,16);
 NNS_G2dInitOamManagerModule();OamManager_Create(0,126,0,32,0,126,0,32,144);data->heap=144;FontID_Alloc(4,144);ov96_021FCF00(data->bg);ov96_021FFD4C(data);gSystem[0x69]=1;GfGfx_SwapDisplay();PokeathlonCourse_IncrementField1ED(course);break;}
 case 1:{s->screen=ov96_021FFF3C(s->heap,s->bg,course);int count=PokeathlonCourse_GetParticipantCount(course);int mode=PokeathlonCourse_GetMode(course);s->secondary=ov96_02200E3C(s->heap,4-count,mode);s->resource=PokeathlonCourseSprites_CreateResources(s->heap,0x2bf,1);PokeathlonCourse_IncrementField1ED(course);break;}
 case 2:{VecFx32 pos=ov96_0221C5E4;s->renderer=PokeathlonCourseSpritePool_New(s->heap,&pos);PokeathlonCourseSpritePool_SetSurfaceCoords(s->renderer,0,0,0,0x200000);
 s->icons=PokeathlonCourseMonObjects_Create(s->heap,12,4,s->resource,PokeathlonCourseSpritePool_GetSpriteList(s->renderer));
 PokeathlonCourseSpritePool_SetResourceId(s->renderer,0,101);PokeathlonCourseSpritePool_SetResourceId(s->renderer,1,102);PokeathlonCourseSpritePool_SetResourceId(s->renderer,2,103);PokeathlonCourseSpritePool_SetResourceId(s->renderer,3,104);
 ov96_022000E4(s->screen,s->renderer);ov96_021FD0E4(s->renderer);PokeathlonCourseSpritePool_TransferResources(s->renderer);Sprite_SetDrawPriority(*ov96_021E6290(course,0,s->resource,s->renderer),1);
 ov96_021FD128(s,s->renderer);ov96_02200180(s->screen,s->renderer,s->resource);PokeathlonCourse_IncrementField1ED(course);break;}
 case 3:{
 MonInfo info[12];
 Point points[12];
 Params params;
 int i;
 u8 team;
 for(i=0;i<12;i++){
  int slot=i%3;
  int row=i/3;
  PokeathlonCourse_GetMonInfo(course,row,slot,&info[i]);
  params.ability[i]=ov96_021E6108(PokeathlonCourse_GetPerformance(course,row,slot));
  points[i].x=info[i].species;
  points[i].y=info[i].form;
 }
 ov96_022002F8(s->screen,points);
 team=ov96_021E5F24(course);
 ov96_022003E8(s->screen,&info[team*3]);
 ov96_02200454(s->screen,0,1);
 ov96_02200454(s->screen,1,2);
 params.a=0;params.b=2;params.c=0;params.d=1;params.e=1;
 PokeathlonCourseMonObjects_Load(s->icons,12,info,&params,0,0);
 ov96_02200B04(s->screen,ov96_021E5F24(course));
 PokeathlonCourse_IncrementField1ED(course);
 break;}
 case 4:{
 int own;
 MemberCoordinates memberCoords[3];
 Point selected;
 int x1,y1;
 u8 setup[160];
 Actor *a;
 int n;
 int i,j;
 if(!PokeathlonCourseMonObjects_IsLoaded(s->icons))break;
 own=ov96_021E5F24(course);
 PokeathlonCourse_SetVBlankIntrCB(s->bg);
 PokeathlonCourse_SetField1F4(course,1);
 ReadWholeNarcMemberByIdPair(memberCoords,170,12);
 for(i=0;i<12;i++){
  void *sprite=PokeathlonCourseMonObjects_Get(s->icons,i);
  int slot=SlotNum(i);
  int team;
  int y;
  if(slot==0)PokeathlonCourseMonObject_SetDrawFlag(sprite,1);
  {MemberCoordinates *mc=&memberCoords[ov96_021E6138(PokeathlonCourse_GetPerformance(course,TeamNum(i),SlotNum(i)))];PokeathlonCourseObject_SetCollision(sprite,mc[-1].x,mc[-1].y);}
  sprite=PokeathlonCourseMonObjects_Get(s->icons,i);
  team=TeamNum(i);
  a=&s->actors[team];
  a->sprites[slot]=sprite;
  y=(team+1)*27+40;
  a->mode=4;
  PokeathlonCourseMonObject_SetAnimState(sprite,4);
  PokeathlonCourseObject_SetGroundPosition(sprite,80,y);
  PokeathlonCourseObject_SetTouchRadius(sprite,ov96_021E6104());
  a->x=0;
  a->y=y<<12;
  if(team==own&&slot==0){
   PokeathlonCourseObject_GroundToCenter(sprite,80,y,&x1,&y1);
   selected.x=x1;
   selected.y=y1;
  }
 }
 for(i=0;i<4;i++){
  Actor *a=&s->actors[i];
  ov96_021FFB7C(a,a->y/FX32_ONE,i==ov96_021E5F24(course));
 }
 ReadWholeNarcMemberByIdPair(setup,170,2);
 s->duration=2700;
 ov96_02200A64(s->screen,s->duration);
 for(n=0;n<4;n++){
  s->future[n]=NULL;
  for(j=0;j<3;j++)ov96_021FEFE8(course,setup,n,j,&s->actors[n]);
  if(ov96_021E5F24(course)==0)ov96_02200E78(s->secondary,n,&s->actors[n]);
  s->actors[n].participant=n;
 }
 if(ov96_021E5F24(course)==0)ov96_021FDA30(s,ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(course)+40));
 ov96_021E634C(course,0,(int)s->resource,s->renderer,1,1,(u32)&selected);
 ov96_021FD060(s);
 GfGfx_EngineATogglePlanes(16,1);
 GfGfx_EngineBTogglePlanes(16,1);
 WirelessStatusIcon_CreateOnScreenIfConnected(1);
 PokeathlonCourse_IncrementField1ED(course);
 break;}
 case 5:if(ov96_021E5F24(course)==0){u8 *stage=s->stage;u8 idx=LCRandom()%10;ReadWholeNarcMemberByIdPair(stage,229,idx);ov96_02200E80(s->secondary,stage);}PokeathlonCourse_IncrementField1ED(course);break;
 case 6:GF_ASSERT(GF_heap_c_dummy_return_true((enum HeapID)92));return 1;
 }
 return 0;
}




#pragma pop
