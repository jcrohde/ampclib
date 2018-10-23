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

#include <primes.h>

#include <algorithm>

namespace ampc {

const Primes &Primes::getInstance() {
    static Primes p;
    return p;
}

bool Primes::isPrime(const uint32_t i) const {
    if (i <= std::numeric_limits<uint16_t>::max())
        return std::find(primes.begin(), primes.end(), i) != primes.end();

    for (const uint32_t prime : primes) {
        if (i % prime == 0) return false;
    }
    return true;
}

uint32_t Primes::getPrimeFactor(const uint32_t i) const {
    const uint32_t root = (int)std::sqrt(i) + 1;

    for (const uint32_t prime : primes) {
        if (prime > root) break;
        if (i % prime == 0) return prime;
    }

    return i;
}

void Primes::getPrimeFactors(uint32_t i, std::vector<uint32_t> &primeFactors) {
    primeFactors.clear();

    while (i > 1) {
        const uint32_t p = getInstance().getPrimeFactor(i);
        primeFactors.push_back(p);
        while (i % p == 0) i /= p;
    }
}

void Primes::getPrimeFactorsWithMultiplicity(uint32_t i, std::vector<uint32_t> &primeFactors) {
    primeFactors.clear();

    while (i > 1) {
        const uint32_t p = getInstance().getPrimeFactor(i);
        primeFactors.push_back(p);
        i /= p;
    }
}

void Primes::getFactors(uint32_t i, std::vector<uint32_t> &factors) {
    factors.resize(1, 1);

    if (i == 0) {
        factors.push_back(0);
        return;
    }

    while (i > 1) {
        const uint32_t p = Primes::getInstance().getPrimeFactor(i);
        uint32_t q = 1;
        std::vector<uint32_t> pPowerProducts;

        while (i % p == 0) {
            i /= p;
            q *= p;
            for (uint32_t factor : factors) {
                pPowerProducts.push_back(factor * q);
            }
        }

        factors.insert(factors.end(), pPowerProducts.begin(), pPowerProducts.end());
    }
}

uint32_t Primes::factorNumber(uint32_t i) {
    if (i == 0) return 2;

    uint32_t  number = 1;

    while (i > 1) {
        const uint32_t p = getInstance().getPrimeFactor(i);
        uint32_t pProductNumber = 0;

        while (i % p == 0) {
            i /= p;
            pProductNumber += number;
        }

        number += pProductNumber;
    }

    return number;
}

uint64_t  Primes::factorSum(const uint32_t i) {
    std::vector<uint32_t> factors;

    getFactors(i, factors);

    uint64_t  sum = 0;
    for (uint32_t factor : factors) {
        sum += factor;
    }

    return sum;
}

uint32_t Primes::eulerPhi(uint32_t i) {
    uint32_t phi = 1;

    if (i == 0) return 0;

    while (i > 1) {
        uint32_t  p = getInstance().getPrimeFactor(i);
        phi *= p - 1;
        i /= p;
        while (i % p == 0) {
            phi *= p;
            i /= p;
        }
    }

    return phi;
}

uint64_t  Primes::psi(uint32_t i) {
    uint64_t psi = 1;

    if (i == 0) return 0;

    while (i > 1) {
        uint32_t p = getInstance().getPrimeFactor(i);
        psi *= p + 1;
        i /= p;
        while (i % p == 0) {
            psi *= p;
            i /= p;
        }
    }

    return psi;
}

uint16_t Primes::pi(const uint16_t i) const {
    int count = 0;

    for (const uint32_t p : primes) {
        if (p > i) break;
        ++count;
    }

    return count;
}

uint32_t Primes::nextPrime(const uint16_t i) const {
    for (const uint32_t p : primes) {
        if (p > i) return p;
    }

    return 0;
}

int32_t Primes::lambda(uint32_t i) {
    if (i == 0) return 0;

    int32_t lambda = 1;

    while (i > 1) {
        uint32_t p = getInstance().getPrimeFactor(i);
        lambda *= -1;
        i /= p;
        while (i % p == 0) {
            lambda *= -1;
            i /= p;
        }
    }

    return lambda;
}

int32_t Primes::legendreSymbol(const uint32_t a, const uint32_t b) {
    if (!getInstance().isPrime(b)) return 0;

    uint64_t r = a % b;

    if (r == 0) return 0;

    if (b == 2) return 1;

    uint32_t exponent = (b - 1) / 2;
    uint64_t power = 1;

    for (uint32_t c = 0; c < exponent; ++c) {
        power *= r;
        power %= b;
    }

    return power == 1 ? 1 : -1;
}

int32_t Primes::jacobiSymbol(const uint32_t a, const uint32_t b) {
    std::vector<uint32_t> primeFactors;
    getPrimeFactorsWithMultiplicity(b, primeFactors);

    int32_t result = 1;

    for (const uint32_t p : primeFactors) {
        result *= legendreSymbol(a, p);
        if (result == 0) break;
    }

    return result;
}

Primes::Primes() : primes{2, 3} {
    for (uint32_t i = 4; i <= std::numeric_limits<uint16_t>::max(); ++i) {
        checkPrime(i);
    }

    uint32_t i = std::numeric_limits<uint16_t>::max();
    do {
        ++i;
    } while (!checkPrime(i));
}

bool Primes::checkPrime(uint32_t i) {
    for (uint32_t prime : primes) {
        if (i % prime == 0) return false;
    }

    primes.push_back(i);
    return true;
}

}
