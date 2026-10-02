#pragma force_active on
#include "global.h"
extern void *NewString_ReadMsgData(void *,int);
extern void ov70_02245084(void *,void *,int,int,int,int),String_Delete(void *);
void ov70_0223F244(void *,void *,void *,int,int,int,int);
void ov70_0223F244(void *window,void *msg,void *fallback,int id,int a,int b,int c){void *s;if(id){s=NewString_ReadMsgData(msg,id);ov70_02245084(window,s,0,b,a,c);String_Delete(s);}else{s=NewString_ReadMsgData(fallback,173);ov70_02245084(window,s,0,b,a,c);String_Delete(s);}}
