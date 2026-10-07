#pragma force_active on
#include "global.h"
typedef struct Tail {u8 prefix[28];u8 kind;} Tail;typedef struct Child {u8 prefix[0x30];Tail tail;} Child;typedef struct Item {u8 prefix[0x50];Child child;} Item;
struct A{u8 pad[0x110];Item*items[];};struct G{u32 a;Tail*current;};extern struct G _021D41CC;int sub_02035798(int);void sub_0205ABB0(struct A*);int sub_0205A200(struct A*,int);
// @fn sub_0205A200
int sub_0205A200(struct A*s,int n){Item*item;Child*c;int ok=sub_02035798(n-1);sub_0205ABB0(s);if(!ok)return 5;item=s->items[n-1];if(item==NULL)return 5;c=(Child*)((u8*)item+0x50);_021D41CC.current=(Tail*)((u8*)c+0x30);switch(_021D41CC.current->kind){case 0:return 1;case 1:return 2;case 2:return 3;case 3:case 13:return 4;case 4:case 5:case 6:case 7:case 8:case 11:goto unavailable;default:goto unsupported;}unavailable:return 5;unsupported:return 5;}
