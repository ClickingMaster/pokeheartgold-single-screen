#pragma force_active on
#include "global.h"
#pragma force_active on
extern void *sub_02034818(int),*MessageFormat_New(int),*NewMsgDataFromNarc(int,int,int,int);
extern int ov74_0222E85C(u32 *);
extern void FillWindowPixelBuffer(void *,int),BufferPlayersName(void *,int,void *),String_Delete(void *),CopyWindowToVram(void *),DestroyMsgData(void *),MessageFormat_Delete(void *);
extern void *ReadMsgData_ExpandPlaceholders(void *,void *,int,int);
extern int PlayerProfile_GetTrainerGender(void *);
extern u32 PlayerProfile_GetTrainerID(void *);
extern void AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,u32,void *);
extern void BufferIntegerAsString(void *,int,u32,int,int,int);
int ov74_0222E8B4(u8 *,void *);
typedef struct State {u8 pad[0x2c08];void *profiles[5];u32 order[5];u8 pad2c30[12];u32 counter;} State;
int ov74_0222E8B4(u8 *raw,void *window){
 State *p=(State *)raw;int n=0,changes=0,i;u32 temp[4];int ids[4];void *fmt,*str,*msg,*profile;int y;
 for(i=1;i<5;i++){
  profile=sub_02034818(i);
  if(!profile){if(p->profiles[i])changes++;p->profiles[i]=NULL;p->order[i]=0x3fff0001;}
  else{if(p->profiles[i]!=profile){changes++;p->profiles[i]=profile;p->order[i]=p->counter++;n++;}else n++;}
 }
 if(!changes)return n;
 temp[0]=p->order[1];temp[1]=p->order[2];temp[2]=p->order[3];temp[3]=p->order[4];
 ids[0]=ov74_0222E85C(temp);ids[1]=ov74_0222E85C(temp);ids[2]=ov74_0222E85C(temp);ids[3]=ov74_0222E85C(temp);
 fmt=MessageFormat_New(0x55);msg=NewMsgDataFromNarc(1,27,0xf7,0x55);y=0;FillWindowPixelBuffer(window,0);
 for(i=0;i<n;i++){
  profile=sub_02034818(ids[i]);
  if(profile){
   BufferPlayersName(fmt,0,profile);str=ReadMsgData_ExpandPlaceholders(fmt,msg,0x36,0x55);
   if(PlayerProfile_GetTrainerGender(profile)==0)AddTextPrinterParameterizedWithColor(window,0,str,0,y,255,0x50600,NULL);
   else AddTextPrinterParameterizedWithColor(window,0,str,0,y,255,0x30400,NULL);
   String_Delete(str);BufferIntegerAsString(fmt,0,(u16)PlayerProfile_GetTrainerID(profile),5,2,1);
   str=ReadMsgData_ExpandPlaceholders(fmt,msg,0x37,0x55);AddTextPrinterParameterizedWithColor(window,0,str,0x50,y,255,0xe0f00,NULL);String_Delete(str);y+=24;
  }
 }
 if(n)CopyWindowToVram(window);DestroyMsgData(msg);MessageFormat_Delete(fmt);return n;
}
