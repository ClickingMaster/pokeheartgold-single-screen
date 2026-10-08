#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x30];s32 x,y;} Position;
typedef struct {u32 unknown;Position *pos;} Item;
typedef struct {Item item[3];u32 pad[3];} Group;
extern int ov96_02215614(int,int,int,int);extern u16 LCRandom(void);extern void GF_AssertFail(void);
int ov96_02215478(u8 *,Group *,u8);
// @fn ov96_02215478
int ov96_02215478(u8 *out,Group *groups,u8 excluded) {
 u8 counts[4]={0,0,0,0};u8 i,j;int chosen,x,y;

 for(i=0;i<4;i++)for(j=0;j<3;j++) {
  Position *p=groups[i].item[j].pos;
  int k=ov96_02215614(128,96,p->x/4096,p->y/4096);
  if(k==4)goto next_count;counts[k]++;next_count:;
 }
 chosen=0;for(i=1;i<4;i++)if(counts[i-1]>counts[i] && i!=excluded)chosen=i;
 x=(int)LCRandom()%64;y=(int)LCRandom()%32;
 switch(chosen){case 0:*(s32*)(out+0x24)=(128-x)<<12;*(s32*)(out+0x28)=(96-y)<<12;break;
 case 1:*(s32*)(out+0x24)=(128+x)<<12;*(s32*)(out+0x28)=(96-y)<<12;break;
 case 2:*(s32*)(out+0x24)=(128-x)<<12;*(s32*)(out+0x28)=(96+y)<<12;break;
 case 3:*(s32*)(out+0x24)=(128+x)<<12;*(s32*)(out+0x28)=(96+y)<<12;break;
 default:GF_AssertFail();break;}
 return 10;
}
