#include <bits/stdc++.h>

namespace algorithm {
    template<class T>
    std::vector<std::complex<T>> fft_inner(std::vector<std::complex<T>> x, size_t offset, size_t step, size_t N) {
        if (N == 1) {
            return {x[offset]};
        }
        auto ret = std::vector<std::complex<T>>{};
        ret.resize(N);
        auto e = fft_inner(x, offset, step*2, N/2);
        auto o = fft_inner(x, offset+step, step*2, N/2);
        for (size_t k = 0; k < N/2; k++) {
            auto p = e[k];
            auto q = o[k]*std::complex<T>{cos(2*M_PI*k/N), -sin(2*M_PI*k/N)};
            ret[k] = p + q;
            ret[k + N/2] = p - q; 
        }
        return ret;
    }


    template<class T>
    std::vector<std::complex<T>> fft(std::vector<std::complex<T>> x) {
        int N = x.size();
        assert(((-N) & N) == N);
        return fft_inner(x, 0, 1, N);
    } 
}