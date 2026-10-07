#pragma force_active on
#include "global.h"
#pragma force_active on
extern void *AddCharResObjFromNarc(void *,int,int,int,int,int,int);
extern void *AddPlttResObjFromNarc(void *,int,int,int,int,int,int,int);
extern void *AddCellOrAnimResObjFromNarc(void *,int,int,int,int,int,int);
extern void SpriteTransfer_CreateCharTransferTask(void *);
extern void SpriteTransfer_CreatePlttTransferTask(void *);
extern void CreateSpriteResourcesHeader(void *,int,int,int,int,int,int,int,int,void *,void *,void *,void *,void *,void *);
typedef struct {u8 pad[0x12c];void *mgr[4];} Managers;
typedef struct {u8 data[36];} Header;
typedef struct {u8 pad[12];Managers *m;Header headers[2];void *a[4];u8 gap[8];void *b[4];} State;
void ov74_0222AF28(State *s);
void ov74_0222AF28(State *s){
s->a[0]=AddCharResObjFromNarc(s->m->mgr[0],113,43,1,20001,1,84);
s->a[1]=AddPlttResObjFromNarc(s->m->mgr[1],113,40,0,20001,1,1,84);
s->a[2]=AddCellOrAnimResObjFromNarc(s->m->mgr[2],113,42,1,20001,2,84);
s->a[3]=AddCellOrAnimResObjFromNarc(s->m->mgr[3],113,41,1,20001,3,84);
s->b[0]=AddCharResObjFromNarc(s->m->mgr[0],113,43,1,20002,2,84);
s->b[1]=AddPlttResObjFromNarc(s->m->mgr[1],113,40,0,20002,2,1,84);
s->b[2]=AddCellOrAnimResObjFromNarc(s->m->mgr[2],113,42,1,20002,2,84);
s->b[3]=AddCellOrAnimResObjFromNarc(s->m->mgr[3],113,41,1,20002,3,84);
SpriteTransfer_CreateCharTransferTask(s->a[0]);SpriteTransfer_CreateCharTransferTask(s->b[0]);SpriteTransfer_CreatePlttTransferTask(s->a[1]);SpriteTransfer_CreatePlttTransferTask(s->b[1]);
CreateSpriteResourcesHeader(&s->headers[0],20001,20001,20001,20001,-1,-1,0,0,s->m->mgr[0],s->m->mgr[1],s->m->mgr[2],s->m->mgr[3],0,0);
{ Header *header = s->headers; CreateSpriteResourcesHeader(header+1,20002,20002,20002,20002,-1,-1,0,0,s->m->mgr[0],s->m->mgr[1],s->m->mgr[2],s->m->mgr[3],0,0); }
}
