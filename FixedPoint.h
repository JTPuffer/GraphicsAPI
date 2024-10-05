//
// Created by matthew on 8/23/24.
//

#ifndef GRAPHICSAPI_FIXEDPOINT_H
#define GRAPHICSAPI_FIXEDPOINT_H
#include <limits>
#include <type_traits>
template <typename T>
concept Integral = std::is_integral_v<T>;

template <typename T, int Precision>
concept ValidPrecision = (Precision >= 0 &&
                          Precision < std::numeric_limits<T>::digits);

template <Integral T, int Precision, Integral Intermediate = long long>
requires ValidPrecision<T, Precision>
class FixedPoint {
  public:
    static constexpr T FIXED_POINT_SCALE = 1 << Precision;
    static constexpr int FIXED_POINT_SHIFT = Precision;

    FixedPoint() : value(0) {}
    constexpr FixedPoint(float f)
        : value(static_cast<T>(f * FIXED_POINT_SCALE)) {}
    constexpr FixedPoint(int v)
        : value(static_cast<T>(v) << FIXED_POINT_SHIFT) {
    } // mark me as excplicit

    static constexpr inline FixedPoint fromRaw(T rawValue) {
        FixedPoint fp;
        fp.value = rawValue;
        return fp;
    }
    static constexpr inline FixedPoint maxValue() {
        // Compute the maximum value for the fixed-point representation
        return FixedPoint::fromRaw(std::numeric_limits<T>::max());
    }

    [[nodiscard]] inline float toFloat() const {
        return static_cast<float>(value) / FIXED_POINT_SCALE;
    }
    friend std::ostream &operator<<(std::ostream &os, const FixedPoint &fp) {
        os << std::to_string(fp.toFloat());
        return os;
    }
    explicit operator int() const {
        return static_cast<int>(value >> FIXED_POINT_SHIFT);
    }
    explicit operator float() const {
        return static_cast<float>(value) / FIXED_POINT_SCALE;
        ;
    }
    inline FixedPoint operator*(const FixedPoint &other) const {
        return FixedPoint::fromRaw(
            ((static_cast<Intermediate>(value) * other.value) >>
             FIXED_POINT_SHIFT) |
            static_cast<T>(1));
    }

    inline FixedPoint operator/(const FixedPoint &other) const {
        return FixedPoint::fromRaw(
            (static_cast<Intermediate>(value) << FIXED_POINT_SHIFT) /
            other.value);
    }

    inline FixedPoint &operator*=(const FixedPoint &other) {
        value = ((static_cast<Intermediate>(value) * other.value) >>
                 FIXED_POINT_SHIFT) |
                static_cast<T>(1);
        return *this;
    }

    inline FixedPoint &operator/=(const FixedPoint &other) {
        value = (static_cast<Intermediate>(value) << FIXED_POINT_SHIFT) /
                other.value;
        return *this;
    }

    inline FixedPoint &operator+=(const FixedPoint &other) {
        value += other.value;
        return *this;
    }

    inline FixedPoint &operator-=(const FixedPoint &other) {
        value -= other.value;
        return *this;
    }

    inline FixedPoint operator+(const FixedPoint &other) const {
        return FixedPoint::fromRaw(value + other.value);
    }

    inline FixedPoint operator-(const FixedPoint &other) const {
        return FixedPoint::fromRaw(value - other.value);
    }
    inline FixedPoint operator-() const { return FixedPoint::fromRaw(-value); }
    inline bool operator==(const FixedPoint &other) const {
        return value == other.value;
    }

    inline bool operator!=(const FixedPoint &other) const {
        return value != other.value;
    }

    inline bool operator<(const FixedPoint &other) const {
        return value < other.value;
    }

    inline bool operator<=(const FixedPoint &other) const {
        return value <= other.value;
    }

    inline bool operator>(const FixedPoint &other) const {
        return value > other.value;
    }

    inline bool operator>=(const FixedPoint &other) const {
        return value >= other.value;
    }

    FixedPoint &operator++() {
        value += FIXED_POINT_SCALE;
        return *this;
    }
    FixedPoint operator++(int) {
        FixedPoint temp = *this;
        value += FIXED_POINT_SCALE;
        return temp;
    }

  private:
    T value;
};
using Ffloat = FixedPoint<int, 10>;

#endif // GRAPHICSAPI_FIXEDPOINT_H
