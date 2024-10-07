

#include "Buffer.h"
#include "Graphics/BasicRasterizer.h"
#include "Graphics/GraphicsPipe.h" // Include graphics pipeline header
#include "Graphics/Shaders/FragmentShaderImpl.h"
#include "Graphics/Shaders/VertexShaderImpl.h"
#include "IWindow.h"
#include <chrono> // For time handling
#include <thread> // For sleep function
int main() {
    // Create a graphics pipeline

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


    using VertexShader = VertexShaderImpl<Ffloat>;
    using FragmentShader = FragmentShaderImpl<Ffloat>;
    using Rasterizer = BasicRasterizer<40, 20, Math::FixedVector4>;
    using GraphicsPipelineType = GraphicsPipeline<VertexShader, FragmentShader, Rasterizer, 40, 20>;

    IWindow<Buffer, GraphicsPipelineType, 40, 20> window;
    auto &pipeline = window.getPipe();

    pipeline.getVertexShader().setModelMatrix(Math::FixedMatrix4f::rotateZ(0.5) * Math::FixedMatrix4f::rotateX(0.5f));
    pipeline.getFragmentShader().pos +=0.01;

    window.bindVBO(*vertices);
    float theta = 0;

    using namespace std::chrono;

    auto frameDuration = milliseconds(100); // Duration of 100ms for 10fps
    auto previousTime = steady_clock::now();

    while (true) {
        // Get current time
        auto currentTime = steady_clock::now();
        // Calculate elapsed time
        auto elapsedTime =
                duration_cast<milliseconds>(currentTime - previousTime);

        if (elapsedTime >= frameDuration) {

            // Update time

            previousTime = currentTime;

            // Perform rendering

            window.drawVBO();
            window.swapBuffers();

            window.display();

            // Update shader parameters
            theta += 0.1f;
            pipeline.getVertexShader().setModelMatrix(
                    Math::FixedMatrix4f::rotateZ(theta) *
                    Math::FixedMatrix4f::rotateX(theta ));
            //pipeline.getFragmentShader().pos +=0.05;
        } else {
            // Sleep for the remaining time to cap the frame rate
            std::this_thread::sleep_for(frameDuration - elapsedTime);
        }
    }

}