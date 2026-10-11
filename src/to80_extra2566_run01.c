#pragma force_active on
#include "global.h"
#include "heap.h"
#include "sys_task_api.h"
#include <stddef.h>
#include "sprite_system.h"
#include "palette.h"
#include "font_sprite.h"
typedef struct {int blend,count,started,done;} Work;typedef struct {s16 x,y,tx,ty,dx,dy;u8 pad0c[0x10];u8 speed,state,pad1e[2];void *sprite,*text,*cursor,*count,*done;} Motion;typedef struct Choice {int enabled, reserved04, message, anim, action, reserved14;u32 selection;int reserved1c;const struct Choice *child;} Choice;
extern void VsRecorder_GetSpritePos(void *,s16 *,s16 *);
extern void ov40_022318C8(SysTask *,void *);
extern void VsRecorder_SetMenuRowText(u8 *,u8 *,int,u8 *);
extern void VsRecorder_MenuRunAction(void *);
extern void VsRecorder_SetSubState(void *,int);
extern BOOL VsRecorder_MenuEnterChild(void *);
int ov40_022319A4(void *);
#define I(p,o) (*(int*)((p)+(o)))
#define V(p,o) (*(void**)((p)+(o)))
#pragma force_active on
typedef struct{void*sprite;u8 gap04[16];void*text;u8 gap18[16];} SpriteRow;
typedef struct{u8 gap0[8];int state;u8 gapc[16];void*manager;u8 gap20[8];void*palette;u8 gap2c[44];int color;u8 gap5c[0x4d8];SpriteRow left[5],right[5];u8 gap6c4[20];int selected;u32 gap6dc;int count,choice;u8 gap6e8[8];void*cursor;u8 gap6f4[0x124];void*choices;u8 gap81c[0x44];void*animation;u8 remaining[0x4170-0x864];} Scene;
int ov40_022319A4(void *raw){Scene*p=(Scene*)raw;Motion *m;u8 *q;int i;const Choice *choices;void *list;int *counter2,*finished,*counter;Work *w2,*w;switch(p->state){case 0:{w=Heap_Alloc((enum HeapID)109,16);MI_CpuFill8(w,0,16);p->animation=w;w->started=0;i=0;if(p->count>0){q=(u8*)p;counter=&w->count;finished=&w->done;do{m=Heap_Alloc((enum HeapID)109,0x34);memset(m,0,0x34);VsRecorder_GetSpritePos(*(void **)(q+offsetof(Scene,right[0].sprite)),&m->x,&m->y);m->sprite=*(void **)(q+offsetof(Scene,right[0].sprite));m->text=*(void **)(q+offsetof(Scene,right[0].text));m->count=counter;m->done=finished;if(i==p->choice){m->cursor=p->cursor;m->tx=m->x;m->ty=-48;m->speed=8;}else{m->cursor=0;m->tx=m->x;m->ty=205+(5-p->count)*16;if(m->ty>=221)m->ty=221;m->speed=8;}SysTask_CreateOnMainQueue(ov40_022318C8,m,8192);i++;q+=sizeof(SpriteRow);}while(i<p->count);}w->blend=0;p->state++;break;}case 1:{w2=p->animation;if(w2->blend!=16){w2->blend+=2;PaletteData_BlendPalettes(p->palette,PLTTBUF_MAIN_OBJ,(u16)12,(u8)w2->blend,(u16)p->color);}if(w2->done==1){i=0;if(w2->started==0){w2->started=1;list=p->left;counter2=&w2->count;q=(u8*)p;do{m=Heap_Alloc((enum HeapID)109,0x34);memset(m,0,0x34);VsRecorder_GetSpritePos(*(void **)(q+offsetof(Scene,left[0].sprite)),&m->x,&m->y);m->sprite=*(void **)(q+offsetof(Scene,left[0].sprite));m->text=*(void **)(q+offsetof(Scene,left[0].text));m->count=counter2;m->cursor=0;m->tx=m->x;if(p->selected==i){m->tx=m->x;m->ty=169;choices=p->choices;ManagedSprite_SetAnim(*(void **)(q+offsetof(Scene,left[0].sprite)),choices[p->choice].anim);{VsRecorder_SetMenuRowText((u8*)p,(u8*)list+p->selected*sizeof(SpriteRow),((const Choice*)p->choices)[p->choice].message,(u8*)&((const Choice*)p->choices)[p->choice]);}TextOBJ_SetSpritesDrawFlag(p->left[p->selected].text,1);ManagedSprite_SetDrawFlag(*(void **)(q+offsetof(Scene,left[0].sprite)),1);}else{m->tx=m->x-4;m->ty=(p->selected-i)*16+((4-(p->selected-i))*36+25);}m->speed=4;SysTask_CreateOnMainQueue(ov40_022318C8,m,8192);i++;q+=sizeof(SpriteRow);}while(i<=p->selected);}w2->count=w2->done;w2->done=0;}i=0;if(p->selected>0){q=(u8*)p;do{if(i==p->selected)ManagedSprite_SetPaletteOverrideOffset(*(void **)(q+offsetof(Scene,left[0].sprite)),1);else ManagedSprite_SetPaletteOverrideOffset(*(void **)(q+offsetof(Scene,left[0].sprite)),2);i++;q+=sizeof(SpriteRow);}while(i<p->selected);}if(w2->count==0)p->state++;w2->count=0;break;}case 2:choices=p->choices;if(choices[p->choice].child==0)VsRecorder_MenuRunAction(p);else VsRecorder_SetSubState(p,5);VsRecorder_MenuEnterChild(p);Heap_Free(p->animation);break;}return 0;}
