/**
 * @file instrinsic_two.h
 * @author lyjstudy@gmail.com
 * @date 2018-07-15
 */
#pragma once

#include <cstdint>
#include "compact.h"

namespace fingera {

class instrinsic_two {
private:
    static inline uint32_t *b1(uint64_t *value) {
        return (uint32_t *)value;
    }
    static inline uint32_t *b2(uint64_t *value) {
        return ((uint32_t *)value) + 1;
    }
public:
    using type = uint64_t;

    static inline type vector_mirror(uint32_t x) {
        return (type)x | (((type)x) << 32);
    }
    static inline type vector_add(type x, type y) {
        uint64_t r;
        *b1(&r) = *b1(&x) + *b1(&y);
        *b2(&r) = *b2(&x) + *b2(&y);
        return r;
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
        return (~x) & y;
    }
    template<int N>
    static inline type vector_shr(type x) {
        uint64_t r;
        *b1(&r) = *b1(&x) >> N;
        *b2(&r) = *b2(&x) >> N;
        return r;
    }
    template<int N>
    static inline type vector_shl(type x) {
        uint64_t r;
        *b1(&r) = *b1(&x) << N;
        *b2(&r) = *b2(&x) << N;
        return r;
    }
    template<int N>
    static inline type vector_rol(type x) {
        uint64_t r;
        *b1(&r) = (*b1(&x) << N) | (*b1(&x) >> (32 - N));
        *b2(&r) = (*b2(&x) << N) | (*b2(&x) >> (32 - N));
        return r;
    }

    static inline type load(const void *trunk, int offset) {
        type first = read_be32(trunk, offset + 64 * 0);
        type second = read_be32(trunk, offset + 64 * 1);
        return first | (second << 32);
    }
    static inline void save(void *out, int offset, type v, size_t hash_size = 32) {
        write_be32(out, offset + hash_size * 0, v);
        write_be32(out, offset + hash_size * 1, v >> 32);
    }
    static inline type load_le(const void *trunk, int offset) {
        type first = read_le32(trunk, offset + 64 * 0);
        type second = read_le32(trunk, offset + 64 * 1);
        return first | (second << 32);
    }
    static inline void save_le(void *out, int offset, type v, size_t hash_size = 32) {
        write_le32(out, offset + hash_size * 0, v);
        write_le32(out, offset + hash_size * 1, v >> 32);
    }
};

} // namespace fingera
