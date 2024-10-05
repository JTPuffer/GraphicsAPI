# Create a static library for graphics code
add_library(GraphicsLib STATIC
    BasicRasterizer.h
    GraphicsPipe.cpp
    GraphicsPipe.h
    IGraphicsPipe.h
    IRasterizer.h
    VertexBuffer.h
    Shaders/FragmentShader.h
    Shaders/FragmentShaderImpl.h
    Shaders/VertexShader.h
    Shaders/VertexShaderImpl.h
)

# Add include directories if necessary
target_include_directories(GraphicsLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})