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

#include <rational.h>

#include <iomanip>
#include <sstream>

namespace ampc {

Rational::Rational(std::string str) {
    size_t index = str.find('/');
    if (index == std::string::npos) {
        numerator.set(str);
        denominator.set(1);
    }
    else {
        numerator.set(str.substr(0, index));
        denominator.set(str.substr(index + 1));
    }
}

bool Rational::isInteger() {
    canonicalize();
    return denominator == Integer(1);
}

void Rational::print(std::string &str, const int32_t precision) const {
    std::stringstream stream;
    stream << std::setprecision(precision) << toDouble();
    str = stream.str();
}

Rational Rational::operator +(const Rational &rhs) const {
    Rational result(numerator * rhs.denominator + rhs.numerator * denominator, denominator * rhs.denominator);
    result.canonicalize();
    return result;
}

Rational Rational::operator -(const Rational &rhs) const {
    Rational result(numerator * rhs.denominator - rhs.numerator * denominator, denominator * rhs.denominator);
    result.canonicalize();
    return result;
}

Rational Rational::operator *(const Rational &rhs) const {
    Rational result(numerator * rhs.numerator, rhs.denominator * denominator);
    result.canonicalize();
    return result;
}

Rational Rational::operator /(const Rational &rhs) const {
    Rational result(numerator * rhs.denominator, rhs.numerator * denominator);
    result.canonicalize();
    return result;
}

Rational Rational::operator +(const Integer &rhs) const {
    Rational result(numerator  + rhs * denominator, denominator);
    result.canonicalize();
    return result;
}

Rational Rational::operator -(const Integer &rhs) const {
    Rational result(numerator - rhs * denominator, denominator);
    result.canonicalize();
    return result;
}

Rational Rational::operator *(const Integer &rhs) const {
    Rational result(numerator * rhs, denominator);
    result.canonicalize();
    return result;
}

Rational Rational::operator /(const Integer &rhs) const {
    Rational result(numerator, rhs * denominator);
    result.canonicalize();
    return result;
}

Rational &Rational::operator +=(const Rational &rhs) {
    *this = *this + rhs;
    return *this;
}

Rational &Rational::operator -=(const Rational &rhs) {
    *this = *this - rhs;
    return *this;
}

Rational &Rational::operator *=(const Rational &rhs) {
    *this = *this * rhs;
    return *this;
}

Rational &Rational::operator /=(const Rational &rhs) {
    *this = *this / rhs;
    return *this;
}

Rational &Rational::operator +=(const Integer &rhs) {
    *this = *this + rhs;
    return *this;
}

Rational &Rational::operator -=(const Integer &rhs) {
    *this = *this - rhs;
    return *this;
}

Rational &Rational::operator *=(const Integer &rhs) {
    *this = *this * rhs;
    return *this;
}

Rational &Rational::operator /=(const Integer &rhs) {
    *this = *this / rhs;
    return *this;
}

Rational Rational::operator +=(const int32_t i) {
    *this = *this + Integer(i);
    return *this;
}

Rational Rational::operator -=(const int32_t i) {
    *this = *this - Integer(i);
    return *this;
}

Rational Rational::operator *=(const int32_t i) {
    *this = *this * Integer(i);
    return *this;
}

Rational Rational::operator /=(const int32_t i) {
    *this = *this / Integer(i);
    return *this;
}

Rational &Rational::operator --() {
    *this = *this - 1;
    return *this;
}

Rational &Rational::operator ++() {
    *this = *this + 1;
    return *this;
}

Rational Rational::operator --(int) {
    Rational q(*this);
    *this = *this - 1;
    return q;
}

Rational Rational::operator ++(int) {
    Rational q(*this);
    *this = *this + 1;
    return q;
}

void Rational::canonicalize() {
    Integer g = Integer::gcd(numerator, denominator);
    numerator /= g;
    denominator /= g;

    if (numerator.isPositive() == denominator.isPositive())
        numerator.abs();
    else
        numerator.minusAbs();

    denominator.abs();
}

}
