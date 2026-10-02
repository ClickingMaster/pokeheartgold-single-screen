#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u32 a;void *save;} Args;
typedef struct {int heap;Args *args;u8 p[0x44-8];void *msg,*format,*string,*scratch,*first,*second,*name;} State;
extern void FontID_Alloc(int,int);extern void *NewMsgDataFromNarc(int,int,int,int),*MessageFormat_New_Custom(int,int,int),*String_New(int,int),*NewString_ReadMsgData(void *,int),*Save_PlayerData_GetProfile(void *),*PlayerProfile_GetPlayerName_NewString(void *,int);
void ov113_021E6680(State *);
void ov113_021E6680(State *s){FontID_Alloc(4,s->heap);s->msg=NewMsgDataFromNarc(0,27,1,s->heap);s->format=MessageFormat_New_Custom(16,16,s->heap);s->string=String_New(512,s->heap);s->scratch=String_New(512,s->heap);s->first=NewString_ReadMsgData(s->msg,0);s->second=NewString_ReadMsgData(s->msg,5);s->name=PlayerProfile_GetPlayerName_NewString(Save_PlayerData_GetProfile(s->args->save),s->heap);}
