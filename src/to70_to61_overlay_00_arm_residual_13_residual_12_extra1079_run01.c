#pragma force_active on
// clang-format off
#include "global.h"
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct Entry { u32 ip; u16 port,pad6; u32 altIp; u16 altPort,padE; u32 flagIp; u8 pad14,flags; u8 pad16[10]; struct Entry *next; } Entry;
typedef struct { int mode,pad4; Entry *list; u8 padC[20]; int socket,otherSocket; u32 ip; } Work;
typedef struct { u16 family,port; u32 ip; } Addr;
extern int ov00_021FF694(int);
extern int ov00_021FF940(int,void*,int,int,Addr*,int*);
extern int ov00_022128BC(Work*,Entry*,void*,int);
extern void ov00_02212724(Work*,Entry*,void*,int);
extern void ov00_02212824(Work*,Entry*,void*,int);
void ov00_022128C4(Work*,int);
void ov00_022128C4(Work *work,int flag) {
 char buffer[0x800]; Addr address; int size=8;
 int socket=flag?work->otherSocket:work->socket;
 if(!ov00_021FF694(socket)) return;
 do {
  int got=ov00_021FF940(socket,buffer,0x7ff,0,&address,&size);
  Entry *entry;
  if(got==-1) return;
  buffer[got]=0;
  for(entry=work->list;entry;entry=entry->next) {
   if((flag && (entry->flags&8) && entry->flagIp==address.ip) ||
    (entry->ip==address.ip && (entry->port==address.port || flag)) ||
    (entry->ip==work->ip && (entry->flags&2) && entry->altIp==address.ip && entry->altPort==address.port)) {
     if(flag) { if(ov00_022128BC(work,entry,buffer,got)) break; }
     else { if(work->mode==1) ov00_02212724(work,entry,buffer,got); else ov00_02212824(work,entry,buffer,got); break; }
   }
  }
 } while(ov00_021FF694(socket));
}
