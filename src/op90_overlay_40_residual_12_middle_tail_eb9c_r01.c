#pragma force_active on
#include "global.h"
extern void *VsRecorder_NewMessageFormat(int),*NewString_ReadMsgData(void *,int),*String_New(int,int);
extern void String_Delete(void *),InitWindow(void *),FillWindowPixelBuffer(void *,int),ScheduleWindowCopyToVram(void *),MessageFormat_Delete(void *),MessageFormat_ResetBuffers(void *),StringExpandPlaceholders(void *,void *,void *),BufferString(void *,int,void *,int,int,int);
extern void AddWindowParameterized(void *,void *,u8,u8,u8,u8,u8,u8,u16);
extern void AddTextPrinterParameterizedWithColor(void *,int,void *,int,int,int,u32,void *);
extern void String16_FormatInteger(void *,int,int,int,int);
typedef struct {u32 unknown;int value;u64 time;} Entry;
typedef struct {Entry *entries;int count,spacing,bg,x,y,width,base,layer,rows;} Config;
typedef struct {u32 words[4];} Window;
typedef struct {int state,count,unknown8,unknownC,rows,pages;Window window;Config *config;u32 pad[2];void *messages;u32 pad2[2];int top,bottom;} State;
typedef struct {u8 pad[0x24];void *bg;u8 pad2[0x20];void *msg;} Context;
typedef struct {int v[3];} Limits;
extern Limits ov40_02244E10;
extern void BufferMonthNameAbbr(void *,int,int),GetSpeciesNameIntoArray(int,int,u16 *),CopyU16ArrayToString(void *,const u16 *),String16_FormatUnsignedLongLong(void *,u64,int,int,int);
extern int ov40_022307B0(u64);
void ov40_0222EB9C(State *,Context *,void *,Config *,int,int,void *);
// @fn ov40_0222EB9C
void ov40_0222EB9C(State *w,Context *ctx,void *msg,Config *cfg,int unused,int type,void *extra) {


 Window *win;void *label;void *number;void *fieldString;int lastGroup=0;u64 previous;void *valueString;int month;int spec;u64 value;int *grp;int msgId;int i;void *fmt;void *expanded;int groups[30]={0};u16 species[256];Limits limits;win=&w->window;

 w->count=cfg->count;w->state=0;w->unknownC=0;w->rows=cfg->rows;w->config=cfg;if(msg)w->messages=msg;else w->messages=ctx->msg;
 w->pages=w->count/w->rows+1;
 {int count=w->count;if(count<w->rows){w->rows=count;w->top=count-1;w->bottom=w->rows-1;}}
 w->top=w->rows/2;w->bottom=w->rows/2;
 InitWindow(win);AddWindowParameterized(ctx->bg,win,(u8)cfg->layer,(u8)cfg->bg,(u8)cfg->x,(u8)cfg->y,(u8)cfg->width,14,(u16)cfg->base);FillWindowPixelBuffer(win,0);
 fmt=VsRecorder_NewMessageFormat(109);limits=ov40_02244E10;previous=w->config->entries[0].time;groups[0]=0;
 {for(i=1;i<limits.v[type];i++){
  u64 current=w->config->entries[i].time;
  if(current==previous)groups[i]=lastGroup;else{previous=current;groups[i]=i;lastGroup=i;}
 }
 }
 i=0;
 if(i<w->rows){
 grp=groups;
 msgId=type+88;
 do {
  label=NewString_ReadMsgData(ctx->msg,msgId);number=String_New(255,109);expanded=String_New(255,109);valueString=String_New(255,109);
  switch(type) {
  case 0:fieldString=NewString_ReadMsgData(w->messages,w->config->entries[i].value);BufferString(fmt,1,fieldString,0,1,2);break;
  case 1:month=w->config->entries[i].value;fieldString=String_New(255,109);BufferMonthNameAbbr(fmt,1,month);break;
  case 2:spec=w->config->entries[i].value;fieldString=String_New(255,109);GetSpeciesNameIntoArray(spec,109,species);CopyU16ArrayToString(fieldString,species);BufferString(fmt,1,fieldString,0,1,2);break;

  }
  value=w->config->entries[i].time;String16_FormatUnsignedLongLong(valueString,value,ov40_022307B0(value),0,1);
  String16_FormatInteger(number,*grp+1,2,1,1);BufferString(fmt,0,number,0,1,2);StringExpandPlaceholders(fmt,expanded,label);
  AddTextPrinterParameterizedWithColor(win,0,expanded,0,(s32)((u32)w->config->spacing*16U*(u32)i),255,0xf0d00,0);
  if(w->config->spacing==2 && extra){BufferString(fmt,2,valueString,0,1,2);StringExpandPlaceholders(fmt,expanded,extra);AddTextPrinterParameterizedWithColor(win,0,expanded,16,(s32)((u32)w->config->spacing*16U*(u32)i+16U),255,0xf0d00,0);}
  String_Delete(label);String_Delete(number);String_Delete(fieldString);String_Delete(expanded);String_Delete(valueString);MessageFormat_ResetBuffers(fmt);
 grp++;
 } while(++i<w->rows);
 }
 ScheduleWindowCopyToVram(win);MessageFormat_Delete(fmt);
}
