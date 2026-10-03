#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Record{u32 a;u8 type,hidden;u16 pad;u16 value,flip;u32 tail;}Record;
typedef struct State{u8 pad0[0x190];void*msg1;u8 pad194[8];void*msg2;u8 pad1a0[0x26c];void*sprites[164];Record records[12];}State;
extern void*Party_GetMonByIndex(void*,int);
extern int sub_0207083C(void*,int);
extern void Sprite_SetDrawFlag(void*,int),Sprite_SetFlipMode(void*,int),Sprite_SetAnimCtrlSeq(void*,int);
typedef struct Coord{int a,b,c;}Coord;
extern Coord ov65_0221FEA4[],ov65_0221FEA8[];
extern void ov65_0221F748(void*,int,int),ov65_0221CA64(State*,int,int),ov65_0221D5FC(void*,void*,int,int,int);
extern int ov65_0221D648(Record*,void*,int,int);
extern void*String_New(int,int);
extern void FillWindowPixelBuffer(void*,int),ReadMsgDataIntoString(void*,int,void*),ov65_0221FB4C(void*,void*,int,int,int,int);
extern u32 GetMonData(void*,int,void*);
extern void String16_FormatInteger(void*,int,int,int,int),String_Delete(void*),ClearWindowTilemapAndCopyToVram(void*),GetItemNameIntoString(void*,int,int);
void ov65_0221D674(u8*,int,void*,int,State*);
void ov65_0221D674(u8*windows,int slot,void*party,int idx,State*s){struct Frame{void*party;int idx,value,slotOffset;u8*flag;void**sprites0;int offset0;void**sprites1;int offset1;int windowOffset;u8*record;int row;}f;void*str;int result;u16 item;void**sprites;
 f.party=party;f.idx=idx;f.value=sub_0207083C(Party_GetMonByIndex(party,idx),2);sprites=s->sprites;f.slotOffset=slot*4;Sprite_SetDrawFlag(*(void**)((u8*)sprites+f.slotOffset),1);ov65_0221F748(*(void**)((u8*)sprites+f.slotOffset),ov65_0221FEA4[slot].a,f.value+ov65_0221FEA8[slot].a+192);
 if(slot==0)Sprite_SetFlipMode(*(void**)((u8*)sprites+f.slotOffset),s->records[f.idx].flip);
 ov65_0221CA64(s,slot+2,s->records[f.idx].type);Sprite_SetDrawFlag(s->sprites[slot+2],1);ov65_0221D5FC(windows+(slot+26)*16,f.party,f.idx,9,6);
 f.row=(f.idx+slot*6)*16;f.record=(u8*)s+f.row;result=ov65_0221D648((Record*)((u8*)s+0x69c+f.row),f.party,f.idx,*(u16*)(f.record+0x6a4));f.flag=(u8*)s+0x6a1;if(f.flag[f.row])result=2;
 switch(result){case 2:Sprite_SetDrawFlag(s->sprites[slot+4],0);break;case 0:f.sprites0=s->sprites;f.offset0=(slot+4)*4;Sprite_SetDrawFlag(*(void**)((u8*)f.sprites0+f.offset0),1);Sprite_SetAnimCtrlSeq(*(void**)((u8*)f.sprites0+f.offset0),7);break;case 1:f.sprites1=s->sprites;f.offset1=(slot+4)*4;Sprite_SetDrawFlag(*(void**)((u8*)f.sprites1+f.offset1),1);Sprite_SetAnimCtrlSeq(*(void**)((u8*)f.sprites1+f.offset1),6);break;}
 if(f.flag[f.row]==0){str=String_New(10,26);f.windowOffset=(slot+28)*16;FillWindowPixelBuffer(windows+f.windowOffset,0);ReadMsgDataIntoString(s->msg1,41,str);ov65_0221FB4C(windows+f.windowOffset,str,9,255,6,0);item=(u16)GetMonData(Party_GetMonByIndex(f.party,f.idx),161,0);String16_FormatInteger(str,item,3,0,1);ov65_0221FB4C(windows+f.windowOffset,str,9,0,30,0);String_Delete(str);}else ClearWindowTilemapAndCopyToVram(windows+(slot+28)*16);
 ov65_0221FB4C(windows+(slot+30)*16,s->msg2,7,0,3,0);item=(u16)GetMonData(Party_GetMonByIndex(f.party,f.idx),6,0);slot+=32;FillWindowPixelBuffer(windows+slot*16,0);str=String_New(20,26);GetItemNameIntoString(str,item,26);ov65_0221FB4C(windows+slot*16,str,9,0,3,0);String_Delete(str);
}
