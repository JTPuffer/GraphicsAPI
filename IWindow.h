#ifndef GRAPHICSAPI_IWINDOW_H
#define GRAPHICSAPI_IWINDOW_H

#include "Graphics/GraphicsPipe.h"
#include <memory>
#include <utility>

namespace {
template <typename Buffer>
concept BufferConcept = requires(Buffer buf, int x, int y, uint8_t r, uint8_t g,
                                 uint8_t b) {
    {
        buf.setValue(x, y, r, g, b)
        } -> std::same_as<void>;           // Requires setValue method
    { buf.clear() } -> std::same_as<void>; // Requires clear method (black)
    {
        buf.clear(r, g, b)
        } -> std::same_as<void>; // Requires clear method (color)
};

    template <typename Pipeline, typename Buffer>
    concept GraphicsPipelineConcept = requires(Pipeline pipeline, std::shared_ptr<Buffer> buf) {
        { pipeline.flush() } -> std::same_as<void>;
        //{ pipeline.bindVBO(std::declval<auto>()...) } -> std::same_as<void>;
        { pipeline.setTargetBuffer(buf) } -> std::same_as<void>;
        { pipeline.drawVBO() } -> std::same_as<void>;
    };
} // namespace

template <BufferConcept BufferType, GraphicsPipelineConcept<BufferType> PipelineType, size_t WIDTH, size_t HEIGHT> class IWindow {
  public:
    IWindow()
        : front(std::make_shared<BufferType>(WIDTH, HEIGHT)),
          back(std::make_shared<BufferType>(WIDTH, HEIGHT))
           {}

    ~IWindow() = default; // Non-virtual destructor, no vtable needed.

    // change this so the defonition is in the pico or desktop rather than
    // having this weird arse thing CRTP
    void display() const;

    void swapBuffers() {
        std::swap(front, back);
        graphicsPipeline.flush();
    }

    void clear() { back->clear(); }

    // Templated bindVBO method with perfect forwarding
    template <typename... Args> void bindVBO(Args &&...args) {
        graphicsPipeline.bindVBO(std::forward<Args>(args)...);
    }
    PipelineType & getPipe(){
        return graphicsPipeline;
    }
    void drawVBO() {
        graphicsPipeline.setTargetBuffer(back);
        graphicsPipeline.drawVBO();
    }

  protected:
    std::shared_ptr<BufferType>
        front; // id rather pass a refrence round to the buffer and then have
               // the data inside each buffer be a shared ptr
    std::shared_ptr<BufferType> back;
    PipelineType graphicsPipeline;

    [[nodiscard]] constexpr int getWidth() const { return WIDTH; }
    [[nodiscard]] constexpr int getHeight() const { return HEIGHT; }
};

#ifdef PLATFORM_DESKTOP
#include "Desktop_window.h"
#elif defined(PLATFORM_PICO)
#include "Pico_window.h"
#endif

#endif // GRAPHICSAPI_IWINDOW_H
