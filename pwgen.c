#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static void print_pass(int len) {
    register int i;

    static char chars[] = {
	'a', 'c', 'e', 'f', 'g', 'h', 'j', 'k',
	'm', 'n', 'p', 'q', 'r', 's', 't', 'u',
	'v', 'w', 'x', 'y', 'z', 'A', 'C', 'E',
	'F', 'G', 'H', 'J', 'K', 'M', 'N', 'P',
	'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',
	'Z', '2', '3', '4', '5', '6', '7', '9'
    };

    for (i = 0; i < len; i++) {
	putchar(chars[rand() % sizeof(chars)]);
    }
}

int main(int argc, char **argv)
{
    register int i;
    int len = 5;

    srand(time(NULL));

    if (argc > 1) {
	len = atoi(argv[1]);
    }

    for (i = 1; i <= 100; i++) {
	print_pass(len);

	if (i % 10) {
	    putchar(' ');
	} else {
	    putchar('\n');
	}
    }

    return 0;
}
