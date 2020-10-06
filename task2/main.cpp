#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <sstream>
#include <set>
#include <map>

struct Printer {
    std::string str() const {
        return ss.str();
    }; // возвращает строковое представление
// далее перегруженные/шаблонные функции вида:
    template<typename T>
    Printer& format(const T& v) {
        ss << v;
        return *this;
    };

    template <typename T1, typename T2>
    Printer& format(const std::pair<T1, T2>& v) {
        ss << '(';
        format(v.first);
        format(',');
        format(v.second);
        ss << ')';
        return *this;
    }

    template <typename T>
    Printer& format(const std::vector<T>& v) {
        ss << "[ ";
        for (auto& x : v ) {
            format(x);
            format(", ");
        }
        ss.seekp(-2, std::stringstream::cur);
        ss << " ]";
        return *this;
    }
    template <typename T>
    Printer& format(const std::set<T>& v) {
        ss << "{ ";
        for (auto& x : v ) {
            format(x);
            format(", ");
        }
        ss.seekp(-1, std::stringstream::cur);
        ss << "} ";
        return *this;
    }

    template <typename...Args>
    Printer& format(const std::tuple<Args...>& v) {
        ss << "( ";
        tuple_format<0, Args...>(v);
        ss.seekp(-2, std::stringstream::cur);
        ss << " )";
        return *this;
    }

    template<int Pos, typename...Args>
    void tuple_format(const std::tuple<Args...>& v) {
        if constexpr (Pos > sizeof...(Args) - 1) {
            return;
        } else {
            format(std::get<Pos>(v));
            format(", ");
            tuple_format<Pos + 1>(v);
        }
    }


private:
    std::stringstream ss;
};

template<typename T>
std::string format(const T& t) {
    return Printer().format(t).str();
}

int main() {
    std::tuple<std::string, int, int> t = {"xyz", 1, 2};
    std::vector<std::pair<int, int> > v = {{1, 4}, {5, 6}};
    std::string s1 = Printer().format(" vector: ").format(v).str();
// " vector: [ (1, 4), (5, 6) ]"
    std::string s2 = Printer().format(t).format(" ! ").format(0).str();
    std::cout << s1 << std::endl << s2;
// "( xyz, 1, 2 ) ! 0"
}
