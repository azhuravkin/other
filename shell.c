#include <stdio.h>
#include <string.h>
#include <sys/utsname.h>

char *context[] = {
    "",			/* 0 */
    "(config)",		/* 1 */
    "(config-if)"	/* 2 */
};

int cmp(char *buffer, char *string, int min)
{
    int len = strlen(buffer);

    if (len < min)
	return 0;

    buffer[len - 1] = '\0';

    if (strncmp(buffer, string, strlen(buffer)))
	return 0;

    return 1;
}

int main(void)
{
    struct utsname host;
    char buffer[1024];
    unsigned cont_num = 0;

    if (uname(&host))
	return 1;

    while (1) {
	printf("%s%s# ", host.nodename, context[cont_num]);
	fgets(buffer, sizeof(buffer), stdin);

	if (cont_num == 0 && cmp(buffer, "configure", 3)) {
	    cont_num = 1;
	}
	if (cont_num == 1 && cmp(buffer, "interface", 3)) {
	    cont_num = 2;
	}
	if (cmp(buffer, "exit", 4)) {
	    if (cont_num) {
		cont_num--;
	    } else {
		return 0;
	    }
	}
	if (feof(stdin)) {
	    printf("\n");
	    return 0;
	}
    }

    return 0;
}
