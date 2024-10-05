//
// Created by matthew on 8/23/24.
//

#ifndef GRAPHICSAPI_VERTEXBUFFER_H
#define GRAPHICSAPI_VERTEXBUFFER_H

#include "FixedPoint.h"
#include <array>
#include <cmath>
#include <iostream>
#include <type_traits>

struct Vertex {
    Ffloat x;
    Ffloat y;
    Ffloat z;
};

template <size_t VertexCount, typename VertexType> class VertexBufferObject {
public:
    // Constructor
    VertexBufferObject() = default;

    template <typename... Args> explicit VertexBufferObject(Args &&...args) {
        static_assert(sizeof...(Args) == VertexCount,
                      "Number of arguments must match VertexCount");
        initializeBuffer(std::index_sequence_for<Args...>{},
                         std::forward<Args>(args)...);
    }
    VertexBufferObject(std::array<VertexType, VertexCount> &&array) {
        data = std::move(array);
    }
    VertexBufferObject(std::array<VertexType, VertexCount> &array) {
        data = array;
    }
    template <typename... Args> void setVertex(size_t index, Args &&...args) {
        data[index] = VertexType(std::forward<Args>(args)...);
    }
    template <typename... Args>
    const VertexType &getVertex(size_t index) const {
        return data[index];
    }

private:
    // Fixed-size array to store vertex data
    std::array<VertexType, VertexCount> data;
    template <size_t... Is, typename... Args>
    void initializeBuffer(std::index_sequence<Is...>, Args &&...args) {
        ((data[Is] = VertexType(std::forward<Args>(args))), ...);
    }
};

#endif // GRAPHICSAPI_VERTEXBUFFER_H
