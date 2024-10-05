#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

#include "Buffer.h"

struct Pixel {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    Pixel() : r(0), g(0), b(0) {}
    Pixel(uint8_t _r, uint8_t _g, uint8_t _b) : r(_r), g(_g), b(_b){};
};

Buffer::Buffer(size_t width, size_t height)
    : width_(width), height_(height), data_(new Pixel[width * height]),
      external_(false) {}

// Define Buffer destructor
Buffer::~Buffer() {
    if (!external_) {
        delete[] data_;
    }
}

// Define setValue method
[[gnu::always_inline]] inline void Buffer::setValue(int x, int y, uint8_t r,
                                                    uint8_t g, uint8_t b) {
    if (x >= width_ || y >= height_)
        return;
    data_[y * width_ + x] = Pixel(r, g, b);
}

// Define clear method
[[gnu::always_inline]] inline void Buffer::clear() {
    std::memset(data_, 0, width_ * height_ * sizeof(Pixel)); // Clear to black
}

// Define clear method with color
[[gnu::always_inline]] inline void Buffer::clear(uint8_t r, uint8_t g,
                                                 uint8_t b) {
    Pixel color(r, g, b);
    for (size_t i = 0; i < width_ * height_; ++i) {
        data_[i] = color;
    }
}