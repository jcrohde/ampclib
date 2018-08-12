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

#include <integer.h>

namespace ampc {

class Rational {
public:
    Rational() : numerator(0), denominator(1) {}

    Rational(uint32_t i) : numerator(i), denominator(1) {}
    Rational(int32_t i) : numerator(i), denominator(1) {}
    Rational(uint64_t i) : numerator(i), denominator(1) {}
    Rational(int64_t i) : numerator(i), denominator(1) {}

    Rational(int64_t p, int64_t q) : numerator(p), denominator(q) {canonicalize();}
    Rational(const Integer &p, const Integer &q)  : numerator(p), denominator(q) {canonicalize();}

    Rational(std::string str);

    double toDouble() const {return numerator.toDouble() / denominator.toDouble();}
    Integer toInteger() const {return numerator / denominator;}

    bool isZero() const {return numerator.isZero();}
    bool isInteger();

    void print(std::string &str, const int32_t precision = 6) const;

    bool operator >(const Rational &rhs) const {return numerator * rhs.denominator > rhs.numerator * denominator;}
    bool operator >=(const Rational &rhs) const {return numerator * rhs.denominator >= rhs.numerator * denominator;}
    bool operator <(const Rational &rhs) const {return numerator * rhs.denominator < rhs.numerator * denominator;}
    bool operator <=(const Rational &rhs) const {return numerator * rhs.denominator <= rhs.numerator * denominator;}
    bool operator ==(const Rational &rhs) const {return numerator * rhs.denominator == rhs.numerator * denominator;}
    bool operator !=(const Rational &rhs) const {return numerator * rhs.denominator != rhs.numerator * denominator;}

    Rational operator -() const {return Rational(-numerator, denominator);}
    Rational operator +(const Rational &rhs) const;
    Rational operator -(const Rational &rhs) const;
    Rational operator *(const Rational &rhs) const;
    Rational operator /(const Rational &divisor) const;

    Rational operator +(const Integer &rhs) const;
    Rational operator -(const Integer &rhs) const;
    Rational operator *(const Integer &rhs) const;
    Rational operator /(const Integer &divisor) const;

    double operator +(const double d) const {return toDouble() + d;}
    double operator -(const double d) const {return toDouble() - d;}
    double operator *(const double d) const {return toDouble() * d;}
    double operator /(const double d) const {return toDouble() / d;}

    Rational operator +(const int32_t i) const {return *this + Integer(i);}
    Rational operator -(const int32_t i) const {return *this - Integer(i);}
    Rational operator *(const int32_t i) const {return *this * Integer(i);}
    Rational operator /(const int32_t i) const {return *this / Integer(i);}

    Rational &operator +=(const Rational &rhs);
    Rational &operator -=(const Rational &rhs);
    Rational &operator *=(const Rational &rhs);
    Rational &operator /=(const Rational &rhs);

    Rational &operator +=(const Integer &rhs);
    Rational &operator -=(const Integer &rhs);
    Rational &operator *=(const Integer &rhs);
    Rational &operator /=(const Integer &rhs);

    Rational operator +=(const int32_t i);
    Rational operator -=(const int32_t i);
    Rational operator *=(const int32_t i);
    Rational operator /=(const int32_t i);

    Rational &operator --();
    Rational &operator ++();
    Rational operator --(int);
    Rational operator ++(int);

private:
    Integer numerator;
    Integer denominator;

    void canonicalize();
};

}
