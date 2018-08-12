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

#include <primetest.h>

#include <primes.h>

#include <set>

using namespace ampc;

void PrimeTest::isPrimeTest() {
    QVERIFY(!Primes::getInstance().isPrime(0));
    QVERIFY(!Primes::getInstance().isPrime(1));
    QVERIFY(Primes::getInstance().isPrime(2));
    QVERIFY(!Primes::getInstance().isPrime(1000));
    QVERIFY(Primes::getInstance().isPrime(1073676287));
}

void PrimeTest::primeFactorTest() {
    QVERIFY(Primes::getInstance().getPrimeFactor(0) == 0);
    QVERIFY(Primes::getInstance().getPrimeFactor(1) == 1);
    QVERIFY(Primes::getInstance().getPrimeFactor(2) == 2);
    QVERIFY(Primes::getInstance().getPrimeFactor(4) == 2);
    QVERIFY(Primes::getInstance().getPrimeFactor(55) == 5);
    QVERIFY(Primes::getInstance().getPrimeFactor(305175781) == 305175781);
}

void PrimeTest::getPrimeFactorsTest() {
    std::vector<unsigned int> factors;

    Primes::getPrimeFactors(0, factors);
    QVERIFY(factors.size() == 0);

    Primes::getPrimeFactors(1, factors);
    QVERIFY(factors.size() == 0);

    Primes::getPrimeFactors(2, factors);
    QVERIFY(factors.size() == 1);

    Primes::getPrimeFactors(12, factors);
    QVERIFY(factors.size() == 2);
}

void PrimeTest::getPrimeFactorsMultiplicityTest() {
    std::vector<unsigned int> factors;

    Primes::getPrimeFactorsWithMultiplicity(0, factors);
    QVERIFY(factors.size() == 0);

    Primes::getPrimeFactorsWithMultiplicity(1, factors);
    QVERIFY(factors.size() == 0);

    Primes::getPrimeFactorsWithMultiplicity(2, factors);
    QVERIFY(factors.size() == 1);

    Primes::getPrimeFactorsWithMultiplicity(12, factors);
    QVERIFY(factors.size() == 3);

    Primes::getPrimeFactorsWithMultiplicity(100, factors);
    QVERIFY(factors.size() == 4);

    Primes::getPrimeFactorsWithMultiplicity(1000000, factors);
    QVERIFY(factors.size() == 12);
}

void PrimeTest::getFactorNumberTest() {
    QVERIFY(Primes::factorNumber(0) == 2);
    QVERIFY(Primes::factorNumber(1) == 1);
    QVERIFY(Primes::factorNumber(2) == 2);
    QVERIFY(Primes::factorNumber(12) == 6);
    QVERIFY(Primes::factorNumber(100) == 9);
}

void PrimeTest::getFactorsTest() {
    std::vector<unsigned int> factors;

    Primes::getFactors(12, factors);
    QVERIFY(factors.size() == 6);
    std::set<unsigned int> s(factors.begin(), factors.end());
    std::set<unsigned int> t{1, 2, 3, 4, 6, 12};
    QVERIFY(s == t);
}

void PrimeTest::factorSumTest() {
    QVERIFY(Primes::factorSum(0) == 1);
    QVERIFY(Primes::factorSum(1) == 1);
    QVERIFY(Primes::factorSum(3) == 4);
    QVERIFY(Primes::factorSum(100) == 1 + 2 + 4 + 5 + 10 + 20 + 25 + 50 + 100);
    QVERIFY(Primes::factorSum(305175781) == 305175782);
}

void PrimeTest::phiTest() {
    QVERIFY(Primes::eulerPhi(0) == 0);
    QVERIFY(Primes::eulerPhi(1) == 1);
    QVERIFY(Primes::eulerPhi(2) == 1);
    QVERIFY(Primes::eulerPhi(3) == 2);
    QVERIFY(Primes::eulerPhi(4) == 2);
    QVERIFY(Primes::eulerPhi(5) == 4);
    QVERIFY(Primes::eulerPhi(100) == 1 * 2 * 4 * 5);
}

void PrimeTest::psiTest() {
    QVERIFY(Primes::psi(0) == 0);
    QVERIFY(Primes::psi(1) == 1);
    QVERIFY(Primes::psi(2) == 3);
    QVERIFY(Primes::psi(3) == 4);
    QVERIFY(Primes::psi(4) == 6);
    QVERIFY(Primes::psi(5) == 6);
    QVERIFY(Primes::psi(100) == 3 * 2 * 6 * 5);
}

void PrimeTest::piTest() {
    QVERIFY(Primes::getInstance().pi(0) == 0);
    QVERIFY(Primes::getInstance().pi(1) == 0);
    QVERIFY(Primes::getInstance().pi(2) == 1);
    QVERIFY(Primes::getInstance().pi(3) == 2);
    QVERIFY(Primes::getInstance().pi(4) == 2);
    QVERIFY(Primes::getInstance().pi(18) == 7);
}

void PrimeTest::nextTest() {
    QVERIFY(Primes::getInstance().nextPrime(0) == 2);
    QVERIFY(Primes::getInstance().nextPrime(1) == 2);
    QVERIFY(Primes::getInstance().nextPrime(2) == 3);
    QVERIFY(Primes::getInstance().nextPrime(12) == 13);
    QVERIFY(Primes::getInstance().nextPrime(23) == 29);
}

void PrimeTest::lambdaTest() {
    QVERIFY(Primes::lambda(0) == 0);
    QVERIFY(Primes::lambda(1) == 1);
    QVERIFY(Primes::lambda(2) == -1);
    QVERIFY(Primes::lambda(3) == -1);
    QVERIFY(Primes::lambda(4) == 1);
    QVERIFY(Primes::lambda(12) == -1);
    QVERIFY(Primes::lambda(10000000) == 1);
}

void PrimeTest::legendreTest() {
    QVERIFY(Primes::legendreSymbol(0, 0) == 0);
    QVERIFY(Primes::legendreSymbol(1, 4) == 0);
    QVERIFY(Primes::legendreSymbol(2, 1) == 0);
    QVERIFY(Primes::legendreSymbol(2, 0) == 0);

    QVERIFY(Primes::legendreSymbol(0, 7) == 0);
    QVERIFY(Primes::legendreSymbol(1, 7) == 1);
    QVERIFY(Primes::legendreSymbol(2, 7) == 1);
    QVERIFY(Primes::legendreSymbol(3, 7) == -1);
    QVERIFY(Primes::legendreSymbol(4, 7) == 1);
    QVERIFY(Primes::legendreSymbol(5, 7) == -1);
    QVERIFY(Primes::legendreSymbol(6, 7) == -1);
}

void PrimeTest::jacobiTest() {
    QVERIFY(Primes::jacobiSymbol(0, 0) == 1);
    QVERIFY(Primes::jacobiSymbol(1, 4) == 1);
    QVERIFY(Primes::jacobiSymbol(2, 1) == 1);
    QVERIFY(Primes::jacobiSymbol(2, 0) == 1);

    QVERIFY(Primes::jacobiSymbol(0, 1) == 1);
    QVERIFY(Primes::jacobiSymbol(0, 2) == 0);

    QVERIFY(Primes::jacobiSymbol(2, 9) == 1);
    QVERIFY(Primes::jacobiSymbol(2, 27) == -1);
    QVERIFY(Primes::jacobiSymbol(3, 27) == 0);

    QVERIFY(Primes::jacobiSymbol(5, 12) == -1);
    QVERIFY(Primes::jacobiSymbol(6, 12) == 0);
    QVERIFY(Primes::jacobiSymbol(7, 12) == 1);

    QVERIFY(Primes::jacobiSymbol(0, 7) == 0);
    QVERIFY(Primes::jacobiSymbol(1, 7) == 1);
    QVERIFY(Primes::jacobiSymbol(2, 7) == 1);
    QVERIFY(Primes::jacobiSymbol(3, 7) == -1);
    QVERIFY(Primes::jacobiSymbol(4, 7) == 1);
    QVERIFY(Primes::jacobiSymbol(5, 7) == -1);
    QVERIFY(Primes::jacobiSymbol(6, 7) == -1);
}
