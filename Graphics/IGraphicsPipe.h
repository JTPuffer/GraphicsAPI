//
// Created by matthew on 8/30/24.
//

#ifndef GRAPHICSAPI_IGRAPHICSPIPE_H
#define GRAPHICSAPI_IGRAPHICSPIPE_H

#include "Buffer.h"
#include <memory>

class IGraphicsPipeline {
public:
    virtual ~IGraphicsPipeline() = default;

    virtual void bindVBO(void *vbo, size_t size) = 0;

    virtual void drawVBO() = 0;

    virtual void setTargetBuffer(std::shared_ptr<Buffer> buffer) = 0;

    virtual void flush() = 0;
};

#endif // GRAPHICSAPI_IGRAPHICSPIPE_H
