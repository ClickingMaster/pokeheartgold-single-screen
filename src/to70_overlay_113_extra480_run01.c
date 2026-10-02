#pragma force_active on
#include "global.h"
extern void ov113_021E59F8(void*),Heap_Create(int,int,u32),ov113_021E5A7C(void*),SpriteSystem_DrawSprites(void*);
extern void *OverlayManager_CreateAndGetData(void*,u32,int),*OverlayManager_GetArgs(void*),*OverlayManager_GetData(void*);
extern int ov113_021E5B70(void*),ov113_021E5BC4(void*),ov113_021E5C0C(void*),ov113_021E5C60(void*),ov113_021E5C68(void*);
typedef struct{int heap;void*args;u8 pad[0xa8];void*s1,*s2;}Work;
int UnownReport_Init(void*,int*);int UnownReport_Main(void*,int*);
int UnownReport_Init(void*m,int*s){Work*w;switch(*s){case 0:ov113_021E59F8(m);Heap_Create(3,152,0x20000);w=OverlayManager_CreateAndGetData(m,352,152);MI_CpuFill8(w,0,352);w->heap=152;w->args=OverlayManager_GetArgs(m);ov113_021E5A7C(w);(*s)++;break;case 1:if(ov113_021E5B70(OverlayManager_GetData(m)))return 1;break;}return 0;}
int UnownReport_Main(void*m,int*s){Work*w=OverlayManager_GetData(m);switch(*s){case 0:*s=ov113_021E5BC4(w);break;case 1:*s=ov113_021E5C0C(w);break;case 2:*s=ov113_021E5C60(w);break;case 3:*s=ov113_021E5C68(w);break;case 4:return 1;}SpriteSystem_DrawSprites(w->s1);SpriteSystem_DrawSprites(w->s2);return 0;}
