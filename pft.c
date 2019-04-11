// Copyright (c) 2019 Joseph Rothrock
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int used_bits(uint64_t x);
bool has_pft_bits(uint64_t x, int nbits);
int count_bits(uint64_t x, int nbits);
uint64_t count_manually(uint64_t a, uint64_t b, int nbits);
void cancel_out(uint64_t *n, int n_len, uint64_t *d, int d_len);
uint64_t combos(uint64_t n, uint64_t r);
uint64_t pcount(uint64_t x);
uint64_t perfect_binary_digits(uint64_t a, uint64_t b);


int main(int argc, char *argv[]) {

	uint64_t a = strtoull( "1645098712823793798", NULL, 10);
	uint64_t b = strtoull("14889998042940624528", NULL, 10);

	if (argc > 2) {
		a = strtoull(argv[1], NULL, 10);
		b = strtoull(argv[2], NULL, 10);
	};

	printf("%llu\n", perfect_binary_digits(a, b));

	return 0;
}

// Returns the count of numbers with perfect binary digits between
// zero and the supplied integer.
//
// See the 'How The Program Works' section for additional insight.
// 
uint64_t pcount(uint64_t x) {

	int b = used_bits(x);
	uint64_t ttl = b;
	uint64_t saved_x = x;
	int pb_list[7] = {4, 9, 16, 25, 36, 49, 64};
	int r;
	int c;
	int n; 
	int bc = 0;
	// We can do these small numbers easily.
	if (b <= 1) return x;
	if (x < 15) return ttl;

	// Evaluate x at each of the allowable possibilities.
	for (int z = 0; z < 7; z++) {
		r = pb_list[z];
		c = r;
		x = saved_x;

		// The loop considers x as an additive chain of powers of 2.
		// Each iteration looks at only the largest link in the chain.
		// Some examples:
		// 113 = 64 + 32 + 16 + 1. 
		// 127 = 64 + 32 + 16 + 8 + 4 + 2 + 1.
		// 129 = 128 + 1.
		//
		while (true) {
			b = used_bits(x);
			bc = count_bits(x, b);
			// Keep track of the powers of 2.
			if (bc == c) {
				ttl++;
				// can't go wider than 64 bits.
				if (bc == 64) {
					break;
				}
			}
			// No bits set, so leave.
			if (b == 0) break;
			// Determine the set size.
			n = b - 1;
			// Nothing to count.
			if (n == 0) break;
			// Sample size (r) can't be bigger than the set size.
			// Otherwise count the possibilities.
			if (n >= r) ttl += combos(n, r); 
			// Lower the sample size for the next round.
			r--;
			// No sample size, so leave.
			if (r == 0) break;
			// Move on to the next link in the chain.
			x = x - ((uint64_t)1 << n);
		}
		// This section is for those annoying cases where there's
		// still a little bit of x still hanging around.
		if (x > 0) x = x - ((uint64_t)1 << n);
		bc = count_bits(x, b);
		if (bc >= c) {
			ttl++;
		}
	}

	return ttl;
}

// Not used here, but useful to keep around when debugging.
uint64_t count_manually(uint64_t a, uint64_t b, int nbits) {
	uint64_t count = 0;

	for (; a <= b; a++) if (has_pft_bits(a, nbits)) count++; 

	return count;
}

// Used only in the count_manually function above.
bool has_pft_bits(uint64_t x, int nbits) {
	int bit_count = 0;

	for (int i = 0; i < nbits; i++) { 
		bit_count += x & (uint64_t)1;
		x = x >> 1;
	}

	for (int i = 1; i <= 8; i++)
		if (bit_count == i*i) return true;

	return false;
}

// Count the number of 1 bits in x up to position nbits.
int count_bits(uint64_t x, int nbits) {
	int bit_count = 0;

	for (int i = 0; i < nbits; i++) { 
		bit_count += x & (uint64_t)1;
		x = x >> 1;
	}

	return bit_count;
}

// Count the number of bit positions needed to represent x.
int used_bits(uint64_t x) {
	int i = 0;
	uint64_t mask = (uint64_t)1 << 63;

	for (; (!(mask & x) && (i < 64)); i++) {
		mask = mask >> 1;
	}

	return (64 - i);
}

// Cancel common terms from numerator and denominator arrays.
void cancel_out(uint64_t *n, int n_len, uint64_t *d, int d_len) {

	// Three is an arbitrary number of passes to make over the array pairs.
	for (int l = 0; l < 3; l++) {
		for (int j = 0; j < d_len; j++) { // 
			if (d[j] == 1) continue;
			for (int k = 0; k < d_len; k++) {
				// If the denominator term divides the numerator term evenly,
				// then replace the numerator with the result of that division.
				// Then put 1 in the denominator. Example:
				//
				// ( 6 % 3) = 0, 6 / 3 = 2, so replace the 6 in the numerator
				// with 2 and replace the 3 in the denominator with 1.
				if (n[k] % d[j] == 0) {
					n[k] = n[k] / d[j];
					d[j] = 1;
					break;
				}
			}
		}

		for (int j = 0; j < d_len; j++) {
			if (n[j] == 1) continue;
			for (int k = 0; k < d_len; k++) {
				// Same logic as above, just roles reversed.
				if (d[k] % n[j] == 0) {
					d[k] = d[k] / n[j];
					n[j] = 1;
					break;
				}
			}
		}
	}
}

// This is the 'nCr' formula:
// C(n, r) = n! / (r! * (n - r)!)
//
// It finds the number of combinations taking a sample of items (r)
// from a larger set (n).
//
// Although superficially easy to implement, in practice it's tricky 
// because the factorial of even fairly small numbers are ENORMOUS.
// For example:
// 40! = 81591528324789773434561126959611589427200000000
//
// The function overcomes this obstacle by turning factorials into
// arrays of integers and then cancelling terms shared by both arrays.
//
// Here is an example:
//
// C(7, 3) is:
//
//  7 * 6 * 5 * 4 * 3 * 2 * 1
// ---------------------------
//  3 * 2 * 1 * 4 * 3 * 2 * 1
//
// After cancelling obvious terms, it becomes:
//
//  7 * 6 * 5
// -----------
//  3 * 2 * 1
//
// And, since 3 * 2 and 6 cancel each other, what remains is:
//
//  7 *     5
// ----------- = 35
//          1
//
uint64_t combos(uint64_t n, uint64_t r) {

	uint64_t n_minus_r = n - r;
	uint64_t n_ary[n];
	uint64_t r_ary[r];
	uint64_t n_minus_r_ary[n_minus_r];
	uint64_t i;
	int n_count = 0;
	uint64_t numerator;
	uint64_t denominator;

	for (i = 0; i < r; i++) r_ary[i] = 1;
	for (i = 0; i < n; i++) n_ary[i] = 1;
	for (i = 0; i < n_minus_r; i++) n_minus_r_ary[i] = 1;

	// The denominator is made of the two terms r and n - r.
	// Decide which one is more efficient to remove from the numerator.
	if ( r >= n_minus_r) {

		// Build our factorial arrays.
		for (i = 0; i < n_minus_r; i++) n_ary[i] = n - i; 
		for (i = 0; i < n_minus_r; i++) n_minus_r_ary[i] = n_minus_r - i;

		// Remove common terms
		cancel_out(n_ary, n, n_minus_r_ary, n_minus_r);

		// Turn the updated arrays back into integers
		numerator = n_ary[0];
		for (int i = 1; i < n; i++) numerator *= n_ary[i];
		denominator = n_minus_r_ary[0];
		for (int i = 1; i < n_minus_r; i++) denominator *= n_minus_r_ary[i];

	} else {

		// Build our factorial arrays
		for (i = 0; i < r; i++)  n_ary[i] = n - i; 
		for (i = 0; i < r; i++) r_ary[i] = r - i;

		// Remove common terms
		cancel_out(n_ary, n, r_ary, r);

		// Turn the updated arrays back into integers
		numerator = n_ary[0];
		for (int i = 1; i < n; i++) numerator *= n_ary[i];
		denominator = r_ary[0];
		for (int i = 1; i < r; i++) denominator *= r_ary[i];

	}
	
	return numerator/denominator;
}

// Wrap the hard work in something nice.
uint64_t perfect_binary_digits(uint64_t a, uint64_t b) {

	if (a > b) return 0;
	if (a != 0) a--;

	return pcount(b) - pcount(a);
}
