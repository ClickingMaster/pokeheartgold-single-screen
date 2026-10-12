#pragma force_active on
#include "global.h"
typedef struct {u8 p[0xf0];void *raw;u8 q[0x10];void *manager;} W;
typedef struct {u16 a,model;} Entry;
extern int ResourceBank_Contains(void*,int);extern Entry *ov01_021F98B4(int,int,int);extern void *MapObjectManager_GetMapModelNarc(void*);extern void ResourceBank_LoadFromNarc(void*,int,void*,int,int);
extern int GF3dGfxRawResMan_DoesNotHaveObjWithId(void*,int);extern int ov01_021FA524(W*,int);extern int GetMoveModelNoBySpriteId(int);extern int ov01_021FA470(W*,int,int,void*);
int ov01_021F98CC(W*w,void *m,int id,int a,int b);
int ov01_021F9918(W*w,void*m,int id);
int ov01_021F98CC(W*w,void *m,int id,int a,int b) {Entry *e;void *n;if(ResourceBank_Contains(m,id)==1)return 0;e=ov01_021F98B4(id,a,b);if(e==0)GF_AssertFail();n=MapObjectManager_GetMapModelNarc(w->manager);ResourceBank_LoadFromNarc(m,id,n,e->model,0);return 1;}
int ov01_021F9918(W*w,void*m,int id) {int model;if(GF3dGfxRawResMan_DoesNotHaveObjWithId(w->raw,id)==0)return 0;if(ov01_021FA524(w,id))return 1;model=GetMoveModelNoBySpriteId(id);if(model<0)return 2;switch(ov01_021FA470(w,id,model,m)){case 1:return 0;default:return 1;};}
