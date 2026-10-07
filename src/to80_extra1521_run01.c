#include "global.h"
#include "unk_0202C034.h"
#pragma force_active on
extern BOOL sub_0202C2DC(UnkStruct_021D2230*,int);
int ov43_0222B574(int*,void*);
// @fn ov43_0222B574
int ov43_0222B574(int *selection,void *shared){int i;UnkStruct_021D2230 *roster;if(*selection==1){void *save=*(void **)((u8*)shared+4);roster=sub_0202C6F4(save);for(i=0;i<32;i++){if(sub_0202C2DC(roster,i)==0)return 1;}}else return 1;return 0;}
