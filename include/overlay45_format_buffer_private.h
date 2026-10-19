#ifndef OVERLAY45_FORMAT_BUFFER_PRIVATE_H
#define OVERLAY45_FORMAT_BUFFER_PRIVATE_H

#include <nitro/types.h>

#include "lobby_string.h"
#include "lobby_vector.h"

#ifdef __cplusplus
namespace LobbyStringTypes {
class RangeDispatch {};
}

namespace LobbyCharVectorTypes {
class ResizeDispatch {};

class Allocator {
public:
    u32 recommend(LobbyCharVector *vector, u32 growth);
};
}

extern "C" {
#endif

Str *LobbyString_InitFromCStr(Str *, char *);
Str *LobbyString_Destroy(Str *);
LobbyCharVector *LobbyCharVector_Destroy(LobbyCharVector *);
char *LobbyCharVector_Data(LobbyCharVector *);
u32 LobbyCharVector_RecommendSize(LobbyCharVector *, u32);
void LobbyCharVector_AppendZerosSlow(LobbyCharVector *, u32);

#ifdef __cplusplus
void LobbyCharVector_AppendZeros(LobbyCharVector *, u32, LobbyCharVectorTypes::ResizeDispatch);
Str *LobbyString_ReplaceRange(Str *, u32, u32, char *, char *, LobbyStringTypes::RangeDispatch);
void LobbyString_Reserve(Str *, u32);
}

inline u32 LobbyCharVectorTypes::Allocator::recommend(LobbyCharVector *vector, u32 growth) {
    return LobbyCharVector_RecommendSize(vector, growth);
}
#endif

#endif
