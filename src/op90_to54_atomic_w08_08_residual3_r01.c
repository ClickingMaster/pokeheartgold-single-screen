#pragma force_active on
#include "global.h"
#define B(p,o) (*((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define SH(p,o) (*(s16*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
void*NewMsgDataFromNarc(int,int,int,int);void*MessageFormat_New(int);void*ReadMsgData_ExpandPlaceholders(void*,void*,int,int);void CopyStringToU16Array(void*,u16*,u32);void String_Delete(void*);u32 ov74_02235A68(int);void GF_RTC_CopyDate(RTCDate*);void MessageFormat_Delete(void*);void DestroyMsgData(void*);void SaveMysteryGift_TryInsertGift(void*,void*,int);void GfGfx_EngineBTogglePlanes(int,int);void ov74_02235DC4(void*,void*);
typedef struct {
    u16 type;
    u8 unk002[0x102];
    u16 title[36];
    u8 unk14C_pad[0];
    u32 versions;
    u16 unk150;
    union { u8 flags; struct { u8 bit0:1, bit1:1, bit2:1, bit3:1, bit4:1, bit5:1, bit6:1, bit7:1; }; };
    u8 unk153;
    u16 text[250];
    u8 unk348;
    u8 unk349;
    u16 unk34A;
    u16 unk34C;
    u16 unk34E;
    u8 unk350;
    u8 unk351[3];
    u32 day;
} Gift;
void ov74_0222947C(u8*w);
// @fn ov74_0222947C
void ov74_0222947C(u8*w){
 Gift*gift=(Gift*)(w+0x3180);void*msg,*fmt,*str;u32 a,b,c,d,e;RTCDate date;
 MI_CpuFill8(gift,0,0x358);msg=NewMsgDataFromNarc(1,27,247,U(w,0));fmt=MessageFormat_New(U(w,0));gift->type=7;
 str=ReadMsgData_ExpandPlaceholders(fmt,msg,78,U(w,0));CopyStringToU16Array(str,gift->title,36);String_Delete(str);
 a=ov74_02235A68(8);b=ov74_02235A68(7);c=ov74_02235A68(12);d=ov74_02235A68(10);e=ov74_02235A68(11);gift->versions=b|(d|e|c)|a;gift->unk150=1;
 gift->bit0=0;gift->bit1=0;gift->flags=gift->flags|4;gift->flags=gift->flags|8;gift->bit5=0;
 str=ReadMsgData_ExpandPlaceholders(fmt,msg,77,U(w,0));CopyStringToU16Array(str,gift->text,250);String_Delete(str);
 gift->unk348=0;gift->unk34A=490;gift->unk34C=0;gift->unk34E=0;gift->unk350=0;GF_RTC_CopyDate(&date);gift->day=RTC_ConvertDateToDay(&date);
 MessageFormat_Delete(fmt);DestroyMsgData(msg);SaveMysteryGift_TryInsertGift(P(w,0x3174),gift,3);GfGfx_EngineBTogglePlanes(1,0);ov74_02235DC4(P(w,4),gift);
}
