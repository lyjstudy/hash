/**
 * @file helper.h
 * @author lyjstudy@gmail.com
 * @date 2018-07-15
 */
#pragma once

#include <iostream>
#include <iomanip>
#include <cpuid.h>

namespace fingera {

inline void dump_buffer(const void *buf, size_t size) {
    for (size_t i = 0; i < size; i++) {
        uint32_t data = ((uint8_t *)buf)[i];
        std::cout << std::hex << std::setw(2) << std::setfill('0') << data;
    }
    std::cout << std::endl;
}


}