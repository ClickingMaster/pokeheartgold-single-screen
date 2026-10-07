#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u32 heap;u8 mode;u8 p[7];int count;u8 q[0x6f5-16];u8 kind;u8 r[14];u8 records[4][9];}Work;
int sub_02030BD0(u8,void*),ov80_022379C8(Work*);u16 ov80_02231A14(Work*);
static inline int Fixed(int value){float result;if(value>0)result=0.5f+(float)(int)((u32)value<<12);else result=(float)(int)((u32)value<<12)-0.5f;return(int)result;}
u16 ov80_02231A14(Work*w){int value=sub_02030BD0(w->kind,w->records[w->mode]);int remaining;int limit=ov80_022379C8(w);float divisor,score;int i;float total;int result;if(w->mode==2)return(u16)limit;{float fixed;int count;if(limit>0)fixed=0.5f+(float)(int)((u32)limit<<12);else fixed=(float)(int)((u32)limit<<12)-0.5f;count=w->count;remaining=(int)fixed-count*3;divisor=(float)((double)((float)count/4096.0f)*5.0);}if((double)((float)limit/divisor)<1.0)score=(float)value;else score=((float)Fixed(value*limit)/4096.0f)/divisor;total=0.0f;for(i=0;i<18;i++){if(i==w->kind)total+=1.0;else if(sub_02030BD0((u8)i,w->records[w->mode])!=0)total+=1.0;}if((double)total!=0.0)total-=1.0;total*=0.5;total=total+((float)remaining/4096.0f+score);result=(int)total;if(total!=(float)(int)total)result++;if(result>limit)result=limit;if(result>100)result=100;return(u16)result;}
