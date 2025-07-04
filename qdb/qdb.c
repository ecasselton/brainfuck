/*  qdb.c -- quick and dirty brainfuck interpreter.
    Version 1.2
    Daniel B Cristofani  */

/*  Make any use you like of this software. I can't stop you anyway. :)*/

/*  This interpreter uses byte cells, wraps around on overflow or
    underflow, has unpredictable misbehavior if the pointer is moved
    outside the array, and interprets '#' as a command to output the
    first 64 cells of the array, plus a '^' for the pointer. It
    should be fairly fast, but no match for a decent compiler or even
    for a really clever interpreter.  */

#include <stdio.h>
#include <stdlib.h>

#define S 655360

int t[S];
char a[16777216], p[S], bf[256], b;
int c, g, i, j, m, x;
FILE *prog;

int main(int argc, char **argv) {
    if (!(prog = fopen(argv[1], "r"))) {
        fprintf(stderr, "can't open the file \"%s\".\n", argv[1]);
        exit(1);
    }
    g = fread(p, 1, S, prog);
    fclose(prog);
    bf['<'] = bf['>'] = bf['+'] = bf['-'] = bf[','] = bf['.'] = bf['['] = bf[']'] = bf['#'] = 1;
    m = S;
    while (i < g) {
        for (b = 0; i < g && (!bf[p[i]] || (b ? b == p[i] ? ++c : 0 : (b = p[i], c = 1)));)
            i++;
        switch (b) {
            case '+':
                if (c <= 4)
                    p[j++] = 'a' - 1 + c;
                else
                    p[j] = '+', t[j++] = c;
                break;
            case '-':
                if (c <= 4)
                    p[j++] = 'd' + c;
                else
                    p[j] = '-', t[j++] = c;
                break;
            case '>':
                if (c <= 4)
                    p[j++] = 'h' + c;
                else
                    p[j] = '>', t[j++] = c;
                break;
            case '<':
                if (c <= 4)
                    p[j++] = 'l' + c;
                else
                    p[j] = '<', t[j++] = c;
                break;
            case ',':
            case '.':
            case '#':
                while (c--)
                    p[j++] = b;
                break;
            case '[':
                while (c--)
                    t[--m] = j;
                p[j++] = '[';
                break;
            case ']':
                if (m + c > S)
                    fprintf(stderr, "unbalanced ].\n"), exit(1);
                t[j] = t[m] + 1;
                while (c--)
                    t[t[m++]] = j + 1;
                p[j++] = ']';
                break;
        }
    }
    p[j] = 0;
    if (m != S)
        fprintf(stderr, "unbalanced [.\n"), exit(1);
    j = 0;
    while (1) {
        switch (p[j]) {
            case '+':
                a[x] += t[j++];
                break;
            case '-':
                a[x] -= t[j++];
                break;
            case '>':
                x += t[j++];
                break;
            case '<':
                x -= t[j++];
                break;
            case '[':
                if (!a[x])
                    j = t[j];
                else
                    j++;
                break;
            case ']':
                if (a[x])
                    j = t[j];
                else
                    j++;
                break;
#if '\n' == 10
            case ',':
                if ((c = getchar()) != EOF)
                    a[x] = c;
                j++;
                break;
            case '.':
                putchar(a[x]);
                fflush(stdout);
                j++;
                break;
#else
            case ',':
                if ((c = getchar()) != EOF)
                    a[x] = c == '\n' ? 10 : c;
                j++;
                break;
            case '.':
                putchar(a[x] == 10 ? '\n' : a[x]);
                fflush(stdout);
                j++;
                break;
#endif
            case 'a':
                a[x]++;
                j++;
                break;
            case 'b':
                a[x] += 2;
                j++;
                break;
            case 'c':
                a[x] += 3;
                j++;
                break;
            case 'd':
                a[x] += 4;
                j++;
                break;
            case 'e':
                a[x]--;
                j++;
                break;
            case 'f':
                a[x] -= 2;
                j++;
                break;
            case 'g':
                a[x] -= 3;
                j++;
                break;
            case 'h':
                a[x] -= 4;
                j++;
                break;
            case 'i':
                x++;
                j++;
                break;
            case 'j':
                x += 2;
                j++;
                break;
            case 'k':
                x += 3;
                j++;
                break;
            case 'l':
                x += 4;
                j++;
                break;
            case 'm':
                x--;
                j++;
                break;
            case 'n':
                x -= 2;
                j++;
                break;
            case 'o':
                x -= 3;
                j++;
                break;
            case 'p':
                x -= 4;
                j++;
                break;
            case '#':
                printf("\n");
                for (i = 0; i < 64; i++)
                    printf("%4d", (signed char)a[i]);
                printf("\n%*s\n", x * 4 + 4, "^");
                j++;
                break;
            case 0:
                exit(0);
                break;
        }
    }
}
