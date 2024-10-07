//
// Created by matthew on 8/25/24.
//

#ifndef GRAPHICSAPI_FRAGMENTSHADERIMPL_H
#define GRAPHICSAPI_FRAGMENTSHADERIMPL_H

#include "FragmentShader.h"
#include "Math.h"
#include <cstring>
struct IPixel
{
    int8_t  r;
    int8_t  g;
    int8_t  b;
    IPixel() : r(0), g(0), b(0) {}
    IPixel(int8_t _r, int8_t _g, int8_t _b) : r(_r), g(_g), b(_b) {}
};


template<typename Type>
class FragmentShaderImpl : public FragmentShader<Math::Vector<Type,4>,IPixel>{
public:
    inline IPixel apply(Math::Vector<Type,4> &i) override {
        //light source at position 1,1,1
        //ambien occlusios
        if(std::memcmp(i.data,zero,4) == 0){
            return {};
        }
        i[3] = 0;
        auto temp = i.normalize().dotProduct({std::cos(pos),1,std::sin(pos),0});
        temp = std::max(temp,Type{0.1f});
        IPixel p;
        p.r = static_cast<int>( 1 *255);
        p.g =  static_cast<int>(1 *255);
        p.b = static_cast<int>( 1 *255);
        return p;
    }
    Type zero[4] = {0};
    float pos =0;
};
#endif //GRAPHICSAPI_FRAGMENTSHADERIMPL_H
