#pragma force_active on
#include "global.h"
#include "bg_window.h"
#pragma force_active on
#pragma opt_repositioncode off
typedef struct {u8 active;s8 state;u8 pad[2];} RowState;
typedef struct {u16 frame,kind;u32 pad;void *buffers[2],*screens[2];u16 enabled,index;void *palette,*paletteData;u8 flags,pad1,slot,pad2;} Resource;
typedef struct {u16 first,second;s16 x0,y0,x1,y1;} Limits;
typedef struct {
 u8 prefix[0x3c];u16 ticks,count;RowState entries[18];u32 indices[3];
 void *screenData,*tileSource;u32 pad9c;Resource resources[5];
 Window windows[3],blank,frame;u8 animation[0x12c];Limits output;u8 tail[0x28];
} Work;
typedef char WorkSize[(sizeof(Work)==0x318)?1:-1];
typedef char ResourceSize[(sizeof(Resource)==40)?1:-1];
typedef struct Ov48WorkR22 Ov48WorkR22;
extern void *ov48_0225A858(void *,u32);
extern void *ov48_0225A834(void *,u32,u32);
extern void ov48_0225A868(u8 *,int,int,void *);
extern void ov48_0225A57C(Ov48WorkR22 *,int,BgConfig **);
extern void ov48_0225A668(void *,s32,s32);
typedef struct A {u8 pad[16];struct Image *images[2];u16 enabled,index;} A;
extern void ov48_0225AA5C(A *,int,void **);
int ov48_0225A4C0(void *,int,BgConfig **);
int ov48_0225A4C0(void *parent,int index,BgConfig **bg){
 Work *w=parent;u32 n;s8 *cursor;s8 state;u32 group,slot,object;
 if(!w->entries[index].active)return 0;
 cursor=&w->entries[index].state;state=*cursor;if(state>=2)return 1;
 n=17-(u32)index;group=n/6;slot=n%6;object=w->indices[group];
 switch(state){
 case 0:
  ov48_0225A868((u8 *)w,group,slot,ov48_0225A858(w,slot));
  ov48_0225A57C((Ov48WorkR22 *)w,n,bg);
  ov48_0225A668(&w->output,(s16)(n*8),144);break;
 case 1:
  ov48_0225A868((u8 *)w,group,slot,ov48_0225A834(w,group,slot));
  ov48_0225AA5C((A *)&w->resources[object],n,(void **)bg);break;
 }
 (*cursor)++;return 0;
}
