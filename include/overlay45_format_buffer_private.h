#ifndef OVERLAY45_FORMAT_BUFFER_PRIVATE_H
#define OVERLAY45_FORMAT_BUFFER_PRIVATE_H
#include <nitro/types.h>
#ifdef __cplusplus
namespace DwcStringConcat200 {
union Str {struct {u32 flag:1;u32 capacity:31;u32 size;char *data;} l;struct {u8 flag:1;u8 size:7;char data[11];} s;u32 words[3];};
class RangeDispatch {};
}
namespace DwcFormatted216Native {
struct Vec {char *ptr;u32 size,cap;};
class ResizeDispatch {};
class Allocator {
public:
 u32 recommend(Vec *vector,u32 growth);
};
}
using DwcStringConcat200::Str;
using DwcFormatted216Native::Vec;
extern "C" {
#else
typedef union Str {struct {u32 flag:1;u32 capacity:31;u32 size;char *data;} l;struct {u8 flag:1;u8 size:7;char data[11];} s;u32 words[3];} Str;
typedef struct Vec {char *ptr;u32 size,cap;} Vec;
#endif
Str *LobbyString_InitFromCStr(Str *,char *);
Str *LobbyString_Destroy(Str *);
Vec *ov45_022379D4(Vec *);
char *ov45_02232B9C(Vec *);
u32 ov45_02237890(Vec *,u32);
void ov45_02238820(Vec *,u32);
#ifdef __cplusplus
void ov45_0223795C(Vec *,u32,DwcFormatted216Native::ResizeDispatch);
Str *LobbyString_ReplaceRange(Str *,u32,u32,char *,char *,DwcStringConcat200::RangeDispatch);
void LobbyString_Reserve(Str *,u32);
}
inline u32 DwcFormatted216Native::Allocator::recommend(Vec *vector,u32 growth){return ov45_02237890(vector,growth);}
#endif
#endif
