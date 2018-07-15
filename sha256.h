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
class sha256 {
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

    template<typename ...Args>
    static inline type vector_inc(type &x, Args... rest) {
        x = vector_add(x, rest...);
        return x;
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

    template<int N>
    static inline type vector_shr(type x) {
        return Instrinsic::template vector_shr<N>(x);
    }
    template<int N>
    static inline type vector_shl(type x) {
        return Instrinsic::template vector_shl<N>(x);
    }
    template<int N>
    static inline type vector_rol(type x) {
        return Instrinsic::template vector_rol<N>(x);
    }

    static inline type Ch(type x, type y, type z) {
        // z ^ (x & (y ^ z))
        return vector_xor(z, vector_and(x, vector_xor(y, z)));
    }
    static inline type Maj(type x, type y, type z) {
        // (x & y) | (z & (x | y))
        return vector_or(vector_and(x, y), vector_and(z, vector_or(x, y)));
    }
    static inline type Sigma0(type x) {
        // (x >> 2 | x << 30) ^ (x >> 13 | x << 19) ^ (x >> 22 | x << 10)
        return vector_xor(
                vector_rol<30>(x),
                vector_rol<19>(x), 
                vector_rol<10>(x)
        );
    }
    static inline type Sigma1(type x) {
        // (x >> 6 | x << 26) ^ (x >> 11 | x << 21) ^ (x >> 25 | x << 7);
        return vector_xor(
                vector_rol<26>(x),
                vector_rol<21>(x),
                vector_rol<7>(x)
        );
    }
    static inline type sigma0(type x) {
        // (x >> 7 | x << 25) ^ (x >> 18 | x << 14) ^ (x >> 3)
        return vector_xor(
                vector_rol<25>(x),
                vector_rol<14>(x),
                vector_shr<3>(x)
        );
    }
    static inline type sigma1(type x) {
        // (x >> 17 | x << 15) ^ (x >> 19 | x << 13) ^ (x >> 10)
        return vector_xor(
                vector_rol<15>(x),
                vector_rol<13>(x),
                vector_shr<10>(x)
        );
    }

    static inline void round(type a, type b, type c, type& d, type e, type f, type g, type& h, type k) {
        /*
            uint32_t t1 = h + Sigma1(e) + Ch(e, f, g) + k;
            uint32_t t2 = Sigma0(a) + Maj(a, b, c);
            d += t1;
            h = t1 + t2;
        */
        type t1 = vector_add(h, Sigma1(e), Ch(e, f, g), k);
        type t2 = vector_add(Sigma0(a), Maj(a, b, c));
        d = vector_add(d, t1);
        h = vector_add(t1, t2);
    }

public:
    static inline size_t way() {
        return sizeof(type) / sizeof(uint32_t);
    }

    static inline void process_block(
            type &a, type &b, type &c, type &d, 
            type &e, type &f, type &g, type &h,
            const void *block) {

        type w0, w1, w2, w3, w4, w5, w6, w7, w8, w9, w10, w11, w12, w13, w14, w15;

        type oa = a;
        type ob = b;
        type oc = c;
        type od = d;
        type oe = e;
        type of = f;
        type og = g;
        type oh = h;

        round(a, b, c, d, e, f, g, h, vector_add(vector_mirror(0x428a2f98ul), w0 = Instrinsic::load(block, 0)));
        round(h, a, b, c, d, e, f, g, vector_add(vector_mirror(0x71374491ul), w1 = Instrinsic::load(block, 4)));
        round(g, h, a, b, c, d, e, f, vector_add(vector_mirror(0xb5c0fbcful), w2 = Instrinsic::load(block, 8)));
        round(f, g, h, a, b, c, d, e, vector_add(vector_mirror(0xe9b5dba5ul), w3 = Instrinsic::load(block, 12)));

        round(e, f, g, h, a, b, c, d, vector_add(vector_mirror(0x3956c25bul), w4 = Instrinsic::load(block, 16)));
        round(d, e, f, g, h, a, b, c, vector_add(vector_mirror(0x59f111f1ul), w5 = Instrinsic::load(block, 20)));
        round(c, d, e, f, g, h, a, b, vector_add(vector_mirror(0x923f82a4ul), w6 = Instrinsic::load(block, 24)));
        round(b, c, d, e, f, g, h, a, vector_add(vector_mirror(0xab1c5ed5ul), w7 = Instrinsic::load(block, 28)));

        round(a, b, c, d, e, f, g, h, vector_add(vector_mirror(0xd807aa98ul), w8 = Instrinsic::load(block, 32)));
        round(h, a, b, c, d, e, f, g, vector_add(vector_mirror(0x12835b01ul), w9 = Instrinsic::load(block, 36)));
        round(g, h, a, b, c, d, e, f, vector_add(vector_mirror(0x243185beul), w10 = Instrinsic::load(block, 40)));
        round(f, g, h, a, b, c, d, e, vector_add(vector_mirror(0x550c7dc3ul), w11 = Instrinsic::load(block, 44)));

        round(e, f, g, h, a, b, c, d, vector_add(vector_mirror(0x72be5d74ul), w12 = Instrinsic::load(block, 48)));
        round(d, e, f, g, h, a, b, c, vector_add(vector_mirror(0x80deb1feul), w13 = Instrinsic::load(block, 52)));
        round(c, d, e, f, g, h, a, b, vector_add(vector_mirror(0x9bdc06a7ul), w14 = Instrinsic::load(block, 56)));
        round(b, c, d, e, f, g, h, a, vector_add(vector_mirror(0xc19bf174ul), w15 = Instrinsic::load(block, 60)));

        round(a, b, c, d, e, f, g, h, vector_add(vector_mirror(0xe49b69c1ul), vector_inc(w0, sigma1(w14), w9, sigma0(w1))));
        round(h, a, b, c, d, e, f, g, vector_add(vector_mirror(0xefbe4786ul), vector_inc(w1, sigma1(w15), w10, sigma0(w2))));
        round(g, h, a, b, c, d, e, f, vector_add(vector_mirror(0x0fc19dc6ul), vector_inc(w2, sigma1(w0), w11, sigma0(w3))));
        round(f, g, h, a, b, c, d, e, vector_add(vector_mirror(0x240ca1ccul), vector_inc(w3, sigma1(w1), w12, sigma0(w4))));

        round(e, f, g, h, a, b, c, d, vector_add(vector_mirror(0x2de92c6ful), vector_inc(w4, sigma1(w2), w13, sigma0(w5))));
        round(d, e, f, g, h, a, b, c, vector_add(vector_mirror(0x4a7484aaul), vector_inc(w5, sigma1(w3), w14, sigma0(w6))));
        round(c, d, e, f, g, h, a, b, vector_add(vector_mirror(0x5cb0a9dcul), vector_inc(w6, sigma1(w4), w15, sigma0(w7))));
        round(b, c, d, e, f, g, h, a, vector_add(vector_mirror(0x76f988daul), vector_inc(w7, sigma1(w5), w0, sigma0(w8))));

        round(a, b, c, d, e, f, g, h, vector_add(vector_mirror(0x983e5152ul), vector_inc(w8, sigma1(w6), w1, sigma0(w9))));
        round(h, a, b, c, d, e, f, g, vector_add(vector_mirror(0xa831c66dul), vector_inc(w9, sigma1(w7), w2, sigma0(w10))));
        round(g, h, a, b, c, d, e, f, vector_add(vector_mirror(0xb00327c8ul), vector_inc(w10, sigma1(w8), w3, sigma0(w11))));
        round(f, g, h, a, b, c, d, e, vector_add(vector_mirror(0xbf597fc7ul), vector_inc(w11, sigma1(w9), w4, sigma0(w12))));

        round(e, f, g, h, a, b, c, d, vector_add(vector_mirror(0xc6e00bf3ul), vector_inc(w12, sigma1(w10), w5, sigma0(w13))));
        round(d, e, f, g, h, a, b, c, vector_add(vector_mirror(0xd5a79147ul), vector_inc(w13, sigma1(w11), w6, sigma0(w14))));
        round(c, d, e, f, g, h, a, b, vector_add(vector_mirror(0x06ca6351ul), vector_inc(w14, sigma1(w12), w7, sigma0(w15))));
        round(b, c, d, e, f, g, h, a, vector_add(vector_mirror(0x14292967ul), vector_inc(w15, sigma1(w13), w8, sigma0(w0))));

        round(a, b, c, d, e, f, g, h, vector_add(vector_mirror(0x27b70a85ul), vector_inc(w0, sigma1(w14), w9, sigma0(w1))));
        round(h, a, b, c, d, e, f, g, vector_add(vector_mirror(0x2e1b2138ul), vector_inc(w1, sigma1(w15), w10, sigma0(w2))));
        round(g, h, a, b, c, d, e, f, vector_add(vector_mirror(0x4d2c6dfcul), vector_inc(w2, sigma1(w0), w11, sigma0(w3))));
        round(f, g, h, a, b, c, d, e, vector_add(vector_mirror(0x53380d13ul), vector_inc(w3, sigma1(w1), w12, sigma0(w4))));

        round(e, f, g, h, a, b, c, d, vector_add(vector_mirror(0x650a7354ul), vector_inc(w4, sigma1(w2), w13, sigma0(w5))));
        round(d, e, f, g, h, a, b, c, vector_add(vector_mirror(0x766a0abbul), vector_inc(w5, sigma1(w3), w14, sigma0(w6))));
        round(c, d, e, f, g, h, a, b, vector_add(vector_mirror(0x81c2c92eul), vector_inc(w6, sigma1(w4), w15, sigma0(w7))));
        round(b, c, d, e, f, g, h, a, vector_add(vector_mirror(0x92722c85ul), vector_inc(w7, sigma1(w5), w0, sigma0(w8))));

        round(a, b, c, d, e, f, g, h, vector_add(vector_mirror(0xa2bfe8a1ul), vector_inc(w8, sigma1(w6), w1, sigma0(w9))));
        round(h, a, b, c, d, e, f, g, vector_add(vector_mirror(0xa81a664bul), vector_inc(w9, sigma1(w7), w2, sigma0(w10))));
        round(g, h, a, b, c, d, e, f, vector_add(vector_mirror(0xc24b8b70ul), vector_inc(w10, sigma1(w8), w3, sigma0(w11))));
        round(f, g, h, a, b, c, d, e, vector_add(vector_mirror(0xc76c51a3ul), vector_inc(w11, sigma1(w9), w4, sigma0(w12))));

        round(e, f, g, h, a, b, c, d, vector_add(vector_mirror(0xd192e819ul), vector_inc(w12, sigma1(w10), w5, sigma0(w13))));
        round(d, e, f, g, h, a, b, c, vector_add(vector_mirror(0xd6990624ul), vector_inc(w13, sigma1(w11), w6, sigma0(w14))));
        round(c, d, e, f, g, h, a, b, vector_add(vector_mirror(0xf40e3585ul), vector_inc(w14, sigma1(w12), w7, sigma0(w15))));
        round(b, c, d, e, f, g, h, a, vector_add(vector_mirror(0x106aa070ul), vector_inc(w15, sigma1(w13), w8, sigma0(w0))));

        round(a, b, c, d, e, f, g, h, vector_add(vector_mirror(0x19a4c116ul), vector_inc(w0, sigma1(w14), w9, sigma0(w1))));
        round(h, a, b, c, d, e, f, g, vector_add(vector_mirror(0x1e376c08ul), vector_inc(w1, sigma1(w15), w10, sigma0(w2))));
        round(g, h, a, b, c, d, e, f, vector_add(vector_mirror(0x2748774cul), vector_inc(w2, sigma1(w0), w11, sigma0(w3))));
        round(f, g, h, a, b, c, d, e, vector_add(vector_mirror(0x34b0bcb5ul), vector_inc(w3, sigma1(w1), w12, sigma0(w4))));

        round(e, f, g, h, a, b, c, d, vector_add(vector_mirror(0x391c0cb3ul), vector_inc(w4, sigma1(w2), w13, sigma0(w5))));
        round(d, e, f, g, h, a, b, c, vector_add(vector_mirror(0x4ed8aa4aul), vector_inc(w5, sigma1(w3), w14, sigma0(w6))));
        round(c, d, e, f, g, h, a, b, vector_add(vector_mirror(0x5b9cca4ful), vector_inc(w6, sigma1(w4), w15, sigma0(w7))));
        round(b, c, d, e, f, g, h, a, vector_add(vector_mirror(0x682e6ff3ul), vector_inc(w7, sigma1(w5), w0, sigma0(w8))));

        round(a, b, c, d, e, f, g, h, vector_add(vector_mirror(0x748f82eeul), vector_inc(w8, sigma1(w6), w1, sigma0(w9))));
        round(h, a, b, c, d, e, f, g, vector_add(vector_mirror(0x78a5636ful), vector_inc(w9, sigma1(w7), w2, sigma0(w10))));
        round(g, h, a, b, c, d, e, f, vector_add(vector_mirror(0x84c87814ul), vector_inc(w10, sigma1(w8), w3, sigma0(w11))));
        round(f, g, h, a, b, c, d, e, vector_add(vector_mirror(0x8cc70208ul), vector_inc(w11, sigma1(w9), w4, sigma0(w12))));

        round(e, f, g, h, a, b, c, d, vector_add(vector_mirror(0x90befffaul), vector_inc(w12, sigma1(w10), w5, sigma0(w13))));
        round(d, e, f, g, h, a, b, c, vector_add(vector_mirror(0xa4506cebul), vector_inc(w13, sigma1(w11), w6, sigma0(w14))));
        round(c, d, e, f, g, h, a, b, vector_add(vector_mirror(0xbef9a3f7ul), vector_inc(w14, sigma1(w12), w7, sigma0(w15))));
        round(b, c, d, e, f, g, h, a, vector_add(vector_mirror(0xc67178f2ul), vector_inc(w15, sigma1(w13), w8, sigma0(w0))));

        a = vector_add(a, oa);
        b = vector_add(b, ob);
        c = vector_add(c, oc);
        d = vector_add(d, od);
        e = vector_add(e, oe);
        f = vector_add(f, of);
        g = vector_add(g, og);
        h = vector_add(h, oh);
    }

    static void process_trunk(void *out, const void *blocks, int count = 1) {
        type a = vector_mirror(0x6a09e667ul);
        type b = vector_mirror(0xbb67ae85ul);
        type c = vector_mirror(0x3c6ef372ul);
        type d = vector_mirror(0xa54ff53aul);
        type e = vector_mirror(0x510e527ful);
        type f = vector_mirror(0x9b05688cul);
        type g = vector_mirror(0x1f83d9abul);
        type h = vector_mirror(0x5be0cd19ul);

        char *cur_block = (char *)blocks;
        while (count--) {
            process_block(a, b, c, d, e, f, g, h, cur_block);
            cur_block += 64 * way();
        }

        Instrinsic::save(out,  0, a);
        Instrinsic::save(out,  4, b);
        Instrinsic::save(out,  8, c);
        Instrinsic::save(out, 12, d);
        Instrinsic::save(out, 16, e);
        Instrinsic::save(out, 20, f);
        Instrinsic::save(out, 24, g);
        Instrinsic::save(out, 28, h);
    }
};

} // namespace fingera
