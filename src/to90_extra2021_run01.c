#pragma force_active on
#include "global.h"
#include "heap.h"
#pragma force_active on

typedef struct{u32 pad;void*save;u8 pad8[3];s8 index;u8 padC[8];int number;u8 members[32];u8 levels[32];}Source;
typedef struct{void*bg;u8 pad[0x4c];void*format;}UI;
typedef struct{u32 file,unused1,palette,unused3,unused4,image;}Image;
extern void*sub_0202C6F4(void*);extern int sub_0202C090(void*,int,int),GetUnionRoomAvatarAttrBySprite(int,int,int),ov43_0222AB94(UI*,void*,int);
extern void FillWindowPixelBuffer(void*,int),BufferIntegerAsString(void*,int,int,int,int,int),ov43_0222ECD4(void*,Source*,UI*,int,int,int,u32),ov43_0222AB20(UI*,void*,int,int),ov43_0222AB5C(UI*,void*,int,int),sub_02070D84(int,int,Image*),sub_020141C4(int,int,int,int,int,int,int,void*),BlitBitmapRectToWindow(void*,void*,int,int,int,int,int,int,int,int),GfGfxLoader_GXLoadPal(int,int,int,int,int,int),BgTilemapRectChangePalette(void*,int,int,int,int,int,int),ScheduleWindowCopyToVram(void*);
void ov43_0222D8B8(void*,Source*,UI*,int);
void ov43_0222D8B8(void*windows,Source*s,UI*ui,int heap){Image image;void*record=sub_0202C6F4(s->save);int style=sub_0202C090(record,s->members[s->index],8);u32 i;u32 color;void*bitmap;for(i=0;i<8;i++)FillWindowPixelBuffer((u8*)windows+0x118+i*16,0);BufferIntegerAsString(ui->format,0,s->levels[s->index]+1,2,0,1);BufferIntegerAsString(ui->format,1,s->number,2,0,1);ov43_0222ECD4((u8*)windows+0x118,s,ui,18,0,0,0xf0200);if(style==0)color=0x50600;else if(style==1)color=0x30400;else color=0x10200;ov43_0222AB20(ui,s->save,s->members[s->index],heap);ov43_0222ECD4((u8*)windows+0x138,s,ui,17,0,0,color);ov43_0222ECD4((u8*)windows+0x178,s,ui,15,0,0,0xf0200);ov43_0222AB5C(ui,s->save,s->members[s->index],heap);ov43_0222ECD4((u8*)windows+0x148,s,ui,16,0,0,0x10200);ov43_0222ECD4((u8*)windows+0x188,s,ui,27,0,0,0xf0200);if(ov43_0222AB94(ui,s->save,s->members[s->index]))ov43_0222ECD4((u8*)windows+0x158,s,ui,28,0,0,0x10200);sub_02070D84(GetUnionRoomAvatarAttrBySprite(style,sub_0202C090(record,s->members[s->index],7),1),2,&image);bitmap=Heap_Alloc((enum HeapID)heap,3200);sub_020141C4(image.file,image.image,heap,0,0,10,10,bitmap);BlitBitmapRectToWindow((u8*)windows+0x168,bitmap,0,0,80,80,0,0,80,80);Heap_Free(bitmap);GfGfxLoader_GXLoadPal(image.file,image.palette,4,480,32,heap);BgTilemapRectChangePalette(ui->bg,4,4,4,10,10,15);for(i=0;i<8;i++)ScheduleWindowCopyToVram((u8*)windows+0x118+i*16);}
