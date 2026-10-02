#pragma force_active on
#include "global.h"
typedef struct{void*unused;u16*cursor;int state;}Work;
extern void CopyU16ArrayToStringN(void*,const u16*,int);extern u16*MsgArray_SkipControlCode(u16*);
int ov102_021EAC7C(Work*,void*);
int ov102_021EAC7C(Work*w,void*out){u16*start=w->cursor;int result;switch(w->state){case 0:while(w->state==0){switch(*w->cursor){case 0xe000:w->state=2;break;case 0xffff:w->state=3;break;case 0xfffe:w->state=1;break;default:w->cursor++;break;}}CopyU16ArrayToStringN(out,start,w->cursor-start+1);return 0;case 1:w->cursor=MsgArray_SkipControlCode(start);result=1;break;case 2:w->cursor=start+1;result=2;break;case 3:default:return 3;}switch(*w->cursor){case 0xe000:w->state=2;break;case 0xffff:w->state=3;break;case 0xfffe:w->state=1;break;default:w->state=0;break;}return result;}
