#pragma force_active on
#include "global.h"
#include <nitro/gx/gx.h>
#include "heap.h"
#include "system.h"
#include "pokeathlon/pokeathlon.h"
#include "bg_window.h"
#include "obj_char_transfer.h"
#include "font.h"
#include "unk_0200B150.h"

typedef struct Member {u8 p0[8];float factor,other;u8 p1[7];u8 tag,mode;u8 p2[3];} Member;
typedef struct Actor {void *sprites[3];u8 p0[12];Member members[3];void *sprite6c;u8 p1[8];void *sprite78;fx32 x,y;u8 p2[7];u8 current;VecFx32 velocity;u8 collision[4];u8 p3;u8 blocked;u8 p4[6];u8 tag;u8 p5[4];u8 cooldown;u8 p6[0x26];u8 participant;u8 p7[3];} Actor;
typedef struct ActorB {void*sprites[3];u8 pad0c[0x4c];fx32 x,y;u8 pad60[0x38];int participant;u8 pad9c[0x14];u8 current;u8 tail[7];} ActorB;
typedef struct State {void*bg;u8 p04[0x10];u32 heap;u8 p18[8];void*renderer;ActorB actors[4];u8 p304[0x30];void*future[4];void*resource;void*icons;u8 p34c[0x20];void*secondary;void*screen;u32 random,ready;u8 p37c[0x190];u16 duration;u8 tail[0x1b2];} State;



typedef struct MonInfo {u16 species,form;u8 pad[3],gender;u32 flags,personality;} MonInfo;
typedef struct Point {u16 x,y;} Point;

typedef struct Params {int a,b,c,d,e;u32 ability[12];} Params;

extern void GfGfx_DisableEngineAPlanes(void);
extern void GfGfx_DisableEngineBPlanes(void);
extern void ov96_021FCEE0(void);
extern void MI_CpuFill8(void *,u8,u32);

extern void ov96_021E6670(u8 *,u32);
extern void ov96_021E92B0(int,int,int,int,int);


extern void ov96_021FCF00(void *);
extern void ov96_021FFD4C(State *);

extern void GfGfx_SwapDisplay(void);
extern void *ov96_021FFF3C(u32,void *,void *);
extern void *ov96_02200E3C(u32,int,int);
extern void *ov96_021E9A78(u32,int,int);
extern VecFx32 ov96_0221C5E4;
extern void *ov96_021EB180(u32,VecFx32 *);
extern void ov96_021EB5C8(void *,int,int,int,int);
extern void *ov96_021EB5E8(void *);
extern void *ov96_021EA854(enum HeapID,void*,void*,void *,void *);
extern void ov96_021EB29C(void *,int,int);
extern void ov96_022000E4(void *,void *);
extern void ov96_021FD0E4(void *);
extern void ov96_021EB3A4(void *);
extern void **ov96_021E6290(void *,int,void *,void *);
extern void Sprite_SetDrawPriority(void *,int);
extern void ov96_021FD128(State *,void *);
extern void ov96_02200180(void *,void *,void *);
extern void ov96_021E6168(void *,int,int,MonInfo *);
extern void *ov96_021E60C0(void *,int,int);
extern u32 ov96_021E6108(void *);
extern void ov96_022002F8(void *,Point *);
extern void ov96_022003E8(void *,MonInfo *);
extern void ov96_02200454(void *,int,int);
extern void ov96_021EA8A8(void *,int,MonInfo *,Params *,int,int);
extern void ov96_02200B04(void *,int);
extern int ov96_021EAA00(void *);
extern void ReadWholeNarcMemberByIdPair(void *,int,int);
extern void *ov96_021EAA04(void *,u8);
extern void ov96_021EAB38(void *,int);
extern u32 ov96_021E6138(void *);
extern void ov96_021EAF70(void *,int,int);
extern void ov96_021EAC0C(void *,int);
extern void ov96_021EAF94(void *,int,int);
extern int ov96_021E6104(void);
extern void ov96_021EAF6C(void *,int);
extern void ov96_021EB0A4(void *,int,int,int *,int *);
extern void ov96_021FFB7C(Actor *,u8,int);
extern void ov96_02200A64(void *,int);
extern void ov96_021FEFE8(void *,void *,u8,u8,Actor *);
extern void ov96_02200E78(void *,u8,Actor *);
extern void ov96_021FDA30(State *,void *);
extern void ov96_021E634C(void *,int,void *,void *,int,int,Point *);
extern void ov96_021FD060(State *);


extern void sub_0203A994(int);
extern u16 LCRandom(void);
extern void ov96_02200E80(void *,void *);
static inline int TeamNum(int i){return i/3;}
static inline int SlotNum(int i){return i%3;}
static inline void *GetMember(void *course,int i){return ov96_021E60C0(course,i/3,i%3);}

extern void ov96_02204F20(void),ov96_02204F40(void *),ov96_02207740(u8 *);
extern void *ov96_02207CCC(u32,void *,void *),*ov96_02208AA8(u32,int,int);
extern VecFx32 ov96_0221CAD4;
extern void ov96_02207E7C(void *,void *),ov96_022050B4(void *),ov96_022050F8(State *,void *),ov96_02207F18(void *,void *,void *);
extern void Sprite_SetDrawPriority(void *,int);
extern void ov96_022080F4(void *,Point *),ov96_02208250(void *,MonInfo *),ov96_022082BC(void *,int,int),ov96_02208784(void *,int),ov96_02208740(void *,int),ov96_022077F4(State *),ov96_02206E88(void *,void *,u8,u8,void *),ov96_02208AE8(void *,u8,void *),ov96_02205AFC(State *,void *),ov96_02205048(State *),ov96_021EABDC(void *,int);

int ov96_022047EC(void *);
static inline void ReadCompetitor(void *course,MonInfo *info,Params *params,Point *points,int i){int slot=SlotNum(i);int team=TeamNum(i);ov96_021E6168(course,team,slot,info+i);params->ability[i]=ov96_021E6108(ov96_021E60C0(course,team,slot));points[i].x=info[i].species;points[i].y=info[i].form;}
static inline State *CreateCourseState(void *course){State *data=PokeathlonCourse_AllocPtr4FromHeap(course,0x6c0);MI_CpuFill8(data,0,0x6c0);data->bg=BgConfig_Alloc((enum HeapID)139);return data;}
static inline void InitializeCourseScene(void *course,ObjCharTransferTemplate *cfg){
 Heap_Create((enum HeapID)92,(enum HeapID)139,0x68000);Main_SetVBlankIntrCB(NULL,NULL);Main_SetHBlankIntrCB(NULL,NULL);GfGfx_DisableEngineAPlanes();GfGfx_DisableEngineBPlanes();
 GX_SetVisiblePlane(0);GXS_SetVisiblePlane(0);ov96_02204F20();
 State *data=CreateCourseState(course);
 PokeathlonCourse_SetField3A4(course,(u32)&data->random,(u32)&data->ready,4);ov96_021E6670(course,8);
 cfg->maxTasks=202;cfg->sizeMain=0x40000;cfg->sizeSub=0x4000;cfg->heapID=(enum HeapID)139;ov96_021E92B0((int)cfg,22,139,0x300010,16);
 NNS_G2dInitOamManagerModule();OamManager_Create(0,126,0,32,0,126,0,32,(enum HeapID)139);data->heap=139;FontID_Alloc(4,(enum HeapID)139);ov96_02204F40(data->bg);ov96_02207740((u8*)data);gSystem.screensFlipped=1;GfGfx_SwapDisplay();PokeathlonCourse_IncrementField1ED(course);}
static inline void Place3(ActorB *actor, void *sprite, int base, u32 x, int team4, int own, int slot4, Point *selected, int *x1, int *y1) { actor->current=1;ov96_021EAC0C(sprite,1);x+=128;ov96_021EAF94(sprite,x,96);ov96_021EAF6C(sprite,ov96_021E6104());base+=208;actor->x=(fx32)((u32)base<<12);actor->y=0x1a0000;ov96_021EABDC(sprite,3000);
 if(team4==own && slot4==0){ov96_021EB0A4(sprite,x,96,x1,y1);selected->x=128;selected->y=*y1;} }
// @fn ov96_022047EC
int ov96_022047EC(void *course){
 MonInfo info[12];Params params;u8 setup[100];ObjCharTransferTemplate config;VecFx32 pos;Point points[12];int memberCoords[6];Point selected;int x1,y1;
 int team4;int team3;State *s;int own;s32 i2;int slot3;int slot4;
 s=PokeathlonCourse_GetHeapAllocPtr4(course);
 switch(PokeathlonCourse_GetField1ED(course)) {
 case 0:InitializeCourseScene(course,&config);break;
 case 1:{s->screen=ov96_02207CCC(s->heap,s->bg,course);int count=PokeathlonCourse_GetParticipantCount(course);int mode=PokeathlonCourse_GetMode(course);s->secondary=ov96_02208AA8(s->heap,4-count,mode);s->resource=ov96_021E9A78(s->heap,0x2bf,1);PokeathlonCourse_IncrementField1ED(course);break;}
 case 2:{pos=ov96_0221CAD4;s->renderer=ov96_021EB180(s->heap,&pos);ov96_021EB5C8(s->renderer,0,0,0,0x2bc000);
 s->icons=ov96_021EA854((enum HeapID)s->heap,(void*)12,(void*)2,s->resource,ov96_021EB5E8(s->renderer));
 ov96_021EB29C(s->renderer,0,101);ov96_021EB29C(s->renderer,1,102);ov96_021EB29C(s->renderer,2,103);ov96_021EB29C(s->renderer,3,104);
 ov96_02207E7C(s->screen,s->renderer);ov96_022050B4(s->renderer);ov96_021EB3A4(s->renderer);Sprite_SetDrawPriority(*ov96_021E6290(course,0,s->resource,s->renderer),1);
 ov96_022050F8(s,s->renderer);ov96_02207F18(s->screen,s->renderer,s->resource);PokeathlonCourse_IncrementField1ED(course);break;}
 case 3:{int i;
 for(i=0;i<12;i++){slot3=SlotNum(i);team3=TeamNum(i);ov96_021E6168(course,team3,slot3,info+i);params.ability[i]=ov96_021E6108(ov96_021E60C0(course,team3,slot3));points[i].x=info[i].species;points[i].y=info[i].form;}
 ov96_022080F4(s->screen,points);u8 team=ov96_021E5F24(course);ov96_02208250(s->screen,info+team*3);ov96_022082BC(s->screen,0,1);ov96_022082BC(s->screen,1,2);
 params.a=0;params.b=2;params.c=0;params.d=1;params.e=1;ov96_021EA8A8(s->icons,12,info,&params,0,0);ov96_02208784(s->screen,ov96_021E5F24(course));PokeathlonCourse_IncrementField1ED(course);break;}
 case 4:{if(!ov96_021EAA00(s->icons))break;
 own=ov96_021E5F24(course);PokeathlonCourse_SetVBlankIntrCB((PokeathlonCourseData*)s->bg);PokeathlonCourse_SetField1F4(course,1);ReadWholeNarcMemberByIdPair(memberCoords,170,14);s32 i;int j;
 for(i=0;i<12;i++){void *sprite=ov96_021EAA04(s->icons,(u8)i);slot4=i%3;if(slot4==0)ov96_021EAB38(sprite,1);
 u32 member=ov96_021E6138(GetMember(course,i));ov96_021EAF70(sprite,memberCoords[(u32)(member-1)*2],memberCoords[(u32)(member-1)*2+1]);
 sprite=ov96_021EAA04(s->icons,(u8)i);team4=i/3;ActorB *actor=s->actors+team4;
 actor->sprites[slot4]=sprite;Place3(actor,sprite,team4*32,team4*32,team4,own,slot4,&selected,&x1,&y1);}
 ReadWholeNarcMemberByIdPair(setup,170,4);s->duration=1800;ov96_02208740(s->screen,s->duration);ov96_022077F4(s);
 for(i2=0;i2<4;i2++){s->future[i2]=NULL;ActorB *actor=s->actors+i2;for(j=0;j<3;j++)ov96_02206E88(course,setup,(u8)i2,(u8)j,actor);
 if(ov96_021E5F24(course)==0)ov96_02208AE8(s->secondary,(u8)i2,actor);s->actors[i2].participant=i2;}
 if(ov96_021E5F24(course)==0)ov96_02205AFC(s,ov96_021E8A20((u8 *)PokeathlonCourse_GetDataCopyArea(course)+40));
 ov96_021E634C(course,0,s->resource,s->renderer,1,1,&selected);ov96_02205048(s);GfGfx_EngineATogglePlanes(16,1);GfGfx_EngineBTogglePlanes(16,1);
 ActorB *actor=s->actors+own;int y=actor->y/4096;int ax=actor->x/4096;ScheduleSetBgPosText(s->bg,0,BG_POS_OP_SET_X,ax-128);ScheduleSetBgPosText(s->bg,0,BG_POS_OP_SET_Y,y-96);
 sub_0203A994(1);PokeathlonCourse_IncrementField1ED(course);break;}
 case 5:if(ov96_021E5F24(course)==0)s->random=LCRandom()%20;PokeathlonCourse_IncrementField1ED(course);break;
 case 6:return 1;
 }
 return 0;
}
