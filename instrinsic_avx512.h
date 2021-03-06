/**
 * @file instrinsic_avx512.h
 * @author lyjstudy@gmail.com
 * @date 2018-07-15
 */
#pragma once

#include <cstdint>
#include <immintrin.h>
#include "compact.h"

// _mm512_rol_epi32 CPUID Flags: AVX512VL + AVX512F
// _mm512_xxx CPUID Flags: AVX512F

namespace fingera {

class instrinsic_avx512 {
public:
    using type = __m512i;

    static inline type vector_mirror(uint32_t x) {
        return _mm512_set1_epi32(x);
    }
    static inline type vector_add(type x, type y) {
        return _mm512_add_epi32(x, y);
    }
    static inline type vector_xor(type x, type y) {
        return _mm512_xor_si512(x, y);
    }
    static inline type vector_or(type x, type y) {
        return _mm512_or_si512(x, y);
    }
    static inline type vector_and(type x, type y) {
        return _mm512_and_si512(x, y);
    }
    static inline type vector_andnot(type x, type y) {
        return _mm512_andnot_si512(x, y);
    }
    template<int N>
    static inline type vector_shr(type x) {
        return _mm512_srli_epi32(x, N);
    }
    template<int N>
    static inline type vector_shl(type x) {
        return _mm512_slli_epi32(x, N);
    }
    template<int N>
    static inline type vector_rol(type x) {
        // CPUID Flags: AVX512VL + AVX512F
        return _mm512_rol_epi32(x, N);
        // return vector_or(vector_shl<N>(x), vector_shr<32 - N>(x));
    }

    static inline type load(const void *trunk, int offset) {
        return _mm512_set_epi32(
            read_be32(trunk, offset + 64 * 0),
            read_be32(trunk, offset + 64 * 1),
            read_be32(trunk, offset + 64 * 2),
            read_be32(trunk, offset + 64 * 3),
            read_be32(trunk, offset + 64 * 4),
            read_be32(trunk, offset + 64 * 5),
            read_be32(trunk, offset + 64 * 6),
            read_be32(trunk, offset + 64 * 7),

            read_be32(trunk, offset + 64 * 8),
            read_be32(trunk, offset + 64 * 9),
            read_be32(trunk, offset + 64 * 10),
            read_be32(trunk, offset + 64 * 11),
            read_be32(trunk, offset + 64 * 12),
            read_be32(trunk, offset + 64 * 13),
            read_be32(trunk, offset + 64 * 14),
            read_be32(trunk, offset + 64 * 15)
        );
    }
    static inline void save(void *data, int offset, type d, size_t hash_size = 32) {
        char *out = (char *)data;

        __m128i v = _mm512_extracti32x4_epi32(d, 3);
        write_be32(out, offset + hash_size * 0, _mm_extract_epi32(v, 3));
        write_be32(out, offset + hash_size * 1, _mm_extract_epi32(v, 2));
        write_be32(out, offset + hash_size * 2, _mm_extract_epi32(v, 1));
        write_be32(out, offset + hash_size * 3, _mm_extract_epi32(v, 0));
        out += hash_size * 4;

        v = _mm512_extracti32x4_epi32(d, 2);
        write_be32(out, offset + hash_size * 0, _mm_extract_epi32(v, 3));
        write_be32(out, offset + hash_size * 1, _mm_extract_epi32(v, 2));
        write_be32(out, offset + hash_size * 2, _mm_extract_epi32(v, 1));
        write_be32(out, offset + hash_size * 3, _mm_extract_epi32(v, 0));
        out += hash_size * 4;

        v = _mm512_extracti32x4_epi32(d, 1);
        write_be32(out, offset + hash_size * 0, _mm_extract_epi32(v, 3));
        write_be32(out, offset + hash_size * 1, _mm_extract_epi32(v, 2));
        write_be32(out, offset + hash_size * 2, _mm_extract_epi32(v, 1));
        write_be32(out, offset + hash_size * 3, _mm_extract_epi32(v, 0));
        out += hash_size * 4;

        v = _mm512_extracti32x4_epi32(d, 0);
        write_be32(out, offset + hash_size * 0, _mm_extract_epi32(v, 3));
        write_be32(out, offset + hash_size * 1, _mm_extract_epi32(v, 2));
        write_be32(out, offset + hash_size * 2, _mm_extract_epi32(v, 1));
        write_be32(out, offset + hash_size * 3, _mm_extract_epi32(v, 0));
    }


    static inline type load_le(const void *trunk, int offset) {
        return _mm512_set_epi32(
            read_le32(trunk, offset + 64 * 0),
            read_le32(trunk, offset + 64 * 1),
            read_le32(trunk, offset + 64 * 2),
            read_le32(trunk, offset + 64 * 3),
            read_le32(trunk, offset + 64 * 4),
            read_le32(trunk, offset + 64 * 5),
            read_le32(trunk, offset + 64 * 6),
            read_le32(trunk, offset + 64 * 7),

            read_le32(trunk, offset + 64 * 8),
            read_le32(trunk, offset + 64 * 9),
            read_le32(trunk, offset + 64 * 10),
            read_le32(trunk, offset + 64 * 11),
            read_le32(trunk, offset + 64 * 12),
            read_le32(trunk, offset + 64 * 13),
            read_le32(trunk, offset + 64 * 14),
            read_le32(trunk, offset + 64 * 15)
        );
    }
    static inline void save_le(void *data, int offset, type d, size_t hash_size = 32) {
        char *out = (char *)data;

        __m128i v = _mm512_extracti32x4_epi32(d, 3);
        write_le32(out, offset + hash_size * 0, _mm_extract_epi32(v, 3));
        write_le32(out, offset + hash_size * 1, _mm_extract_epi32(v, 2));
        write_le32(out, offset + hash_size * 2, _mm_extract_epi32(v, 1));
        write_le32(out, offset + hash_size * 3, _mm_extract_epi32(v, 0));
        out += hash_size * 4;

        v = _mm512_extracti32x4_epi32(d, 2);
        write_le32(out, offset + hash_size * 0, _mm_extract_epi32(v, 3));
        write_le32(out, offset + hash_size * 1, _mm_extract_epi32(v, 2));
        write_le32(out, offset + hash_size * 2, _mm_extract_epi32(v, 1));
        write_le32(out, offset + hash_size * 3, _mm_extract_epi32(v, 0));
        out += hash_size * 4;

        v = _mm512_extracti32x4_epi32(d, 1);
        write_le32(out, offset + hash_size * 0, _mm_extract_epi32(v, 3));
        write_le32(out, offset + hash_size * 1, _mm_extract_epi32(v, 2));
        write_le32(out, offset + hash_size * 2, _mm_extract_epi32(v, 1));
        write_le32(out, offset + hash_size * 3, _mm_extract_epi32(v, 0));
        out += hash_size * 4;

        v = _mm512_extracti32x4_epi32(d, 0);
        write_le32(out, offset + hash_size * 0, _mm_extract_epi32(v, 3));
        write_le32(out, offset + hash_size * 1, _mm_extract_epi32(v, 2));
        write_le32(out, offset + hash_size * 2, _mm_extract_epi32(v, 1));
        write_le32(out, offset + hash_size * 3, _mm_extract_epi32(v, 0));
    }
};

} // namespace fingera
