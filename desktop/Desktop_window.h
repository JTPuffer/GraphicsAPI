#pragma once

#include "Buffer.h"
#include "IWindow.h"
#include <iostream>
template <class BufferType, class PipelineType,long unsigned int WIDTH, long unsigned int HEIGHT>
void IWindow<BufferType,PipelineType, WIDTH, HEIGHT>::display() const {
    static const char shadingArray[11] = " .:-=+*%@#";
    Pixel *data = this->front->getData();
    int bufferWidth = this->front->getWidth();
    int bufferHeight = this->front->getHeight();
    std::cout << "\033[2J";
    for (int y = 0; y < bufferHeight; ++y) {
        for (int x = 0; x < bufferWidth; ++x) {
            putchar(shadingArray[static_cast<int>(
                (data[y * bufferWidth + x].r + data[y * bufferWidth + x].g +
                 data[y * bufferWidth + x].b) /
                128)]);
        }
        putchar('\n');
    }
    //std::cout << bufferHeight << std::endl;
}
