#pragma force_active on
#include "global.h"
#include "heap.h"
#include "font.h"
#include "bg_window.h"
#include <string.h>
#pragma force_active on
typedef struct{int heap;u8 pad4[0x29f8];void*bg;u8 pad2a00[0x1b4];void*input;u8 pad2bb8[8];u32 flag;u8 remaining[0x1190];} Work;
typedef char Work40541Size[(sizeof(Work)==0x3d54)?1:-1];
typedef char Work40541Bg[(offsetof(Work,bg)==0x29fc)?1:-1];
typedef char Work40541Input[(offsetof(Work,input)==0x2bb4)?1:-1];
typedef char Work40541Flag[(offsetof(Work,flag)==0x2bc0)?1:-1];
extern void ov74_0222FCC4(BgConfig*),ov74_0222D824(u8*,int,int);void ov74_0222EC08(void*,void*,int);
void ov74_0222EC08(void*bg,void*input,int heap){Work*w;ov74_0222FCC4(bg);w=Heap_AllocAtEnd((enum HeapID)heap,0x3d54);memset(w,0,0x3d54);w->bg=bg;LoadFontPal0((enum GFPalLoadLocation)0,(enum GFPalSlotOffset)480,(enum HeapID)(w->heap=heap));w->input=input;w->flag=0;ov74_0222D824((u8*)w,1,0);Heap_Free(w);}
