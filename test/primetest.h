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

#include <QtTest>

class PrimeTest: public QObject
{
    Q_OBJECT
private slots:
    void isPrimeTest();
    void primeFactorTest();
    void getPrimeFactorsTest();
    void getPrimeFactorsMultiplicityTest();
    void getFactorNumberTest();
    void getFactorsTest();
    void factorSumTest();
    void phiTest();
    void psiTest();
    void piTest();
    void nextTest();
    void lambdaTest();
    void legendreTest();
    void jacobiTest();
};
