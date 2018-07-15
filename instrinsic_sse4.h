/**
 * @file instrinsic_sse4.h
 * @author lyjstudy@gmail.com
 * @date 2018-07-15
 */
#pragma once

#include <cstdint>
#include <immintrin.h>
#include "compact.h"

// _mm_extract_epi32 CPUID Flags: SSE4.1
// _mm_rol_epi32 CPUID Flags: AVX512VL + AVX512F(disabled)
// _mm_xxx CPUID Flags: SSE2

namespace fingera {

class instrinsic_sse4 {
public:
    using type = __m128i;

    static inline type vector_mirror(uint32_t x) {
        return _mm_set1_epi32(x);
    }
    static inline type vector_add(type x, type y) {
        return _mm_add_epi32(x, y);
    }
    static inline type vector_xor(type x, type y) {
        return _mm_xor_si128(x, y);
    }
    static inline type vector_or(type x, type y) {
        return _mm_or_si128(x, y);
    }
    static inline type vector_and(type x, type y) {
        return _mm_and_si128(x, y);
    }
    static inline type vector_andnot(type x, type y) {
        return _mm_andnot_si128(x, y);
    }
    template<int N>
    static inline type vector_shr(type x) {
        return _mm_srli_epi32(x, N);
    }
    template<int N>
    static inline type vector_shl(type x) {
        return _mm_slli_epi32(x, N);
    }
    template<int N>
    static inline type vector_rol(type x) {
        // CPUID Flags: AVX512VL + AVX512F
        // return _mm_rol_epi32(x, N);
        return vector_or(vector_shl<N>(x), vector_shr<32 - N>(x));
    }

    static inline type load(const void *trunk, int offset) {
        return _mm_set_epi32(
            read_be32(trunk, offset + 64 * 0),
            read_be32(trunk, offset + 64 * 1),
            read_be32(trunk, offset + 64 * 2),
            read_be32(trunk, offset + 64 * 3)
        );
    }
    static inline void save(void *out, int offset, type v, size_t hash_size = 32) {
        write_be32(out, offset + hash_size * 0, _mm_extract_epi32(v, 3));
        write_be32(out, offset + hash_size * 1, _mm_extract_epi32(v, 2));
        write_be32(out, offset + hash_size * 2, _mm_extract_epi32(v, 1));
        write_be32(out, offset + hash_size * 3, _mm_extract_epi32(v, 0));
    }
};

} // namespace fingera
