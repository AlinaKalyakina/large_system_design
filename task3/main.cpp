#include <iostream>
#include <memory>

class A {
public:
    static void* operator new(size_t size) {
        std::cout << "operator new!"<< std::endl;
        return ::operator new(size);
    }
    static void operator delete(void* p, size_t size) {
        std::cout<< "operator delete!" <<std::endl;
        return ::operator delete(p);
    }
};

template<typename T1, typename T2>
struct MyAlloc {
    typedef T1 value_type;

    MyAlloc() noexcept = default;
    template <class U1, class U2>
    constexpr MyAlloc (const MyAlloc<U1, U2>&) noexcept {};
    T1* allocate (std::size_t n) {
        return reinterpret_cast<T1*>( T2::operator new(n*sizeof(T1)));
    }
    void deallocate (T1* p, std::size_t n) {
        T2::operator delete(p, n*sizeof(T1));
    }
};

int main() {
    auto alloc = MyAlloc<A, A>();
    auto p = std::allocate_shared<A>(alloc);
}