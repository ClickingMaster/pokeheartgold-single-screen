#include "global.h"
#include "bg_window.h"
#include "text.h"
#include "pm_string.h"
#include "unk_0202C034.h"
#include "overlay_42.h"
#pragma force_active on
extern const s16 ov43_0222EFA0[],ov43_0222EFA2[];extern const u32 ov43_0222ED94[];
void ov43_0222CB34(s16*,void*,void*,int);
void ov43_0222CA50(s16*,void*,void*,void*,int,void*,int,int);
void ov43_0222CA50(s16 *work,void *manager,void *context,void *resource,int slot,void *input,int entry,int heap){
 int value,type;s16 position[4];String *string;u8 *owner=(u8*)work;
 value=sub_0202C090(input,entry,7);type=sub_0202C090(input,entry,8);
 if(type==2)ov43_0222CB34(work,context,resource,slot);
 else{position[0]=(work[0]+*(const s16*)((const u8*)ov43_0222EFA0+slot*10))*8+8;position[1]=(work[1]+*(const s16*)((const u8*)ov43_0222EFA2+slot*10))*8+262;position[2]=8;position[3]=1;*(void**)(owner+24+slot*4)=ov42_022272BC(manager,position,value,(enum HeapID)heap);}
 string=String_New(128,(enum HeapID)heap);CopyU16ArrayToString(string,sub_0202C254(input,entry));
 AddTextPrinterParameterizedWithColor((Window*)(owner+8),4,string,*(const s16*)((const u8*)ov43_0222EFA0+slot*10)*8,*(const s16*)((const u8*)ov43_0222EFA2+slot*10)*8-24,255,ov43_0222ED94[type],NULL);String_Delete(string);
}
