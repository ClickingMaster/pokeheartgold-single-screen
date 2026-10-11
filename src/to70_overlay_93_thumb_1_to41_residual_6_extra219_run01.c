#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct Data{u8 pad[0x2c];u8 ids[4];u8 count;}Data;typedef struct W{Data*data;u8 pad[0x28];void*bg;u8 windows[4][16];}W;
extern void FillBgTilemapRect(void*,int,int,int,int,int,int,int),ScheduleBgTilemapBufferTransfer(void*,int);void ov93_0225D5AC(W*,int);
extern int Comm_GetLocalNetId(void),ov93_0225E3C4(W*,int),ov90_022588A4(Data*,int),FontID_String_GetWidth(int,void*,int);extern void*PlazaGameRoster_GetProfile(Data*),*PlayerProfile_GetPlayerName_NewString(void*,int);extern const u8 ov93_02262A68[];extern void Heap_Free(void*),AddTextPrinterParameterizedWithColor(void*,int,void*,int,int,int,int,void*);void ov93_0225D4EC(W*);
void ov93_0225D5AC(W*w,int mode){if(mode==1){FillBgTilemapRect(w->bg,4,1023,0,13,12,4,17);FillBgTilemapRect(w->bg,4,1023,17,0,12,4,17);FillBgTilemapRect(w->bg,4,1023,20,13,12,4,17);}else{switch(w->data->count){case 2:FillBgTilemapRect(w->bg,4,1023,0,13,12,4,17);FillBgTilemapRect(w->bg,4,1023,20,13,12,4,17);break;case 3:FillBgTilemapRect(w->bg,4,1023,17,0,12,4,17);break;}}ScheduleBgTilemapBufferTransfer(w->bg,4);}
