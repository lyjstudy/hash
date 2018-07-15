/**
 * @file sha256.h
 * @author lyjstudy@gmail.com
 * @date 2018-07-15
 */
#pragma once

#include <cstdint>
#include <string>
#include "compact.h"

namespace fingera {

template<typename Instrinsic>
class ripemd160 {
public:
    using type = typename Instrinsic::type;
protected:
    static inline type vector_mirror(uint32_t x) {
        return Instrinsic::vector_mirror(x);
    }
    static inline type vector_add(type x) {
        return x;
    }
    template<typename ...Args>
    static inline type vector_add(type x, Args... rest) {
        return Instrinsic::vector_add(x, vector_add(rest...));
    }

    static inline type vector_xor(type x, type y) {
        return Instrinsic::vector_xor(x, y);
    }
    static inline type vector_xor(type x, type y, type z) {
        return vector_xor(vector_xor(x, y), z);
    }

    static inline type vector_and(type x, type y) {
        return Instrinsic::vector_and(x, y);
    }

    static inline type vector_andnot(type x, type y) {
        return Instrinsic::vector_andnot(x, y);
    }

    static inline type vector_f1(type x, type y, type z) {
        // x ^ y ^ z
        return vector_xor(x, y, z);
    }
    static inline type vector_f2(type x, type y, type z) {
        // (x & y) | (~x & z)
        return vector_or(vector_and(x, y), vector_andnot(x, z));
    }
    static inline type vector_f3(type x, type y, type z) {
        // (x | ~y) ^ z
        return vector_xor(vector_or(x, vector_xor(y, vector_mirror(0xFFFFFFFFul))), z);
    }
    static inline type vector_f4(type x, type y, type z) {
        // (x & z) | (y & ~z);
        return vector_or(vector_and(x, z), vector_andnot(z, y));
    }
    static inline type vector_f5(type x, type y, type z) {
        // x ^ (y | ~z)
        return vector_xor(x, vector_or(y, vector_xor(z, vector_mirror(0xFFFFFFFFul))));
    }

    template<int N>
    static inline type vector_rol(type x) {
        return Instrinsic::template vector_rol<N>(x);
    }

    static inline void round(type& a, type b, type& c, type d, type e, type f, type x, type k, int r) {
        /*
        a = rol(a + f + x + k, r) + e;
        c = rol(c, 10);
        */
        a = vector_add(vector_rol(vector_add(a, f, x, k), r));
        c = vector_rol(c, 10);
    }

    static inline void R11(type& a, type b, type& c, type d, type e, type x, int r) {
        round(a, b, c, d, e, vector_f1(b, c, d), x, 0, r);
    }
    static inline void R21(type& a, type b, type& c, type d, type e, type x, int r) {
        round(a, b, c, d, e, vector_f2(b, c, d), x, 0x5A827999ul, r);
    }
    static inline void R31(type& a, type b, type& c, type d, type e, type x, int r) {
        round(a, b, c, d, e, vector_f3(b, c, d), x, 0x6ED9EBA1ul, r);
    }
    static inline void R41(type& a, type b, type& c, type d, type e, type x, int r) {
        round(a, b, c, d, e, vector_f4(b, c, d), x, 0x8F1BBCDCul, r);
    }
    static inline void R51(type& a, type b, type& c, type d, type e, type x, int r) {
        round(a, b, c, d, e, vector_f5(b, c, d), x, 0xA953FD4Eul, r);
    }
    static inline void R12(type& a, type b, type& c, type d, type e, type x, int r) {
        round(a, b, c, d, e, vector_f5(b, c, d), x, 0x50A28BE6ul, r);
    }
    static inline void R22(type& a, type b, type& c, type d, type e, type x, int r) {
        round(a, b, c, d, e, vector_f4(b, c, d), x, 0x5C4DD124ul, r);
    }
    static inline void R32(type& a, type b, type& c, type d, type e, type x, int r) {
        round(a, b, c, d, e, vector_f3(b, c, d), x, 0x6D703EF3ul, r);
    }
    static inline void R42(type& a, type b, type& c, type d, type e, type x, int r) {
        round(a, b, c, d, e, vector_f2(b, c, d), x, 0x7A6D76E9ul, r);
    }
    static inline void R52(type& a, type b, type& c, type d, type e, type x, int r) {
        round(a, b, c, d, e, vector_f1(b, c, d), x, 0, r);
    }

};

} // namespace fingera
