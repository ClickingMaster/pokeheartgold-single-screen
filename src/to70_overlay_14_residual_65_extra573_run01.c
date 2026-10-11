#pragma force_active on
#include "global.h"
typedef struct {int a,b,c;}Entry;
extern Entry sPCBoxGridLayouts[];
typedef struct{u8 pad[8];u32 kind;}Input;
typedef struct{u8 pad[0x2c];void*grid;}Grid;
typedef struct{Input*input;u8 pad[0x28];int value;int unused;Grid*holder;}Work;
extern void PCBox_FreeGridInput(Work*),PCBox_PlaceCursorOnGridInput(Work*,int);extern void*GridInputHandler_Create(int,int,int,Work*,int,u8,int);
void PCBox_CreateModeGridInput(Work*);void PCBox_CreateGridInput(Work*,int,int);
void PCBox_CreateModeGridInput(Work*w){int i;switch(w->input->kind){case 0:i=0;break;case 1:i=2;break;case 2:i=3;break;case 3:i=6;break;}PCBox_FreeGridInput(w);w->holder->grid=GridInputHandler_Create(sPCBoxGridLayouts[i].a,((Entry*)((u8*)sPCBoxGridLayouts+4))[i].a,((Entry*)((u8*)sPCBoxGridLayouts+8))[i].a,w,1,(u8)w->value,10);PCBox_PlaceCursorOnGridInput(w,w->value);}
void PCBox_CreateGridInput(Work*w,int i,int value){PCBox_FreeGridInput(w);w->holder->grid=GridInputHandler_Create(sPCBoxGridLayouts[i].a,((Entry*)((u8*)sPCBoxGridLayouts+4))[i].a,((Entry*)((u8*)sPCBoxGridLayouts+8))[i].a,w,1,(u8)value,10);PCBox_PlaceCursorOnGridInput(w,value);}
