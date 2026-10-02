#pragma force_active on
#include "global.h"
typedef struct{u8 pad[2];u8 choice;u8 pad2[5];void*menu,*save;}Args;
typedef struct{int heap,unused[2],input;Args*args;u8 pad[5];u8 group,pages,col,row;u8 pad2[5];u8 delay,frame;u8 pad3[0xa0];u8 count,total;u16 pad4;int loaded;u8 pad5[0x120];void*album;}Work;
extern u8 ov109_021E7890[];
extern int MenuInputStateMgr_GetState(void*),Options_GetTextFrameDelay(void*),Options_GetFrame(void*),PhotoAlbum_LoadAllInUsePhotos(void*,int),PhotoAlbum_GetNumSaved(void*);extern void*Save_PlayerData_GetOptionsAddr(void*),*Save_PhotoAlbum_Get(void*);extern void ov109_021E5D08(Work*);
void ov109_021E5A70(Work*);
void ov109_021E5A70(Work*w){void*options;int choice,value;w->input=MenuInputStateMgr_GetState(w->args->menu);options=Save_PlayerData_GetOptionsAddr(w->args->save);w->delay=Options_GetTextFrameDelay(options);w->frame=Options_GetFrame(options);choice=w->args->choice;value=ov109_021E7890[choice%12];w->group=choice/12;w->col=value%4;w->row=(u32)value>>2;w->album=Save_PhotoAlbum_Get(w->args->save);w->loaded=PhotoAlbum_LoadAllInUsePhotos(w->album,w->heap);w->count=PhotoAlbum_GetNumSaved(w->album);w->total=w->count;w->pages=w->total/12;if(w->total%12!=0)w->pages++;if(w->total==0)w->pages=1;ov109_021E5D08(w);}
