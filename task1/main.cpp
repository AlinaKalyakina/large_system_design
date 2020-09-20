#include <iostream>
#include <array>

template <int N>
constexpr int det(const std::array<std::array<int, N>, N>& a) {
    int m = 1;
    int res = 0;
    for (int i = 0; i < N; i++) {
        std::array<std::array<int, N-1>, N-1> new_array {};
        for (int k = 0; k < N-1; k++) {
            int real_l = 0;
            for (int l = 0; l < N; l++) {
                if (l != i) {
                    new_array[k][real_l] = a[k+1][l];
                    real_l++;
                }
            }
        }
        res += m*a[0][i] * det<N-1>(new_array);
        m *= -1;
    }
    return res;
}

template <>
constexpr int det<1>(const std::array<std::array<int, 1>, 1>& a) {return a[0][0];}


int main() {
    constexpr std::array<std::array<int, 3>, 3> A = {{
                                                   {0, 1, 2}
                                                   ,{1, 2, 3}
                                                   ,{2, 3, 7}
                                           }};
    constexpr int res = det<3>(A);
    static_assert(res == -3);
}
