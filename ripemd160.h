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

    static inline type vector_or(type x, type y) {
        return Instrinsic::vector_or(x, y);
    }
    
    static inline type vector_and(type x, type y) {
        return Instrinsic::vector_and(x, y);
    }

    static inline type vector_andnot(type x, type y) {
        return Instrinsic::vector_andnot(x, y);
    }
    template<int N>
    static inline type vector_rol(type x) {
        return Instrinsic::template vector_rol<N>(x);
    }

    static inline type vector_f1(type x, type y, type z) {
        return x ^ y ^ z;
        //return vector_xor(x, y, z);
    }
    static inline type vector_f2(type x, type y, type z) {
        return (x & y) | (~x & z);
        //return vector_or(vector_and(x, y), vector_andnot(x, z));
    }
    static inline type vector_f3(type x, type y, type z) {
        return (x | ~y) ^ z;
        //return vector_xor(vector_or(x, vector_xor(y, vector_mirror(0xFFFFFFFFul))), z);
    }
    static inline type vector_f4(type x, type y, type z) {
        return (x & z) | (y & ~z);
        //return vector_or(vector_and(x, z), vector_andnot(z, y));
    }
    static inline type vector_f5(type x, type y, type z) {
        return x ^ (y | ~z);
        //return vector_xor(x, vector_or(y, vector_xor(z, vector_mirror(0xFFFFFFFFul))));
    }

    template<int N>
    static inline void round(type& a, type b, type& c, type d, type e, type f, type x, type k) {
        /*
            a = rol(a + f + x + k, r) + e;
            c = rol(c, 10);
        */
        a = vector_add(vector_rol<N>(vector_add(a, f, x, k)));
        c = vector_rol<10>(c);
    }

    template<int N>
    static inline void R11(type& a, type b, type& c, type d, type e, type x) {
        round<N>(a, b, c, d, e, vector_f1(b, c, d), x, 0);
    }
    template<int N>
    static inline void R21(type& a, type b, type& c, type d, type e, type x) {
        round<N>(a, b, c, d, e, vector_f2(b, c, d), x, 0x5A827999ul);
    }
    template<int N>
    static inline void R31(type& a, type b, type& c, type d, type e, type x) {
        round<N>(a, b, c, d, e, vector_f3(b, c, d), x, 0x6ED9EBA1ul);
    }
    template<int N>
    static inline void R41(type& a, type b, type& c, type d, type e, type x) {
        round<N>(a, b, c, d, e, vector_f4(b, c, d), x, 0x8F1BBCDCul);
    }
    template<int N>
    static inline void R51(type& a, type b, type& c, type d, type e, type x) {
        round<N>(a, b, c, d, e, vector_f5(b, c, d), x, 0xA953FD4Eul);
    }
    template<int N>
    static inline void R12(type& a, type b, type& c, type d, type e, type x) {
        round<N>(a, b, c, d, e, vector_f5(b, c, d), x, 0x50A28BE6ul);
    }
    template<int N>
    static inline void R22(type& a, type b, type& c, type d, type e, type x) {
        round<N>(a, b, c, d, e, vector_f4(b, c, d), x, 0x5C4DD124ul);
    }
    template<int N>
    static inline void R32(type& a, type b, type& c, type d, type e, type x) {
        round<N>(a, b, c, d, e, vector_f3(b, c, d), x, 0x6D703EF3ul);
    }
    template<int N>
    static inline void R42(type& a, type b, type& c, type d, type e, type x) {
        round<N>(a, b, c, d, e, vector_f2(b, c, d), x, 0x7A6D76E9ul);
    }
    template<int N>
    static inline void R52(type& a, type b, type& c, type d, type e, type x) {
        round<N>(a, b, c, d, e, vector_f1(b, c, d), x, 0);
    }

public:
    static inline size_t way() {
        return sizeof(type) / sizeof(uint32_t);
    }

    static inline void process_block(
            type &a1, type &b1, type &c1, type &d1, type &e1,
            const void *block) {
        type a2 = a1, b2 = b1, c2 = c1, d2 = d1, e2 = e1;
        type oa = a1, ob = b1, oc = c1, od = d1, oe = e1;

        const type *w = (type*)block;
        type w0 = w[0], w1 = w[1], w2 = w[2], w3 = w[3];
        type w4 = w[4], w5 = w[5], w6 = w[6], w7 = w[7];
        type w8 = w[8], w9 = w[9], w10 = w[10], w11 = w[11];
        type w12 = w[12], w13 = w[13], w14 = w[14], w15 = w[15];

#define __R11(a, b, c, d, e, f, g) R11<g>(a, b, c, d, e, f);
#define __R21(a, b, c, d, e, f, g) R21<g>(a, b, c, d, e, f);
#define __R31(a, b, c, d, e, f, g) R31<g>(a, b, c, d, e, f);
#define __R41(a, b, c, d, e, f, g) R41<g>(a, b, c, d, e, f);
#define __R51(a, b, c, d, e, f, g) R51<g>(a, b, c, d, e, f);
#define __R12(a, b, c, d, e, f, g) R12<g>(a, b, c, d, e, f);
#define __R22(a, b, c, d, e, f, g) R22<g>(a, b, c, d, e, f);
#define __R32(a, b, c, d, e, f, g) R32<g>(a, b, c, d, e, f);
#define __R42(a, b, c, d, e, f, g) R42<g>(a, b, c, d, e, f);
#define __R52(a, b, c, d, e, f, g) R52<g>(a, b, c, d, e, f);

        __R11(a1, b1, c1, d1, e1, w0, 11);
        __R12(a2, b2, c2, d2, e2, w5, 8);
        __R11(e1, a1, b1, c1, d1, w1, 14);
        __R12(e2, a2, b2, c2, d2, w14, 9);
        __R11(d1, e1, a1, b1, c1, w2, 15);
        __R12(d2, e2, a2, b2, c2, w7, 9);
        __R11(c1, d1, e1, a1, b1, w3, 12);
        __R12(c2, d2, e2, a2, b2, w0, 11);
        __R11(b1, c1, d1, e1, a1, w4, 5);
        __R12(b2, c2, d2, e2, a2, w9, 13);
        __R11(a1, b1, c1, d1, e1, w5, 8);
        __R12(a2, b2, c2, d2, e2, w2, 15);
        __R11(e1, a1, b1, c1, d1, w6, 7);
        __R12(e2, a2, b2, c2, d2, w11, 15);
        __R11(d1, e1, a1, b1, c1, w7, 9);
        __R12(d2, e2, a2, b2, c2, w4, 5);
        __R11(c1, d1, e1, a1, b1, w8, 11);
        __R12(c2, d2, e2, a2, b2, w13, 7);
        __R11(b1, c1, d1, e1, a1, w9, 13);
        __R12(b2, c2, d2, e2, a2, w6, 7);
        __R11(a1, b1, c1, d1, e1, w10, 14);
        __R12(a2, b2, c2, d2, e2, w15, 8);
        __R11(e1, a1, b1, c1, d1, w11, 15);
        __R12(e2, a2, b2, c2, d2, w8, 11);
        __R11(d1, e1, a1, b1, c1, w12, 6);
        __R12(d2, e2, a2, b2, c2, w1, 14);
        __R11(c1, d1, e1, a1, b1, w13, 7);
        __R12(c2, d2, e2, a2, b2, w10, 14);
        __R11(b1, c1, d1, e1, a1, w14, 9);
        __R12(b2, c2, d2, e2, a2, w3, 12);
        __R11(a1, b1, c1, d1, e1, w15, 8);
        __R12(a2, b2, c2, d2, e2, w12, 6);
        __R21(e1, a1, b1, c1, d1, w7, 7);
        __R22(e2, a2, b2, c2, d2, w6, 9);
        __R21(d1, e1, a1, b1, c1, w4, 6);
        __R22(d2, e2, a2, b2, c2, w11, 13);
        __R21(c1, d1, e1, a1, b1, w13, 8);
        __R22(c2, d2, e2, a2, b2, w3, 15);
        __R21(b1, c1, d1, e1, a1, w1, 13);
        __R22(b2, c2, d2, e2, a2, w7, 7);
        __R21(a1, b1, c1, d1, e1, w10, 11);
        __R22(a2, b2, c2, d2, e2, w0, 12);
        __R21(e1, a1, b1, c1, d1, w6, 9);
        __R22(e2, a2, b2, c2, d2, w13, 8);
        __R21(d1, e1, a1, b1, c1, w15, 7);
        __R22(d2, e2, a2, b2, c2, w5, 9);
        __R21(c1, d1, e1, a1, b1, w3, 15);
        __R22(c2, d2, e2, a2, b2, w10, 11);
        __R21(b1, c1, d1, e1, a1, w12, 7);
        __R22(b2, c2, d2, e2, a2, w14, 7);
        __R21(a1, b1, c1, d1, e1, w0, 12);
        __R22(a2, b2, c2, d2, e2, w15, 7);
        __R21(e1, a1, b1, c1, d1, w9, 15);
        __R22(e2, a2, b2, c2, d2, w8, 12);
        __R21(d1, e1, a1, b1, c1, w5, 9);
        __R22(d2, e2, a2, b2, c2, w12, 7);
        __R21(c1, d1, e1, a1, b1, w2, 11);
        __R22(c2, d2, e2, a2, b2, w4, 6);
        __R21(b1, c1, d1, e1, a1, w14, 7);
        __R22(b2, c2, d2, e2, a2, w9, 15);
        __R21(a1, b1, c1, d1, e1, w11, 13);
        __R22(a2, b2, c2, d2, e2, w1, 13);
        __R21(e1, a1, b1, c1, d1, w8, 12);
        __R22(e2, a2, b2, c2, d2, w2, 11);
        __R31(d1, e1, a1, b1, c1, w3, 11);
        __R32(d2, e2, a2, b2, c2, w15, 9);
        __R31(c1, d1, e1, a1, b1, w10, 13);
        __R32(c2, d2, e2, a2, b2, w5, 7);
        __R31(b1, c1, d1, e1, a1, w14, 6);
        __R32(b2, c2, d2, e2, a2, w1, 15);
        __R31(a1, b1, c1, d1, e1, w4, 7);
        __R32(a2, b2, c2, d2, e2, w3, 11);
        __R31(e1, a1, b1, c1, d1, w9, 14);
        __R32(e2, a2, b2, c2, d2, w7, 8);
        __R31(d1, e1, a1, b1, c1, w15, 9);
        __R32(d2, e2, a2, b2, c2, w14, 6);
        __R31(c1, d1, e1, a1, b1, w8, 13);
        __R32(c2, d2, e2, a2, b2, w6, 6);
        __R31(b1, c1, d1, e1, a1, w1, 15);
        __R32(b2, c2, d2, e2, a2, w9, 14);
        __R31(a1, b1, c1, d1, e1, w2, 14);
        __R32(a2, b2, c2, d2, e2, w11, 12);
        __R31(e1, a1, b1, c1, d1, w7, 8);
        __R32(e2, a2, b2, c2, d2, w8, 13);
        __R31(d1, e1, a1, b1, c1, w0, 13);
        __R32(d2, e2, a2, b2, c2, w12, 5);
        __R31(c1, d1, e1, a1, b1, w6, 6);
        __R32(c2, d2, e2, a2, b2, w2, 14);
        __R31(b1, c1, d1, e1, a1, w13, 5);
        __R32(b2, c2, d2, e2, a2, w10, 13);
        __R31(a1, b1, c1, d1, e1, w11, 12);
        __R32(a2, b2, c2, d2, e2, w0, 13);
        __R31(e1, a1, b1, c1, d1, w5, 7);
        __R32(e2, a2, b2, c2, d2, w4, 7);
        __R31(d1, e1, a1, b1, c1, w12, 5);
        __R32(d2, e2, a2, b2, c2, w13, 5);
        __R41(c1, d1, e1, a1, b1, w1, 11);
        __R42(c2, d2, e2, a2, b2, w8, 15);
        __R41(b1, c1, d1, e1, a1, w9, 12);
        __R42(b2, c2, d2, e2, a2, w6, 5);
        __R41(a1, b1, c1, d1, e1, w11, 14);
        __R42(a2, b2, c2, d2, e2, w4, 8);
        __R41(e1, a1, b1, c1, d1, w10, 15);
        __R42(e2, a2, b2, c2, d2, w1, 11);
        __R41(d1, e1, a1, b1, c1, w0, 14);
        __R42(d2, e2, a2, b2, c2, w3, 14);
        __R41(c1, d1, e1, a1, b1, w8, 15);
        __R42(c2, d2, e2, a2, b2, w11, 14);
        __R41(b1, c1, d1, e1, a1, w12, 9);
        __R42(b2, c2, d2, e2, a2, w15, 6);
        __R41(a1, b1, c1, d1, e1, w4, 8);
        __R42(a2, b2, c2, d2, e2, w0, 14);
        __R41(e1, a1, b1, c1, d1, w13, 9);
        __R42(e2, a2, b2, c2, d2, w5, 6);
        __R41(d1, e1, a1, b1, c1, w3, 14);
        __R42(d2, e2, a2, b2, c2, w12, 9);
        __R41(c1, d1, e1, a1, b1, w7, 5);
        __R42(c2, d2, e2, a2, b2, w2, 12);
        __R41(b1, c1, d1, e1, a1, w15, 6);
        __R42(b2, c2, d2, e2, a2, w13, 9);
        __R41(a1, b1, c1, d1, e1, w14, 8);
        __R42(a2, b2, c2, d2, e2, w9, 12);
        __R41(e1, a1, b1, c1, d1, w5, 6);
        __R42(e2, a2, b2, c2, d2, w7, 5);
        __R41(d1, e1, a1, b1, c1, w6, 5);
        __R42(d2, e2, a2, b2, c2, w10, 15);
        __R41(c1, d1, e1, a1, b1, w2, 12);
        __R42(c2, d2, e2, a2, b2, w14, 8);
        __R51(b1, c1, d1, e1, a1, w4, 9);
        __R52(b2, c2, d2, e2, a2, w12, 8);
        __R51(a1, b1, c1, d1, e1, w0, 15);
        __R52(a2, b2, c2, d2, e2, w15, 5);
        __R51(e1, a1, b1, c1, d1, w5, 5);
        __R52(e2, a2, b2, c2, d2, w10, 12);
        __R51(d1, e1, a1, b1, c1, w9, 11);
        __R52(d2, e2, a2, b2, c2, w4, 9);
        __R51(c1, d1, e1, a1, b1, w7, 6);
        __R52(c2, d2, e2, a2, b2, w1, 12);
        __R51(b1, c1, d1, e1, a1, w12, 8);
        __R52(b2, c2, d2, e2, a2, w5, 5);
        __R51(a1, b1, c1, d1, e1, w2, 13);
        __R52(a2, b2, c2, d2, e2, w8, 14);
        __R51(e1, a1, b1, c1, d1, w10, 12);
        __R52(e2, a2, b2, c2, d2, w7, 6);
        __R51(d1, e1, a1, b1, c1, w14, 5);
        __R52(d2, e2, a2, b2, c2, w6, 8);
        __R51(c1, d1, e1, a1, b1, w1, 12);
        __R52(c2, d2, e2, a2, b2, w2, 13);
        __R51(b1, c1, d1, e1, a1, w3, 13);
        __R52(b2, c2, d2, e2, a2, w13, 6);
        __R51(a1, b1, c1, d1, e1, w8, 14);
        __R52(a2, b2, c2, d2, e2, w14, 5);
        __R51(e1, a1, b1, c1, d1, w11, 11);
        __R52(e2, a2, b2, c2, d2, w0, 15);
        __R51(d1, e1, a1, b1, c1, w6, 8);
        __R52(d2, e2, a2, b2, c2, w3, 13);
        __R51(c1, d1, e1, a1, b1, w15, 5);
        __R52(c2, d2, e2, a2, b2, w9, 11);
        __R51(b1, c1, d1, e1, a1, w13, 6);
        __R52(b2, c2, d2, e2, a2, w11, 11);

        a1 = ob + c1 + d2;
        b1 = oc + d1 + e2;
        c1 = od + e1 + a2;
        d1 = oe + a1 + b2;
        e1 = oa + b1 + c2;

#undef __R11
#undef __R21
#undef __R31
#undef __R41
#undef __R51
#undef __R12
#undef __R22
#undef __R32
#undef __R42
#undef __R52
    }

    static void process_trunk(void *out, const void *blocks, int count = 1) {
        type a = vector_mirror(0x67452301ul);
        type b = vector_mirror(0xEFCDAB89ul);
        type c = vector_mirror(0x98BADCFEul);
        type d = vector_mirror(0x10325476ul);
        type e = vector_mirror(0xC3D2E1F0ul);

        char *cur_block = (char *)blocks;
        while (count--) {
            process_block(a, b, c, d, e, cur_block);
            cur_block += 64 * way();
        }

        Instrinsic::save(out,  0, a, 20);
        Instrinsic::save(out,  4, b, 20);
        Instrinsic::save(out,  8, c, 20);
        Instrinsic::save(out, 12, d, 20);
        Instrinsic::save(out, 16, e, 20);
    }
};

} // namespace fingera
