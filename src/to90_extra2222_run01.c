#pragma force_active on
#include "global.h"
#pragma force_active on
struct S{u8 pad[31];u8 failed;};void*Party_GetMonByIndex(void*,int);u32 GetMonData(void*,int,void*);void SetMonData(void*,int,const void*);void ov80_02234F28(struct S*,void*,int);void ov80_02234FAC(struct S*,void*,int);void ov80_02235024(struct S*,void*,int);
typedef struct {struct S*s;void*party;volatile int n;int immune,status;} Frame;
void ov80_02234F28(struct S*s,void*party,int count){Frame f;int i;f.n=count;f.s=s;f.party=party;f.immune=0;for(i=0;i<f.n;i++){void*m=Party_GetMonByIndex(f.party,i);int a=GetMonData(m,177,0);int b=GetMonData(m,178,0);int ability=GetMonData(m,10,0);if(a==3 || b==3 || a==8 || b==8 || ability==17)f.immune++;else{f.status=8;SetMonData(m,160,&f.status);}}if(f.immune>=f.n)f.s->failed=1;}
