#include <nitro/types.h>
#include <nitro/code32.h>
#pragma force_active on
typedef unsigned long size_t;
extern "C" void *malloc(size_t);
namespace std {
class exception {
public:
    virtual ~exception() throw();
    virtual const char *what() const throw();
};
class bad_alloc : public exception {
public:
    virtual ~bad_alloc() throw();
    virtual const char *what() const throw();
};
struct nothrow_t {};
typedef void (*new_handler)();
extern new_handler __new_handler;
void __throw_bad_alloc();
}
void *operator new(size_t) throw(std::bad_alloc);
void *operator new(size_t, const std::nothrow_t &) throw();
void *operator new(size_t size) throw(std::bad_alloc) {
    void *pointer;
    if (size == 0) size = 4;
    for (;;) {
        pointer = malloc(size);
        if (pointer != 0) break;
        if (std::__new_handler != 0) {
            std::__new_handler();
        } else {
            std::__throw_bad_alloc();
            break;
        }
    }
    return pointer;
}
void *operator new(size_t size, const std::nothrow_t &) throw() {
    try {
        return ::operator new(size);
    } catch (...) {
        return 0;
    }
}
