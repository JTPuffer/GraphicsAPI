//
// Created by matthew on 8/24/24.
//

#ifndef GRAPHICSAPI_MATH_H
#define GRAPHICSAPI_MATH_H
#include "FixedPoint.h"
#include <cmath>
#include <iostream>

namespace Math {

template <typename T, int8_t size> class Vector;
template <typename T, int8_t row, int8_t colum> class Matrix;

typedef Matrix<float, 4, 4> Matrix4f;
typedef Matrix<float, 3, 3> Matrix3f;

typedef Vector<float, 4> Vector4f;
typedef Vector<float, 3> Vector3f;

using FixedVector4 = Vector<Ffloat, 4>;
using FixedVector3 = Vector<Ffloat, 3>;
using FixedMatrix4f = Matrix<Ffloat, 4, 4>;
using FixedMatrix3f = Matrix<Ffloat, 3, 3>;

template <typename T, int8_t row, int8_t colum> class Matrix {
  public:
    T data[row][colum];

    // Default constructor initializing an identity matrix YOU DUMB FUCK
    constexpr Matrix() : data{} {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < colum; j++) {
                data[i][j] = 0;
            }
        }
    }
    //        static constexpr Matrix<T, 4, 4> iDentityMatrix() {
    //
    //            for (int i = 0; i < row; i++) {
    //                for (int j = 0; j < colum; j++) {
    //                    data[i][j] = (i == j) ? 1 : 0;
    //                }
    //            }
    //        }

    // Constructor using parameter pack to initialize the matrix
    template <typename... Args>
    constexpr Matrix(Args... args) : data{static_cast<T>(args)...} {
        static_assert(row > 0 && colum > 0,
                      "Matrix dimensions must be greater than zero");
        static_assert(sizeof...(args) == row * colum,
                      "Number of elements must match matrix size");
    }

    // Destructor
    constexpr ~Matrix() = default;

    static constexpr Matrix<T, 4, 4> rotateX(float angle) {
        T cosTheta = std::cos(angle);
        T sinTheta = std::sin(angle);

        return Matrix<T, 4, 4>(1, 0, 0, 0, 0, cosTheta, -sinTheta, 0, 0,
                               sinTheta, cosTheta, 0, 0, 0, 0, 1);
    }

    static constexpr Matrix<T, 4, 4> rotateY(T angle) {
        T cosTheta = std::cos(angle);
        T sinTheta = std::sin(angle);

        return Matrix<T, 4, 4>(cosTheta, 0, sinTheta, 0, 0, 1, 0, 0, -sinTheta,
                               0, cosTheta, 0, 0, 0, 0, 1);
    }
    static constexpr Matrix<T, 4, 4> rotateZ(float angle) {
        T cosTheta = std::cos(angle);
        T sinTheta = std::sin(angle);

        return Matrix<T, 4, 4>(cosTheta, -sinTheta, 0, 0, sinTheta, cosTheta, 0,
                               0, 0, 0, 1, 0, 0, 0, 0, 1);
    }

    constexpr Vector<T, colum> operator*(const Vector<T, colum> &v) const {
        Vector<T, colum> temp;
        for (int8_t i = 0; i < colum; i++) {
            for (int8_t j = 0; j < row; j++) {
                temp.data[i] += data[i][j] * v.data[j];
            }
        }
        return temp;
    }

    constexpr Matrix operator*(const Matrix<T, row, colum> &m) const {
        Matrix<T, row, colum> temp{};
        for (int i = 0; i < row; ++i) {
            for (int k = 0; k < colum; ++k) {
                for (int j = 0; j < row; ++j) {
                    temp.data[i][j] += data[i][k] * m.data[k][j];
                }
            }
        }
        return temp;
    }

    void print_matrix() const {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < colum; j++) {
                std::cout << "\t" << data[i][j];
            }
            printf("\n");
        }
    }
};

template <typename T, int8_t Size> class Vector {
  public:
    union { // better hope its bigger than 3
        T data[Size];
        struct {
            T x, y, z;
        };
    };

    // Constructor with parameter pack to initialize the vector
    template <typename... Args>
    constexpr Vector(Args... args) : data{static_cast<T>(args)...} {
        static_assert(sizeof...(args) <= Size, "Too many elements in vector");
    }

    // Default constructor
    constexpr Vector() : data{} {}

    // Destructor
    ~Vector() = default;

    // Addition operator
    constexpr Vector operator+(const Vector &v) const {
        Vector result;
        for (int8_t i = 0; i < Size; ++i) {
            result.data[i] = data[i] + v.data[i];
        }
        return result;
    }

    // Component-wise multiplication operator
    constexpr Vector operator*(const Vector &v) const {
        Vector result;
        for (int8_t i = 0; i < Size; ++i) {
            result.data[i] = data[i] * v.data[i];
        }
        return result;
    }

    constexpr Vector operator*(const T &v) const {
        Vector result;
        for (int8_t i = 0; i < Size; ++i) {
            result.data[i] = data[i] * v;
        }
        return result;
    }
    // Matrix multiplication operator
    template <int8_t MatrixSize>
    constexpr Vector<T, Size>
    operator*(const Matrix<T, MatrixSize, MatrixSize> &m) const {
        static_assert(
            Size == MatrixSize,
            "Matrix multiplication requires matching vector and matrix sizes.");
        Vector<T, Size> temp;
        for (int8_t i = 0; i < Size; i++) {
            for (int8_t j = 0; j < Size; j++) {
                temp.data[i] += m.data[i][j] * data[j];
            }
        }
        return temp;
    }

    // Indexing operator
    constexpr T &operator[](int num) { return data[num]; }

    constexpr const T &operator[](int num) const { return data[num]; }

    constexpr Vector<T, Size> normalize() const {
        T total = 0;
        for (int8_t i = 0; i < Size; i++) {
            total += (data[i] * data[i]);
        }
        total = std::sqrt(static_cast<float>(total));
        Vector<T, Size> temp;
        for (int8_t i = 0; i < Size; i++) {
            temp.data[i] = data[i] / total;
        }
        return temp;
    }

    constexpr T dotProduct(const Vector<T, Size> &v1) const {
        T total = 0;
        for (int8_t i = 0; i < Size; i++) {
            total += (data[i] * v1.data[i]);
        }
        return total;
    }

    template <int8_t newSize, typename... Args>
    constexpr Vector<T, newSize> changeShape(Args... args) const {
        Vector<T, newSize> ret;

        if constexpr (newSize > Size) {
            for (int8_t i = 0; i < Size; ++i) {
                ret.data[i] = data[i];
            }
            static_assert(sizeof...(args) >= (newSize - Size),
                          "Not enough arguments provided");
            auto argIt = std::begin(std::array{args...});
            for (int8_t i = Size; i < newSize; ++i) {
                ret.data[i] = *(argIt++);
            }
        } else {
            // Just copy existing elements
            for (int8_t i = 0; i < newSize; ++i) {
                ret.data[i] = data[i];
            }
        }

        return ret;
    }
};

template <typename T>
Vector<T, 3> crossProduct3(Vector<T, 3> &edge1, Vector<T, 3> &edge2) {
    return Vector<T, 3>(
        edge1.data[1] * edge2.data[2] -
            edge1.data[2] * edge2.data[1], // cross product
        edge1.data[2] * edge2.data[0] - edge1.data[0] * edge2.data[2],
        edge1.data[0] * edge2.data[1] - edge1.data[1] * edge2.data[0]);
}
template <typename T>
Vector<T, 3> normal3(Vector<T, 3> &v0, Vector<T, 3> &v1, Vector<T, 3> &v2) {
    Vector<T, 3> edge1 = Vector<T, 3>(
        v1[0] - v0.data[0], v1.data[1] - v0.data[1], v1.data[2] - v0.data[2]);
    Vector<T, 3> edge2 =
        Vector<T, 3>(v2.data[0] - v0.data[0], v2.data[1] - v0.data[1],
                     v2.data[2] - v0.data[2]);

    return crossProduct3(edge1, edge2);
}
template <typename T>
inline constexpr Vector<T, 4>
multiplyMatrix4x4WithVector3(const Matrix<T, 4, 4> &matrix,
                             const Vector<T, 3> &vec) {
    // Extend the 3D vector to 4D by adding a w-component of 1
    Vector<T, 4> result;

    // Perform the matrix multiplication
    result.data[0] = matrix.data[0][0] * vec.data[0] +
                     matrix.data[0][1] * vec.data[1] +
                     matrix.data[0][2] * vec.data[2] + matrix.data[0][3];

    result.data[1] = matrix.data[1][0] * vec.data[0] +
                     matrix.data[1][1] * vec.data[1] +
                     matrix.data[1][2] * vec.data[2] + matrix.data[1][3];

    result.data[2] = matrix.data[2][0] * vec.data[0] +
                     matrix.data[2][1] * vec.data[1] +
                     matrix.data[2][2] * vec.data[2] + matrix.data[2][3];

    result.data[3] = matrix.data[3][0] * vec.data[0] +
                     matrix.data[3][1] * vec.data[1] +
                     matrix.data[3][2] * vec.data[2] + matrix.data[3][3];

    return result;
}

} // namespace Math
#endif // GRAPHICSAPI_MATH_H
