#pragma force_active on
#include "global.h"
extern void PlaySE(int);extern void ov109_021E7388(void *,int);extern void ov109_021E7584(void *,int);extern void ov109_021E75C8(void *);extern void ov109_021E7248(void *,void *);extern void ov109_021E74D4(void *,int);extern u8 _021E7884[];
typedef struct {u8 pad[0x10];u8 *flags;u8 p14[5];u8 page,pages,a,b,old_a,old_b,selected;u8 p20[0xa5];u8 count;u8 pc6[10];struct {void *obj;u32 unused;} entries[256];} Work;
int ov109_021E5F40(Work *,int);
int ov109_021E5F40(Work *w,int key){u8 selected;switch(key){case 12:w->flags[1]=0;PlaySE(1500);ov109_021E7388(w,1);return 8;case 13:if(w->page==0)return 1;ov109_021E7584(w,2);PlaySE(2336);return 2;case 14:if(w->page==w->pages-1)return 1;ov109_021E7584(w,3);PlaySE(2336);return 3;}selected=(u8)(_021E7884[key]+w->page*12);if(selected>=w->count)return 1;w->selected=selected;w->old_a=w->a;w->old_b=w->b;PlaySE(1500);ov109_021E75C8(w);ov109_021E7248(w,w->entries[selected].obj);ov109_021E74D4(w,1);return 4;}
