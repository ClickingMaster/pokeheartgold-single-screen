#pragma force_active on
#include "global.h"
extern void*AddCharResObjFromOpenNarc(void*,void*,int,int,int,int,int),*AddPlttResObjFromOpenNarc(void*,void*,int,int,int,int,int,int),*AddCellOrAnimResObjFromOpenNarc(void*,void*,int,int,int,int,int);
extern void SpriteTransfer_CreateCharTransferTask_AllocAtEnd(void*),sub_0200A740(void*),SpriteTransfer_CreatePlttTransferTask(void*);
void ov28_0225D92C(void**,void**,void*,int,int,int,int,int,int,int,int,int);
void ov28_0225D92C(void**out,void**man,void*narc,int chr,int pltt,int cell,int anim,int count,int id0,int id1,int id2,int id3){out[0]=AddCharResObjFromOpenNarc(man[0],narc,chr,1,id0,2,8);SpriteTransfer_CreateCharTransferTask_AllocAtEnd(out[0]);sub_0200A740(out[0]);out[1]=AddPlttResObjFromOpenNarc(man[1],narc,pltt,0,id1,2,count,8);SpriteTransfer_CreatePlttTransferTask(out[1]);sub_0200A740(out[1]);out[2]=AddCellOrAnimResObjFromOpenNarc(man[2],narc,cell,1,id2,2,8);out[3]=AddCellOrAnimResObjFromOpenNarc(man[3],narc,anim,1,id3,3,8);}
