//
// Created by alina on 01.11.2020.
//

#ifndef TEST4_INTRUSIVE_PTR_H
#define TEST4_INTRUSIVE_PTR_H

template <typename T>
class TIntrusivePtr;

template<typename T>
class TRefCounter{
public:
    int RefCount() {return refcount;}
private:
    int refcount = 0;
    friend class TIntrusivePtr<T>;
};

template<typename T>
struct TBasePtr{
public:
    explicit TBasePtr(T* _x) :x(_x){}
    T* operator->() const {return x;}
    T& operator*() const {return *x;}
    bool operator==(const TBasePtr<T>& other) {
        return other.x == x;
    }
    bool operator!=(const TBasePtr<T>& other) {
        return other.x != x;
    }
    explicit operator bool() {
        return x != nullptr;
    }

protected:
    T* x;
};

template<typename T>
class TIntrusivePtr : public TBasePtr<T> {
public:
    constexpr TIntrusivePtr( std::nullptr_t _x) noexcept : TBasePtr<T>(_x) {}

    explicit TIntrusivePtr(T* _x) :TBasePtr<T>(_x) {
        if (_x) {
            _x->refcount +=1;
        }
    }
    TIntrusivePtr(const TIntrusivePtr<T>& other) : TBasePtr<T>(other.x){
        other->refcount ++;
    }
    TIntrusivePtr(TIntrusivePtr<T>&& other)  noexcept : TBasePtr<T>(other.x){
        this->x->refcount++;
        other.Reset();
    }
    TIntrusivePtr& operator=(const TIntrusivePtr& other) {
        Reset();
        this->x = other.x;
        this->x->refcount++;
        return *this;
    }

    void Reset(TIntrusivePtr<T> other = nullptr) {
        if (this->x) {
            this->x->refcount--;
            check_delete();
        }
        this->x = other.x;
        if (this->x) {
            this->x->refcount++;
        }
    }

    int UseCount(){return this->x? this->x->RefCount(): 0;}

    T* Get() {return this->x;}

    T* Release() {
        auto ret = this->x;
        if (this->x) {
            this->x->refcount--;
        }
        this->x = nullptr;
        return ret;
    }

    ~TIntrusivePtr() {
        if (this->x) {
            this->x->refcount--;
            if (this->x->RefCount() == 0) {
                delete this->x;
            }
        }
    }


private:
    void check_delete(){
        if (this->x->refcount == 0) {
            delete this->x;
        }
    }

};

template< class T, class... Args >
TIntrusivePtr<T> MakeIntrusive( Args&&... args ) {
    return TIntrusivePtr<T>(new T(args...));
}

#endif //TEST4_INTRUSIVE_PTR_H
