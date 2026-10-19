#ifndef POKEHEARTGOLD_LOBBY_VECTOR_H
#define POKEHEARTGOLD_LOBBY_VECTOR_H

// The lobby vectors (overlay 45) and the buffers they grow through: a growing vector builds its new storage in a
// buffer, filled from start, and then takes the buffer's storage over.

#include <nitro/types.h>

#include "lobby_string.h"

// The empty tag objects (allocators, type traits) the container code passes by value. C has no empty structs, so
// its view is one byte, the size of the C++ one.
#ifdef __cplusplus
struct LobbyTag {};
#else
typedef struct LobbyTag {
    u8 unused;
} LobbyTag;
#endif

typedef struct LobbyCharVector {
    char *data;
    u32 size;
    u32 capacity;
} LobbyCharVector;

typedef struct LobbyU32Vector {
    u32 *data;
    u32 size;
    u32 capacity;
} LobbyU32Vector;

typedef struct LobbyStringVector {
    Str *data;
    u32 size;
    u32 capacity;
} LobbyStringVector;

// A growth buffer's capacity, kept together with a pointer to the capacity of the vector it grows (where that vector
// keeps its allocator).
typedef struct LobbyVectorBufferCapacity {
    u32 value;
    u32 *allocator;
} LobbyVectorBufferCapacity;

typedef struct LobbyCharVectorBuffer {
    char *data;
    u32 size;
    LobbyVectorBufferCapacity capacity;
    u32 start;
} LobbyCharVectorBuffer;

typedef struct LobbyStringVectorBuffer {
    Str *data;
    u32 size;
    LobbyVectorBufferCapacity capacity;
    u32 start;
} LobbyStringVectorBuffer;

#ifdef __cplusplus
extern "C" {
#endif

LobbyCharVectorBuffer *LobbyCharVectorBuffer_Destroy(LobbyCharVectorBuffer *);
void LobbyCharVectorBuffer_Allocate(LobbyCharVectorBuffer *, u32, LobbyTag);
void LobbyStringVector_EraseAtEnd(LobbyStringVector *, u32, LobbyTag);
void LobbyStringVectorBuffer_Allocate(LobbyStringVectorBuffer *, u32, LobbyTag);
LobbyStringVectorBuffer *LobbyStringVectorBuffer_Deallocate(LobbyStringVectorBuffer *);
LobbyStringVectorBuffer *LobbyStringVectorBuffer_Destroy(LobbyStringVectorBuffer *);

#ifdef __cplusplus
}
#endif

#endif // POKEHEARTGOLD_LOBBY_VECTOR_H
