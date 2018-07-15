/**
 * @file instrinsic_avx2.h
 * @author lyjstudy@gmail.com
 * @date 2018-07-15
 */
#pragma once

#include <cstdint>
#include <immintrin.h>
#include "compact.h"

namespace fingera {

class instrinsic_avx2 {
public:
    using type = __m256i;

    static inline type vector_mirror(uint32_t x) {
        return _mm256_set1_epi32(x);
    }
    static inline type vector_add(type x, type y) {
        return _mm256_add_epi32(x, y);
    }
    static inline type vector_xor(type x, type y) {
        return _mm256_xor_si256(x, y);
    }
    static inline type vector_or(type x, type y) {
        return _mm256_or_si256(x, y);
    }
    static inline type vector_and(type x, type y) {
        return _mm256_and_si256(x, y);
    }
    template<int N>
    static inline type vector_shr(type x) {
        return _mm256_srli_epi32(x, N);
    }
    template<int N>
    static inline type vector_shl(type x) {
        return _mm256_slli_epi32(x, N);
    }

    static inline type load(const void *trunk, int offset) {
        return _mm256_set_epi32(
            read_be32(trunk, offset + 64 * 0),
            read_be32(trunk, offset + 64 * 1),
            read_be32(trunk, offset + 64 * 2),
            read_be32(trunk, offset + 64 * 3),
            read_be32(trunk, offset + 64 * 4),
            read_be32(trunk, offset + 64 * 5),
            read_be32(trunk, offset + 64 * 6),
            read_be32(trunk, offset + 64 * 7)
        );
    }
    static inline void save(void *out, int offset, type v) {
        write_be32(out, offset + 32 * 0, _mm256_extract_epi32(v, 7));
        write_be32(out, offset + 32 * 1, _mm256_extract_epi32(v, 6));
        write_be32(out, offset + 32 * 2, _mm256_extract_epi32(v, 5));
        write_be32(out, offset + 32 * 3, _mm256_extract_epi32(v, 4));
        write_be32(out, offset + 32 * 4, _mm256_extract_epi32(v, 3));
        write_be32(out, offset + 32 * 5, _mm256_extract_epi32(v, 2));
        write_be32(out, offset + 32 * 6, _mm256_extract_epi32(v, 1));
        write_be32(out, offset + 32 * 7, _mm256_extract_epi32(v, 0));
    }
};

} // namespace fingera
