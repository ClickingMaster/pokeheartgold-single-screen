#pragma force_active on
#include "global.h"
#pragma force_active on
extern void ov96_021EB564(void *,int),ov96_021EB52C(void *,int,int),ov96_02214258(void *,int,int,int);
extern int PlaySE(int);
void ov96_022123B0(void *,int,int,int,u8,u8);
#define PTR(p,o) (*(void **)((u8 *)(p)+(o)))
void ov96_022123B0(void *p,int idx,int flag,int unused,u8 code,u8 selected) {
 u8 c,group,remainder;int anim,offset;
 u8 *item;
 if(code<0xd8)return;
 item=(u8 *)p+0x62c+idx*0x4c;
 if(item[0x3f]!=0)return;
 item[0x3f]=1;c=code-0xd8;GF_ASSERT(c<16);
 group=c/4;offset=group*4;
 ov96_021EB564(PTR((u8 *)p+offset,0x6c4),15);
 remainder=c%4;
 if(remainder!=group){int yes=0;if(flag)yes=1;ov96_02214258(p,remainder,1,yes);ov96_02214258(p,group,0,0);}else{ov96_02214258(p,group,0,0);}
 if(c%5==0 && selected==c/5){PlaySE(0x8c6);return;}
 if(selected==remainder){
 switch(group){case 0:case 2:anim=17;break;case 1:case 3:anim=18;break;default:GF_AssertFail();return;}
 ov96_021EB52C(PTR((u8 *)p+0x6d4,offset),1,1);
 ov96_021EB564(PTR((u8 *)p+0x6d4,offset),anim);
 ov96_021EB52C(PTR((u8 *)p+0x6e4,offset),1,1);
 ov96_021EB564(PTR((u8 *)p+0x6e4,offset),group+19);
 PlaySE(0x8cb);
 }else PlaySE(0x8c6);
}
