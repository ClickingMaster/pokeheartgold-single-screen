#pragma force_active on
#include "global.h"
#pragma force_active on
typedef struct {u8 active;u8 pad[7];void *item;u8 p[16];u32 data;} Entry;
typedef struct {Entry entries[3];u8 pad[0x90-96];void *value,*q;int heap;} State;
extern u32 ov88_02259910[];extern void ov45_0222AB38(void *,void *),ov88_022595DC(Entry *),ov45_0222DCE8(void *);extern int ov88_022595E4(Entry *),ov45_0222DD44(void *),ov45_0222DD78(void *,void *,void *,void *,void *);extern void *ov45_0222DD5C(void *);extern void ov88_02259560(Entry *,void *,u32,void *,int),ov88_02259280(void *,void *,void *,void *);
int ov88_02259404(State *,void *,void *,void *);
int ov88_02259404(State *s,void *connection,void *context,void *arg){u32 info[5],data;int i;Entry *cursor;u32 *table;void *item;ov45_0222AB38(context,info);i=2;cursor=&s->entries[2];table=ov88_02259910;for(;i>=0;i--){if(cursor->active){if(ov88_022595E4(cursor)==1){if(i+1<3)ov88_02259560(&s->entries[i+1],cursor->item,table[1],&cursor->data,s->heap);ov88_022595DC(cursor);}}cursor--;table--;}if(s->entries[0].active==0){if(ov45_0222DD44(connection)==1){do{item=ov45_0222DD5C(connection);if(ov45_0222DD78(connection,info,item,s->value,arg)==1){ov88_02259280(&data,connection,item,context);ov88_02259560(&s->entries[0],s->value,336,&data,s->heap);ov45_0222DCE8(connection);return 1;}ov45_0222DCE8(connection);}while(ov45_0222DD44(connection)==1);}}return 0;}
