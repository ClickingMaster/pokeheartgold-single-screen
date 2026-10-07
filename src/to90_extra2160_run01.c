#pragma force_active on
#include <nitro/types.h>
// clang-format off
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
typedef struct{u32*data;int count;}Big;int BN_num_bits_word(u32),BN_num_bits(const Big*);
int BN_num_bits(const Big*n){int last;if(!n->count)return 0;last=n->count-1;return BN_num_bits_word(n->data[last])+(last<<5);}
