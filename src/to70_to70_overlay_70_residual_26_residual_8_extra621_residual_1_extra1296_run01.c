#pragma force_active on
#include "global.h"
typedef struct {u16 a,b;s16 x,y;} Entry;extern Entry ov70_02245A4C[],ov70_022459F4[];int ov70_0223F864(int,int,int);
int ov70_0223F864(int x,int y,int kind){Entry*table;int count;if(kind==0){table=ov70_02245A4C;count=12;}else{table=ov70_022459F4;count=11;}for(kind=0;kind<count;kind++){if(x==table[kind].x&&y==table[kind].y)return kind;}return 0;}
