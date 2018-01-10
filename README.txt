INTEGERS WITH PERFECT BITS
==========================

A perfect bit integer means that the count of the 1 digits in its binary representation add up to a number that is a square number.

http://example.com/https://en.wikipedia.org/wiki/Square_number

Two examples:
-------------

The integer 23 is 10111 in binary. 
It has four 1 bits.  
Four is a square number (sqrt(4) = 2),
so 23 is a perfect bit integer.

The integer 7 is 111 in binary.
It has three 1 bits.
Seven is not a square (sqrt(7) is about 2.6something),
so seven is not a perfect bit integer.

This program finds the number of perfect bits between two positive integers. 

	./pft 1 7
	3

Included is a shell script called testit.sh to compare results against https://perfect-bits.binti.com/.

$ ./testit.sh 7 14
7  binti=3  pft=3
8  binti=4  pft=4
9  binti=4  pft=4
10  binti=4  pft=4
11  binti=4  pft=4
12  binti=4  pft=4
13  binti=4  pft=4
