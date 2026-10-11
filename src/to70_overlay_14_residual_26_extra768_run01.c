#pragma force_active on
#include "global.h"
typedef struct {u8 pad[0x2f0];void*manager;}Detail;
typedef struct {u8 pad[0x2b];u8 box;u8 q[8];Detail*detail;}Work;
extern void ov14_021E81A8(void*,int),ov14_021E7ED0(void*),PCBox_SlideInPartyPanel(void*),ov14_021F63F0(void*),ov14_021F63B8(void*),ov14_021F3044(void*),PCBox_DrawSummaryButton(void*,int),PCBox_SlideInSummaryButton(void*),ov14_021E9518(void*),PCBox_WaitForUpdateFunc(void*,void(*)(void*),int);
extern int PCBox_GetSlotMonData(void*,int,int,void*);
void ov14_021ED2DC(Work*);
void ov14_021ED2DC(Work*w){ov14_021E81A8(w->detail->manager,1);ov14_021E7ED0(w->detail->manager);PCBox_SlideInPartyPanel(w->detail->manager);ov14_021F63F0(w->detail);ov14_021F63B8(w->detail);ov14_021F3044(w);PCBox_DrawSummaryButton(w,0);if(PCBox_GetSlotMonData(w,w->box,172,0))PCBox_SlideInSummaryButton(w->detail->manager);PCBox_WaitForUpdateFunc(w,ov14_021E9518,40);}
