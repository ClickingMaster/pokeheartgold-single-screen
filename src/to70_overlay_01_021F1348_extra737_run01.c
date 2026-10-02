#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 p[0x10];void *model,*animation,*textures;} Data;
typedef struct {u8 p[0x20];Data *data;} State;
extern void *ov01_021FC5A4(void *,int),*GF3dGfxRawResMan_GetObjById(void *,int),*GF3dGfxRawResObj_GetTex(void *),*ov01_021F1824(Data *,void *,void *,void *,void *,void *,int);extern void sub_02026E18(void *,void *);
static inline Data *GetData(State *s){return s->data;}
void *ov01_021F1758(State *,void *,int,int,int,int,int);
void *ov01_021F1758(State *s,void *arg,int model,int anim,int texture,int flag,int extra){u32 local[4];int cachedAnim=anim;Data *d=GetData(s);void *m=ov01_021FC5A4(d->model,model),*res,*tex,*result;sub_02026E18(ov01_021FC5A4(d->animation,cachedAnim),local);res=GF3dGfxRawResMan_GetObjById(d->textures,texture);GF_ASSERT(res);tex=GF3dGfxRawResObj_GetTex(res);if(flag==1)res=0;result=ov01_021F1824(d,arg,m,local,tex,res,extra);GF_ASSERT(result);return result;}
