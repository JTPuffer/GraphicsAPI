//
// Created by matthew on 8/24/24.
//

#ifndef GRAPHICSAPI_BASICRASTERIZER_H
#define GRAPHICSAPI_BASICRASTERIZER_H
#include "../Buffer.h"
#include "IRasterizer.h"
#include <memory>

namespace {
    template <typename T>
    concept HasXYZ = requires(T v) {
        {v.x};
        {v.y};
        {v.z};
    };
    // 4couldnt figure out compile time reflection, cba to download boost
    template <typename T, typename A>
    concept SetVar = requires(T v, A b) {
        { v *b } -> std::same_as<T>;
        { v + v } -> std::same_as<T>;
    };
} // namespace
template <size_t WIDTH, size_t HEIGHT, typename VertexType>
requires HasXYZ<VertexType> && SetVar<VertexType, decltype(VertexType::x)>
class BasicRasterizer : public IRasterizer<WIDTH, HEIGHT, VertexType> {
public:
    // BasicRasterizer()  =default;
    explicit BasicRasterizer(
        std::array<VertexType, WIDTH * HEIGHT> &_targetBuffer)
        : IRasterizer<WIDTH, HEIGHT, VertexType>(_targetBuffer) {}

    [[gnu::always_inline]] inline void
    rasterizeTriangle(VertexType t0, VertexType t1, VertexType t2) override {
        auto v0 = scaleToScreen(t0);
        auto v1 = scaleToScreen(t1);
        auto v2 = scaleToScreen(t2);

        if (v0.z <= 0 || v1.z <= 0 || v2.z <= 0)
            return;

        auto z0 = static_cast<decltype(VertexType::z)>(1) / (v0.z);
        auto z1 = static_cast<decltype(VertexType::z)>(1) / (v1.z);
        auto z2 = static_cast<decltype(VertexType::z)>(1) / (v2.z);

        auto maxX = std::max(std::max(v0.x, v1.x), v2.x); // right
        auto minX = std::min(std::min(v0.x, v1.x), v2.x); // left
        auto minY = std::min(std::min(v0.y, v1.y), v2.y); // top
        auto maxY = std::max(std::max(v0.y, v1.y), v2.y); // bottom

        auto A01 = -(v0.y - v1.y), B01 = -(v1.x - v0.x);
        auto A12 = -(v1.y - v2.y), B12 = -(v2.x - v1.x);
        auto A20 = -(v2.y - v0.y), B20 = -(v0.x - v2.x);

        VertexType pixel;
        pixel.x = minX;
        pixel.y = minY;
        // area of triangle is givben by the cross product times 1/2
        auto w0_row = edgeFunction(v1, v2, pixel);
        auto w1_row = edgeFunction(v2, v0, pixel);
        auto w2_row = edgeFunction(v0, v1, pixel);
        // area of whole triangle is given by this times a half
        auto area = edgeFunction(v0, v1, v2);
        if (area == 0)
            return;
        for (; pixel.y <= maxY; pixel.y++) {
            // Barycentric coordinates at start of row
            auto w0 = w0_row;
            auto w1 = w1_row;
            auto w2 = w2_row;
            for (pixel.x = minX; pixel.x <= maxX; pixel.x++) {
                // If p is on or inside all edges, render pixel.
                if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                    auto r0 =
                        w0 /
                        area; // the half is canclelled out by hte divvision
                    auto r1 = w1 / area;
                    auto r2 = w2 / area;
                    auto z = (z0 * r0 + z1 * r1 + z2 * r2);

                    int x = static_cast<int>(pixel.x);
                    int y = static_cast<int>(pixel.y);
                    int index = y * WIDTH + x;
                    if (z > zBuffer[index]) {
                        zBuffer[index] = z;
                        auto output =
                            ((t0 * r0) + (t1 * r1) + (t2 * r2)) *
                            (static_cast<decltype(VertexType::z)>(1) / z);

                        this->targetBuffer[static_cast<int>(pixel.y) * WIDTH +
                                           static_cast<int>(pixel.x)] = output;
                    }
                }
                // One step to the right
                w0 += A12;
                w1 += A20;
                w2 += A01;
            }

            // One row step
            w0_row += B12;
            w1_row += B20;
            w2_row += B01;
        }
    }
    void ClearZBuffer() { std::memset(zBuffer.data(), 0, sizeof(zBuffer)); }

private:
    inline decltype(VertexType::x) edgeFunction(const VertexType &a,
                                                const VertexType &b,
                                                const VertexType &c) {
        return -((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
    }

    std::array<decltype(VertexType::x), WIDTH * HEIGHT> zBuffer;

    inline VertexType scaleToScreen(VertexType a) {
        a.x = (a.x + 1) * 0.5f *
              static_cast<int>(WIDTH); // height and width should be tempalted
        a.y = (((a.y + 1) * -0.5f) + 1) * static_cast<int>(HEIGHT);
        return std::move(a);
    }
};

#endif // GRAPHICSAPI_BASICRASTERIZER_H
