
#include <string>
#include <cstring>
#include <cmath>
#include "sha256.h"
#include "ripemd160.h"
#include "instrinsic_sse4.h"
#include "instrinsic_one.h"
#include "instrinsic_two.h"
#include "instrinsic_avx2.h"
#include "instrinsic_avx512.h"
#include "helper.h"

uint8_t sha256_single_block[] = {
    // data
    0x30,
    // pad
    0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // length
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08,
};

void fill_sha256_trunk(void *buffer, size_t way) {
    char *trunk_value = (char *)buffer;
    for (size_t i = 0; i < way; i++) {
        memcpy(trunk_value + i * 64, sha256_single_block, sizeof(sha256_single_block));
        trunk_value[i * 64] = '0' + i;
    }
}

void add(int &out) {
}

template<typename ...Args>
void add(int &out, int value, Args... rest) {
    out += value;
    add(out, rest...);
}

int main(int argc, char const *argv[]) {
    int a;
    add(a, 1, 2, 3, 4, 5);
    printf("%d\n", a);

    // 1 路 one
    // 2 路 two
    // 4 路 sse4
    // 8 路 avx2
    // 16 路 avx512

    uint8_t result_hash[5][16 * 32];
    uint8_t trunk[5][16 * 64];

    for (size_t i = 0; i < 5; i++) {
        fill_sha256_trunk(trunk[i], std::pow(2, i));
    }
    memset(result_hash, 0, sizeof(result_hash));

    using namespace fingera;

    // ba5ed015715da74cf1e87230ba73d4855edaf6f6
    // 2becb378925a88317b7721968b810b3436f75b61
    ripemd160<instrinsic_one>::process_trunk(&result_hash[0][0], trunk[0]);
    std::cout << "1 way ripemd160" << std::endl;
    dump_buffer(&result_hash[0][0], 20);

    sha256<instrinsic_one>::process_trunk(&result_hash[0][0], trunk[0]);
    std::cout << "1 way sha256" << std::endl;
    dump_buffer(&result_hash[0][0], 32);
/*
    ripemd160<instrinsic_two>::process_trunk(&result_hash[1][0], trunk[1]);
    std::cout << "2 way ripemd160" << std::endl;
    dump_buffer(&result_hash[1][0], 20);
    dump_buffer(&result_hash[1][20], 20);
*/
    sha256<instrinsic_two>::process_trunk(&result_hash[1][0], trunk[1]);
    std::cout << "2 way sha256" << std::endl;
    dump_buffer(&result_hash[1][0], 32);
    dump_buffer(&result_hash[1][32], 32);

    sha256<instrinsic_sse4>::process_trunk(&result_hash[2][0], trunk[2]);
    std::cout << "4 way sha256" << std::endl;
    for (size_t i = 0; i < 4; i++)
        dump_buffer(&result_hash[2][i * 32], 32);

    sha256<instrinsic_avx2>::process_trunk(&result_hash[3][0], trunk[3]);
    std::cout << "8 way sha256" << std::endl;
    for (size_t i = 0; i < 8; i++)
        dump_buffer(&result_hash[3][i * 32], 32);

    sha256<instrinsic_avx512>::process_trunk(&result_hash[4][0], trunk[4]);
    std::cout << "16 way" << std::endl;
    for (size_t i = 0; i < 16; i++)
        dump_buffer(&result_hash[4][i * 32], 32);

    return 0;
}
