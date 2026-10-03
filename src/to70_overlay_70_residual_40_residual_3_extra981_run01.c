#pragma force_active on
#include "global.h"
typedef struct {int a,count;} Entry;extern int ov70_02245E84[];extern Entry ov70_02245F58[],ov70_02245F5C[];extern int ov70_02243F54(void*,int);

int ov70_02243F7C(void*w,int index);

int ov70_02243F7C(void*w,int index){int first=ov70_02245E84[index];int count=ov70_02245F5C[index].a;if(count>0){int i;for(i=0;i<count;i++){if(ov70_02243F54(w,first+i)>0)return 1;count=ov70_02245F58[index].count;}}else if(ov70_02243F54(w,first)>0)return 1;return 0;}
