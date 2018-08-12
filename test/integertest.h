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

#include <QtTest>

class IntegerTest: public QObject
{
    Q_OBJECT
private slots:
    void generationTest();
    void comparisionTest();
    void additionTest();
    void subtractionTest();
    void multiplicationTest();
    void divisionTest();
    void moduloTest();
    void incrementTest();
    void decrementTest();
    void unsignedIntTest();
    void functionTest();
    void powerTest();
    void powerModuloTest();
    void millerRabinTest();

    void performanceAdditionTest();
    void performanceSubtractionTest();
    void performanceMultiplicationTest();

    void performanceFibonacciTest();
    void performanceFacultyTest();
};
