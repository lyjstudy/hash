/**
 * @file instrinsic_one.h
 * @author lyjstudy@gmail.com
 * @date 2018-07-15
 */
#pragma once

#include <cstdint>
#include "compact.h"

namespace fingera {

class instrinsic_one {
public:
    using type = uint32_t;

    static inline type vector_mirror(uint32_t x) {
        return x;
    }
    static inline type vector_add(type x, type y) {
        return x + y;
    }
    static inline type vector_xor(type x, type y) {
        return x ^ y;
    }
    static inline type vector_or(type x, type y) {
        return x | y;
    }
    static inline type vector_and(type x, type y) {
        return x & y;
    }
    static inline type vector_andnot(type x, type y) {
        return ~x & y;
    }
    template<int N>
    static inline type vector_shr(type x) {
        return x >> N;
    }
    template<int N>
    static inline type vector_shl(type x) {
        return x << N;
    }
    template<int N>
    static inline type vector_rol(type x) {
        return (x << N) | (x >> (32 - N));
    }

    static inline type load(const void *trunk, int offset) {
        return read_be32(trunk, offset + 64 * 0);
    }
    static inline void save(void *out, int offset, type v, size_t hash_size = 32) {
        write_be32(out, offset + hash_size * 0, v);
    }
    static inline type load_le(const void *trunk, int offset) {
        return read_le32(trunk, offset + 64 * 0);
    }
    static inline void save_le(void *out, int offset, type v, size_t hash_size = 32) {
        write_le32(out, offset + hash_size * 0, v);
    }
};

} // namespace fingera
