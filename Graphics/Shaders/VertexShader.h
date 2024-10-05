//
// Created by matthew on 8/25/24.
//

#ifndef GRAPHICSAPI_VERTEXSHADER_H
#define GRAPHICSAPI_VERTEXSHADER_H
#include "Math.h"

template<class in,class out>
class VertexShader {
public:
    virtual inline out apply(const in&) =0;
    virtual ~VertexShader() =default;
    using In = in;
    using Out = out;
private:
};


#endif //GRAPHICSAPI_VERTEXSHADER_H
