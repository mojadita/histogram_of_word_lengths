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
#include <sys/ioctl.h>
#include <unistd.h>

#define RESIZE_INCREMENT    (8)
#define DEFAULT_COLUMNS     (70)

#define F(F_fmt) "%s:%d:%s:"F_fmt, __FILE__, __LINE__, __func__
#define WRN_TAIL(WT_fmt, ...) fprintf(stderr, WT_fmt, ##__VA_ARGS__)
#define WRN(W_fmt, ...) WRN_TAIL(F("WARNING: "W_fmt), ##__VA_ARGS__)

size_t num_columns = DEFAULT_COLUMNS;
int    debug       = 0;

const char *seq    = "..................................................";
size_t seq_l;

/* forward reference */
void process(const char *file_name, FILE *in, FILE *out);

int main(int argc, char **argv)
{
    char *out_name = NULL;
    int opt;

    if (isatty(1)) {
        struct winsize ws;
        int res = ioctl(1, TIOCGWINSZ, &ws);
        if (res >= 0) {
            num_columns = ws.ws_col;
        }
    }

    while ((opt = getopt(argc, argv, "c:do:s:")) != EOF) {
        switch (opt) {
        case 'c': num_columns = atoi(optarg); break;
		case 'd': debug = 1; break;
        case 'o': out_name = optarg; break;
        case 's': seq = optarg; break;
        }
    }

    seq_l = strlen(seq); 
    argc -= optind;
    argv += optind;

    FILE *out = NULL;
    if (out_name) {
        out = fopen(out_name, "a");
        if (!out)
            WRN("%s: %s, switching to stdout\n",
                out_name, strerror(errno));
    }
    if (!out)
        out = stdout;

    if (!argc) {
        process("stdin", stdin, out);
    } else {
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

#define GROW_ARRAY(_type, _name, _increm) do { \
            if (_name##_len == _name##_cap) {  \
                _name##_cap += (_increm);      \
                _name        = realloc(_name,  \
                        (sizeof *_name)        \
                        * (_name##_cap));      \
            }                                  \
        } while (0)

void process(
        const char *file_name,
        FILE *in,
        FILE *out)
{
    int c, pc = ' ';

    size_t word_length,
        most_frequent = 0,
        most_frequent_len;

    size_t *word_freqs = NULL,
           word_freqs_len = 0,
           word_freqs_cap = 0;
	char buffer[1000], *p;
    while ((c = fgetc(in)) != EOF) {
        if (isalpha(c)) {
            /* in word */
            if (!isalpha(pc)) {
                /* start of word */
                word_length = 0;
				if (debug) p = buffer;
            }
            word_length++;
			if (debug) *p++ = c;
        } else if (isalpha(pc)) {
            while (word_length >= word_freqs_len) {
                GROW_ARRAY(size_t, word_freqs, RESIZE_INCREMENT);
                word_freqs[word_freqs_len++] = 0;
            }
            /* end of word */
			if (debug) {
				*p = 0;
				printf("%zd %s\n", word_length, buffer);
			}
            word_freqs[word_length - 1]++;
            if (most_frequent < word_freqs[word_length - 1]) {
                most_frequent = word_freqs[word_length - 1];
                most_frequent_len = word_length;
            }
        }
        pc = c;
    }

    /* print the histogram */
    int size_of_word_lengths = snprintf(NULL, 0, "%zu", word_freqs_len),
        to_discount          = snprintf(NULL, 0, ":  <%zu>", most_frequent);
    int max = num_columns - size_of_word_lengths - to_discount;

    fprintf(out, "[%s]\n", file_name);
    for (int i = 0; i < word_freqs_len; ++i) {
        if (word_freqs[i] > 0) {
            /* only print the values > 0 */
            fprintf(out, "%*u: ", size_of_word_lengths, i + 1);

            int n = (max * word_freqs[i] + (most_frequent >> 1)) / most_frequent;
            while (n >= seq_l) {
                n -= fprintf(out, "%s", seq);
            }
            fprintf(out, "%.*s <%zu>\n", n, seq, word_freqs[i]);
        }
    }
    if (word_freqs) {
        free(word_freqs);
    }
} /* process */
