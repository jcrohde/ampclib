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

#include <rationaltest.h>

#include <rational.h>

using namespace ampc;

void RationalTest::generationTest() {
    std::string str;

    Rational("1/2").print(str, 2);
    QVERIFY(str == "0.5");

    Rational("1/3").print(str, 9);
    QVERIFY(str == "0.333333333");

    Rational(Integer("-100000000000000000000000000000000"), Integer("1000000000000000000000000000000000000")).print(str, 4);
    QVERIFY(str == "-0.0001");
}

void RationalTest::comparisonTest() {
    QVERIFY(Rational(8, 3) == Rational(40, 15));

    QVERIFY(Rational(8, 3) >= Rational(40, 15));
    QVERIFY(Rational(8, 3) <= Rational(40, 15));

    QVERIFY(Rational(8, 4) < Rational(40, 15));
    QVERIFY(Rational(8, 4) <= Rational(40, 15));

    QVERIFY(Rational(14, 5) > Rational(40, 15));
    QVERIFY(Rational(14, 5) >= Rational(40, 15));

    QVERIFY(Rational(0, 1).isInteger());
    QVERIFY(Rational(Integer("10000000000000000000000000000000000000"), Integer("-10000000000000000000000000")).isInteger());
    QVERIFY(!Rational(Integer("10000000000000000000000000000000000000"), Integer("10000000000000000000000001")).isInteger());
}

void RationalTest::arithmeticsTest() {
    QVERIFY(-Rational("66666777777888999006/-567895588") == Rational("-66666777777888999006/-567895588"));

    QVERIFY(Rational(1) + Rational("999999999") == Rational("1000000000"));

    QVERIFY(Rational(1) + Rational("999999999999999999999999999999999999999999999999999999")
            ==    Rational("1000000000000000000000000000000000000000000000000000000"));

    QVERIFY(Rational(1) - Rational("-999999999999999999999999999999999999999999999999999999999")
            == Rational("1000000000000000000000000000000000000000000000000000000000"));

    QVERIFY(Rational(-1, 2) * Rational("88888888888888888888888888888888888888888888")
            == Rational("-44444444444444444444444444444444444444444444"));

    QVERIFY(Rational("200000000000000000000000000000000000000000000") /
            Rational("200000000000000000000000000000000000000000/3")
            == Rational(3000));

    Rational q(Basis + 1, -2);
    q += Rational(-1, -2);
    QVERIFY(q == Rational(-(int64_t)Basis, 2));

    Rational r(Basis + 1, -2);
    r -= Rational(1, -2);
    QVERIFY(r == Rational(-(int64_t)Basis, 2));

    Rational s(-(int64_t)Basis);
    s *= Rational(2, Basis);
    QVERIFY(s == Rational(-2));

    Rational t(144, -5);
    t /= Rational(-12, -25);
    QVERIFY(t == Rational(-60));
}

void RationalTest::integerTest() {
    Rational q("555550000000000/-33");
    QVERIFY(q * 33 == Rational("-555550000000000"));
    QVERIFY(- q / 55555 == Rational("10000000000/33"));

    q *= 3;
    QVERIFY(q  == Rational("555550000000000/-11"));
    q /= 700;
    QVERIFY(q == Rational("5555500000000/-77"));

    Rational p("10000000000000000000001/2");
    QVERIFY(p - 1 == Rational("9999999999999999999999/2"));
    p -= 1;
    QVERIFY(p == Rational("9999999999999999999999/2"));
    QVERIFY(p + 2 == Rational("10000000000000000000003/2"));
    p += 2;
    QVERIFY(p == Rational("10000000000000000000003/2"));

    Rational r("123456789/-100");
    r *= Integer(-100);
    QVERIFY(r == Rational(123456789));

    r /= Integer("100000000000000000000000000000");
    QVERIFY(r == Rational("123456789/100000000000000000000000000000"));

    r = Rational("100000000000000000000000000000000000000/2");
    r += Integer("100000000000000000000000000000000000000");
    QVERIFY(r == Rational("300000000000000000000000000000000000000/2"));
    r -= Integer("200000000000000000000000000000000000000");
    QVERIFY(r == Rational("-100000000000000000000000000000000000000/2"));
}

void RationalTest::doubleTest() {
    double d = Rational("10000/3333") * 9999999999999.45;
    double e = 10000.0/3333.0 * 9999999999999.45;
    QVERIFY(abs(d  -  e) < 0.0001);

    d = Rational("7899999999999/10000000000000") / 12345.6789;
    e = 7899999999999.0/10000000000000.0 / 12345.6789;
    QVERIFY(abs(d  -  e) < 0.0001);

    d = Rational("789999999999/10000000099000") + 12345.6789;
    e = 789999999999.0/10000000099000.0 + 12345.6789;
    QVERIFY(abs(d  -  e) < 0.0001);

    d = Rational("7899999999999/10000000000000") - 12345.6789;
    e = 7899999999999.0/10000000000000.0 - 12345.6789;
    QVERIFY(abs(d  -  e) < 0.0001);
}

void RationalTest::incrementTest() {
    Rational q("1/-10000000000000000000000000000000");
    QVERIFY(++q == Rational("9999999999999999999999999999999/10000000000000000000000000000000"));
    QVERIFY(q++ == Rational("9999999999999999999999999999999/10000000000000000000000000000000"));
    QVERIFY(q == Rational("19999999999999999999999999999999/10000000000000000000000000000000"));
}

void RationalTest::decrementTest() {
    Rational q("1/10000000000000000000000000000000");
    QVERIFY(q-- == Rational("1/10000000000000000000000000000000"));
    QVERIFY(q-- == Rational("-9999999999999999999999999999999/10000000000000000000000000000000"));
    QVERIFY(q == Rational("19999999999999999999999999999999/-10000000000000000000000000000000"));
    QVERIFY(--q == Rational("29999999999999999999999999999999/-10000000000000000000000000000000"));
}
