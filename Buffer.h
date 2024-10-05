#ifndef BUFFER_H
#define BUFFER_H

#include <cstddef>
#include <cstdint>
#include <cstring>

// defined in the implementation file
struct Pixel;

class Buffer {
  public:
    Buffer(size_t width, size_t height);
    ~Buffer();

    [[gnu::always_inline]] inline void setValue(int x, int y, uint8_t r,
                                                uint8_t g, uint8_t b);
    [[gnu::always_inline]] inline void clear();
    [[gnu::always_inline]] inline void clear(uint8_t r, uint8_t g, uint8_t b);
    Pixel *getData() const { return data_; }
    constexpr size_t getWidth() const { return width_; }
    constexpr size_t getHeight() const { return height_; }
    Buffer(const Buffer &) = delete;
    Buffer &operator=(const Buffer &) = delete;

  private:
    Pixel *data_; // Double buffer for Hub75 management (since each pixel has 2
                  // slots) //would prefer a std array
    size_t width_;
    size_t height_;
    bool external_;
};

#ifdef PLATFORM_DESKTOP
#include "DesktopBuffer.h"
#elif defined(PLATFORM_PICO)
#include "Pico_Buffer.h"
#endif

#endif // BUFFER_H
