#include <nitro/types.h>
#include <nitro/code32.h>
#pragma force_active on
namespace std {
class type_info;
namespace tr1 { namespace detail {
class shared_ptr_deleter_common {
public:
    virtual ~shared_ptr_deleter_common();
    virtual void *get_deleter(const std::type_info &) const = 0;
    virtual void dispose() = 0;
    void release();
    void release_weak();
private:
    int references;
    int weakReferences;
};
}
template<class T> class shared_ptr {
public:
~shared_ptr() throw();
private:
T*pointer;detail::shared_ptr_deleter_common*control;
};
template<> shared_ptr<char>::~shared_ptr() throw() {
if(control)control->release();
}
} }
