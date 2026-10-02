#pragma force_active on
#include <nitro/types.h>
#pragma force_active on
extern const char __local_str__msl_assertion_failed[];int printf(const char*,...);void abort(void);void __msl_assertion_failed(const char*,const char*,const char*,int);
void __msl_assertion_failed(const char*a,const char*b,const char*c,int d){printf(__local_str__msl_assertion_failed,a,b,c,d);abort();}
