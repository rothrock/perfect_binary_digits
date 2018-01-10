// Copyright (c) 2018 Joseph Rothrock
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
uint64_t perfect_bits(uint64_t a, uint64_t b);


int main(int argc, char *argv[]) {

	uint64_t a = strtoull( "1645098712823793798", NULL, 10);
	uint64_t b = strtoull("14889998042940624528", NULL, 10);

	if (argc > 2) {
		a = strtoull(argv[1], NULL, 10);
		b = strtoull(argv[2], NULL, 10);
	};

	printf("%llu\n", perfect_bits(a, b));

	return 0;
}

// Most of the magic happens here.
uint64_t pcount(uint64_t x) {

	int b = used_bits(x);
	uint64_t ttl = b;
	uint64_t saved_x = x;
	int pb_list[7] = {4, 9, 16, 25, 36, 49, 64};
	int r;
	int c;
	int n; 
	int bc = 0;
	if (b <= 1) return x;
	if (x < 15) return ttl;

	for (int z = 0; z < 7; z++) {
		r = pb_list[z];
		c = r;
		x = saved_x;

		while (true) {
			b = used_bits(x);
			bc = count_bits(x, b);
			if (bc == c) {
				ttl++;
				if (bc == 64) {
					break;
				}
			}
			if (b == 0) break;
			n = b - 1;
			if (n == 0) break;
			if (n >= r) ttl += combos(n, r);
			r--;
			if (r == 0) break;
			x = x - ((uint64_t)1 << n);
		}
		if (x > 0) x = x - ((uint64_t)1 << n);
		bc = count_bits(x, b);
		if (bc >= c) {
			ttl++;
		}
	};

	return ttl;
}

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

int count_bits(uint64_t x, int nbits) {
	int bit_count = 0;

	for (int i = 0; i < nbits; i++) { 
		bit_count += x & (uint64_t)1;
		x = x >> 1;
	}

	return bit_count;
}

// Not used here, but useful to keep around when debugging.
uint64_t count_manually(uint64_t a, uint64_t b, int nbits) {
	uint64_t count = 0;

	for (; a <= b; a++) if (has_pft_bits(a, nbits)) count++; 

	return count;
}

int used_bits(uint64_t x) {
	int i = 0;
	uint64_t mask = (uint64_t)1 << 63;

	for (; (!(mask & x) && (i < 64)); i++) {
		mask = mask >> 1;
	}

	return (64 - i);
}

// Cancel terms from numerator and denominator arrays using
// factors from the opposite.
void cancel_out(uint64_t *n, int n_len, uint64_t *d, int d_len) {
	for (int l = 0; l < 3; l++) {
		for (int j = 0; j < d_len; j++) {
			if (d[j] == 1) continue;
			for (int k = 0; k < d_len; k++) {
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
				if (d[k] % n[j] == 0) {
					d[k] = d[k] / n[j];
					n[j] = 1;
					break;
				}
			}
		}
	}
}

uint64_t combos(uint64_t n, uint64_t r) {

	uint64_t nr = n-r;
	uint64_t n_ary[n];
	uint64_t r_ary[r];
	uint64_t nr_ary[nr];
	uint64_t i;
	int n_count = 0;
	uint64_t numerator;
	uint64_t denominator;

	for (i = 0; i < r; i++) r_ary[i] = 1;
	for (i = 0; i < n; i++) n_ary[i] = 1;
	for (i = 0; i < nr; i++) nr_ary[i] = 1;

	if ( r >= nr) {

		for (i = 0; i < nr; i++) n_ary[i] = n - i; 
		for (i = 0; i < nr; i++) nr_ary[i] = nr - i;

		cancel_out(n_ary, n, nr_ary, nr);

		numerator = n_ary[0];
		for (int i = 1; i < n; i++) numerator *= n_ary[i];
		denominator = nr_ary[0];
		for (int i = 1; i < nr; i++) denominator *= nr_ary[i];

	} else {

		for (i = 0; i < r; i++)  n_ary[i] = n - i; 
		for (i = 0; i < r; i++) r_ary[i] = r - i;

		cancel_out(n_ary, n, r_ary, r);

		numerator = n_ary[0];
		for (int i = 1; i < n; i++) numerator *= n_ary[i];
		denominator = r_ary[0];
		for (int i = 1; i < r; i++) denominator *= r_ary[i];

	}
	
	return numerator/denominator;
};

uint64_t perfect_bits(uint64_t a, uint64_t b) {

	if (a > b) return 0;
	if (a != 0) a--;

	return pcount(b) - pcount(a);
};
