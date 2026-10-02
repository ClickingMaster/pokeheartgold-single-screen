#pragma force_active on
#include "global.h"
typedef struct{u32 a;int id;u8 p[20];}Entry;typedef struct{Entry rows[25];u8 pad[0x2d0-25*28];int count;}Work;
int sub_02086398(Work*,int);int sub_020863C0(Work*,int);
int sub_02086398(Work*w,int id){int i=0,count=w->count;for(;i<count;i++){if(w->rows[i].id==id)return i;}return 0;}
