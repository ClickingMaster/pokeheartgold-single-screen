#pragma force_active on
#include "global.h"
extern int ov70_0223E490(int);extern int Party_GetCount(void*);extern void *Party_GetMonByIndex(void*,int);extern void *Mon_GetBoxMon(void*);extern void *PCStorage_GetMonByIndexPair(void*,int,int);
void *ov70_0223E49C(void*,void*,int,int);
void *ov70_0223E49C(void*party,void*pc,int box,int index) {if(ov70_0223E490(box)){if(index>Party_GetCount(party)-1)return 0;return Mon_GetBoxMon(Party_GetMonByIndex(party,index));}return PCStorage_GetMonByIndexPair(pc,box,index);}
