#pragma force_active on
#include "global.h"
extern void ov96_021EABA8(void*,int);typedef struct{void* p;u8 pad[120];}Obj;typedef struct{u8*p;Obj*o;}Pair;
void ov96_022144C0(Obj*obj,u8*values);
void ov96_022144C0(Obj*obj,u8*values){Pair a[3];Pair*order[3];Pair*high,*low;u8 i;for(i=0;i<3;i++){a[i].p=values+i;a[i].o=obj+i;}low=a;if(*a[0].p>=*a[1].p){high=low;low=&a[1];}else high=&a[1];if(*high->p>=*a[2].p){if(*low->p>=*a[2].p){order[0]=high;order[1]=low;order[2]=&a[2];}else{order[0]=high;order[1]=&a[2];order[2]=low;}}else{order[0]=&a[2];order[1]=high;order[2]=low;}for(i=0;i<3;i++)ov96_021EABA8(order[i]->o->p,i+8);}
