#include "Buffer.h"
#include <utility>
//
// Created by matthew on 8/24/24.
//

#ifndef GRAPHICSAPI_IRASTERIZER_H
#define GRAPHICSAPI_IRASTERIZER_H

template <size_t WIDTH, size_t HEIGHT, typename VertexType> class IRasterizer {
public:
    using VertexBuffer = std::array<VertexType, WIDTH * HEIGHT>;
    explicit IRasterizer(VertexBuffer &_targetBuffer)
        : targetBuffer(_targetBuffer){};

    IRasterizer() = default;
    virtual ~IRasterizer() = default;

    // Each rasterizer must implement this function
    virtual void rasterizeTriangle(VertexType v0, VertexType v1,
                                   VertexType v2) = 0;
    void setTarget(VertexBuffer &_targetBuffer) {
        targetBuffer = _targetBuffer;
    }

protected:
    VertexBuffer &targetBuffer;
};

// if i have a buffer class

#endif // GRAPHICSAPI_IRASTERIZER_H
