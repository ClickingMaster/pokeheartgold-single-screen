#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {void*narc,*field;int size;NNSG3dRenderObj *render;NNSG3dResFileHeader **model;void*texture;u8 state;u8 pad19[3];int*done;int cancel;int*active;void*aux;int offset;} Work;
void NARC_ReadFile(void*,int,void*),GF3dRender_BindModelSet(void*,void*),Heap_Free(void*),SysTask_Destroy(void*),ov01_02204678(void*,NNSG3dRenderObj*);
int GF3dRender_ResTexIsLoaded(void*),ov01_02204698(void*),sub_02054E20(void*);
void ov01_021F6620(void*,Work*);
void ov01_021F6620(void*task,Work*w){if(w->cancel==1)w->state=5;
 switch(w->state){
 case 0:{int n;w->offset=0;n=w->size;if(n<=0xe000)w->state=2;else{n=0xe000;w->state=1;}NARC_ReadFile(w->narc,n,(u8*)*w->model+w->offset);w->offset+=n;}break;
 case 1:{int last,n=w->size-w->offset;if(n>0xe000){n=0xe000;last=0;}else last=1;NARC_ReadFile(w->narc,n,(u8*)*w->model+w->offset);if(last)w->state=2;else w->offset+=n;}break;
 case 2:if(w->texture && GF3dRender_ResTexIsLoaded(w->texture)==1)GF3dRender_BindModelSet(*w->model,w->texture);w->state=3;break;
 case 3:{NNSG3dResMdlSet*set=NNS_G3dGetMdlSet(*w->model);NNSG3dResMdl*model;GF_ASSERT(((u8*)set)[9]==1);model=NNS_G3dGetMdlByIdx(set,0);GF_ASSERT(model);NNS_G3dRenderObjInit(w->render,model);GF_ASSERT(*(void**)((u8*)w->render+8)==0);*(void**)((u8*)w->render+8)=0;if(ov01_02204698(w->aux) && !sub_02054E20(w->field))ov01_02204678(w->aux,w->render);*w->done=1;w->state=5;}break;
 case 5:*w->active=0;Heap_Free(w);SysTask_Destroy(task);break;
 }
}
