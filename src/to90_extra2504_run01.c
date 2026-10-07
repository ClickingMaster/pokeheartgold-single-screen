#pragma force_active on
#include "global.h"
extern void ov96_021F74D0(void *,void *,int),ov96_021EB564(void *,int),ov96_021EB52C(void *,int,int),ov96_021EB630(void *,int);
extern void *ov96_021EB3E4(void *,int,int,int,int),*ov96_021EB5E8(void *),*ov96_021EA634(void *,void *,int,void *);
extern void Sprite_SetDrawFlag(void *,int),Sprite_SetMatrix(void *,const VecFx32 *),Sprite_SetAnimCtrlSeq(void *,int);
extern const u8 ov96_0221C0B0[],ov96_0221C0B4[];
void ov96_021F6C5C(void *,void *);
typedef struct { u8 unused0[0x54]; void *renderer; u8 unused1[12]; void *main, *group1[4], *group3[3], *icons[2], *group; u8 unused2[4]; void *details; u8 unused3[0xf0]; void *spriteList; } Work;
typedef struct { u8 unused[0x94]; void *sprite; u8 tail[0x34]; } DetailView;
#pragma force_active on
void ov96_021F6C5C(void *argument,void *container){
 Work *work=argument,*cursor;int i;int j;VecFx32 pos;const u8 *x,*anim;
 ov96_021F74D0(work->group,container,2);
 work->main=ov96_021EB3E4(container,1,3,101,3);ov96_021EB564(work->main,12);ov96_021EB52C(work->main,1,1);
 for(i=0,cursor=work;i<4;cursor=(Work *)((u8 *)cursor+4),i++){cursor->group1[0]=ov96_021EB3E4(container,1,3,101,4);ov96_021EB564(cursor->group1[0],13+i);ov96_021EB52C(cursor->group1[0],1,1);}
 for(i=0,cursor=work;i<4;cursor=(Work *)((u8 *)cursor+0x38),i++){((DetailView *)cursor)->sprite=ov96_021EB3E4(container,1,3,101,7);ov96_021EB564(((DetailView *)cursor)->sprite,11);ov96_021EB630(((DetailView *)cursor)->sprite,9);}
 for(i=0,cursor=work;i<4;cursor=(Work *)((u8 *)cursor+4),i++){cursor->group3[0]=ov96_021EB3E4(container,1,3,101,8);ov96_021EB564(cursor->group3[0],9);ov96_021EB630(cursor->group3[0],6);}
 for(j=0,cursor=work,x=ov96_0221C0B0,anim=ov96_0221C0B4;j<2;cursor=(Work *)((u8 *)cursor+4),x++,anim++,j++){
 void *value=ov96_021EB5E8(container);cursor->icons[0]=ov96_021EA634(work->spriteList,value,1,work->renderer);Sprite_SetDrawFlag(cursor->icons[0],1);pos.x=*x*4096;pos.y=0x8000;pos.z=0;Sprite_SetMatrix(cursor->icons[0],&pos);Sprite_SetAnimCtrlSeq(cursor->icons[0],*anim);
 }
}
