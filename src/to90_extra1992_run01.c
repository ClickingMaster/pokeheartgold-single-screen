#pragma force_active on
#include "global.h"
#pragma force_active on
#include "bg_window.h"
#include "heap.h"
#include "sprite.h"
typedef struct {Window *window;u32 pad4;void *target;int index;u8 spriteContext[0x13c];u8 spriteResources[0x34];Sprite *sprite;u8 callbacks[3][8];} Work;
typedef struct {u8 pad[0x1c];void *target;} Nested;
typedef struct {u32 pad;Nested *nested;BgConfig *bg;} Scene;
typedef struct {int state,fade;u32 pad8;Work *work;Scene *scene;int *done;u8 pad18[8];void *source;} Task;
void ov01_021F05C4(void*,int,int);
void ov01_021F05F4(void*);
void ov01_021F06EC(void*,void*);
void ov01_021EFCF8(int,int,int,int*,int);
void ov01_021EFCDC(Task*,void*);
void sub_0200FC20(int);
void ov120_0225F9D4(void*,Sprite**,void*,void*,int);
void ov120_0225F6FC(void*,int(*)(Work*));
int ov120_0225F704(void*,Work*);
int ov120_0225F8B0(Work*),ov120_0225F90C(Work*),ov120_0225F970(Work*);
void ov120_0225F714(void *parent,Task *t);
void ov120_0225F714(void *parent,Task *t){
    Work *w=t->work;
    switch(t->state){
    case 0:{
        u16 black;
        t->work=Heap_Alloc(HEAP_ID_FIELD1,0x19c);memset(t->work,0,0x19c);w=t->work;
        w->window=AllocWindows(HEAP_ID_FIELD1,1);
        AddWindowParameterized(t->scene->bg,w->window,3,0,0,32,32,0,0);
        black=0;BG_LoadPlttData(3,&black,2,30);FillWindowPixelBuffer(w->window,0);ScheduleWindowCopyToVram(w->window);
        ov01_021F05C4(w->spriteContext,2,1);
        ov120_0225F9D4(t->source,&w->sprite,w->spriteContext,w->spriteResources,0);
        GfGfx_EngineATogglePlanes(16,1);w->target=t->scene->nested->target;
        ov120_0225F6FC(w->callbacks[0],ov120_0225F8B0);ov120_0225F6FC(w->callbacks[1],ov120_0225F90C);ov120_0225F6FC(w->callbacks[2],ov120_0225F970);
        t->state=1;break;
    }
    case 1:ov01_021EFCF8(1,-16,-16,&t->fade,2);t->state=2;break;
    case 2:if(t->fade)t->state=3;break;
    case 3:w->index=0;t->state=4;break;
    case 4:if(ov120_0225F704(w->callbacks[w->index],w))t->state=5;break;
    case 5:
        sub_0200FC20(0);if(t->done)*t->done=1;
        ClearWindowTilemapAndCopyToVram(w->window);RemoveWindow(w->window);WindowArray_Delete(w->window,1);Sprite_Delete(w->sprite);
        ov01_021F06EC(w->spriteContext,w->spriteResources);ov01_021F05F4(w->spriteContext);ov01_021EFCDC(t,parent);return;
    }
    if(t->state!=5)SpriteList_RenderAndAnimateSprites(*(SpriteList**)w->spriteContext);
}
