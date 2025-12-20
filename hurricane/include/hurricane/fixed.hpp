#pragma once

#include <cstdint>
#include <cmath>
#include <limits>
#include <type_traits>

class fixed {
public:
    using storage_type = int32_t;
    using storage_type_wide = int64_t;
    static constexpr int frac_bits = 10;
    static constexpr storage_type scale = storage_type{1} << frac_bits;

private:
    storage_type value_{0};

    // Internal ctor from raw value
    constexpr inline explicit fixed(storage_type raw, bool) : value_(raw) {}

public:
    // ---- Constructors ----
    constexpr fixed() = default;

    // From floating point
    template <typename T>
    constexpr inline fixed(T v) : value_(static_cast<storage_type>(std::lround(v * scale))) {}

    // Factory from raw storage
    static constexpr inline fixed from_raw(storage_type raw) {
        return fixed(raw, true);
    }

    // ---- Arithmetic operators ----
    friend constexpr inline fixed operator+(fixed a, fixed b) {
        return fixed::from_raw(a.value_ + b.value_);
    }

    friend constexpr inline fixed operator-(fixed a, fixed b) {
        return fixed::from_raw(a.value_ - b.value_);
    }

    friend constexpr inline fixed operator-(fixed a) {
        return fixed::from_raw(-a.value_);
    }

    friend constexpr inline fixed operator*(fixed a, fixed b) {
        storage_type_wide tmp = static_cast<storage_type_wide>(a.value_) * b.value_;
        return fixed::from_raw(static_cast<storage_type>(tmp >> frac_bits));
    }

    friend constexpr inline fixed operator/(fixed a, fixed b) {
        storage_type_wide tmp = (static_cast<storage_type_wide>(a.value_) << frac_bits) / b.value_;
        return fixed::from_raw(static_cast<storage_type>(tmp));
    }

    inline fixed& operator+=(fixed other) { value_ += other.value_; return *this; }
    inline fixed& operator-=(fixed other) { value_ -= other.value_; return *this; }
    inline fixed& operator*=(fixed other) { *this = *this * other; return *this; }
    inline fixed& operator/=(fixed other) { *this = *this / other; return *this; }

    // ---- Comparison operators ----
    friend constexpr inline bool operator==(fixed a, fixed b) { return a.value_ == b.value_; }
    friend constexpr inline bool operator!=(fixed a, fixed b) { return a.value_ != b.value_; }
    friend constexpr inline bool operator<(fixed a, fixed b)  { return a.value_ < b.value_; }
    friend constexpr inline bool operator<=(fixed a, fixed b) { return a.value_ <= b.value_; }
    friend constexpr inline bool operator>(fixed a, fixed b)  { return a.value_ > b.value_; }
    friend constexpr inline bool operator>=(fixed a, fixed b) { return a.value_ >= b.value_; }
    constexpr inline operator int() { return this->value_ >> frac_bits; }
};

fixed constexpr inline operator "" _f(long double d) { return fixed(d); }

