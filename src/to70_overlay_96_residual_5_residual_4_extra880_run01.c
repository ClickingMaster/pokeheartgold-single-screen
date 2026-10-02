#pragma force_active on
#include "global.h"
extern void PokeathlonCourse_SetStateField07(void*,int),PokeathlonCourse_SetField1F4(void*,int);
typedef int(*Callback)(void*,u8*);
typedef struct{u8 p[0x3b4];void*active;u8 q[8];int a;Callback*table1;int other;Callback*table2;u8 b,state,index;}Work;
int ov96_021E6C40(Work*);
int ov96_021E6C40(Work*w){if(w->table2==0){PokeathlonCourse_SetStateField07(w,14);return 0;}if(w->active==&w->table1){w->active=&w->table2;w->a=0;}if(w->table2[w->index](w,&w->state)){PokeathlonCourse_SetField1F4(w,0);w->active=&w->table1;w->a=0;PokeathlonCourse_SetStateField07(w,14);}return 0;}
