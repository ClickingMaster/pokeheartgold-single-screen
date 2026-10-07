#pragma force_active on
#include "global.h"
#include "bg_window.h"
#include "msgdata.h"
typedef struct {u32 x,y;} Point;
typedef union {Point point;int values[2];} Position;
typedef struct {s16 x,y;u8 middle[36];u16 flag,country,region,tag;} Row;
typedef struct {enum HeapID heap;u8 first[8];u32 count;Row row[1024];BgConfig *bg;Window window;u8 gap[0xc070-0xc014-sizeof(Window)];MsgData *messages;u8 gap2[0xc2c4-0xc074];Point pos;u8 gap3[0xc338-0xc2cc];s32 active;} W;
extern void ov69_021E75A0(int values[2]);
extern int ov69_021E75F8(const Point *,const Point *);
int ov69_021E6C30(void *,int *);
#pragma force_active on
int ov69_021E6C30(void *argument,int*out){W*w=argument;int result=0;int px=w->pos.x,py=w->pos.y;int xmin=(s16)((s32)((u32)px-128)),xmax=(s16)((s32)((u32)px+128)),ymin=(s16)((s32)((u32)py-128)),ymax=(s16)((s32)((u32)py+128)),min2,max2;Position orig,pos;u32 best=256,index=w->count,i;orig.values[0]=(int)w->pos.x;orig.values[1]=(int)w->pos.y;ov69_021E75A0(orig.values);{int diff=ymax-ymin;if(diff<0)diff=-diff;if(diff>256){max2=min2=(s16)w->pos.y;if(min2>0)ymax=ymin=min2-65536;else ymax=ymin=min2+65536;min2-=128;max2+=128;ymin-=128;ymax+=128;}else{min2=ymin;max2=ymax;}}for(i=0;i<w->count;i++){if(w->row[i].x>xmin&&w->row[i].x<xmax&&((w->row[i].y>ymin&&w->row[i].y<ymax)||(w->row[i].y>min2&&w->row[i].y<max2))&&w->row[i].flag){u32 distance;pos.point.x=(u32)w->row[i].x;pos.point.y=(u32)w->row[i].y;ov69_021E75A0(pos.values);distance=(u32)ov69_021E75F8(&orig.point,&pos.point);if(distance<best){best=distance;index=i;}}}*out=(int)index;if(index!=w->count)result=1;return result;}
