/*
 * Replace all occurrences of a POSIX extended regular expression in a text.
 * Usage: ./task1 <regex> <text> <replacement>
 *
 * Algorithm:
 *   1. Compile the regex with regcomp.
 *   2. Loop: regexec finds the next match.
 *      - Append text before the match to the output buffer.
 *      - Append the replacement to the output buffer.
 *      - Advance cursor past the match.
 *   3. Append remaining tail after the last match.
 *   4. Print the result.
 *   Buffer is grown with realloc whenever capacity is insufficient.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

#define INIT_CAP 256

/* Ensure buf has room for at least `need` bytes; updates *cap. */
static char *ensure(char *buf, size_t need, size_t *cap)
{
    if (need <= *cap)
        return buf;
    while (*cap < need)
        *cap *= 2;
    char *tmp = realloc(buf, *cap);
    if (!tmp)
        free(buf);
    return tmp;
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <regex> <text> <replacement>\n", argv[0]);
        return 1;
    }

    const char *pattern     = argv[1];
    const char *text        = argv[2];
    const char *replacement = argv[3];
    size_t      rep_len     = strlen(replacement);

    /* Compile the regular expression (extended syntax). */
    regex_t re;
    int err = regcomp(&re, pattern, REG_EXTENDED);
    if (err) {
        char errbuf[128];
        regerror(err, &re, errbuf, sizeof(errbuf));
        fprintf(stderr, "regcomp: %s\n", errbuf);
        return 1;
    }

    /* Allocate output buffer. */
    size_t cap = INIT_CAP;
    size_t len = 0;
    char  *buf = malloc(cap);
    if (!buf) {
        regfree(&re);
        return 1;
    }

    const char *cursor = text;
    regmatch_t  match;

    while (regexec(&re, cursor, 1, &match, cursor != text ? REG_NOTBOL : 0) == 0) {
        size_t pre = (size_t)match.rm_so; /* bytes before the match */

        /* Grow buffer: need room for pre-match text + replacement + NUL. */
        buf = ensure(buf, len + pre + rep_len + 1, &cap);
        if (!buf) {
            regfree(&re);
            return 1;
        }

        memcpy(buf + len, cursor, pre);   /* copy text before match   */
        len += pre;
        memcpy(buf + len, replacement, rep_len); /* copy replacement  */
        len += rep_len;

        cursor += match.rm_eo;

        /* Guard against infinite loop on a zero-length match. */
        if (match.rm_so == match.rm_eo) {
            if (*cursor == '\0')
                break;
            buf = ensure(buf, len + 2, &cap);
            if (!buf) {
                regfree(&re);
                return 1;
            }
            buf[len++] = *cursor++;
        }
    }

    /* Append the tail (text after the last match). */
    size_t tail = strlen(cursor);
    buf = ensure(buf, len + tail + 1, &cap);
    if (!buf) {
        regfree(&re);
        return 1;
    }
    memcpy(buf + len, cursor, tail);
    len += tail;
    buf[len] = '\0';

    printf("%s\n", buf);

    free(buf);
    regfree(&re);
    return 0;
}
