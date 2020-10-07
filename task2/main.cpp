//Только >= с++17!
#include <iostream>
#include <vector>
#include <string>
#include <tuple>
#include <sstream>
#include <set>

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
        ss << "(";
        tuple_format<0>(std::make_tuple(v.first, v.second));
        ss << ")";
        return *this;
    }

    template<typename T>
    Printer& format(const std::vector<T>& v) {
        ss << "[ ";
        format_container(v);
        ss << " ]";
        return *this;
    }

    template<typename T>
    Printer& format(const std::set<T>& v) {
        ss << "{ ";
        format_container(v);
        ss << " }";
    }


    template <typename...Args>
    Printer& format(const std::tuple<Args...>& v) {
        ss << "( ";
        tuple_format<0>(v);
        ss << " )";
        return *this;
    }
private:
    std::stringstream ss;

    template<template<typename> class ContainerT, typename ValueT>
    void format_container(const ContainerT<ValueT>& v) {
        for (auto&& x : v ) {
            format(x);
            ss << ", ";
        }
        ss.seekp(-2, std::stringstream::cur);
    }

    template<int Pos, typename...Args>
    void tuple_format(const std::tuple<Args...>& v) {
        if constexpr (Pos >= sizeof...(Args)) {
            ss.seekp(-2, std::stringstream::cur);
            return;
        } else {
            format(std::get<Pos>(v));
            ss << ", ";
            tuple_format<Pos + 1>(v);
        }
    }
};

template<typename T>
std::string format(const T& t) {
    return Printer().format(t).str();
}

int main() {
    std::tuple<std::string, int, int> t = {"xyz", 1, 2};
    std::vector<std::pair<int, int> > v = {{1, 4}, {5, 6}};
    std::string s1 = Printer().format(" vector: ").format(v).str();
    std::pair<int, int> m = {1, 1};
// " vector: [ (1, 4), (5, 6) ]"
    std::string s2 = Printer().format(t).format(" ! ").format(0).str();
    std::cout << s1 << std::endl << s2;
// "( xyz, 1, 2 ) ! 0"
}