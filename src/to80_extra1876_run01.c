#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{u8 p[0x2c];u16 values[30];u8 gap[0x148];int index;} Work;
typedef struct {u8 prefix[0x2c];u16 values[30];} Prefix;
void ov40_022361E0(Work *);
void ov40_022361E0(Work *w){int i;Prefix *first;int start=w->index;i=start;if(start<30){first=(Prefix*)((u8*)w+2*start);do{if(first->values[0]){w->index=i;return;}i++;first=(Prefix*)((u8*)first+2);}while(i<30);} {Prefix *second;int j=0;if(start>0){second=(Prefix*)w;do{if(second->values[0]){w->index=j;return;}j++;second=(Prefix*)((u8*)second+2);}while(j<start);}}}
