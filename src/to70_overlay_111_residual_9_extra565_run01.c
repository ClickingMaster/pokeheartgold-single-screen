#pragma force_active on
#include "global.h"
extern void *Heap_Alloc(int,int);
extern void *MessagePrinter_New(int,int,int,int);
extern void *NewMsgDataFromNarc(int,int,int,int);
extern void *MessageFormat_New(int);
extern void ResetAllTextPrinters(void);
extern void ov111_021E6A44(void *,u8,int);
extern void LoadFontPal0(int,int,int);
extern void BG_FillCharDataRange(void *,int,int,int,int);
typedef struct {void *bg,*printer,*format,*msg;int unk10,unk14,heap;u8 pad[0x10];int unk2c;u32 low:16;u32 layer:3;u32 high:13;} Work;
Work *ov111_021E67C4(int heap);
void ov111_021E67EC(Work *w,void *bg,int layer,int x);
Work *ov111_021E67C4(int heap){Work *w=Heap_Alloc(heap,0x34);MI_CpuFill8(w,0,0x34);w->heap=heap;w->printer=MessagePrinter_New(1,2,0,heap);return w;}
void ov111_021E67EC(Work *w,void *bg,int layer,int x){w->bg=bg;w->unk2c=-1;w->layer=layer;w->unk14=x;w->msg=NewMsgDataFromNarc(1,27,29,w->heap);w->format=MessageFormat_New(w->heap);ResetAllTextPrinters();ov111_021E6A44(w->bg,(u8)w->layer,w->heap);LoadFontPal0(0,480,w->heap);BG_FillCharDataRange(w->bg,w->layer,0,1,0);}
