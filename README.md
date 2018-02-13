INTEGERS WITH PERFECT BINARY DIGITS
===================================

A perfect binary digit integer means that the count of the 1 digits in its binary representation add up to a number that is a [square number.](http://example.com/https://en.wikipedia.org/wiki/Square_number)

Two examples:
-------------

The integer 23 is 10111 in binary. 
It has four 1 bits.  
Four is a square number (sqrt(4) = 2),
so 23 is a perfect binary digit integer.

The integer 7 is 111 in binary.
It has three 1 bits.
Seven is not a square (sqrt(7) is about 2.6something),
so seven is not a perfect binary digit integer.

This program finds the number of perfect binary digits between two positive integers. 

	./pft 1 7
	3


How the program works
---------------------
One way the program could work:
1. get the next (or first) number in the given range.
1. count the 1 bits in the number.
1. See if the count is in one of (1, 4, 9, 16, 25, 36, 49, 64)
1. If so, increment a counter.
1. start over until we reach the end of our range.

Although easy to understand, the algorithm above is horribly slow.

To speed things up, the program relies on a formula from discrete mathematics.
See counting problems in the [wikipedia](https://en.wikipedia.org/wiki/nCr)

In particular, it uses the nCr formula. [Here ](https://www.calculatorsoup.com/calculators/discretemathematics/combinations.php)is the definition and an interactive calculator.

Think about counting the numbers with 4 perfect binary digits between 0 and 67.
Also, know that it is easy to find perfect binary digits in a power of 2. 
The power of 2 below 67 is 64. That gets us most of the way there.

64 In binary is a 1 with 6 zeros behind it: 1 0 0 0 0 0 0.

This means that there are 6 slots for sets of 4 bits to slosh around in.
Apply the formula. We have 4 bits in 6 possible places.

	6! / (6 - 4)! * 4! = 15

There aren't any more perfect-bit numbers between 64 and 67, so we're done.

With larger numbers, the program keeps taking them apart.

	113 = 64 + 32 + 16 + 1. 

The program does the work for 64, then 32, and then 16.
Of course choosing the correct 'n' and 'r' for the subsequent powers of two is a little more complicated.
