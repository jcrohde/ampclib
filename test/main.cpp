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

#include "integertest.h"
#include "primetest.h"
#include "rationaltest.h"


int main(int argc, char **argv)
{
   int status = 0;

   {
       IntegerTest t;
       status |= QTest::qExec(&t, argc, argv);
   }

   {
       RationalTest t;
       status |= QTest::qExec(&t, argc, argv);
   }

   {
      PrimeTest tc;
      status |= QTest::qExec(&tc, argc, argv);
   }

   return status;
}
