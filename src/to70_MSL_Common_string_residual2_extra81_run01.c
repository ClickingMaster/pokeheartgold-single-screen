#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
extern char *strtok_ns[2];
char *strtok(char *,const char *);
char *strtok(char *input,const char *delims) {u8 table[32]={0};int c;char *p,*result;const u8 *scan=(const u8 *)delims+1; if(input)strtok_ns[1]=input;c=(u8)*delims;if(c!=0){do{table[(u8)c>>3]|=(u8)(1<<(c&7));c=*scan++;}while(c!=0);}p=strtok_ns[1];c=(u8)*p++;if(c) {do {if(!(table[(u8)c>>3]&(u8)(1<<(c&7))))break;c=(u8)*p++;}while(c);}if(!c) {strtok_ns[1]=strtok_ns[0];return 0;}result=p-1;c=(u8)*p++;if(c) {do {if(table[(u8)c>>3]&(u8)(1<<(c&7)))break;c=(u8)*p++;}while(c);}if(!c)strtok_ns[1]=strtok_ns[0];else {strtok_ns[1]=p;p[-1]=0;}return result;}
