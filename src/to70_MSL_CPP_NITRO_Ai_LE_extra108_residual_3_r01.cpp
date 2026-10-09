#include <nitro/types.h>
#include <nitro/code32.h>
#pragma force_active on

extern "C" {
unsigned long strlen(const char *) throw();
char *strcpy(char *, const char *) throw();
}

namespace std {

namespace detail {
template <class T> struct default_delete {};
}

namespace tr1 {
template <class T> class shared_ptr {
public:
    template <class Y, class D> shared_ptr(Y *p, D d);
    ~shared_ptr() throw();
    T *get() const { return pointer; }

private:
    T *pointer;
    void *control;
};
}

// Owned copy of the message (the logic_error member destroyed during unwinding).
class __error_string {
public:
    explicit __error_string(const char *s) : str(new char[strlen(s) + 1], detail::default_delete<char[]>()) {
        strcpy(str.get(), s);
    }
    ~__error_string() throw();

private:
    tr1::shared_ptr<char> str;
};

class exception {
public:
    exception() throw() {}
    virtual ~exception() throw();
};

class logic_error : public exception {
public:
    explicit logic_error(const char *s) : message(s) {}
    virtual ~logic_error() throw();

private:
    __error_string message;
};

class length_error : public logic_error {
public:
    explicit length_error(const char *s) : logic_error(s) {}
    virtual ~length_error() throw();
};

template <bool b> struct __vector_common {
    static void throw_length_error();
};

// @fn _ZNSt15__vector_commonILb1EE18throw_length_errorEv
template <> void __vector_common<true>::throw_length_error() {
    throw length_error("vector length error");
}

}
