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
class GraphicsPipeline : public IGraphicsPipeline {
public:
    constexpr GraphicsPipeline(Vertex vertexShader1, Fragment fragmentShader1,
                               Rasterizer rasterizer1)
        : vertexShader(vertexShader1), fragmentShader(fragmentShader1),
          rasterizer(rasterizer1), vboSize(0) {
        static_assert(
            std::is_same_v<typename Vertex::Out, typename Fragment::In>,
            "Fragment shader input type must match Vertex shader output type.");
        std::memset(DefferdBuffer.data(), 0,
                    DefferdBuffer.size() * sizeof(typename Vertex::Out));
        rasterizer.setTarget(DefferdBuffer);
    }

    constexpr GraphicsPipeline()
        : vertexShader(Vertex()), fragmentShader(Fragment()), vboSize(0),
          rasterizer(Rasterizer(DefferdBuffer)) {
        static_assert(
            std::is_same_v<typename Vertex::Out, typename Fragment::In>,
            "Fragment shader input type must match Vertex shader output type.");
        std::memset(DefferdBuffer.data(), 0,
                    DefferdBuffer.size() * sizeof(typename Vertex::Out));
    }

    void setTargetBuffer(std::shared_ptr<Buffer> _targetBuffer) {
        targetBuffer = _targetBuffer;
    }

    // Remove this awful void ptr
    void bindVBO(void *_vbo, size_t size) override {
        // Attempt to cast the type-erased pointer to the expected type
        vbo = _vbo;

        // Check if the vertex count is a multiple of 3
        if (size % 3 != 0) {
            std::cerr
                << "Error: Vertex count must be a multiple of 3. Current size: "
                << size << std::endl;
            throw std::runtime_error("Vertex count must be a multiple of 3");
        }
        if (size == 0) {
            throw std::runtime_error("Error: Vertex count is 0:");
        }

        vboSize = size; // Update the size after successful cast and check
    }

    void drawVBO() override {
        // Perform the casting in drawVBO, which should succeed if bindVBO was
        // correct

        // Transform vertices
        std::vector<typename Vertex::Out> transformedVertices;
        transformedVertices.reserve(vboSize);

        auto *end = static_cast<typename Vertex::In *>(vbo) + vboSize;
        auto *start = static_cast<Vertex::In *>(vbo);

        for (typename Vertex::In *it = start; it != end; ++it) {
            transformedVertices.push_back(vertexShader.apply(*it));
        }

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
                if (colour.r != 0) {
                    std::cout << "here";
                }
                targetBuffer->setValue(y, x, colour.r, colour.g, colour.b);
            }
        }
    }

    void flush() override {
        FragmentProcessBuffer();
        std::memset(DefferdBuffer.data(), 0,
                    DefferdBuffer.size() * sizeof(typename Vertex::Out));
        rasterizer.ClearZBuffer();
    }

    Vertex &getVertexShader() { return vertexShader; }

    Fragment &getFragmentShader() { return fragmentShader; }

private:
    void *vbo;
    size_t vboSize;

    std::shared_ptr<Buffer> targetBuffer;
    std::array<typename Vertex::Out, WIDTH * HEIGHT> DefferdBuffer;

    Vertex vertexShader;
    Fragment fragmentShader;
    Rasterizer rasterizer;
};
#endif // GRAPHICSAPI_GRAPHICSPIPE_H
