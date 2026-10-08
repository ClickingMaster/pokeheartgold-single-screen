#pragma force_active on
#include "global.h"
#include <string.h>
int ov18_021F8970(int);
void *ov18_021F8168(int,int*);
void ov18_021F8764(void*,int*,void*,int,void*,int,int,int),Heap_Free(void*);
void ov18_021F831C(int,void*,int*,void*,int,int);
#pragma opt_propagation off
// @fn ov18_021F831C
void ov18_021F831C(int arg,void*dst,int*out,void*src,int count,int flags){void*data;int size;int n;
 switch(ov18_021F8970(arg)){
 case 17:n=count;memcpy(dst,src,n*2);*out=n;return;
case 0:data=ov18_021F8168(51,&size);break;
case 1:data=ov18_021F8168(52,&size);break;
case 2:data=ov18_021F8168(53,&size);break;
case 3:data=ov18_021F8168(54,&size);break;
case 4:data=ov18_021F8168(55,&size);break;
case 5:data=ov18_021F8168(56,&size);break;
case 6:data=ov18_021F8168(57,&size);break;
case 7:data=ov18_021F8168(58,&size);break;
case 8:data=ov18_021F8168(59,&size);break;
case 9:data=ov18_021F8168(60,&size);break;
case 10:data=ov18_021F8168(61,&size);break;
case 11:data=ov18_021F8168(62,&size);break;
case 12:data=ov18_021F8168(63,&size);break;
case 13:data=ov18_021F8168(64,&size);break;
case 14:data=ov18_021F8168(65,&size);break;
case 15:data=ov18_021F8168(66,&size);break;
case 16:data=ov18_021F8168(67,&size);break;
default:GF_AssertFail();break;
}ov18_021F8764(dst,out,data,size,src,count,0,flags);Heap_Free(data);
}
#pragma opt_propagation reset
