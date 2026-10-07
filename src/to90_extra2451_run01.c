#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
#include "../lib/NitroDWC/src/tree-iterator4.h"
// clang-format on
typedef struct Node {u8 pad[12];void*value;} Node;
typedef struct {Iterator advance;void*other;} Iter;
typedef struct {void**data;u32 count,capacity;} Vector;
void ov45_02236E58(Iterator*);
void*ov45_0223EA34(Vector*,u32);
void ov45_022421E4(Vector*,u32,void*);
void ov45_022434C0(Vector*,Iter,Iter);
typedef struct {volatile u8 fillFlag,reserveFlag;u8 pad[2];volatile struct{Node*a;void*b;Node*c;} tail;Iter fill,count;} Frame;
#pragma force_active on
void ov45_022434C0(Vector*v,Iter first,Iter last){Frame frame;Node *node;void *value;void **out;u32 n,capacity;frame.count=first;Iter *counter=&frame.count;n=0;
while(frame.count.advance.current!=last.advance.current){n++;ov45_02236E58(&counter->advance);}
capacity=v->capacity;if(n>capacity){frame.reserveFlag=0;void*p=ov45_0223EA34(v,n-capacity);ov45_022421E4(v,n,p);}
out=v->data;frame.fill=first;
if(frame.fill.advance.current!=last.advance.current){do{node=*(void*volatile*)&frame.fill.advance.current;frame.fillFlag=0;value=*(void*volatile*)&node->value;*out++=value;ov45_02236E58(&frame.fill.advance);}while(frame.fill.advance.current!=last.advance.current);frame.tail.b=value;frame.tail.a=node;frame.tail.c=node;}
v->count=n;}
