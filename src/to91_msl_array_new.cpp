// clang-format off
#include <nitro/types.h>
#include <nitro/code32.h>
// clang-format on
#pragma force_active on
namespace std { class bad_alloc; struct nothrow_t {}; }
void*operator new(unsigned long) throw(std::bad_alloc);
void*operator new(unsigned long,const std::nothrow_t&) throw();
void*operator new[](unsigned long) throw(std::bad_alloc);
void*operator new[](unsigned long,const std::nothrow_t&) throw();
// @fn _Znam
void*operator new[](unsigned long count) throw(std::bad_alloc) {return operator new(count);}
// @fn _ZnamRKSt9nothrow_t
void*operator new[](unsigned long count,const std::nothrow_t&tag) throw() {return operator new(count,tag);}
