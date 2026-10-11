#pragma force_active on
#include "global.h"
#pragma force_active on
typedef union {u32 value;struct {u32 low:24, high:6,index:2;} b;} Index;
typedef union {u32 value;struct {u32 low:7,active:12,timer:12,top:1;} b;} Status;
typedef union {u32 value;struct {u32 low:12;s32 coord:16;u32 high:4;} b;} Coordinate;
typedef union {u32 value;struct {u32 low:8,timer:8,mid:4,number:4,high:8;} b;} Flags;
typedef struct {int unk0,kind,unk8,state;u8 pad[12];Coordinate x,y;u8 pad2[28];Flags flags;u8 pad3[4];} Row;
typedef struct {u32 unk;u8 y[12],x[12];Index index;Status status;} Data;
extern void *PokeathlonCourse_GetDataCopyArea(void*);
extern u8 *ov96_021E8A20(void *ptr);
extern void PokeathlonCourseObject_CenterToGround(int,int,int,int*,int*);
void ov96_0220FA18(void*,void*);
void ov96_0220FA18(void *work,void *course) {
 int y,x; Index *index; Data *data;int i,shift;Status *status;
 data=(Data *)ov96_021E8A20((u8*)PokeathlonCourse_GetDataCopyArea(course)+0x28);
 index=&data->index;
 data->index.b.index++;
 data->status.b.low=((u16)(*(u32*)((u8*)work+data->index.b.index*228+0x170)>>2))&127;
 data->status.b.timer=0;
 data->index.b.low=0;
 data->status.b.active=0;
 status=&data->status;
 work=(u8*)work+0x98;
 for(i=0,shift=0;i<12;i++,shift+=2) {
  int value;Row *row=(Row*)((u8*)work+(i/3)*228+(i%3)*72);
  PokeathlonCourseObject_CenterToGround(row->kind,row->x.b.coord,row->y.b.coord,&y,&x);
  value=y;if(value>255)value=255;else if(value<0)value=0;
  data->y[i]=value;
  value=x;if(value>255)value=255;else if(value<0)value=0;
  data->x[i]=value;
  if(row->flags.b.timer) {status->b.timer|=1<<i;row->flags.b.timer--;}
  if(row->state==3)status->b.active|=1<<i;
  index->b.low+=(row->flags.b.number-1)<<shift;
 }
}
