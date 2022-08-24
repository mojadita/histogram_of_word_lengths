/* Histogram of word lengths.  Second approach.
 * Solution to exercise 1.13 (horizontal version)
 * Author: Luis Colorado <luis.colorado.urcola@gmail.com>
 * Date: Fri Mar  4 21:23:46 EET 2022
 * Copyright: (C) 2022 Luis Colorado.  All rights reserved.
 * License: BSD.
 */

#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RESIZE_INCREMENT    (8)
#define DEFAULT_COLUMNS     (70)
#define F(F_fmt) "%s:%d:%s:"F_fmt, __FILE__, __LINE__, __func__
#define WRN_TAIL(WT_fmt, ...) fprintf(stderr, WT_fmt, ##__VA_ARGS__)
#define WRN(W_fmt, ...) WRN_TAIL(F("WARNING: "W_fmt), ##__VA_ARGS__)

size_t num_columns = DEFAULT_COLUMNS;

/* forward reference */
void process(char *file_name, FILE *in, FILE *out);

int main(int argc, char **argv)
{
    char *out_name = NULL;
    int opt;
    while ((opt = getopt(argc, argv, "c:o:")) != EOF) {
        switch (opt) {
        case 'c': num_columns = atoi(optarg); break;
        case 'o': out_name = optarg; break;
        }
    }
    argc -= optind;
    argv += optind;
    FILE *out = NULL;
    if (out_name) {
        out = fopen(out_name, "r");
        if (!out)
            WRN("%s: %s, switching to stdout\n",
                out_name, strerror(errno));
    }
    if (!out)
        out = stdout;

    if (!argc) {
        process("stdin", stdin, out);
    } else {
        int i;
        for (int i = 0; i < argc; i++) {
            FILE *in = fopen(argv[i], "r");
            if (!in) {
                WRN("%s: %s, ignoring file\n",
                    argv[i], strerror(errno));
                continue;
            }
            process(argv[i], in, out);
            fclose(in);
        }
    }
} /* main */

#define GROW_ARRAY(_type, _name, _increm) do {            \
            if (_name##_len == _name##_cap) {             \
                _name##_cap += (_increm);                 \
                _name = realloc(_name,                    \
                        (sizeof *_name) * (_name##_cap)); \
            }                                             \
        } while (0)

void process(char *file_name, FILE *in, FILE *out)
{
    int c, pc = ' ';
    size_t word_length,
        most_frequent = 0,
        most_frequent_len;
    size_t *word_lengths = NULL,
           word_lengths_len = 0,
           word_lengths_cap = 0;
    while ((c = fgetc(in)) != EOF) {
        if (isalpha(c)) {
            /* in word */
            if (!isalpha(pc)) {
                /* start of word */
                word_length = 0;
            }
            word_length++;
        } else if (isalpha(pc)) {
            while (word_length >= word_lengths_len) {
                GROW_ARRAY(size_t, word_lengths, RESIZE_INCREMENT);
                word_lengths[word_lengths_len++] = 0;
            }
            /* end of word */
            word_lengths[word_length - 1]++;
            if (most_frequent < word_lengths[word_length - 1]) {
                most_frequent = word_lengths[word_length - 1];
                most_frequent_len = word_length;
            }
        }
        pc = c;
    }
    /* print the histogram */
    fprintf(out, "[%s]: most_frequent_len = %zd chars, most_frequent = %zd times\n",
            file_name, most_frequent_len, most_frequent);
    for (int i = 0; i < word_lengths_len; ++i) {
        if (word_lengths[i]) {
            /* only print the values > 0 */
            fprintf(out, "%4d:", i+1);
			size_t n = (word_lengths[i] * num_columns + (most_frequent >> 1)) / most_frequent;
            for (int j = 0; j < n; ++j) {
                fputc('#', out);
            }
            fprintf(out, "<%zu>\n", word_lengths[i]);
        }
    }
    if (word_lengths)
        free(word_lengths);
} /* main */
