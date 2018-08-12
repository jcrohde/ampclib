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
#include <math.h>
#include <cstdint>

namespace ampc {

class Primes {
public:
    static const Primes &getInstance();

    bool isPrime(const uint32_t i) const;

    uint32_t getPrimeFactor(const uint32_t i) const;

    static void getPrimeFactors(uint32_t i, std::vector<uint32_t> &primeFactors);
    static void getPrimeFactorsWithMultiplicity(uint32_t i, std::vector<uint32_t> &primeFactors);
    static void getFactors(uint32_t i, std::vector<uint32_t> &factors);
    static uint32_t factorNumber(uint32_t i);
    static uint64_t factorSum(const uint32_t i);
    static uint32_t eulerPhi(uint32_t i);
    static uint64_t psi(uint32_t i);
    uint16_t pi(const uint16_t i) const;
    uint32_t nextPrime(const uint16_t) const;
    static int32_t lambda(uint32_t i);
    static int32_t legendreSymbol(const uint32_t a, const uint32_t b);
    static int32_t jacobiSymbol(const uint32_t a, const uint32_t b);

private:
    std::vector<uint32_t> primes;

    Primes();

    ~Primes() = default;
    Primes(const Primes&) = delete;
    Primes &operator=(const Primes&) = delete;

    bool checkPrime(uint32_t i);
};

}
