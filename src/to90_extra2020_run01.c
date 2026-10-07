#pragma force_active on
#include "global.h"
#pragma force_active on
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void InitWindow(void*),AddWindowParameterized(void*,void*,int,int,int,int,int,int,int),FillWindowPixelBuffer(void*,int);
void *String_New(int,int),*sub_0202C6F4(void*),*sub_0202C08C(void*);
u64 DWC_CreateFriendKey(void*);
void ReadMsgDataIntoString(void*,int,void*),StringExpandPlaceholders(void*,void*,void*),String_Delete(void*),ov43_0222AAA4(void*,u32,u32);
u32 FontID_String_GetWidthMultiline(int,void*,int);
void AddTextPrinterParameterizedWithColor(void*,int,void*,u32,int,int,u32,void*);
void *GfGfxLoader_GetScrnDataFromOpenNarc(void*,int,int,void**,int);
void ov43_0222CE48(void*,void*,void*,int);
void ov43_0222CE48(void*w,void*arg,void*ctx,int heap){void*s,*temp;u64 key;u32 x;
 InitWindow(w);AddWindowParameterized(P(ctx,0),w,3,4,11,24,8,11,1);
 InitWindow((u8*)w+16);AddWindowParameterized(P(ctx,0),(u8*)w+16,3,9,6,14,2,11,193);
 InitWindow((u8*)w+32);AddWindowParameterized(P(ctx,0),(u8*)w+32,3,21,21,9,2,11,221);
 FillWindowPixelBuffer(w,0);FillWindowPixelBuffer((u8*)w+16,0);FillWindowPixelBuffer((u8*)w+32,0);
 s=String_New(128,heap);temp=String_New(128,heap);key=DWC_CreateFriendKey(sub_0202C08C(sub_0202C6F4(P(arg,4))));
 if(key){ReadMsgDataIntoString(P(ctx,0x54),53,s);x=(192-FontID_String_GetWidthMultiline(0,s,0))/2;AddTextPrinterParameterizedWithColor(w,0,s,x,0,255,0x10200,0);ov43_0222AAA4(ctx,(u32)key,(u32)(key>>32));ReadMsgDataIntoString(P(ctx,0x54),52,temp);StringExpandPlaceholders(P(ctx,0x50),s,temp);AddTextPrinterParameterizedWithColor((u8*)w+16,0,s,0,0,255,0xf0200,0);
 }else {ReadMsgDataIntoString(P(ctx,0x54),54,s);x=(192-FontID_String_GetWidthMultiline(0,s,0))/2;AddTextPrinterParameterizedWithColor(w,0,s,x,0,255,0x10200,0);}
 ReadMsgDataIntoString(P(ctx,0x54),51,s);AddTextPrinterParameterizedWithColor((u8*)w+32,4,s,0,0,255,0x10f00,0);String_Delete(s);String_Delete(temp);P(w,0x30)=GfGfxLoader_GetScrnDataFromOpenNarc(P(ctx,0x58),14,1,(void**)((u8*)w+0x34),heap);
}
