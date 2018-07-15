#pragma once

#include <cstdint>
#include <vector>
#include <endian.h>

namespace fingera {

inline void write_be32(void* ptr, int offset, uint32_t x) {
    *(uint32_t *)((char *)ptr + offset) = htobe32(x);
}
inline uint32_t read_be32(const void *ptr, int offset) {
    return be32toh(*(uint32_t *)((char *)ptr + offset));
}
inline void write_be64(void* ptr, int offset, uint64_t x) {
    *(uint32_t *)((char *)ptr + offset) = htobe64(x);
}

typedef std::vector<uint8_t> data_trunk;


} // namespace fingera
