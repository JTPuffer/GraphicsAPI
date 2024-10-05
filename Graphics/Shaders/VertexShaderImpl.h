//
// Created by matthew on 8/25/24.
//

#ifndef GRAPHICSAPI_VERTEXSHADERIMPL_H
#define GRAPHICSAPI_VERTEXSHADERIMPL_H

#include "VertexShader.h"

//namespace {
//    template<typename T>
//    concept HasXYZ = requires(T v) {
//        { v.x } ;
//        { v.y } ;
//        { v.z } ;
//    };
//}

template<typename Type>
class VertexShaderImpl : public VertexShader<Math::Vector<Type,3>,Math::Vector<Type,4>> {
public:
    void setModelMatrix(const Math::Matrix<Type, 4, 4> &_modelMatrix) {
        VertexShaderImpl::modelMatrix = _modelMatrix;
    }
    void setViewMatrix(const Math::Matrix<Type, 4, 4> &_viewMatrix) {
        VertexShaderImpl::viewMatrix = _viewMatrix;
    }
    void setProjectionMatrix(const Math::Matrix<Type, 4, 4> &_projectionMatrix) {
        VertexShaderImpl::projectionMatrix = _projectionMatrix;
    }
    void setModelMatrix(Math::Matrix<Type, 4, 4>&& _modelMatrix) {
        VertexShaderImpl::modelMatrix = std::move(_modelMatrix);
    }
    void setViewMatrix(Math::Matrix<Type, 4, 4>&& _viewMatrix) {
        VertexShaderImpl::viewMatrix = std::move(_viewMatrix);
    }
    void setProjectionMatrix(Math::Matrix<Type, 4, 4>&& _projectionMatrix) {
        VertexShaderImpl::projectionMatrix = std::move(_projectionMatrix);
    }

    inline Math::Vector<Type,4> apply(const Math::Vector<Type,3>& vector) override {
        //projectionMatrix * viewMatrix * Math::multiplyMatrix4x4WithVector3(modelMatrix , Vector)
        auto temp = Math::multiplyMatrix4x4WithVector3(modelMatrix , vector);
        temp.z +=0.3f;
        return temp;
    }
private:
    Math::Matrix<Type, 4, 4> projectionMatrix;
    Math::Matrix<Type, 4, 4> viewMatrix;
    Math::Matrix<Type, 4, 4> modelMatrix;
};


#endif //GRAPHICSAPI_VERTEXSHADERIMPL_H
