#ifndef OVERLAY45_STRING_SEARCH_H
#define OVERLAY45_STRING_SEARCH_H
#include <nitro/types.h>
#ifdef __cplusplus
struct StringSearchRange {const s8 *data;u32 size;inline StringSearchRange(const s8*p,u32 n):data(p),size(n){}};
#endif
typedef struct StringSearch {
    union {
        struct {u32 flag:1,capacity:31;u32 size;s8 *data;} longForm;
        struct {u8 flag:1,size:7;s8 data[11];} shortForm;
    } raw;
#ifdef __cplusplus
    inline StringSearchRange GetRange() const;
    inline int Find(const StringSearch &other,u32 start) const;
#endif
} StringSearch;
typedef char StringSearchSize12[sizeof(StringSearch)==12?1:-1];
#ifdef __cplusplus
extern "C" {
#endif
int ov45_02237EB0(const StringSearch *,const StringSearch *,u32);
u32 ov45_02237EE4(const StringSearch *,const s8 *,u32,u32);
#ifdef __cplusplus
}
#endif
#endif
