#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{u8 pad[31];u8 failed;};void*Party_GetMonByIndex(void*,int);u32 GetMonData(void*,int,void*);void SetMonData(void*,int,const void*);void ov80_02234F28(struct S*,void*,int);void ov80_02234FAC(struct S*,void*,int);void ov80_02235024(struct S*,void*,int);
void ov80_02235024(struct S*s,void*party,int count){struct{volatile int n;int b,status;}frame;int immune=0,i;frame.n=count;for(i=0;i<frame.n;i++){void*m=Party_GetMonByIndex(party,i);int a=GetMonData(m,177,0);frame.b=GetMonData(m,178,0);int ability=GetMonData(m,10,0);if(a==10 || frame.b==10 || ability==41)immune++;else{frame.status=16;SetMonData(m,160,&frame.status);}}if(immune>=frame.n)s->failed=1;}
