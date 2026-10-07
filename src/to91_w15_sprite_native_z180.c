#include "global.h"
#include "sprite.h"
#pragma force_active on
typedef struct {void *resources[4];Sprite *sprites[20];} Work;
void ov41_0224AFF8(void **,void **,int,void *,int,int,int,int,int,int);
void ov41_0224B0B8(void **,void **,void *,int);
void ov41_0224AED8(void *,void *,void *,int,void *);
void ov41_0224AED8(void *output,void *spriteList,void *managerData,int count,void *narc){
    Work *work=output;void **managers=managerData;
    SpriteResourcesHeader header;SimpleSpriteTemplate template;
    struct {int y,row;} loop;
    int col,index,x;
    ov41_0224AFF8(work->resources,managers,14,narc,103,225,102,101,2,2000);
    ov41_0224B0B8(work->resources,managers,&header,0);
    template.header=&header;
    template.whichScreen=NNS_G2D_VRAM_TYPE_2DSUB;
    loop.row=0;template.priority=0;template.heapID=HEAP_ID_14;template.spriteList=spriteList;
    loop.y=104;index=loop.row;
    for(;loop.row<2;loop.y+=18,loop.row++){
        template.position.y=loop.y;
        template.position.y=(s32)((u32)template.position.y<<12);
        template.position.y+=0x200000;
        for(col=0,x=38;col<10;x+=18,col++){
            template.position.x=x;
            template.position.x=(s32)((u32)template.position.x<<12);
            work->sprites[index+col]=Sprite_Create(&template);
            Sprite_SetAnimCtrlSeq(work->sprites[index+col],1);
            if(col+index>=count)Sprite_SetDrawFlag(work->sprites[index+col],FALSE);
        }
        index+=10;
    }
}
