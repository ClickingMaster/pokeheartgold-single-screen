#pragma force_active on
#include "global.h"
typedef struct{u32 a,b,c;} Triple;
typedef struct{void *a,*b;Triple pos,rot;} Request;
extern Triple ov01_02208B64;
extern void *sub_02023D44(Request *);
extern void sub_02023EA4(void *,int);extern void sub_02023EE0(void *,int);extern void sub_02023F40(void *,int);extern void *sub_02023F90(void *);extern void ov01_021EA3B0(void *);
extern int ov01_021FA28C(int);extern int ov01_021FA2A0(int);extern u32 ov01_021FC5A4(void *,int);extern void BillboardGfxSequence_Init(u32,void *);
extern void *ov01_021FA1F4(void *);extern void *GF3dGfxRawResMan_GetObjById(void *,int);extern void *GF3dGfxRawResObj_GetTex(void *);extern u32 GF3dGfxRawResObj_GetTexKey(void *);extern u32 GF3dGfxRawResObj_GetTex4x4Key(void *);extern u32 GF3dGfxRawResObj_GetPlttKey(void *);extern u32 ov01_021FA2AC(int);
typedef struct {u8 pad[0xf8];void *a,*b;} Work;
typedef struct {u32 a;void *tex;u32 c;u8 data[16];u32 key,key4,pltt;} Out;
void *ov01_021FA31C(void *,void *,Triple *);
void ov01_021FA370(Work *,int,Out *);
void *ov01_021FA31C(void *a,void *b,Triple *p){Request r;void *obj;r.a=a;r.b=b;r.pos=*p;r.rot=ov01_02208B64;obj=sub_02023D44(&r);if(obj){sub_02023EA4(obj,0);sub_02023EE0(obj,0);sub_02023F40(obj,0);ov01_021EA3B0(sub_02023F90(obj));}return obj;}
void ov01_021FA370(Work *w,int id,Out *out){void *obj;out->a=ov01_021FC5A4(w->a,ov01_021FA28C(id));BillboardGfxSequence_Init(ov01_021FC5A4(w->b,ov01_021FA2A0(id)),out->data);obj=GF3dGfxRawResMan_GetObjById(ov01_021FA1F4(w),id);out->tex=GF3dGfxRawResObj_GetTex(obj);out->key=GF3dGfxRawResObj_GetTexKey(obj);out->key4=GF3dGfxRawResObj_GetTex4x4Key(obj);out->pltt=GF3dGfxRawResObj_GetPlttKey(obj);out->c=ov01_021FA2AC(id);}
