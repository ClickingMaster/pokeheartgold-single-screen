#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void *mgr[4];} Managers;
typedef struct {int active;void *res[4];u8 header[64];} Group;
extern void *AddCharResObjFromOpenNarc(void *,void *,int,int,int,int,int);
extern void *AddPlttResObjFromOpenNarc(void *,void *,int,int,int,int,int,int);
extern void *AddCellOrAnimResObjFromOpenNarc(void *,void *,int,int,int,int,int);
extern int SpriteTransfer_CreateCharTransferTask_AllocAtEnd(void *);
extern int SpriteTransfer_CreatePlttTransferTask(void *);
extern void sub_0200A740(void *);
extern void CreateSpriteResourcesHeader(void *,int,int,int,int,int,int,int,int,void *,void *,void *,void *,void *,void *);
void ov90_02258EE0(Managers *,Group *,void *,int,int,int,int,int,int,int);
void ov90_02258EE0(Managers *m,Group *g,void *narc,int pltt,int count,int chr,int cell,int anim,volatile int id,volatile int heap){
 int heapId,resourceId;
 if(g->active)GF_AssertFail();g->active=1;resourceId=id;heapId=heap;
 g->res[0]=AddCharResObjFromOpenNarc(m->mgr[0],narc,chr,0,resourceId,1,heapId);
 g->res[1]=AddPlttResObjFromOpenNarc(m->mgr[1],narc,pltt,0,resourceId,1,count,heapId);
 g->res[2]=AddCellOrAnimResObjFromOpenNarc(m->mgr[2],narc,cell,0,resourceId,2,heapId);
 g->res[3]=AddCellOrAnimResObjFromOpenNarc(m->mgr[3],narc,anim,0,resourceId,3,heapId);
 if(!SpriteTransfer_CreateCharTransferTask_AllocAtEnd(g->res[0]))GF_AssertFail();sub_0200A740(g->res[0]);
 if(!SpriteTransfer_CreatePlttTransferTask(g->res[1]))GF_AssertFail();sub_0200A740(g->res[1]);
 CreateSpriteResourcesHeader(g->header,resourceId,resourceId,resourceId,resourceId,-1,-1,0,1,m->mgr[0],m->mgr[1],m->mgr[2],m->mgr[3],0,0);
}
