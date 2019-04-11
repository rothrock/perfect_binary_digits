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
Three is not a square number. (sqrt(3) ~1.732),
so seven is not a perfect binary digit integer.

This program finds the *number* of perfect binary digits between two positive integers. 

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

In particular, it uses the nCr formula to cut through much of that brute force.. [Here](https://www.calculatorsoup.com/calculators/discretemathematics/combinations.php)is the definition and an interactive calculator.

In order to be included in our count,
a number's binary representation needs to have 1, 4, 9, 16, 25, 36, 49, or 64 bits set to 1.

Consider the decimal number 64.

64 in binary is a 1 with 6 zeros behind it: 1 0 0 0 0 0 0. That's 7 bits total.

7 > 1, 7 > 4, but 7 < 9, so any numbers we care about will have either 4 bits set to 1 or just a single bit set to 1.

Set aside the single-bit case for the moment. Consider finding the numbers up to 64 with exactly 4 bits set.

Observe that the integers between 0 and 63 need at most 6 bits.
They will all fit in the 6 trailing bits after the 1 in 64.
This means that there are 6 slots for sets of 4 bits to slosh around in.

Apply the nCr combinations formula. We have 4 bits in 6 possible places.

	C(n, r) = 
  n! / (r! * (n - r)!)

	6! / (4! * (6 - 4)!) = 15

There are 15 numbers with exactly 4 bits set to 1.

The case of numbers with only 1 bit set to 1 is straightforward.

	Number	In Binary			1-bits
		64		1 0 0 0 0 0 0		1
		32			1 0 0 0 0 0   1
		16				1 0 0 0 0		1
		 8					1 0 0 0		1
		 4						1 0 0		1
		 2							1 0		1
		 1								1		1
	 ---------------------------
	                        7

There are 15 + 7 = 22 numbers with perfect binary digits between 0 and 64.

Larger numbers are broken up into an additive chain of decreasing powers of two and each link evaluated.

	113 = 64 + 32 + 16 + 1.
	127 = 64 + 32 + 16 + 8 + 4 + 2 + 1.
  129 = 128 + 1.

Testing
-------
Included are two tests. One in Go and the other in Bash.
Both rely on an 'Oracle' (not the database) somewhere on the internet that knows the correct answer.

Run the Bash version like so:

	make test

You should see something like this:
	clang -Werror -g   -c -o pft.o pft.c
	gcc -o pft pft.o -Werror -g 
	chmod 755 pft
	./testit.sh 10 100
	10  oracle=4  pft=4   
	11  oracle=4  pft=4   
	12  oracle=4  pft=4   
	13  oracle=4  pft=4   
	14  oracle=4  pft=4   
	15  oracle=5  pft=5   
	16  oracle=6  pft=6   
	17  oracle=6  pft=6   
  ...
	...
	...

The Go version will try 3,000,000,000 random numbers using 40 goroutines.

	 make gotest

The output:

	clang -Werror -g   -c -o pft.o pft.c
	gcc -o pft pft.o -Werror -g
	chmod 755 pft
	go run test-pft.go
	11525030097865261574 oracle = 915096048791149969 beep = 915096048791149969 MATCH
	3619520504219696927 oracle = 275987812508718171 beep = 275987812508718171 MATCH
	12881502906308645028 oracle = 1024072386590392324 beep = 1024072386590392324 MATCH
	18252189985895656162 oracle = 1499614885929439434 beep = 1499614885929439434 MATCH
	7354949233259147073 oracle = 571788042953110264 beep = 571788042953110264 MATCH
	15757828176933690593 oracle = 1269074278812468454 beep = 1269074278812468454 MATCH
  ...
	...
	...

