#pragma force_active on
#include "global.h"
typedef struct {int a,b,c;}Entry;
extern Entry ov14_021F8B10[];
typedef struct{u8 pad[8];u32 kind;}Input;
typedef struct{u8 pad[0x2c];void*grid;}Grid;
typedef struct{Input*input;u8 pad[0x28];int value;int unused;Grid*holder;}Work;
extern void ov14_021F6B10(Work*),ov14_021F6B28(Work*,int);extern void*GridInputHandler_Create(int,int,int,Work*,int,u8,int);
void ov14_021F6A44(Work*);void ov14_021F6AC0(Work*,int,int);
void ov14_021F6A44(Work*w){int i;switch(w->input->kind){case 0:i=0;break;case 1:i=2;break;case 2:i=3;break;case 3:i=6;break;}ov14_021F6B10(w);w->holder->grid=GridInputHandler_Create(ov14_021F8B10[i].a,((Entry*)((u8*)ov14_021F8B10+4))[i].a,((Entry*)((u8*)ov14_021F8B10+8))[i].a,w,1,(u8)w->value,10);ov14_021F6B28(w,w->value);}
void ov14_021F6AC0(Work*w,int i,int value){ov14_021F6B10(w);w->holder->grid=GridInputHandler_Create(ov14_021F8B10[i].a,((Entry*)((u8*)ov14_021F8B10+4))[i].a,((Entry*)((u8*)ov14_021F8B10+8))[i].a,w,1,(u8)value,10);ov14_021F6B28(w,value);}
