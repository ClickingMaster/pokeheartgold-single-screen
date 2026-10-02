#pragma force_active on
#include "global.h"
typedef struct {u8 p[0x11e];u8 country,city;u8 tail[4];} Entry;
typedef struct{u8 p[0x12c];int index;u8 p2[0x130];Entry entries[1];} Select;
typedef struct{u8 p[0xb9c];void *fmt,*msg;u8 p2[0x18];void *str;u8 p3[4];void *a,*b;} Work;
extern void *String_New(int,int),*ReadMsgData_ExpandPlaceholders(void *,void *,int,int);
extern void MessageFormat_ResetBuffers(void *),BufferCountryName(void *,int,int),BufferCityName(void *,int,int,int);
void ov70_0223AE98(Work *);
void ov70_0223AE98(Work *w){Entry *entry=&((Select*)w)->entries[((Select*)w)->index];w->str=String_New(180,61);MessageFormat_ResetBuffers(w->fmt);if(entry->country)BufferCountryName(w->fmt,8,entry->country);if(entry->city)BufferCityName(w->fmt,9,entry->country,entry->city);w->a=ReadMsgData_ExpandPlaceholders(w->fmt,w->msg,82,61);w->b=ReadMsgData_ExpandPlaceholders(w->fmt,w->msg,83,61);}
