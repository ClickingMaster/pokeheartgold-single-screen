#pragma force_active on
#include "global.h"
#define W(p,o) (*(int*)((u8*)(p)+(o)))
#define U(p,o) (*(u32*)((u8*)(p)+(o)))
#define P(p,o) (*(void**)((u8*)(p)+(o)))
#define H(p,o) (*(u16*)((u8*)(p)+(o)))
#define S(p,o) (*(s16*)((u8*)(p)+(o)))
#define B(p,o) (*(u8*)((u8*)(p)+(o)))
typedef struct {void *font,*window,*sprites,*palette;u32 offset;void *sprite;int x,y,mode,unknown,vram,heap;} Template;
extern void *FontSystem_NewInit(int,int),*sub_02013910(void*,int),*NewMsgDataFromNarc(int,int,int,int),*String_New(int,int),*TextOBJ_Create(const Template*,void*);
extern int sub_02013948(void*,int),String_CountLines(void*),FontID_String_GetCenterAlignmentX(int,void*,int,int);
extern void InitWindow(void*),AddTextWindowTopLeftCorner(void*,void*,int,int,int,int),FillWindowPixelBufferText_AssumeTileSize32(void*,int),sub_02021AC8(int,int,int,void*),ReadMsgDataIntoString(void*,int,void*),String_GetLineN(void*,void*,int),AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,int,void*),TextOBJ_SetSpritesDrawFlag(void*,int),TextOBJ_SetPaletteNum(void*,int),sub_020136B4(void*,int,int),String_Delete(void*),DestroyMsgData(void*);
void ov102_021E91C4(void*);
#pragma force_active on
void ov102_021E91C4(void *w){Template t;int i,key;void *str,*msg,*line;u32 lines;u8 *image,*entry;int x;
 P(w,0x1fc)=FontSystem_NewInit(2,35);InitWindow((u8*)w+0x204);AddTextWindowTopLeftCorner(P(w,0x20),(u8*)w+0x204,9,4,0,0);FillWindowPixelBufferText_AssumeTileSize32((u8*)w+0x204,0);P(w,0x200)=sub_02013910((u8*)w+0x204,35);key=sub_02013948(P(w,0x200),1);
 t.font=P(w,0x1fc);t.window=(u8*)w+0x204;t.sprites=P(w,0x24);t.palette=(u8*)w+0x198;t.mode=3;t.unknown=0;t.vram=1;t.x=0;t.y=0;t.heap=35;
 msg=NewMsgDataFromNarc(0,27,283,35);str=String_New(21,35);line=String_New(21,35);
 for(i=0,entry=w,image=(u8*)w+0x218,x=24;i<2;image+=16,entry+=16,x+=136,i++){sub_02021AC8(key,1,1,image);t.sprite=P(entry,0x21c);t.offset=0;ReadMsgDataIntoString(msg,11+i,str);lines=String_CountLines(str);FillWindowPixelBufferText_AssumeTileSize32((u8*)w+0x204,0);u32 y=(32-(lines<<4))>>1;u32 j;for(j=0;j<lines;j++,y+=16){String_GetLineN(line,str,j);int center=FontID_String_GetCenterAlignmentX(4,line,0,72);AddTextPrinterParameterizedWithColor((u8*)w+0x204,4,line,center,y,255,0xe0f00,0);}
 P(entry,0x214)=TextOBJ_Create(&t,P(w,0x200));TextOBJ_SetSpritesDrawFlag(P(entry,0x214),1);TextOBJ_SetPaletteNum(P(entry,0x214),0);sub_020136B4(P(entry,0x214),x,6);}
 String_Delete(line);String_Delete(str);DestroyMsgData(msg);
}
