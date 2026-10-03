#pragma force_active on
#include "global.h"
#pragma force_active on
typedef union{u32 word;struct{u32 x:8,y:8,active:1,state:2,pad:13;}bits;} CellFlags;
typedef union{u32 word;struct{u32 pad:3,count:16,rest:12,done:1;}bits;} Control;
typedef struct{CellFlags flags;u32 pad[7];Control control;} GameCell;
typedef union{u32 word;struct{u32 count:14,first:1,second:1,old:13,top:3;}bits;} EntityFlags;
typedef union{u32 word;struct{u32 pad:24,state:2,active:1,tail:5;}bits;} DrawFlags;
typedef struct{u8 data[0x60];DrawFlags flags;} Draw;
typedef struct{ s16 x,y,oldx,oldy;EntityFlags flags;Draw draw;u8 rest[0xa8-12-sizeof(Draw)];} Entity;
extern u8*PokeathlonCourse_GetHeapAllocPtr4(void*),*PokeathlonCourse_GetDataCopyArea(void*);extern GameCell*ov96_021E8A20(void*);extern int ov96_021E5F24(void*),ov96_022158D4(int,int);
extern void ov96_02218330(void*,void*),ov96_02219460(Draw*,EntityFlags*,int),ov96_02219398(Draw*),ov96_02218A68(Draw*),ov96_02218578(void*,int),ov96_02219FE4(void*),ov96_022180CC(void*,void*),ov96_0221768C(void*,void*);
void ov96_02215FC8(void*);
#pragma opt_lifetimes off
void ov96_02215FC8(void*course){int i;u8*heap;GameCell*control;u8*data;heap=PokeathlonCourse_GetHeapAllocPtr4(course);data=PokeathlonCourse_GetDataCopyArea(course);control=ov96_021E8A20(data+40);Entity*entity;if(ov96_021E5F24(course)==0){if(control->control.bits.done){ov96_02218330(heap+0x188,course);return;}if(*(int*)(heap+0x438)>0){(*(int*)(heap+0x438))--;control->control.bits.count=*(int*)(heap+0x438);}*ov96_021E8A20(data+80)=*ov96_021E8A20(data);i=0;data+=80;entity=(Entity*)(heap+0x198);for(;i<4;entity++,data+=40,i++){GameCell*cell=ov96_021E8A20(data);EntityFlags*flags=&entity->flags;Draw*draw=&entity->draw;if(cell->flags.bits.active){if(flags->bits.first&&flags->bits.second)flags->bits.first=0;else if(!flags->bits.first&&!flags->bits.second){flags->bits.first=1;flags->bits.second=1;entity->oldx=cell->flags.bits.x;entity->oldy=cell->flags.bits.y;}entity->x=cell->flags.bits.x;entity->y=cell->flags.bits.y;flags->bits.count++;}else if(flags->bits.second){flags->bits.old=flags->bits.count;flags->bits.count=0;flags->bits.first=0;flags->bits.second=0;}ov96_02219460(draw,flags,ov96_022158D4(entity->x,entity->y));if(draw->flags.bits.active){if(draw->flags.bits.state==1&&cell->flags.bits.state==1){ov96_02219398(draw);ov96_02218A68(draw);draw->flags.bits.state=2;}else if(draw->flags.bits.state==2&&cell->flags.bits.state==2){draw->flags.bits.active=0;draw->flags.bits.state=0;ov96_02218578(draw,4);}}}ov96_02219FE4(*(void**)(heap+0x184));ov96_022180CC(heap+0x198,course);if(control->control.bits.count==0)control->control.bits.done=1;ov96_02218330(heap+0x188,course);}ov96_0221768C(heap+0x43c,course);}
