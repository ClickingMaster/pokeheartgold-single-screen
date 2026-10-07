#pragma force_active on
#include "global.h"
typedef struct {u32 words[9];} Resource;
typedef struct {void *list;Resource *resource;s32 x,y,z,sx,sy,sz;u16 rotation,pad;u32 priority,screen,heap;} Template;
typedef struct {u8 pad[0x10];void *field;u32 pad2;void *list;u8 pad3[0x128];void *charRes,*palRes,*cellRes,*animRes;u8 pad4[0xb0];Resource res[11];void *sprites[16];u8 pad5[0x14c];u32 bit0:1,kind:4,rest:27;} Work;
typedef struct {u8 pad[0x10];void *mon;u8 pad2[3];u8 active:1,rest:7;} Contest;
typedef struct {u16 x,y;} Coord;
extern u32 ov27_0225CF3C[];extern Coord ov27_0225D038[],ov27_0225D05C[];
extern void CreateSpriteResourcesHeader(Resource *,int,int,int,int,int,int,int,int,void *,void *,void *,void *,int,int);
extern void *Sprite_CreateAffine(Template *),*FieldSystem_BugContest_Get(void *);
extern void Sprite_SetAnimActiveFlag(void *,int),Sprite_SetPriority(void *,int),Sprite_SetAffineOverwriteMode(void *,int),Sprite_SetAnimCtrlSeq(void *,int),Sprite_SetDrawFlag(void *,int),Sprite_SetPalOffsetRespectVramOffset(void *,int),Sprite_SetPositionXYWithSubscreenOffset(void *,int,int,int);
extern u32 _fflt(s32),_fadd(u32,u32),_fsub(u32,u32);extern s32 _ffix(u32);
extern int Pokemon_GetIconPalette(void *);extern void GF_AssertFail(void),ov27_0225A4B8(void *),ov27_0225A9C0(void *,int),ov27_0225AA7C(void *);
static inline s32 rounded(int x) {float f;if(x>0)f=0.5f+(float)(x<<12);else f=(float)(x<<12)-0.5f;return (s32)f;}
static inline s32 coord(u16 x) {float f;if(x!=0)f=0.5f+(float)(x<<12);else f=(float)(x<<12)-0.5f;return (s32)f;}
static inline s32 coordHalf(u16 x,float half) {float f;if(x!=0)f=half+(float)(x<<12);else f=(float)(x<<12)-half;return (s32)f;}
void ov27_0225B010(Work *);
#pragma force_active on
void ov27_0225B010(Work *w) {
 Template t;Resource *resource;int i,y;Contest *contest;Coord *cur;
 for(i=0;i<11;i++)CreateSpriteResourcesHeader(&w->res[i],i+100,i+100,ov27_0225CF3C[i],ov27_0225CF3C[i],-1,-1,0,0,w->charRes,w->palRes,w->cellRes,w->animRes,0,0);
 t.list=w->list;t.resource=w->res;t.z=0;t.sx=4096;t.sy=4096;t.sz=4096;t.rotation=0;t.priority=1;t.screen=2;t.heap=8;
 for(i=0;i<9;i++) {
  t.resource=&w->res[i];t.x=coord(ov27_0225D038[i].x);t.y=coord(ov27_0225D038[i].y)+0x100000;
  w->sprites[i]=Sprite_CreateAffine(&t);Sprite_SetAnimActiveFlag(w->sprites[i],1);Sprite_SetPriority(w->sprites[i],0);Sprite_SetAffineOverwriteMode(w->sprites[i],1);
 }
 {Work*row;int j;float half;
 for(j=9,row=(Work*)((u8*)w+36),cur=ov27_0225D05C,half=0.5f,resource=&w->res[9];j<15;cur++,row=(Work*)((u8*)row+4),j++) {
  t.resource=resource;t.x=coordHalf(cur->x,half);t.y=coordHalf(cur->y,half)+0x100000;
  row->sprites[0]=Sprite_CreateAffine(&t);Sprite_SetAnimActiveFlag(row->sprites[0],1);Sprite_SetPriority(row->sprites[0],2);
 }
 }
 if((u32)(w->kind-1)<=2) {
  if(w->kind==1)y=144;else if((u32)(w->kind-2)<=1)y=104;
  t.resource=&w->res[10];t.x=_ffix(_fadd(0x3f000000,_fflt(100<<12)));t.y=rounded(y)+0x100000;
  w->sprites[15]=Sprite_CreateAffine(&t);Sprite_SetAnimActiveFlag(w->sprites[15],1);Sprite_SetPriority(w->sprites[15],0);Sprite_SetAnimCtrlSeq(w->sprites[15],6);
 }
 if(w->kind==2){contest=FieldSystem_BugContest_Get(w->field);if(!contest)GF_AssertFail();if(contest->active){Sprite_SetPositionXYWithSubscreenOffset(w->sprites[6],104,136,0x100000);Sprite_SetPalOffsetRespectVramOffset(w->sprites[6],Pokemon_GetIconPalette(contest->mon));Sprite_SetAnimCtrlSeq(w->sprites[6],6);}}
 else if(w->kind==3)Sprite_SetDrawFlag(w->sprites[6],0);
 Sprite_SetAnimCtrlSeq(w->sprites[9],0);Sprite_SetAnimCtrlSeq(w->sprites[10],8);Sprite_SetAnimCtrlSeq(w->sprites[11],3);Sprite_SetAnimCtrlSeq(w->sprites[12],5);Sprite_SetAnimCtrlSeq(w->sprites[14],12);Sprite_SetDrawFlag(w->sprites[14],0);Sprite_SetPriority(w->sprites[13],1);
 ov27_0225A4B8(w);ov27_0225A9C0(w,1);ov27_0225AA7C(w);
}
