#pragma force_active on
#include "global.h"
typedef struct {void *templates;u32 count;} Table;extern Table ov86_021E8230[];extern void AddWindow(void *,void *,void *),FillWindowPixelBuffer(void *,int);void ov86_021E7DF8(void *,void *,int);
void ov86_021E7DF8(void *bg,void *windows,int index){u8 i;void *templates=ov86_021E8230[index].templates;u32 count=ov86_021E8230[index].count;for(i=0;i<count;i++){AddWindow(bg,(u8*)windows+i*16,(u8*)templates+i*8);FillWindowPixelBuffer((u8*)windows+i*16,0);}}
