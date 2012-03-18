#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define SOURCE "/proc/net/dev"

int get_counts(FILE *fp, const char dev[16], unsigned long int *in_count, unsigned long int *out_count);

int main(int argc, char **argv) {
    FILE *fp;
    char dev[16];
    time_t t;
    unsigned long int in_count = 0;
    unsigned long int out_count = 0;

    if (argc != 2) {
	fprintf(stderr, "Usage: %s <ifname>\n", argv[0]);
	return 1;
    }

    t = time(NULL);

    if ((fp = fopen(SOURCE, "r")) == NULL) {
	fprintf(stderr, "Error opening source file: %s\n", SOURCE);
	exit(1);
    }

    snprintf(dev, sizeof(dev), "%s:", argv[1]);

    if (get_counts(fp, dev, &in_count, &out_count)) {
        fprintf(stderr, "Error: device %s not exist\n", argv[1]);
	fclose(fp);
    }

    printf("%lu\n%lu\n%lu\n%s\n", in_count, out_count, t, argv[1]);

    return 0;
}

int get_counts(FILE *fp, const char dev[16], unsigned long int *in_count, unsigned long int *out_count) {
    char *line;
    char buff[256];
    char *colon;
    char *in = NULL;
    char *out = NULL;
    int i;

    while ((line = fgets(buff, sizeof(buff), fp))) {
	/* Trim leading whitespace */
	while (isspace(line[0])) line++;

	if ((colon = strchr(line, ':'))) {
	    if (!strncmp(line, dev, strlen(dev))) {
		in = strtok(colon + 1, " ");
		for (i = 0; i < 8; i++)
		    out = strtok(NULL, " ");
		break;
	    }
	}
    }

    rewind(fp);

    if (!in || !out)
	return 1;

    *in_count = strtoul(in, NULL, 10);
    *out_count = strtoul(out, NULL, 10);

    return 0;
}
