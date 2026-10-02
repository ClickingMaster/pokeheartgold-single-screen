#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {int msg;s16 x,y;} Entry;
extern Entry ov70_02245A4C[],ov70_022459F4[];
extern void *ListMenuItems_New(int,int);
extern void ListMenuItems_AppendFromMsgData(void*,void*,int,int);

int ov70_0223F7E4(void **out,void *msg,int mode);
void ov70_0223F828(u8 *out,int index,int mode);
int ov70_0223F864(int x,int y,int mode);

int ov70_0223F7E4(void **out,void *msg,int mode){int i;Entry *table,*p;int count;if(mode==0){table=ov70_02245A4C;count=12;}else{table=ov70_022459F4;count=11;}*out=ListMenuItems_New(count,61);i=0;p=table;if(i<count)do{ListMenuItems_AppendFromMsgData(*out,msg,p->msg,i);i++;p=(Entry*)((u8*)p+8);}while(i<count);return count;}
void ov70_0223F828(u8 *out,int index,int mode){Entry *p;if(mode==0){p=ov70_02245A4C;if(index>=12)GF_AssertFail();}else{p=ov70_022459F4;if(index>=11)GF_AssertFail();}out[3]=p[index].x;out[4]=p[index].y;}
