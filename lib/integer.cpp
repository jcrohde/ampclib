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

#include <integer.h>
#include <primes.h>

namespace ampc {

Integer::Integer(uint64_t i) :
    positive(true),
    absoluteValue{(uint32_t)(i % Basis), (uint32_t)(i / Basis)}
{
    if (absoluteValue[1] == 0) absoluteValue.pop_back();
}

Integer::Integer(int64_t i) :
    positive(i >= 0),
    absoluteValue{(uint32_t)(std::abs(i) % Basis), (uint32_t)(std::abs(i)/Basis)}
{
    if (absoluteValue[1] == 0) absoluteValue.pop_back();
}

void Integer::set(std::string str) {
    absoluteValue.clear();
    absoluteValue.push_back(0);
    positive = true;

    bool positive = true;
    if (str.at(0) == '-') {
        positive = false;
        str = str.substr(1);
    }
    std::string substring;
    while (str.size() > 0) {
        if (str.size() >= TenBasisExp) {
            substring = str.substr(0, TenBasisExp);
            str = str.substr(TenBasisExp);
            *this = *this * TenAdicBasis + Integer(std::stoi(substring));
        }
        else {
            substring = str;
            uint32_t ui = 1;
            for (uint32_t i = 0; i < str.size(); i++) {
                ui *= 10;
            }
            str.clear();
            *this = *this * ui + Integer(std::stoi(substring));
        }
    }
    this->positive = positive;
}

void Integer::set(const uint32_t i) {
    absoluteValue.resize(1, 0);
    absoluteValue[0] = i;
    positive = true;
}

int32_t Integer::toInt() const {
    if (positive) return absoluteValue[0] % ((uint32_t)std::numeric_limits<int>::max() + 1);
    else {
        const int64_t m = -((int64_t)std::numeric_limits<int>::min());
        return -(absoluteValue[0] % (m + 1));
    }
}

uint64_t Integer::toUnsignedLongInt() const {
    if (positive) return absoluteValue.size() >= 2 ? Basis * absoluteValue[1] + absoluteValue[0] : absoluteValue[0];
    else return 0;
}

int64_t Integer::toLongInt() const {
    if (absoluteValue.size() == 1) return (int64_t)absoluteValue[0] * (positive ? 1 : -1);
    else {
        uint64_t value = absoluteValue[0] + Basis*absoluteValue[1];
        if (positive) return (int64_t)value;
        else {
            const uint64_t m = (uint64_t)std::numeric_limits<int64_t>::max() + 2;
            return -(int64_t)(value % m);
        }
    }
}

double Integer::toDouble() const {
    double d = 0.0;
    double sthPowerOfBasis = 1.0;

    size_t size = absoluteValue.size();
    for (size_t s = 0; s < size; ++s) {
        d += sthPowerOfBasis * absoluteValue[s];
        sthPowerOfBasis *= B;
    }
    if (!positive) d = -d;
    return d;
}

void Integer::print(std::string &str) const {
    str.clear();

    Integer local = *this;
    local.positive = true;

    unsigned int count = 0;
    do {
        while (count * TenBasisExp > str.size()) str = "0" + str;

        str = std::to_string(local % TenAdicBasis) + str;

        local = local / TenAdicBasis;

        ++count;
    } while (!local.isZero());

    if (!positive) str = "-" + str;
}

bool Integer::isSigned64Bit() const {
    if (absoluteValue.size() > 2) return false;
    if (absoluteValue.size() == 1) return true;

    if (isPositive())
        return absoluteValue[0] + Basis * absoluteValue[1]
                <= (uint64_t)std::numeric_limits<int64_t>::max();
    else
        return absoluteValue[0] + Basis * absoluteValue[1]
                <= (uint64_t)std::numeric_limits<int64_t>::max() + 1;
}

bool Integer::operator >(const Integer &rhs) const {
    if (!positive && rhs.positive) return false;
    else if (positive && !rhs.positive && (!rhs.isZero() || !isZero())) return true;

    return positive == isAbsoluteValueLarger(absoluteValue, rhs.absoluteValue);
}

bool Integer::operator ==(const Integer &rhs) const {
    return (positive == rhs.positive || isZero())
         && absoluteValue == rhs.absoluteValue;
}

Integer Integer::operator -() const {
    Integer result(*this);
    result.positive = !positive;
    return result;
}

Integer Integer::operator +(const Integer &rhs) const {
    Integer result;
    performAddition(positive == rhs.positive, rhs, result);
    return result;
}

Integer Integer::operator -(const Integer &rhs) const {
    Integer result;
    performAddition(rhs.positive != positive, rhs, result);
    return result;
}

Integer Integer::operator *(const Integer &rhs) const {
    Integer result;

    if (isZero() || rhs.isZero()) {
        result.absoluteValue.resize(1, 0);
        return result;
    }

    result.positive = positive == rhs.positive;

    multiplyAbsoluteValues(absoluteValue, rhs.absoluteValue, result.absoluteValue);

    return result;
}

Integer Integer::operator /(const Integer &divisor) const {
    Integer result;
    result.positive = positive == divisor.positive;
    divide(absoluteValue, divisor.absoluteValue, result.absoluteValue);
    return result;
}

Integer Integer::operator %(const Integer &rhs) const {
    Integer quotient;
    quotient.positive = rhs.positive == positive;
    divide(absoluteValue, rhs.absoluteValue, quotient.absoluteValue);
    if (positive) return *this - rhs * quotient;
    else {
        Integer z = rhs * quotient;
        addAbsValues(z.absoluteValue, rhs.absoluteValue, z.absoluteValue);
        return *this - z;
    }
}

std::pair<Integer, Integer> Integer::divideWithRest(const Integer &rhs) const {
    Integer quotient;
    quotient.positive = positive == rhs.positive;
    divide(absoluteValue, rhs.absoluteValue, quotient.absoluteValue);

    if (positive) return std::make_pair(quotient, *this - rhs * quotient);

    Integer z = rhs * quotient;

    std::vector<uint32_t> sum;
    addAbsValues(z.absoluteValue, rhs.absoluteValue, sum);
    subtractAbsValues(sum, absoluteValue, z.absoluteValue);
    z.positive = true;

    return std::make_pair(quotient, z);
}

Integer Integer::operator +(const uint32_t i) const {
    Integer result;

    if (absoluteValue.size() == 1 && !positive && i >= absoluteValue[0]) {
        result.absoluteValue[0] = i - absoluteValue[0];
        result.positive = true;
    }
    else {
        result.positive = positive;
        if (positive) addInt(result.absoluteValue, i);
        else subtractInt(result.absoluteValue, i);
    }

    return result;
}

Integer Integer::operator -(const uint32_t i) const {
    Integer result;

    if (absoluteValue.size() == 1 && positive && i >= absoluteValue[0]) {
        result.absoluteValue[0] = i - absoluteValue[0];
        result.positive = false;
    }
    else {
        result.positive = positive;
        if (positive) subtractInt(result.absoluteValue, i);
        else addInt(result.absoluteValue, i);
    }

    return result;
}

Integer Integer::operator *(const uint32_t i) const {
    size_t size = absoluteValue.size();
    Integer result;
    result.absoluteValue.resize(size, 0);
    result.positive = positive;

    uint64_t overflow = 0;
    uint64_t sum = 0;

    for (size_t s = 0 ; s < size ; ++s) {
        sum = overflow + (const uint64_t)absoluteValue[s] * i;
        result.absoluteValue[s] = sum % Basis;
        overflow = sum / Basis;
    }

    if (overflow > 0) result.absoluteValue.push_back((uint32_t)overflow);

    return result;
}

Integer Integer::operator /(const uint32_t i) const {
    Integer result;
    result.positive = positive;

    uint32_t overflow = 0;
    performDivision(i, result.absoluteValue, overflow);

    return result;
}

uint32_t Integer::operator %(const uint32_t i) const {
    std::vector<uint32_t> result;

    uint32_t overflow = 0;
    performDivision(i, result, overflow);

    if (!positive) overflow = i - overflow;

    return overflow;
}

std::pair<Integer, uint32_t> Integer::divideWithRest(const uint32_t i) const{
    Integer result;
    result.positive = positive;

    uint32_t overflow = 0;
    performDivision(i, result.absoluteValue, overflow);

    if (!positive) overflow = i - overflow;

    return std::make_pair(result, overflow);
}

Integer &Integer::operator +=(const Integer &rhs) {
    *this = *this + rhs;
    return *this;
}

Integer &Integer::operator -=(const Integer &rhs) {
    *this = *this - rhs;
    return *this;
}

Integer &Integer::operator *=(const Integer &rhs) {
    *this = *this * rhs;
    return *this;
}

Integer &Integer::operator /=(const Integer &rhs) {
    *this = *this / rhs;
    return *this;
}

Integer &Integer::operator %=(const Integer &rhs) {
    *this = *this % rhs;
    return *this;
}

Integer &Integer::operator +=(const uint32_t i) {
    *this = *this + i;
    return *this;
}

Integer &Integer::operator -=(const uint32_t i) {
    *this = *this - i;
    return *this;
}

Integer &Integer::operator *=(const uint32_t i) {
    *this = *this * i;
    return *this;
}

Integer &Integer::operator /=(const uint32_t i) {
    *this = *this / i;
    return *this;
}

Integer &Integer::operator %=(const uint32_t i) {
    *this = *this % i;
    return *this;
}

Integer &Integer::operator --() {
    *this = *this - 1;
    return *this;
}

Integer &Integer::operator ++() {
    *this = *this + 1;
    return *this;
}

Integer Integer::operator --(int) {
    Integer Integer(*this);
    *this = *this - 1;
    return Integer;
}

Integer Integer::operator ++(int) {
    Integer z(*this);
    *this = *this + 1;
    return z;
}

Integer &Integer::power(Integer exponent) {
    if (exponent.isPositive()) {
        Integer base = *this;
        *this = Integer(1);
        while (!exponent.isZero()) {
            if ((exponent % 2) == 1) *this *= base;
            exponent /= 2;
            base *= base;
        }
    }
    return *this;
}

Integer &Integer::invertModulo(Integer n) {
    positive = true;
    n.positive = true;

    Integer r, u;
    if (n > *this) {
        std::swap(*this, n);
        r.set(0);
        u.set(1);
    }
    else {
        r.set(1);
        u.set(0);
    }

    if (!n.isZero()) {
        while (true) {
            auto q = divideWithRest(n);
            if (q.second.isZero()) break;

            *this = n;
            n = q.second;

            Integer memory = u;
            u = r - q.first * u;
            r = memory;
        }
    }

    *this = u;
    return *this;
}

Integer &Integer::powerModulo(Integer exponent, const Integer &mod) {
    if (exponent.isPositive()) {
        Integer base = *this % mod;
        *this = Integer(1);
        while (!exponent.isZero()) {
            if ((exponent % 2) == 1) {
                *this *= base;
                *this %= mod;
            }
            exponent /= 2;
            base *= base;
            base %= mod;
        }
    }
    return *this;
}

Integer &Integer::abs() {
    positive = true;
    return *this;
}

Integer &Integer::minusAbs() {
    positive = false;
    return *this;
}

bool Integer::millerRabinPrimalityTest() {
    if (absoluteValue.size() == 1) return Primes::getInstance().isPrime(absoluteValue[0]);

    if (*this % 2 == 0) return false;

    Integer a = *this / 2;
    Integer d = *this - 1;
    uint64_t powerOfTwo = 0;

    while (d % 2 == 0) {
        d /= 2;
        ++powerOfTwo;
    }

    Integer power = a;
    power.powerModulo(d, *this);

    if (power == Integer(1) || power == *this - 1) return true;

    for (uint64_t j = 1; j < powerOfTwo; ++j) {
        a *= a;
        if (power * a == *this - 1) return true;
    }

    return false;
}

Integer Integer::fibonacci(const uint32_t i) {
    if (i < 2) return Integer(i);

    Integer result;
    std::vector<uint32_t> f, g{1}, h{0};

    for (uint32_t j = 2; j <= i; ++j) {
        result.addAbsValues(g, h, f);
        h = g;
        g = f;
    }

    result.absoluteValue = f;

    return result;
}

Integer Integer::lucas(const uint32_t i) {
    if (i == 0) return Integer(2);
    if (i == 1) return Integer(1);

    Integer result;
    std::vector<uint32_t> f, g{1}, h{2};

    for (uint32_t j = 2; j <= i; ++j) {
        result.addAbsValues(g, h, f);
        h = g;
        g = f;
    }

    result.absoluteValue = f;

    return result;
}

Integer Integer::faculty(const uint32_t i) {
    Integer f(i), g(1);

    for (uint32_t j = 2; j < i; ++j) {
        f *= j;
    }

    return f * g;
}

Integer Integer::binomial(uint32_t a, uint32_t b) {
    if (b > a / 2) b = a - b;

    if (a < b) return Integer(0);

    if (a == b || b == 0) return Integer(1);

    Integer f(a), g = faculty(b);

    for (uint32_t i = 0; i < b - 1; ++i) {
        --a;
        f *= a;
    }

    return f/g;
}

Integer Integer::multiSet(const uint32_t a, const uint32_t b) {
    return binomial(a + b - 1, b);
}

Integer Integer::gcd(Integer a, Integer b) {
    if (a.isAbsoluteValueLarger(b.absoluteValue, a.absoluteValue)) {
        std::swap(a, b);
    }

    if (b.isZero()) return a;

    a.positive = true;
    b.positive = true;

    while (true) {
        Integer rest = a % b;
        if (rest.isZero()) break;
        a = b;
        b = rest;
    }
    return b;
}

Integer Integer::extendedEuclidianAlgo(Integer a, Integer b, Integer &r, Integer &s) {
    a.positive = true;
    b.positive = true;

    bool needToSwap = b > a;

    if (needToSwap) {
        std::swap(a, b);
    }

    r.set(1);
    s.set(0);

    Integer u(0), v(1);

    if (!b.isZero()) {
        while (true) {
            auto q = a.divideWithRest(b);

            Integer memory = u;
            u = r - q.first * u;
            r = memory;

            memory = v;
            v = s - q.first * v;
            s = memory;

            if (q.second.isZero()) break;

            a = b;
            b = q.second;
        }
    }
    if (needToSwap) std::swap(r, s);

    return b;
}

Integer Integer::lcm(const Integer &a, const Integer &b) {
    if (a.isZero() || b.isZero()) return Integer(0);
    else return a * b / gcd(a, b);
}

void Integer::performAddition(bool positiveSignum, const Integer &rhs, Integer &result) const {
    if (positiveSignum) {
        result.positive = positive;
        if (absoluteValue.size() > rhs.absoluteValue.size()) addAbsValues(absoluteValue, rhs.absoluteValue, result.absoluteValue);
        else addAbsValues(rhs.absoluteValue, absoluteValue, result.absoluteValue);
    }
    else {
        if (isAbsoluteValueLarger(rhs.absoluteValue, absoluteValue)) {
            result.positive = !positive;
            subtractAbsValues(rhs.absoluteValue, absoluteValue, result.absoluteValue);
        }
        else {
            result.positive = positive;
            subtractAbsValues(absoluteValue, rhs.absoluteValue, result.absoluteValue);
        }
    }
}

bool Integer::isAbsoluteValueLarger(const std::vector<uint32_t> &lhs, const std::vector<uint32_t> &rhs) const {
    if (lhs.size() > rhs.size()) return true;
    else if (lhs.size() == rhs.size()) {
        size_t size = lhs.size();
        for (size_t s = 0; s < size; ++s) {
            if (lhs[size - s -1] > rhs[size - s -1]) return true;
            if (lhs[size - s -1] < rhs[size - s -1]) return false;
        }
    }
    return false;
}

void Integer::addAbsValues(const std::vector<uint32_t> &larger, const std::vector<uint32_t> &smaller, std::vector<uint32_t> &result) const {
    bool overflow = false;
    size_t maxSize = larger.size();

    result.resize(maxSize);

    size_t size = smaller.size();
    for (size_t s = 0; s < size; ++s) {
        result[s] = overflow + larger[s] + smaller[s];
        overflow = overflow ? result[s] <= larger[s] || result[s] <= smaller[s] : result[s] < larger[s] || result[s] < smaller[s];
    }

    for (size_t s = size ; s < maxSize ; ++s) {
        result[s] = overflow + larger[s];
        overflow = overflow ? result[s] <= larger[s] : result[s] < larger[s];
    }

    if (overflow) result.push_back((uint32_t)overflow);
}

void Integer::subtractAbsValues(const std::vector<uint32_t> &larger, const std::vector<uint32_t> &smaller, std::vector<uint32_t> &result) const {
    bool overflow = 0;
    size_t maxSize = larger.size();

    result.resize(maxSize, 0);

    size_t size = smaller.size();
    for (size_t s = 0; s < size; ++s) {
        result[s] = larger[s] - smaller[s] - overflow;
        overflow = overflow ? larger[s] <= smaller[s] : larger[s] < smaller[s];
    }

    for (size_t s = size ; s < maxSize; ++s) {
        result[s] = larger[s] - overflow;
        overflow = overflow ? larger[s] == 0 : false;
    }

    while (result.back() == 0 && result.size() > 1) result.pop_back();
}

void Integer::addInt(std::vector<uint32_t> &result, const uint32_t i) const {
    size_t size = absoluteValue.size();
    result.resize(size, 0);

    uint64_t overflow = 0;
    uint64_t sum = 0;

    sum = (uint64_t)absoluteValue[0] + i;
    result[0] = sum % Basis;
    overflow = sum / Basis;

    for (size_t s = 1 ; s < size ; ++s) {
        sum = overflow + absoluteValue[s];
        result[s] = sum % Basis;
        overflow = sum / Basis;
    }

    if (overflow > 0) result.push_back((uint32_t)overflow);
}

void Integer::subtractInt(std::vector<uint32_t> &result, const uint32_t i) const {
    size_t size = absoluteValue.size();
    result.resize(size, 0);

    uint64_t overflow = 0;
    uint64_t sum = 0;

    sum = Basis + absoluteValue[0] - i;
    result[0] = sum % Basis;
    overflow = 1 - sum / Basis;

    for (size_t s = 1 ; s < size ; ++s) {
        sum = Basis + absoluteValue[s] - overflow;
        result[s] = sum % Basis;
        overflow = 1 - sum / Basis;
    }

    while (result.back() == 0 && result.size() > 1) result.pop_back();
}

void Integer::divide(const std::vector<uint32_t> &numerator,
                     const std::vector<uint32_t> &divisor,
                     std::vector<uint32_t> &quotient) const
{
    const size_t divisorSize = divisor.size();
    const size_t numeratorSize = numerator.size();

    std::vector<uint32_t> product;
    product.resize(divisorSize);

    const size_t quotientSize = getQuotientSize(numerator, divisor);

    quotient.resize(quotientSize, 0);
    if (isAbsoluteValueLarger(divisor, numerator)) return;

    std::vector<uint32_t> toMuch;
    toMuch.resize(quotientSize, 0);
    bool adding = true, larger = true;

    std::vector<uint32_t> localCopy = numerator;

    bool sameAdicSize = false;

    uint64_t overflow = divisor.back() <= numerator.back() ? 0 : numerator.back() * Basis;
    size_t amend = overflow > 0 ? 1 : 0;

    for (size_t s = amend; s < quotientSize + amend; ++s) {
        size_t currentQuotientIndex = numeratorSize - divisorSize - s;
        do {
            sameAdicSize = false;

            uint32_t quot = (uint32_t)((overflow + localCopy[numeratorSize - s - 1]) / divisor.back());

            if (adding) addFromIndex(quotient, quot, currentQuotientIndex);
            else addFromIndex(toMuch, quot, currentQuotientIndex);

            uint32_t productOverflow = 0;
            multiplyVectorByUnsignedInt(divisor, quot, product, productOverflow);

            larger = isLargerFromIndex(localCopy, product, overflow, productOverflow, currentQuotientIndex);

            if (larger) {
                uint64_t sum = 0;
                overflow = 0;

                size_t size = product.size();
                for (size_t t = 0; t < size; ++t) {
                    sum = Basis + localCopy[currentQuotientIndex + t] - product[t] - overflow;
                    localCopy[currentQuotientIndex + t] = sum % Basis;
                    overflow = 1 - sum / Basis;
                }
            }
            else {
                uint64_t sum = 0;
                overflow = 0;

                for (size_t t = 0; t < currentQuotientIndex; ++t) {
                    sum = Basis - localCopy[t] - overflow;
                    overflow = (localCopy[t] > 0 || overflow > 0) ? 1 : 0;
                    localCopy[t] = sum % Basis;

                }

                size_t size = product.size();
                for (size_t t = 0; t < size; ++t) {
                    sum = Basis + product[t] - localCopy[currentQuotientIndex + t] - overflow;
                    localCopy[currentQuotientIndex + t] = sum % Basis;
                    overflow = 1 - sum / Basis;
                }
                sameAdicSize = localCopy[numeratorSize - s - 1] != 0;
                overflow = 0;

                adding = !adding;
            }
        } while (sameAdicSize);

        overflow = localCopy[numeratorSize - s - 1] * Basis;
    }

    subtractAbsValues(quotient, toMuch, product);
    quotient = product;

    product.clear();
    multiplyAbsoluteValues(quotient, divisor, product);
    if (isAbsoluteValueLarger(product, absoluteValue)) {
        overflow = 1;

        size_t qSize = quotient.size(), sum;
        for (size_t s = 0 ; s < qSize && overflow; ++s) {
            sum = Basis + quotient[s] - overflow;
            quotient[s] = sum % Basis;
            overflow = 1 - sum / Basis;
        }

        while (quotient.back() == 0 && quotient.size() > 1) quotient.pop_back();
    }
}

size_t Integer::getQuotientSize(const std::vector<uint32_t> &numerator, const std::vector<uint32_t> &divisor) const {
    if (divisor.size() >= numerator.size()) return 1;
    else if (divisor.back() <= numerator.back()) return numerator.size() - divisor.size() + 1;
    else if (numerator.size() - divisor.size() > 1) return numerator.size()-divisor.size();
    else return 1;
}

bool Integer::isLargerFromIndex(const std::vector<uint32_t> &lhs,
                                const std::vector<uint32_t> &rhs,
                                const uint64_t lhsoverflow,
                                const uint32_t rhsoverflow,
                                size_t lhsIndex) const
{
    size_t size = rhs.size();
    if (rhsoverflow * Basis == lhsoverflow) {
        for (size_t t = 0; t < size; ++t) {
            if (lhs[lhsIndex + size - t - 1] > rhs[size - t - 1]) return true;
            if (lhs[lhsIndex + size - t - 1] < rhs[size - t - 1]) return false;
        }
    }
    else if (rhsoverflow * Basis > lhsoverflow) return false;
    return true;
}

void Integer::addFromIndex(std::vector<uint32_t> &value, uint32_t i, const size_t index) const {
    uint32_t oldEntry;

    size_t size = value.size();
    for (size_t s = index; s < size && i > 0; ++s) {
        oldEntry = value[s];
        value[s] = oldEntry + i;
        i = value[s] > oldEntry ? 0 : 1;
    }

    if (i > 0) value.push_back(i);
}

void Integer::multiplyAbsoluteValues(const std::vector<uint32_t> &factor1, const std::vector<uint32_t> &factor2, std::vector<uint32_t> &product) const
{
    size_t size1 = factor1.size(), size2 = factor2.size();

    uint64_t overflow = 0;
    uint64_t sum = 0;
    size_t u = 0, resultSize = size1 + size2 - 1;
    bool resized = false;

    product.resize(resultSize, 0);

    for (size_t s = 0; s < size1; ++s) {
        for (size_t t = 0; t < size2; ++t) {
            sum = (uint64_t)factor1[s] * factor2[t] + product[s+t];
            product[s+t] = sum % Basis;
            overflow = sum / Basis;
            u = s+t;
            while (overflow > 0) {
                u++;
                if (u >= resultSize) {
                    if (resized) product[u] += (uint32_t)overflow;
                    else {
                        product.push_back((uint32_t)overflow);
                        resized = true;
                    }
                    overflow = 0;
                }
                else {
                    sum = overflow + product[u];
                    product[u] = sum % Basis;
                    overflow = sum / Basis;
                }
            }
        }
    }
}

void Integer::multiplyVectorByUnsignedInt(const std::vector<uint32_t> &vec, const uint32_t i, std::vector<uint32_t> &result, uint32_t &overflow) const {
    uint64_t product = 0;

    size_t size = vec.size();
    for (size_t s = 0; s < size; ++s) {
        product = (uint64_t)vec[s] * i + overflow;
        result[s] = product % Basis;
        overflow = (uint32_t)(product / Basis);
    }
}

void Integer::performDivision(const uint32_t i, std::vector<uint32_t> &result, uint32_t &overflow) const {
    overflow = 0;
    uint64_t sum = 0;

    size_t size = absoluteValue.size();
    result.resize(absoluteValue[size-1] >= i || size == 1 ? size : size - 1, 0);

     for (size_t s = 0; s < size; ++s) {
         sum = overflow * Basis + absoluteValue[size - s - 1];
         overflow = sum % i;
         if (sum >= i) {
             result[size - s -1] = sum / i;
         }
     }
}

}
