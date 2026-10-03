#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x7c];void*sprite[12],*palette[4],*chars[12],*cell[4],*anim[4];}O;
typedef struct{void*bg,*list;u8 pad[0x128];void*chars,*palette,*cell,*anim;}B;
typedef struct{u16 narc,member,count;}R;
typedef struct{u8 source,palette;u16 member;s16 x,y;}I;
typedef struct{int x,y,z;}V;
typedef struct{int words[9];}H;
typedef struct{void*list;H*header;V pos;int priority,vram,heap;}T;
extern const R ov49_02269774[];extern const u16 ov49_02269704[],ov49_022696FC[];extern const V ov49_0226991C[];
extern void*NARC_New(int,int),*AddPlttResObjFromOpenNarc(void*,void*,int,int,int,int,int,int),*AddCellOrAnimResObjFromOpenNarc(void*,void*,int,int,int,int,int),*AddCharResObjFromOpenNarc(void*,void*,int,int,int,int,int),*Sprite_Create(T*);extern I*ov49_0225C828(int,int,int,int);
extern int ov45_0222A99C(void*),ov45_0222A9CC(void*),SpriteTransfer_CreatePlttTransferTask(void*),SpriteTransfer_CreateCharTransferTask_AllocAtEnd(void*),ov45_0222A92C(void*,int),ov45_0222A964(void*,int);
extern void sub_0200A740(void*),CreateSpriteResourcesHeader(H*,int,int,int,int,int,int,int,int,void*,void*,void*,void*,void*,void*),Sprite_SetPalOffsetRespectVramOffset(void*,int),NARC_Delete(void*);
void ov49_0225C4CC(O*,B*,void*,int,void*,void*);
void ov49_0225C4CC(O*out,B*bg,void*input,int heap,void*net,void*other){void*narc=NARC_New(215,heap);int version;const V*pos;int ver=ov45_0222A99C(other);const R*r;const u16*cell,*anim;void*use;int i;O*cursor;T t;H h;
version=ov45_0222A9CC(other);r=ov49_02269774;i=0;cell=ov49_02269704;cursor=out;anim=ov49_022696FC;
for(;i<4;i++){if(i==2){if(version==1){cursor->palette[0]=0;cursor->cell[0]=0;cursor->anim[0]=0;goto next;}}else if(i==3){if(version==0){cursor->palette[0]=0;cursor->cell[0]=0;cursor->anim[0]=0;goto next;}}
use=r->narc==215?narc:input;cursor->palette[0]=AddPlttResObjFromOpenNarc(bg->palette,use,r->member,0,150+i,2,r->count,heap);if(!SpriteTransfer_CreatePlttTransferTask(cursor->palette[0]))GF_AssertFail();sub_0200A740(cursor->palette[0]);cursor->cell[0]=AddCellOrAnimResObjFromOpenNarc(bg->cell,use,*cell,0,150+i,2,heap);cursor->anim[0]=AddCellOrAnimResObjFromOpenNarc(bg->anim,use,*anim,0,150+i,3,heap);
next:r++;cursor=(O*)((u8*)cursor+4);cell++;anim++;}
pos=ov49_0226991C;for(i=0;i<12;i++){int species=ov45_0222A92C(net,i);int form=ov45_0222A964(net,i);if(species==24){out->chars[0]=0;out->sprite[0]=0;}else{I*info=ov49_0225C828(species,form,ver,version);out->chars[0]=AddCharResObjFromOpenNarc(bg->chars,info->source==1?input:narc,info->member,0,150+i,2,heap);if(!SpriteTransfer_CreateCharTransferTask_AllocAtEnd(out->chars[0]))GF_AssertFail();sub_0200A740(out->chars[0]);CreateSpriteResourcesHeader(&h,150+i,150+info->source,150+info->source,150+info->source,-1,-1,0,0,bg->chars,bg->palette,bg->cell,bg->anim,0,0);t.list=bg->list;t.header=&h;t.priority=0;t.vram=2;t.heap=heap;t.pos=*pos;t.pos.x+=(int)(info->x>0?0.5f+(float)(info->x<<12):(float)(info->x<<12)-0.5f);t.pos.y+=(int)(info->y>0?0.5f+(float)(info->y<<12):(float)(info->y<<12)-0.5f);out->sprite[0]=Sprite_Create(&t);Sprite_SetPalOffsetRespectVramOffset(out->sprite[0],info->palette);}pos++;out=(O*)((u8*)out+4);}
NARC_Delete(narc);}
