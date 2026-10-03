#pragma force_active on
#include "global.h"
typedef struct{u8 pad[0x64];s32 frame;int progress[3],target[3];void*unused;void*bitmap;}Work;
extern void BlitBitmapRectToWindow(void*,void*,int,int,int,int,u16,int,int,int),ScheduleWindowCopyToVram(void*);
int ov47_02259B74(Work*);
int ov47_02259B74(Work*w){int limit,i,finished,len,start,j,target;Work*cursor;if(w->frame>=20)return 1;w->frame++;limit=80*w->frame/20;finished=0;i=0;cursor=w;for(;i<3;cursor=(Work*)((u8*)cursor+4),i++){target=cursor->target[0];start=cursor->progress[0];if(start==target){finished++;continue;}if(target>=limit){len=limit-start;cursor->progress[0]=limit;}else{len=target-start;cursor->progress[0]=target;}for(j=0;j<len;j++)BlitBitmapRectToWindow((u8*)w+(i+2)*16,*(void**)((u8*)w->bitmap+0x14),247,16,256,32,start+4+j,0,1,16);ScheduleWindowCopyToVram((u8*)w+(i+2)*16);}if(finished==3)return 1;return 0;}
