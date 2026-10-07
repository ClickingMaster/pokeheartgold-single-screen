#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct{int pad[20];int ids[9][3];}S;
int ov45_0222C5B4(void*parent,int key);
int ov45_0222C5B4(void*parent,int key){S*s=parent;int i,j;for(i=0;i<9;i++){for(j=0;j<3;j++){if(key==s->ids[0][j])return i*3+j;}s=(S*)((u8*)s+12);}return -1;}
