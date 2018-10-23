/*
Copyright (C) 2018 Jan Christian Rohde

This file is part of ampclib.

ampclib is free software; you can redistribute it and/or modify it under the terms of the
GNU General Public License as published by the Free Software Foundation; either version 3
of the License, or (at your option) any later version.

ampclib is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with ampclib; if not, see http://www.gnu.org/licenses.
*/

#pragma once

#include <vector>
#include <stdlib.h>
#include <string>
#include <algorithm>

namespace ampc {

constexpr uint64_t MaxLimit = std::numeric_limits<uint32_t>::max();
constexpr uint64_t Basis = MaxLimit + 1;
constexpr double B = (double)Basis;

constexpr uint32_t TenAdicBasis = 1000000000;
constexpr uint32_t TenBasisExp = 9;

class Integer {
public:
    Integer() : positive(true), absoluteValue{0} {}
    Integer(uint32_t i) : positive(true), absoluteValue{i} {}
    Integer(int32_t i) : positive(i >= 0), absoluteValue{(uint32_t)std::abs(i)} {}
    Integer(uint64_t i);
    Integer(int64_t i);
    Integer(std::string str) : Integer() {set(str);}

    void set(std::string str);
    void set(const uint32_t i);

    uint16_t toUnsignedShort() const {return positive ? absoluteValue[0] % (uint32_t(std::numeric_limits<uint16_t>::max()) + 1) : 0;}
    uint32_t toUnsignedInt() const {return positive ? absoluteValue[0] : 0;}
    int32_t toInt() const;
    uint64_t toUnsignedLongInt() const;
    int64_t toLongInt() const;

    double toDouble() const;

    void print(std::string &str) const;

    bool isZero() const {return absoluteValue.size() == 1 && absoluteValue[0] == 0;}
    bool isPositive() const {return positive || isZero();}
    bool isUnsigned16Bit() const {return isUnsigned32Bit() && absoluteValue[0] <= std::numeric_limits<uint16_t>::max();}
    bool isUnsigned32Bit() const {return isPositive() && absoluteValue.size() == 1;}
    bool isUnsigned64Bit() const {return isPositive() && absoluteValue.size() < 3;}
    bool isSigned64Bit() const;

    bool operator >(const Integer &rhs) const;
    bool operator >=(const Integer &rhs) const {return !(rhs > *this);}
    bool operator <(const Integer &rhs) const {return rhs >= *this;}
    bool operator <=(const Integer &rhs) const {return !(*this > rhs);}
    bool operator ==(const Integer &rhs) const;
    bool operator !=(const Integer &rhs) const {return !(*this == rhs);}

    Integer operator -() const;
    Integer operator +(const Integer &rhs) const;
    Integer operator -(const Integer &rhs) const;
    Integer operator *(const Integer &rhs) const;
    Integer operator /(const Integer &divisor) const;
    Integer operator %(const Integer &rhs) const;

    std::pair<Integer, Integer> divideWithRest(const Integer &rhs) const;

    Integer operator +(const uint32_t i) const;
    Integer operator -(const uint32_t i) const;
    Integer operator *(const uint32_t i) const;
    Integer operator /(const uint32_t i) const;
    uint32_t operator %(const uint32_t i) const;

    std::pair<Integer, uint32_t> divideWithRest(const uint32_t i) const;

    Integer &operator +=(const Integer &rhs);
    Integer &operator -=(const Integer &rhs);
    Integer &operator *=(const Integer &rhs);
    Integer &operator /=(const Integer &rhs);
    Integer &operator %=(const Integer &rhs);

    Integer &operator +=(const uint32_t i);
    Integer &operator -=(const uint32_t i);
    Integer &operator *=(const uint32_t i);
    Integer &operator /=(const uint32_t i);
    Integer &operator %=(const uint32_t i);

    Integer &operator --();
    Integer &operator ++();
    Integer operator --(int);
    Integer operator ++(int);

    Integer &power(Integer exponent);

    Integer &invertModulo(Integer n);
    Integer &powerModulo(Integer exponent, const Integer &mod);

    Integer &abs();
    Integer &minusAbs();

    bool millerRabinPrimalityTest();

    static Integer fibonacci(const uint32_t i);
    static Integer lucas(const uint32_t i);
    static Integer faculty(const uint32_t i);
    static Integer binomial(uint32_t a, const uint32_t b);
    static Integer multiSet(const uint32_t a, const uint32_t b);
    static Integer gcd(Integer a, Integer b);
    static Integer extendedEuclidianAlgo(Integer a, Integer b, Integer &r, Integer &s);
    static Integer lcm(const Integer &a, const Integer &b);

private:
    bool positive;
    std::vector<uint32_t> absoluteValue;

    void performAddition(bool positiveSignum, const Integer &rhs, Integer &result) const;

    bool isAbsoluteValueLarger(const std::vector<uint32_t> &lhs, const std::vector<uint32_t> &rhs) const;

    void addAbsValues(const std::vector<uint32_t> &larger, const std::vector<uint32_t> &smaller, std::vector<uint32_t> &result) const;
    void subtractAbsValues(const std::vector<uint32_t> &larger, const std::vector<uint32_t> &smaller, std::vector<uint32_t> &result) const;

    void addInt(std::vector<uint32_t> &result, const uint32_t i) const;
    void subtractInt(std::vector<uint32_t> &result, const uint32_t i) const;

    void divide(const std::vector<uint32_t> &numerator, const std::vector<uint32_t> &divisor, std::vector<uint32_t> &quotient) const;
    size_t getQuotientSize(const std::vector<uint32_t> &numerator, const std::vector<uint32_t> &divisor) const;
    bool isLargerFromIndex(const std::vector<uint32_t> &lhs,
                           const std::vector<uint32_t> &rhs,
                           const uint64_t lhsoverflow,
                           const uint32_t rhsoverflow,
                           size_t lhsIndex) const;
    void addFromIndex(std::vector<uint32_t> &value, uint32_t i, const size_t index) const;

    void multiplyAbsoluteValues(const std::vector<uint32_t> &factor1, const std::vector<uint32_t> &factor2, std::vector<uint32_t> &product) const;

    void multiplyVectorByUnsignedInt(const std::vector<uint32_t> &vec, const uint32_t i, std::vector<uint32_t> &result, uint32_t &overflow) const;

    void performDivision(const uint32_t i, std::vector<uint32_t> &result, uint32_t &overflow) const;
};

}
