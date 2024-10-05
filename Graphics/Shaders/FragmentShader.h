//
// Created by matthew on 8/25/24.
//

#ifndef GRAPHICSAPI_FRAGMENTSHADER_H
#define GRAPHICSAPI_FRAGMENTSHADER_H

#include "FixedPoint.h"


namespace {
    template<typename T>
    concept HasRGB = requires(T v) {
        { v.r } ;
        { v.g } ;
        { v.b } ;
    };
}

template<class in,class out>
class FragmentShader {
public:
    virtual inline out apply( in&) =0;

    virtual ~FragmentShader() =default;



    using In = in;
    using Out = out;
private:
};



#endif //GRAPHICSAPI_FRAGMENTSHADER_H
