//
// Created by matthew on 8/25/24.
//

#ifndef GRAPHICSAPI_GRAPHICSPIPE_H
#define GRAPHICSAPI_GRAPHICSPIPE_H

#include "IGraphicsPipe.h"
#include "IRasterizer.h"
#include "Shaders/FragmentShader.h"
#include "Shaders/FragmentShaderImpl.h"
#include "Shaders/VertexShader.h"
#include "VertexBuffer.h"
#include <concepts>
#include <memory>
#include <vector>
#include <span>
namespace {
    template <typename T>
    concept Shader = requires(T a, typename T::In b) {
        typename T::Out;
        typename T::In;
        a.apply(b);
    };

} // namespace
// I need my own buffer for the fragment shader which is the same as the
// fragment shader input, WHY ARE THE SHADERS TEMPLATED
template <Shader Vertex, Shader Fragment, typename Rasterizer, size_t WIDTH,
          size_t HEIGHT>
class GraphicsPipeline {
public:
    constexpr GraphicsPipeline(Vertex vertexShader1, Fragment fragmentShader1,
                               Rasterizer rasterizer1)
        : vertexShader(vertexShader1), fragmentShader(fragmentShader1),
          rasterizer(rasterizer1) {
        static_assert(
            std::is_same_v<typename Vertex::Out, typename Fragment::In>,
            "Fragment shader input type must match Vertex shader output type.");
        std::memset(DefferdBuffer.data(), 0,
                    DefferdBuffer.size() * sizeof(typename Vertex::Out));
        rasterizer.setTarget(DefferdBuffer);
    }

    constexpr GraphicsPipeline()
        : vertexShader(Vertex()), fragmentShader(Fragment()),
          rasterizer(Rasterizer(DefferdBuffer)) {
        static_assert(
        static_assert(std::is_same_v<typename Vertex::Out, typename Fragment::In>,
                      "Vertex shader output type (Vertex::Out) does not match Fragment shader input type (Fragment::In)");
        std::memset(DefferdBuffer.data(), 0,
                    DefferdBuffer.size() * sizeof(typename Vertex::Out));
    }

    void setTargetBuffer(std::shared_ptr<Buffer> _targetBuffer) {
        targetBuffer = _targetBuffer;
    }
    void setTargetBuffer(std::shared_ptr<Buffer>&& _targetBuffer) {
        targetBuffer = std::move(_targetBuffer);
    }

    void bindVBO(std::span<typename Vertex::In> _vbo)  {
        vbo = _vbo;

        if (_vbo.size() % 3 != 0 || _vbo.empty()) {
            throw std::runtime_error("Invalid VBO: must be a multiple of 3 and not empty");
        }

    }

    void drawVBO() {
        // Transform vertices
        std::vector<typename Vertex::Out> transformedVertices;
        transformedVertices.reserve(vbo.size());

        std::transform(vbo.begin(), vbo.end(), std::back_inserter(transformedVertices),
                       [&](auto&& vertex) { return vertexShader.apply(vertex); });

        // Rasterize triangles
        for (size_t i = 0; i < transformedVertices.size(); i += 3) {
            rasterizer.rasterizeTriangle(transformedVertices[i],
                                         transformedVertices[i + 1],
                                         transformedVertices[i + 2]);
        }
    }

    void FragmentProcessBuffer() {
        auto *data = this->DefferdBuffer.data();
        for (int y = 0; y < HEIGHT; y++) {
            for (int x = 0; x < WIDTH; ++x) {
                auto colour = fragmentShader.apply(
                    data[(y * WIDTH) +
                         x]); // should change this to an it instead of set
                              // value to avoid the constant indexing
                targetBuffer->setValue(x, y, colour.r, colour.g, colour.b);
            }
        }
    }

    void flush()  {
        FragmentProcessBuffer();
        std::memset(DefferdBuffer.data(), 0,
                    DefferdBuffer.size() * sizeof(typename Vertex::Out));
        rasterizer.ClearZBuffer();
    }

    Vertex &getVertexShader() { return vertexShader; }

    Fragment &getFragmentShader() { return fragmentShader; }

private:
    std::span<typename Vertex::In> vbo;

    std::shared_ptr<Buffer> targetBuffer;
    std::array<typename Vertex::Out, WIDTH * HEIGHT> DefferdBuffer;

    Vertex vertexShader;
    Fragment fragmentShader;
    Rasterizer rasterizer;
};
#endif // GRAPHICSAPI_GRAPHICSPIPE_H
