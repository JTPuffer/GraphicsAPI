#pragma once

#include "Buffer.h"
#include "drivers/hub75/hub75.hpp" // Include the Pixel definition
#include <cstddef>
#include <cstdint>
#include <cstring> // for std::memset
using namespace pimoroni;

Buffer::Buffer(size_t width, size_t height)
    : width_(width), height_(height), data_(new Pixel[width * height]),
      external_(false) {}

Buffer::~Buffer() {
    if (!external_) {
        delete[] data_; // Clean up allocated memory
    }
}

[[gnu::always_inline]] inline void Buffer::setValue(int x, int y, uint8_t r,
                                                    uint8_t g, uint8_t b) {
    if (x < 0 || x >= static_cast<int>(width_) || y < 0 ||
        y >= static_cast<int>(height_)) {
        return; // Out of bounds check
    }

    int offset = (y < height_ / 2) ? (y * width_ + x) * 2
                                   : ((y - height_ / 2) * width_ + x) * 2 + 1;

    data_[offset] = {r, g, b}; // Assign pixel value
}

[[gnu::always_inline]] inline void Buffer::clear() {
    std::memset(data_, 0, width_ * height_ * sizeof(Pixel)); // Clear to black
}

[[gnu::always_inline]] inline void Buffer::clear(uint8_t r, uint8_t g,
                                                 uint8_t b) {
    Pixel pixel{r, g, b};
    for (size_t i = 0; i < width_ * height_; ++i) {
        data_[i] = pixel;
    }
}
