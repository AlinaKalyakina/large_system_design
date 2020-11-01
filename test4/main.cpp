#include <iostream>
#include <cassert>
#include "intrusive_ptr.h"
class TDoc: public TRefCounter<TDoc> {
public:
    TDoc() { array = new int;}
    ~TDoc() {delete array;}
    int* array;
};

int main() {
    TIntrusivePtr<TDoc> ptr = nullptr;
    ptr = MakeIntrusive<TDoc>();
    assert((ptr.UseCount() == 1));
    TIntrusivePtr<TDoc> ptr2 = ptr;
    assert(ptr.UseCount() == 2);
    TIntrusivePtr<TDoc> ptr3 = MakeIntrusive<TDoc>();
    ptr3.Reset(ptr2);
    assert( ptr.UseCount() == ptr3.UseCount() && ptr3.UseCount() == 3);
    ptr3.Reset();
    assert(ptr.UseCount() == 2 && ptr3.UseCount() == 0);
    ptr3.Reset(std::move(ptr2));
    assert(ptr->RefCount() == ptr3.UseCount() && ptr3.UseCount() == 2 &&  (ptr2.UseCount() == 0));
    static_assert(sizeof(TDoc*) == sizeof(ptr));
}
