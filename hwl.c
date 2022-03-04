/* Histogram of word lengths.  First approach.
 * Solution to exercise 1.13 (horizontal version)
 * Author: Luis Colorado <luis.colorado.urcola@gmail.com>
 * Date: Fri Mar  4 21:23:46 EET 2022
 * Copyright: (C) 2022 Luis Colorado.  All rights reserved.
 * License: BSD.
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define MAX_LENGTH	4

unsigned word_lengths[MAX_LENGTH],
	word_excess_length;


int main()
{
	int c, pc = ' ';
	unsigned word_length;
	while ((c = getchar()) != EOF) {
		if (isalpha(c)) {
			/* in word */
			if (!isalpha(pc)) {
				/* start of word */
				word_length = 0;
			}
			word_length++;
		} else if (isalpha(pc)) {
			/* end of word */
			if (word_length > MAX_LENGTH) {
				word_excess_length++;
			} else {
				word_lengths[word_length - 1]++;
			}
		}
		pc = c;
	}
	/* print the histogram */
	for (int i = 0; i < MAX_LENGTH; ++i) {
		if (word_lengths[i]) {
			/* only print the values > 0 */
			printf(" %3d:", i+1);
			for (int j = 0; j < word_lengths[i]; ++j) {
				putchar('#');
			}
			printf(" [%u]\n", word_lengths[i]);
		}
	}
	if (word_excess_length) {
		/* only print if > 0 */
		printf(">%3d:", MAX_LENGTH);
		for (int j = 0; j < word_excess_length; ++j) {
			putchar('#');
		}
		printf(" [%u]\n", word_excess_length);
	}
} /* main */
