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

#include <integertest.h>

#include <integer.h>

#include <QtTest>

using namespace ampc;

void IntegerTest::generationTest() {
    Integer a(123);
    QVERIFY(a.toInt() == 123);

    Integer b(Basis + 1);
    QVERIFY(b.toLongInt() == Basis + 1);

    Integer c(-(int64_t)Basis - 1);
    QVERIFY(c.toLongInt() == -(long long int)Basis - 1);

    Integer d("10000000000000000000000000000000000000000000000000000000000000000000000000");
    std::string str;
    d.print(str);
    QVERIFY(str == "10000000000000000000000000000000000000000000000000000000000000000000000000");

    Integer e("-1");
    QVERIFY(e.toInt() == -1);
    e.print(str);
    QVERIFY(str == "-1");
    QVERIFY(e.toUnsignedLongInt() == 0);

    QVERIFY(500 * Basis + 3 == Integer(500 * Basis + 3).toUnsignedLongInt());
}

void IntegerTest::comparisionTest() {
    Integer a("123");
    Integer b("10000000000000000000000000000000000000");
    Integer c("-1");
    Integer d(0);
    Integer e(-1000);

    QVERIFY(a < b);
    QVERIFY(d >= c);
    QVERIFY(a > e);
    QVERIFY(c < d);
    QVERIFY(a > e);
    QVERIFY(d != e);
    QVERIFY(b == b);
    QVERIFY(!(e < e));

    QVERIFY(Integer("0") == Integer(0));
    QVERIFY(Integer("-0") == Integer(0));
    QVERIFY(Integer(-0) == Integer(0));

    QVERIFY(Integer(Basis + 1) < Integer(2*Basis));
}

void IntegerTest::additionTest() {
    QVERIFY(Integer("123456789123456789") == Integer("103050709020406080") + Integer("20406080103050709"));
    QVERIFY(Integer("-1") == Integer("100000000000000000000000000000000000") + Integer("-100000000000000000000000000000000001"));
    QVERIFY(Integer("-2000000000000000000000000000000000000000000000001")
            == Integer("-1000000000000000000000000000000000000000000000001") + Integer("-1000000000000000000000000000000000000000000000000"));
    QVERIFY(Integer("10") == Integer("-100000000000000000000000000000000") + Integer("100000000000000000000000000000010"));
}

void IntegerTest::subtractionTest() {
    QVERIFY(Integer("6666555544443333222211110000") == Integer("7777777777777777777777777777") - Integer("1111222233334444555566667777"));
    QVERIFY(Integer("-1") == Integer("100000000000000000000000000000000000") - Integer("100000000000000000000000000000000001"));
    QVERIFY(Integer("200000000000000000000000000000000001")
            == Integer("100000000000000000000000000000000000") - Integer("-100000000000000000000000000000000001"));
    QVERIFY(Integer("-2000000000000000000000000000000000000000000000001")
            == Integer("-1000000000000000000000000000000000000000000000001") - Integer("1000000000000000000000000000000000000000000000000"));
    QVERIFY(Integer("10") == Integer("-100000000000000000000000000000000") - Integer("-100000000000000000000000000000010"));
}

void IntegerTest::multiplicationTest() {
    QVERIFY(Integer("10") == Integer("2") * Integer("5"));

    QVERIFY(Integer("0") == Integer("2") * Integer("-0"));
    QVERIFY(Integer("-0") == Integer("0") * Integer("5"));

    QVERIFY(Integer("9999999999999999999999999999999999999999999999000000000000000000000000000000000000000000000000000000000")
            == Integer("9999999999999999999999999999999999999999999999")
            * Integer("1000000000000000000000000000000000000000000000000000000000"));
    QVERIFY(Integer("-999999999999999999999999999999999999999999990000000000000000000000000")
            == Integer("99999999999999999999999999999999999999999999") * Integer("-10000000000000000000000000"));
    QVERIFY(Integer("-9999999999999999999999999999999990000000000000000000000000")
            == Integer("-999999999999999999999999999999999") * Integer("10000000000000000000000000"));
    QVERIFY(Integer("999999999999999999999999999999999000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000")
            == Integer("-999999999999999999999999999999999")
            * Integer("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"));
}

void IntegerTest::divisionTest() {
    QVERIFY(Integer("2") == Integer("11") / Integer("5"));
    QVERIFY(Integer("-2") == Integer("11") / Integer("-5"));
    QVERIFY(Integer("0") == Integer("1") / Integer("5"));
    QVERIFY(Integer("0") == Integer("1") / Integer("-5"));

    QVERIFY(Integer("990000000000000000000000000000000000000000000") / Integer("10000000000000000000000000000000000000000000") == Integer("99"));
    QVERIFY(Integer("990000000000000000000000000000000000000000000") / Integer("99") == Integer("10000000000000000000000000000000000000000000"));

    QVERIFY(Integer(Basis * 2) / Integer(Basis + 1) == Integer(1));
    QVERIFY(Integer(Basis * 6) / Integer(Basis * 2 + 1) == Integer(2));
    QVERIFY(Integer(Basis * 9) / Integer(18) == Integer(Basis * 9 / 18));
}

void IntegerTest::moduloTest() {
    QVERIFY(Integer("1") == Integer("11") % Integer("5"));
    QVERIFY(Integer("1") == Integer("11") % Integer("-5"));
    QVERIFY(Integer("4") == Integer("-11") % Integer("5"));
    QVERIFY(Integer("4") == Integer("-11") % Integer("-5"));

    QVERIFY(Integer("100000000000000000000000000009000000000000000125") % Integer("100000000000000000000000000000") == Integer("9000000000000000125"));
    QVERIFY(Integer("100000000000000000000000000000000000000000000125") % Integer("100000000000000000000000000000") == Integer("125"));
    QVERIFY((Integer(2*Basis + 1) * Integer(Basis)) / Integer(Basis + 1) == Integer(2*Basis - 1));
    QVERIFY(Integer("1000000000000000000000000000000000000000125") % Integer("1000000000000000000000000") == Integer("125"));
    QVERIFY(Integer("1000000000000000000000000000000000000000125") / Integer("1000000000000000000000000") - Integer("1000000000000000000") == Integer(0));
    QVERIFY(Integer("1000000000000000000000000000000000125") % Integer("1000000000000000000") == Integer("125"));
    QVERIFY(Integer("1000000000000000000000000000125") % Integer("1000000000000") == Integer("125"));
    QVERIFY(Integer("100000000000000000000000000000000") / Integer("1000000000000") - Integer("100000000000000000000") == Integer(0));

    Integer i("10000000000000000000000000000000000000000003");
    auto result = i.divideWithRest(1000000);
    QVERIFY(result.first == Integer("10000000000000000000000000000000000000"));
    QVERIFY(result.second == 3);

    auto result2 = i.divideWithRest(Integer("1000000000"));
    QVERIFY(result2.first == Integer("10000000000000000000000000000000000"));
    QVERIFY(result2.second == Integer(3));

    Integer w("-3000000000000000000000000000004");
    auto result3 = w.divideWithRest(1000000);
    QVERIFY(result3.first == Integer("-3000000000000000000000000"));
    QVERIFY(result3.second == 999996);

    auto result4 = w.divideWithRest(Integer("10000000000000000000000000000"));
    QVERIFY(result4.first.toInt() == -300);
    QVERIFY(result4.second == Integer("9999999999999999999999999996"));

    QVERIFY(Integer(7).invertModulo(Integer(9)) == Integer(4));
}

void IntegerTest::incrementTest() {
    Integer i(Basis - 1);
    QVERIFY(i++ == Integer(Basis - 1));
    QVERIFY(i.toLongInt() == Basis);

    Integer a(-1);
    QVERIFY(++a == Integer(0));
    QVERIFY(++a == Integer(1));

    a = Integer(-(int64_t)Basis);
    QVERIFY(++a == Integer(-(int64_t)Basis + 1));
}

void IntegerTest::decrementTest() {
    Integer i(1);
    QVERIFY(i-- == Integer(1));
    QVERIFY(i-- == Integer(0));
    QVERIFY(i-- == Integer(-1));

    Integer a(-(int64_t)Basis + 1);
    QVERIFY(--a == Integer(-(int64_t)Basis));
    QVERIFY(--a == Integer(-(int64_t)Basis - 1));
    QVERIFY(--a == Integer(-(int64_t)Basis - 2));
}

void IntegerTest::unsignedIntTest() {
    QVERIFY(Integer(Basis + 1) == Integer(Basis) + 1);

    QVERIFY(Integer("1000000000000000000000000000000000000000000000") == Integer("999999999999999999999999999999999999999999999") + 1);
    QVERIFY(Integer("1000000000000000000000000000000000000000000000") - 1 == Integer("999999999999999999999999999999999999999999999"));
    QVERIFY(Integer("-500") == Integer("500") - 1000);
    QVERIFY(Integer("100000000000000000000000000000000000000000000000000000000") * 12345
            == Integer("1234500000000000000000000000000000000000000000000000000000000"));
    QVERIFY(Integer("999999999999999999999999999999999999000") / 1000 == Integer("999999999999999999999999999999999999"));
    QVERIFY(Integer("10000000000000000000125") % Integer("1000") == Integer("125"));
    QVERIFY(Integer(5) - 7 == Integer(-2));
}

void IntegerTest::functionTest() {
    Integer i("100000000000000000000000000000000");
    Integer inverse = i;
    QVERIFY((inverse.invertModulo(89989) * i) % 89989 == 1);

    QVERIFY(Integer::fibonacci(0).toInt() == 0);
    QVERIFY(Integer::fibonacci(1).toInt() == 1);
    QVERIFY(Integer::fibonacci(10).toInt() == 55);

    QVERIFY(Integer::lucas(0).toInt() == 2);
    QVERIFY(Integer::lucas(1).toInt() == 1);
    QVERIFY(Integer::lucas(2).toInt() == 3);
    QVERIFY(Integer::lucas(10).toInt() == 123);

    QVERIFY(Integer::faculty(0).toInt() == 0);
    QVERIFY(Integer::faculty(1).toInt() == 1);
    QVERIFY(Integer::faculty(4).toInt() == 24);

    QVERIFY(Integer::binomial(3,0).toInt() == 1);
    QVERIFY(Integer::binomial(3,3).toInt() == 1);
    QVERIFY(Integer::binomial(3,4).toInt() == 0);
    QVERIFY(Integer::binomial(7,3).toInt() == 35);
    QVERIFY(Integer::binomial(8,6).toInt() == 28);

    QVERIFY(Integer::multiSet(0, 0) == 1);
    QVERIFY(Integer::multiSet(3, 0) == 1);
    QVERIFY(Integer::multiSet(0, 5) == 0);
    QVERIFY(Integer::multiSet(1, 4) == 1);
    QVERIFY(Integer::multiSet(2, 4) == 5);
    QVERIFY(Integer::multiSet(5, 10) == 1001);
    QVERIFY(Integer::multiSet(4, 18) == 1330);

    QVERIFY(Integer::gcd(Integer(55), Integer(2222200)) == Integer(5));
    QVERIFY(Integer::gcd(Integer(20000001), Integer(-33333)) == Integer(3));
    QVERIFY(Integer::gcd(Integer(-2000000), Integer(5000000)) == Integer(1000000));
    QVERIFY(Integer::gcd(Integer(0), Integer(2222200)) == Integer(2222200));

    Integer r, s;

    QVERIFY(Integer::extendedEuclidianAlgo(Integer(2), Integer(1), r, s) == Integer(1));
    QVERIFY(r == Integer(0));
    QVERIFY(s == Integer(1));

    QVERIFY(Integer::extendedEuclidianAlgo(Integer(3), Integer(5), r, s) == Integer(1));
    QVERIFY(r == Integer(2));
    QVERIFY(s == Integer(-1));

    QVERIFY(Integer::lcm(Integer(2222200), Integer(0)) == Integer(0));
    QVERIFY(Integer::lcm(Integer(100), Integer(12)) == Integer(300));
}

void IntegerTest::powerTest() {
    Integer a(3), p(-2);

    QVERIFY(a.power(p) == Integer(3));

    QVERIFY(a.power(Integer(0)) == Integer(1));

    a = Integer(3);
    QVERIFY(a.power(Integer(1)) == Integer(3));

    a = Integer(3);
    QVERIFY(a.power(Integer(2)) == Integer(9));

    a = Integer(3);
    QVERIFY(a.power(Integer(3)) == Integer(27));

    a = Integer(-2);
    QVERIFY(a.power(Integer(3)) == Integer(-8));

    a = Integer(10);
    QVERIFY(a.power(Integer(25)) == Integer("10000000000000000000000000"));

    QVERIFY(Integer("100000000000000000000").power(3) == Integer(10).power(60));
}

void IntegerTest::powerModuloTest() {
    QVERIFY(Integer(2).powerModulo(Integer(3), Integer(3)) == Integer(2));
    QVERIFY(Integer(4).powerModulo(Integer(2), Integer(7)) == Integer(2));
    QVERIFY(Integer(4).powerModulo(Integer(3), Integer(25)) == Integer(14));

    QVERIFY(Integer(-2).powerModulo(Integer(3), Integer(7)) == Integer(6));
    QVERIFY(Integer(-2).powerModulo(Integer(-3), Integer(7)) == Integer(-2));
    QVERIFY(Integer(-2).powerModulo(Integer(3), Integer(-7)) == Integer(6));

    QVERIFY(Integer(2).powerModulo(Integer(4), Integer(5)) == Integer(1));

    QVERIFY(Integer("1000000000000000000000000000000000003").powerModulo(Integer(2), Integer("10000000000000000000000000")) == Integer(9));

    QVERIFY(Integer(10).powerModulo(Integer("23"), Integer("6643838879")) == Integer(5559987813));
    QVERIFY(Integer(10).powerModulo(Integer(97), Integer(6643838879)) == Integer(5668857816));
    QVERIFY(Integer(10).powerModulo(Integer("6643838878"), Integer("6643838879")) == Integer(1));
}

void IntegerTest::millerRabinTest() {
    QVERIFY(Integer("1111111111111111111").millerRabinPrimalityTest());
    QVERIFY(Integer("11111111111111111111111").millerRabinPrimalityTest());
    QVERIFY(Integer("87178291199").millerRabinPrimalityTest());
    QVERIFY(Integer("99194853094755497").millerRabinPrimalityTest());
    QVERIFY(Integer("200560490131").millerRabinPrimalityTest());

    QVERIFY(!Integer("9999999999999999").millerRabinPrimalityTest());
    QVERIFY(!Integer("10000000000000000").millerRabinPrimalityTest());
    QVERIFY(!Integer("555555555555555555").millerRabinPrimalityTest());
}

void IntegerTest::performanceAdditionTest() {
    Integer a("1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"), b("1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"), c;
    QBENCHMARK {
        c = a + b;
    }
}

void IntegerTest::performanceSubtractionTest() {
    Integer a("1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"), b("1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"), c;
    QBENCHMARK {
        c = a - b;
    }
}

void IntegerTest::performanceMultiplicationTest() {
    Integer a("1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"), b("1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001"), c;
    QBENCHMARK {
        c = a * b;
    }
}

void IntegerTest::performanceFibonacciTest() {
    QBENCHMARK {
        Integer::fibonacci(100000);
    }
}

void IntegerTest::performanceFacultyTest() {
    QBENCHMARK {
        Integer::faculty(10000);
    }
}
