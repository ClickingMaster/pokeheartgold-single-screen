#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
extern const u8 __lower_mapC[];
static inline int Lower(int c) {return c<0 || c>=128?c:__lower_mapC[c];}
int stricmp(const char *,const char *);
int strnicmp(const char *,const char *,u32);
int stricmp(const char *a,const char *b) {u8 x,y;do{x=(u8)Lower((u8)*a++);y=(u8)Lower((u8)*b++);if(x<y)return -1;if(x>y)return 1;}while(x!=0);return 0;}
int strnicmp(const char *a,const char *b,u32 count) {return stricmp(a,b);}
