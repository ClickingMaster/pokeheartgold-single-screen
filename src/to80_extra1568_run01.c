#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{int state,pad[2],timer,pad2[8],count;}S;extern void ov85_021E7400(S*,int);
int ov85_021E5F9C(S*s);
int ov85_021E5F9C(S*s){int i;if(++s->timer<95)return 0;for(i=0;i<s->count;i++)ov85_021E7400(s,i);s->timer=0;s->state=26;return 0;}
