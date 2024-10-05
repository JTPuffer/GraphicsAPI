

#include "Buffer.h"
#include "Graphics/BasicRasterizer.h"
#include "Graphics/GraphicsPipe.h" // Include graphics pipeline header
#include "Graphics/Shaders/FragmentShaderImpl.h"
#include "Graphics/Shaders/VertexShaderImpl.h"
#include "IWindow.h"

int main() {
    // Create a graphics pipeline
    auto pipeline = std::make_shared<GraphicsPipeline<
        VertexShaderImpl<Ffloat>, FragmentShaderImpl<Ffloat>,
        BasicRasterizer<80, 60, Math::FixedVector4>, 80, 60>>();

    // Create a window
    auto vertices = std::make_shared<std::array<Math::FixedVector3, 36>>(
        std::array<Math::FixedVector3, 36>({
            // back face
            {-0.5f, -0.5f, -0.5f}, // bottom-left
            {0.5f, 0.5f, -0.5f},   // top-right
            {0.5f, -0.5f, -0.5f},  // bottom-right
            {0.5f, 0.5f, -0.5f},   // top-right
            {-0.5f, -0.5f, -0.5f}, // bottom-left
            {-0.5f, 0.5f, -0.5f},  //
            //
            {-0.5f, -0.5f, 0.5f}, // bottom-left
            {0.5f, -0.5f, 0.5f},  // bottom-right
            {0.5f, 0.5f, 0.5f},   // top-right
            {0.5f, 0.5f, 0.5f},   // top-right
            {-0.5f, 0.5f, 0.5f},  // top-left
            {-0.5f, -0.5f, 0.5f}, // bo
            //
            {-0.5f, 0.5f, 0.5f},   // top-right
            {-0.5f, 0.5f, -0.5f},  // top-left
            {-0.5f, -0.5f, -0.5f}, // bottom-left
            {-0.5f, -0.5f, -0.5f}, // bottom-left
            {-0.5f, -0.5f, 0.5f},  // bottom-right
            {-0.5f, 0.5f, 0.5f},   //
            //
            {0.5f, 0.5f, 0.5f},   // top-left
            {0.5f, -0.5f, -0.5f}, // bottom-right
            {0.5f, 0.5f, -0.5f},  // top-right
            {0.5f, -0.5f, -0.5f}, // bottom-right
            {0.5f, 0.5f, 0.5f},   // top-left
            {0.5f, -0.5f, 0.5f},  // bo
            // b
            {-0.5f, -0.5f, -0.5f}, // top-right
            {0.5f, -0.5f, -0.5f},  // top-left
            {0.5f, -0.5f, 0.5f},   // bottom-left
            {0.5f, -0.5f, 0.5f},   // bottom-left
            {-0.5f, -0.5f, 0.5f},  // bottom-right
            {-0.5f, -0.5f, -0.5f}, //

            {-0.5f, 0.5f, -0.5f}, // top-left
            {0.5f, 0.5f, 0.5f},   // bottom-right
            {0.5f, 0.5f, -0.5f},  // top-right
            {0.5f, 0.5f, 0.5f},   // bottom-right
            {-0.5f, 0.5f, -0.5f}, // top-left
            {-0.5f, 0.5f, 0.5f}   // bottom-left
        }));
    pipeline->getVertexShader().setModelMatrix(
        Math::FixedMatrix4f::rotateZ(0.5) * Math::FixedMatrix4f::rotateX(0.5f));
    // pipeline->getFragmentShader().pos +=0.01;
    IWindow<Buffer, 80, 60> window(pipeline);
    window.bindVBO(static_cast<void *>(vertices->data()), vertices->size());
    window.drawVBO();

    window.swapBuffers();
    // window.flush();
    window.display();
}