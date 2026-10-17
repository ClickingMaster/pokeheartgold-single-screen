#ifndef OVERLAY45_FORMAT_BUFFER_PRIVATE_H
#define OVERLAY45_FORMAT_BUFFER_PRIVATE_H
#include <nitro/types.h>
#ifdef __cplusplus
namespace LobbyStringTypes {
union Str {struct {u32 flag:1;u32 capacity:31;u32 size;char *data;} l;struct {u8 flag:1;u8 size:7;char data[11];} s;u32 words[3];};
class RangeDispatch {};
}
namespace LobbyCharVectorTypes {
struct Vec {char *ptr;u32 size,cap;};
class ResizeDispatch {};
class Allocator {
public:
 u32 recommend(Vec *vector,u32 growth);
};
}
using LobbyStringTypes::Str;
using LobbyCharVectorTypes::Vec;
extern "C" {
#else
typedef union Str {struct {u32 flag:1;u32 capacity:31;u32 size;char *data;} l;struct {u8 flag:1;u8 size:7;char data[11];} s;u32 words[3];} Str;
typedef struct Vec {char *ptr;u32 size,cap;} Vec;
#endif
Str *LobbyString_InitFromCStr(Str *,char *);
Str *LobbyString_Destroy(Str *);
Vec *LobbyCharVector_Destroy(Vec *);
char *LobbyCharVector_Data(Vec *);
u32 LobbyCharVector_RecommendSize(Vec *,u32);
void LobbyCharVector_AppendZerosSlow(Vec *,u32);
#ifdef __cplusplus
void LobbyCharVector_AppendZeros(Vec *,u32,LobbyCharVectorTypes::ResizeDispatch);
Str *LobbyString_ReplaceRange(Str *,u32,u32,char *,char *,LobbyStringTypes::RangeDispatch);
void LobbyString_Reserve(Str *,u32);
}
inline u32 LobbyCharVectorTypes::Allocator::recommend(Vec *vector,u32 growth){return LobbyCharVector_RecommendSize(vector,growth);}
#endif
#endif
