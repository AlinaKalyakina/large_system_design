#include <memory>
#include <cassert>
#include <string>

class Any {
public:
    template <typename T, typename = std::enable_if_t<!std::is_same<Any, std::decay_t<T>>::value>>
    explicit Any(T&& val):v(std::make_unique<THolder < std::decay_t<T>> > (std::forward<T>(val))){}

    template <typename U>
    U get() const { return dynamic_cast<THolder<std::decay_t<U>>&>(*v).get(); };

    template <typename U, typename = std::enable_if_t<!std::is_same<Any, std::decay_t<U>>::value>>
    void set(U&& val) {v = std::make_unique<THolder < std::decay_t<U>> > (std::forward<U>(val));}

private:
    class IFunctor {
    public:
        virtual ~IFunctor() = default;
    };

    template <typename F>
    class THolder: public IFunctor {
    private:
        std::decay_t<F> f;
    public:
        explicit THolder(F f) : f(std::move(f)){}
        F get() const {return f;}
    };

    std::unique_ptr<IFunctor> v;
};

int main() {
    Any a(5);
    assert(a.get<int>() == 5); // 5
    try {
        a.get<std::string>();
        assert(false);
    } catch(...) {}
    a.set(std::string("123"));
    assert(a.get<std::string>() == "123");
}