#pragma force_active on
#include "global.h"
typedef struct{u8 p[0x3f4];void *msg,*fmt,*text;} Work;
extern void BufferIntegerAsString(void *,int,int,int,int,int),StringExpandPlaceholders(void *,void *,void *),String_Delete(void *),ov39_02227D50(Work *,void *);
extern void *NewString_ReadMsgData(void *,int);
void ov39_02227D5C(Work *,int,int);
void ov39_02227D5C(Work *w,int id,int number){void *s;if(id==-1)id=11;BufferIntegerAsString(w->fmt,0,number,5,2,1);s=NewString_ReadMsgData(w->msg,id);StringExpandPlaceholders(w->fmt,w->text,s);String_Delete(s);ov39_02227D50(w,w->text);}
